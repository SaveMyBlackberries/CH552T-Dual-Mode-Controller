// ===================================================================================
// Dual Mode Controller - HID Joystick + MIDI Controller
// CH552T SuperMini USB 开发板
// ===================================================================================
//
// 硬件连接：
// - P1.4 (AIN1) - 旋钮1/电位器1 → HID油门轴 / MIDI CC1
// - P1.1 (AIN0) - 旋钮2/电位器2 → HID反推轴 / MIDI Pitch Bend
// - P3.1 - 按钮1 → HID按钮1 / MIDI音符C1 (36)
// - P3.2 - 按钮2 → HID按钮2 / MIDI音符A1 (33)
// - P1.6 - 按钮3 → HID按钮3 / MIDI功能键(+12半音)
// - P1.7 - 按钮4 → HID按钮4 / MIDI功能键(-12半音)
// - 按钮1 (P3.1) - 模式选择 → 上电检测（松开=HID, 按住=MIDI）
// - P3.0 - LED指示 → HID闪3次 / MIDI闪5次
//
// ===================================================================================

#include "src/config.h"
#include "src/system.h"
#include "src/delay.h"
#include "src/usb_composite.h"
#include "src/usb_midi.h"          // MIDI通信函数

// USB中断
void USB_interrupt(void);
void USB_ISR(void) __interrupt(INT_NO_USB) {
  USB_interrupt();
}

#pragma disable_warning 110

// ===================================================================================
// 模式定义
// ===================================================================================
#define MODE_HID    0
#define MODE_MIDI   1

uint8_t device_mode = MODE_HID;  // 全局模式变量

// ===================================================================================
// ADC 相关定义和函数
// ===================================================================================
#define ADC_CHANNEL_POT1    1    // P1.4 = AIN1 (旋钮1)
#define ADC_CHANNEL_POT2    0    // P1.1 = AIN0 (旋钮2)

// ADC初始化
void ADC_init(void) {
  ADC_CFG |= bADC_EN;                // 使能ADC
  ADC_CFG &= ~bADC_CLK;              // 快速模式 (96 Fosc)
}

// 读取指定ADC通道
uint8_t ADC_read(uint8_t channel) {
  if(channel == 0) {
    ADC_CHAN1 = 0; ADC_CHAN0 = 0;    // AIN0 (P1.1)
    P1_DIR_PU &= ~(1<<1);
  }
  else if(channel == 1) {
    ADC_CHAN1 = 0; ADC_CHAN0 = 1;    // AIN1 (P1.4)
    P1_DIR_PU &= ~(1<<4);
  }
  
  ADC_START = 1;
  while(ADC_START);
  return ADC_DATA;
}

// 滤波后读取ADC（16次平均）
uint8_t ADC_read_filtered(uint8_t channel) {
  uint16_t sum = 0;
  for(uint8_t i = 0; i < 16; i++) {
    sum += ADC_read(channel);
    DLY_us(50);
  }
  return (uint8_t)(sum >> 4);
}

// 将ADC值映射到HID轴值(-127到127)
int8_t ADC_to_axis(uint8_t adc_value) {
  return (int8_t)(adc_value - 128);
}

// 将ADC值映射到MIDI值(0-127)，根据实测数据校准
uint8_t ADC_to_MIDI(uint8_t adc_value) {
  // 实测：电位器ADC范围 0-172，映射到 MIDI 0-127
  // 公式：adc * 127 / 172
  if(adc_value >= 172) return 127;
  return (uint8_t)((uint16_t)adc_value * 127 / 172);
}

// 将ADC值映射到MIDI Pitch Bend值(-8192到8191)，根据实测数据校准
int16_t ADC_to_PitchBend(uint8_t adc_value) {
  // 实测：电位器ADC范围 0-172，中心值 86
  // 公式：(adc - 86) * 8192 / 86
  int16_t offset = (int16_t)adc_value - 86;
  if(offset >= 86) return 8191;   // 限制上限
  if(offset <= -86) return -8192; // 限制下限
  return (int16_t)((int32_t)offset * 8192 / 86);
}

// ===================================================================================
// 模式检测和LED指示函数
// ===================================================================================

// 检测工作模式（上电时调用）
uint8_t detect_mode(void) {
  // 注意：按钮引脚已在main中配置为输入上拉
  DLY_ms(10);  // 等待稳定
  
  // 检测按钮1(P3.1)状态
  // 按钮按下=低电平=MIDI模式
  if(P3 & (1<<1)) {
    return MODE_HID;   // 按钮1松开 = HID飞行控制器模式
  } else {
    return MODE_MIDI;  // 按钮1按下 = MIDI控制器模式
  }
}

// LED指示当前模式
void indicate_mode(uint8_t mode) {
  if(mode == MODE_HID) {
    // HID模式：快速闪3次
    for(uint8_t i = 0; i < 3; i++) {
      P3 |= (1<<0);   DLY_ms(150);
      P3 &= ~(1<<0);  DLY_ms(150);
    }
  } else {
    // MIDI模式：慢速闪5次
    for(uint8_t i = 0; i < 5; i++) {
      P3 |= (1<<0);   DLY_ms(300);
      P3 &= ~(1<<0);  DLY_ms(300);
    }
  }
}

// ===================================================================================
// HID模式主循环
// ===================================================================================
void run_hid_mode(void) {
  __bit btn1_last = 0, btn2_last = 0, btn3_last = 0, btn4_last = 0;
  uint8_t joy_buttons = 0;
  uint8_t adc_pot1 = 128, adc_pot2 = 128;
  int8_t axis_x = 0, axis_y = 0;
  
  while(1) {
    // ========== 读取ADC（带16次平均滤波）==========
    adc_pot1 = ADC_read_filtered(ADC_CHANNEL_POT1);  // 旋钮1 (P1.4)
    adc_pot2 = ADC_read_filtered(ADC_CHANNEL_POT2);  // 旋钮2 (P1.1)
    
    // 转换为轴值
    axis_x = ADC_to_axis(adc_pot1);  // X轴 = 旋钮1
    axis_y = ADC_to_axis(adc_pot2);  // Y轴 = 旋钮2
    
    // ========== 读取4个按钮 ==========
    joy_buttons = 0;
    
    __bit btn1 = !(P3 & (1<<1));  // 按钮1 (P3.1)
    __bit btn2 = !(P3 & (1<<2));  // 按钮2 (P3.2)
    __bit btn3 = !(P1 & (1<<6));  // 按钮3 (P1.6)
    __bit btn4 = !(P1 & (1<<7));  // 按钮4 (P1.7)
    
    if(btn1) joy_buttons |= 0x01;
    if(btn2) joy_buttons |= 0x02;
    if(btn3) joy_buttons |= 0x04;
    if(btn4) joy_buttons |= 0x08;
    
    // LED指示（任何按钮按下时点亮）
    if(joy_buttons) {
      P3 |= (1<<0);
    } else {
      P3 &= ~(1<<0);
    }
    
    // ========== 发送Joystick数据 ==========
    JOY_move(axis_x, axis_y);
    
    if(joy_buttons) {
      JOY_press(joy_buttons);
    } else {
      JOY_release(0xFF);
    }
    
    // ========== 延时和看门狗 ==========
    DLY_ms(10);
    WDT_reset();
  }
}

// ===================================================================================
// MIDI模式主循环
// ===================================================================================
void run_midi_mode(void) {
  uint8_t cc13_last = 0, pitch_last = 128;
  uint8_t btn_state[4] = {0,0,0,0};  // 按钮状态
  uint8_t btn_note_active[2] = {0, 0};  // 音符按钮当前激活的音符
  
  // 音符定义（中央C那一组）
  const uint8_t base_notes[2] = {MIDI_NOTE_C4, MIDI_NOTE_A4};  // C4(60), A4(69)
  
  while(1) {
    // ===== 读取旋钮（CC13和Pitch Bend）=====
    uint8_t adc1 = ADC_read_filtered(ADC_CHANNEL_POT1);
    uint8_t cc13 = ADC_to_MIDI(adc1);
    if(cc13 != cc13_last && (cc13 > cc13_last + 1 || cc13 < cc13_last - 1)) {
      MIDI_send_CC(MIDI_CC13, cc13);  // CC13 (Effect Control 1)
      cc13_last = cc13;
    }
    
    uint8_t adc2 = ADC_read_filtered(ADC_CHANNEL_POT2);
    if(adc2 != pitch_last && (adc2 > pitch_last + 2 || adc2 < pitch_last - 2)) {
      int16_t pitch_value = ADC_to_PitchBend(adc2);
      MIDI_send_PitchBend(pitch_value);
      pitch_last = adc2;
    }
    
    // ===== 读取按钮 =====
    uint8_t btn[4] = {
      !(P3 & (1<<1)),  // 按钮1 (P3.1)
      !(P3 & (1<<2)),  // 按钮2 (P3.2)
      !(P1 & (1<<6)),  // 按钮3 (P1.6) - 功能键+12
      !(P1 & (1<<7))   // 按钮4 (P1.7) - 功能键-12
    };
    
    // ===== 处理音符按钮1和2（带功能键移调）=====
    for(uint8_t i = 0; i < 2; i++) {
      uint8_t note = base_notes[i];
      
      // 根据功能键调整音高
      if(btn[2]) note += 12;  // 按钮3: +12半音
      if(btn[3]) note -= 12;  // 按钮4: -12半音
      
      // 按钮按下
      if(btn[i] && !btn_state[i]) {
        MIDI_send_NoteOn(note, 64);  // 中等力度
        btn_note_active[i] = note;    // 记录当前音符
        btn_state[i] = 1;
      }
      // 按钮松开或功能键改变
      else if(btn_state[i]) {
        if(!btn[i] || btn_note_active[i] != note) {
          MIDI_send_NoteOff(btn_note_active[i]);  // 关闭旧音符
          if(btn[i]) {
            MIDI_send_NoteOn(note, 64);  // 开启新音符
            btn_note_active[i] = note;
          } else {
            btn_state[i] = 0;
          }
        }
      }
    }
    
    // 更新功能键状态
    btn_state[2] = btn[2];
    btn_state[3] = btn[3];
    
    // LED指示（任何按钮按下时点亮）
    if(btn[0] || btn[1] || btn[2] || btn[3]) {
      P3 |= (1<<0);
    } else {
      P3 &= ~(1<<0);
    }
    
    // ========== 延时和看门狗 ==========
    DLY_ms(10);
    WDT_reset();
  }
}

// ===================================================================================
// 主函数
// ===================================================================================
void main(void) {
  // 系统初始化
  CLK_config();
  DLY_ms(10);
  ADC_init();
  
  // 配置LED (P3.0)
  P3_MOD_OC &= ~(1<<0);
  P3_DIR_PU |= (1<<0);
  P3 &= ~(1<<0);
  
  // 配置按钮引脚（内部上拉）
  // P3.1, P3.2
  P3_MOD_OC &= ~((1<<1) | (1<<2));
  P3_DIR_PU |= (1<<1) | (1<<2);
  
  // P1.6, P1.7
  P1_MOD_OC &= ~((1<<6) | (1<<7));
  P1_DIR_PU |= (1<<6) | (1<<7);
  
  // ========== 检测是否进入ISP烧录模式 ==========
  // 如果同时按住按钮3(P1.6)和按钮4(P1.7)上电，则进入ISP模式
  DLY_ms(10);  // 等待按钮稳定
  if(!(P1 & (1<<6)) && !(P1 & (1<<7))) {
    // 两个按钮都按下，进入ISP模式
    // LED快速闪烁10次提示
    for(uint8_t i = 0; i < 10; i++) {
      P3 |= (1<<0);
      DLY_ms(50);
      P3 &= ~(1<<0);
      DLY_ms(50);
    }
    // 跳转到Bootloader
    BOOT_now();
  }
  
  // 检测工作模式
  device_mode = detect_mode();
  
  // 指示当前模式
  indicate_mode(device_mode);
  
  // 初始化USB（根据模式加载不同描述符）
  if(device_mode == MODE_HID) {
    HID_init();    // HID Game Pad描述符
  } else {
    MIDI_init();   // MIDI Audio Class描述符
  }
  
  DLY_ms(500);
  WDT_start();
  
  // 进入对应模式主循环
  if(device_mode == MODE_HID) {
    run_hid_mode();
  } else {
    run_midi_mode();
  }
}

