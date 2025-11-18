// ===================================================================================
// USB MIDI Communication Functions
// ===================================================================================

#include "ch554.h"
#include "usb_midi.h"
#include "usb_handler.h"

// ===================================================================================
// MIDI初始化函数
// ===================================================================================
void MIDI_init(void) {
  USB_init();           // 初始化USB底层
  UEP1_T_LEN = 0;       // 复位端点1发送长度
}

// ===================================================================================
// MIDI消息发送函数 - 通过USB Bulk IN端点
// ===================================================================================

// 发送4字节MIDI消息到USB Endpoint 1
void MIDI_send_packet(uint8_t *data) {
  // 等待端点空闲
  while((UEP1_CTRL & MASK_UEP_T_RES) == UEP_T_RES_ACK);
  
  // 复制4字节MIDI数据到EP1缓冲区
  // EP1缓冲区地址在初始化时设置
  __xdata uint8_t *ep1_buf = (__xdata uint8_t *)UEP1_DMA;
  ep1_buf[0] = data[0];  // Cable Number + Code Index
  ep1_buf[1] = data[1];  // MIDI Status Byte
  ep1_buf[2] = data[2];  // MIDI Data Byte 1
  ep1_buf[3] = data[3];  // MIDI Data Byte 2
  
  // 设置传输长度为4字节
  UEP1_T_LEN = 4;
  
  // 触发传输
  UEP1_CTRL = UEP1_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_ACK;
}

// ===================================================================================
// MIDI Control Change消息
// ===================================================================================
void MIDI_send_CC(uint8_t cc_number, uint8_t value) {
  uint8_t midi_msg[4] = {
    0x0B,           // Cable 0, Code Index 0xB (Control Change)
    0xB0,           // Control Change on Channel 1
    cc_number,      // CC编号 (1-127)
    value           // 值 (0-127)
  };
  MIDI_send_packet(midi_msg);
}

// ===================================================================================
// MIDI Pitch Bend消息
// ===================================================================================
void MIDI_send_PitchBend(int16_t value) {
  // Pitch Bend范围: -8192到8191
  // 转换为14位无符号值: 0-16383
  uint16_t pitch_value = (uint16_t)(value + 8192);
  
  uint8_t midi_msg[4] = {
    0x0E,                           // Cable 0, Code Index 0xE (Pitch Bend)
    0xE0,                           // Pitch Bend on Channel 1
    (uint8_t)(pitch_value & 0x7F),  // LSB (低7位)
    (uint8_t)(pitch_value >> 7)     // MSB (高7位)
  };
  MIDI_send_packet(midi_msg);
}

// ===================================================================================
// MIDI Note On消息
// ===================================================================================
void MIDI_send_NoteOn(uint8_t note, uint8_t velocity) {
  uint8_t midi_msg[4] = {
    0x09,           // Cable 0, Code Index 0x9 (Note On)
    0x90,           // Note On on Channel 1
    note,           // 音符号 (0-127)
    velocity        // 力度 (1-127)
  };
  MIDI_send_packet(midi_msg);
}

// ===================================================================================
// MIDI Note Off消息
// ===================================================================================
void MIDI_send_NoteOff(uint8_t note) {
  uint8_t midi_msg[4] = {
    0x08,           // Cable 0, Code Index 0x8 (Note Off)
    0x80,           // Note Off on Channel 1
    note,           // 音符号
    0x00            // Release velocity
  };
  MIDI_send_packet(midi_msg);
}

