// ===================================================================================
// USB HID Functions - 简化版（仅Game Pad）
// ===================================================================================

#include "usb_composite.h"
#include "usb_hid.h"
#include "usb_handler.h"

#define JOY_sendReport()    HID_sendReport(JOY_report, sizeof(JOY_report))

// ===================================================================================
// HID报告 - 只有Joystick
// ===================================================================================
// 报告格式：[按钮字节, X轴, Y轴]
// - 按钮字节：bit0=按钮1, bit1=按钮2, bit2-7=填充
// - X轴：-127到127 (int8_t)
// - Y轴：-127到127 (int8_t)
__xdata uint8_t JOY_report[] = {0, 0, 0};

// ===================================================================================
// Joystick Functions
// ===================================================================================

// 按下joystick按钮
void JOY_press(uint8_t buttons) {
  JOY_report[0] |= buttons;         // 设置按钮位
  JOY_sendReport();                 // 发送报告
}

// 释放joystick按钮
void JOY_release(uint8_t buttons) {
  JOY_report[0] &= ~buttons;        // 清除按钮位
  JOY_sendReport();                 // 发送报告
}

// 移动joystick轴
void JOY_move(int8_t x_axis, int8_t y_axis) {
  JOY_report[1] = (uint8_t)x_axis;  // X轴
  JOY_report[2] = (uint8_t)y_axis;  // Y轴
  JOY_sendReport();                 // 发送报告
}


