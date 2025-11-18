// ===================================================================================
// USB MIDI Communication Functions - Header File
// ===================================================================================

#pragma once
#include <stdint.h>

// ===================================================================================
// MIDI初始化
// ===================================================================================
void MIDI_init(void);

// ===================================================================================
// MIDI消息发送函数
// ===================================================================================

// 发送4字节MIDI包
void MIDI_send_packet(uint8_t *data);

// 发送MIDI Control Change消息
void MIDI_send_CC(uint8_t cc_number, uint8_t value);

// 发送MIDI Pitch Bend消息
void MIDI_send_PitchBend(int16_t value);

// 发送MIDI Note On消息
void MIDI_send_NoteOn(uint8_t note, uint8_t velocity);

// 发送MIDI Note Off消息
void MIDI_send_NoteOff(uint8_t note);

