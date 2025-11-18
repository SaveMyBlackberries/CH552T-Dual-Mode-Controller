#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
MIDI控制器测试工具
用于测试双模式控制器的MIDI功能
"""

import tkinter as tk
from tkinter import ttk, scrolledtext
import pygame.midi
import threading
import time
import winsound
from datetime import datetime

class MIDITester:
    def __init__(self, root):
        self.root = root
        self.root.title("MIDI控制器测试工具")
        self.root.geometry("800x600")
        self.root.configure(bg='#2b2b2b')
        
        # MIDI变量
        self.midi_in = None
        self.midi_running = False
        self.selected_device = None
        
        # 当前值
        self.cc13_value = 0
        self.pitch_bend_value = 0
        self.active_notes = set()
        
        # 音符名称映射（中央C那一组）
        self.note_names = {
            48: "C3", 57: "A3",      # -12半音
            60: "C4", 69: "A4",      # 基础音符（中央C）
            72: "C5", 81: "A5"       # +12半音
        }
        
        # 音符频率映射（用于播放）
        self.note_freqs = {
            48: 131, 57: 220,        # C3, A3
            60: 262, 69: 440,        # C4 (中央C), A4
            72: 523, 81: 880         # C5, A5
        }
        
        self.setup_ui()
        self.init_midi()
        
    def setup_ui(self):
        """设置界面"""
        # 标题
        title = tk.Label(self.root, text="🎹 MIDI控制器测试工具", 
                        font=("Arial", 20, "bold"), bg='#2b2b2b', fg='white')
        title.pack(pady=10)
        
        # 设备选择框
        device_frame = tk.Frame(self.root, bg='#2b2b2b')
        device_frame.pack(pady=10)
        
        tk.Label(device_frame, text="MIDI设备:", bg='#2b2b2b', fg='white',
                font=("Arial", 12)).pack(side=tk.LEFT, padx=5)
        
        self.device_combo = ttk.Combobox(device_frame, width=40, state='readonly')
        self.device_combo.pack(side=tk.LEFT, padx=5)
        self.device_combo.bind('<<ComboboxSelected>>', self.on_device_selected)
        
        refresh_btn = tk.Button(device_frame, text="刷新", command=self.refresh_devices,
                               bg='#4CAF50', fg='white', font=("Arial", 10))
        refresh_btn.pack(side=tk.LEFT, padx=5)
        
        # 主显示区域
        main_frame = tk.Frame(self.root, bg='#2b2b2b')
        main_frame.pack(pady=10, fill=tk.BOTH, expand=True, padx=20)
        
        # 左侧：CC和Pitch Bend
        left_frame = tk.Frame(main_frame, bg='#3b3b3b', relief=tk.RAISED, bd=2)
        left_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=5)
        
        # CC13显示
        cc_frame = tk.LabelFrame(left_frame, text=" CC13 (Effect Control 1) ", 
                                bg='#3b3b3b', fg='white', font=("Arial", 12, "bold"))
        cc_frame.pack(pady=10, padx=10, fill=tk.X)
        
        self.cc13_canvas = tk.Canvas(cc_frame, width=300, height=40, bg='#1b1b1b', 
                                   highlightthickness=0)
        self.cc13_canvas.pack(pady=10, padx=10)
        
        self.cc13_label = tk.Label(cc_frame, text="0", font=("Arial", 24, "bold"),
                                 bg='#3b3b3b', fg='#4CAF50')
        self.cc13_label.pack(pady=5)
        
        # Pitch Bend显示
        pb_frame = tk.LabelFrame(left_frame, text=" Pitch Bend (弯音轮) ", 
                                bg='#3b3b3b', fg='white', font=("Arial", 12, "bold"))
        pb_frame.pack(pady=10, padx=10, fill=tk.X)
        
        self.pb_canvas = tk.Canvas(pb_frame, width=300, height=40, bg='#1b1b1b',
                                  highlightthickness=0)
        self.pb_canvas.pack(pady=10, padx=10)
        
        self.pb_label = tk.Label(pb_frame, text="0", font=("Arial", 24, "bold"),
                                bg='#3b3b3b', fg='#2196F3')
        self.pb_label.pack(pady=5)
        
        # 右侧：音符显示
        right_frame = tk.Frame(main_frame, bg='#3b3b3b', relief=tk.RAISED, bd=2)
        right_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=5)
        
        note_frame = tk.LabelFrame(right_frame, text=" 音符 ", 
                                  bg='#3b3b3b', fg='white', font=("Arial", 12, "bold"))
        note_frame.pack(pady=10, padx=10, fill=tk.BOTH, expand=True)
        
        # 音符按钮（显示激活状态）
        self.note_buttons = {}
        notes = [
            ("C0 (24)", 24), ("A0 (21)", 21),
            ("C1 (36)", 36), ("A1 (33)", 33),
            ("C2 (48)", 48), ("A2 (45)", 45)
        ]
        
        for i, (name, note_num) in enumerate(notes):
            btn = tk.Button(note_frame, text=name, font=("Arial", 14, "bold"),
                          bg='#5b5b5b', fg='white', width=15, height=2,
                          relief=tk.RAISED, state=tk.DISABLED)
            btn.pack(pady=5, padx=10)
            self.note_buttons[note_num] = btn
        
        # 底部：日志区域
        log_frame = tk.LabelFrame(self.root, text=" MIDI消息日志 ", 
                                 bg='#2b2b2b', fg='white', font=("Arial", 10))
        log_frame.pack(pady=10, padx=20, fill=tk.BOTH, expand=True)
        
        self.log_text = scrolledtext.ScrolledText(log_frame, height=8, 
                                                  bg='#1b1b1b', fg='#00ff00',
                                                  font=("Consolas", 10))
        self.log_text.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
    def init_midi(self):
        """初始化MIDI"""
        pygame.midi.init()
        self.refresh_devices()
        
    def refresh_devices(self):
        """刷新MIDI设备列表"""
        devices = []
        for i in range(pygame.midi.get_count()):
            info = pygame.midi.get_device_info(i)
            # info = (interface, name, is_input, is_output, opened)
            if info[2]:  # is_input
                device_name = info[1].decode('utf-8', errors='ignore')
                devices.append((i, device_name))
        
        self.device_combo['values'] = [f"{i}: {name}" for i, name in devices]
        if devices:
            self.device_combo.current(0)
            self.on_device_selected(None)
        
    def on_device_selected(self, event):
        """设备选择事件"""
        if self.midi_running:
            self.stop_midi()
        
        selection = self.device_combo.get()
        if selection:
            device_id = int(selection.split(':')[0])
            self.selected_device = device_id
            self.start_midi()
            self.log(f"✓ 已连接到设备: {selection}")
        
    def start_midi(self):
        """启动MIDI监听"""
        if self.selected_device is None:
            return
        
        try:
            self.midi_in = pygame.midi.Input(self.selected_device)
            self.midi_running = True
            
            # 启动MIDI读取线程
            self.midi_thread = threading.Thread(target=self.midi_loop, daemon=True)
            self.midi_thread.start()
            
        except Exception as e:
            self.log(f"✗ 错误: {str(e)}")
        
    def stop_midi(self):
        """停止MIDI监听"""
        self.midi_running = False
        if self.midi_in:
            self.midi_in.close()
            self.midi_in = None
        
    def midi_loop(self):
        """MIDI消息读取循环"""
        while self.midi_running:
            if self.midi_in and self.midi_in.poll():
                midi_events = self.midi_in.read(10)
                for event in midi_events:
                    self.process_midi_event(event)
            time.sleep(0.01)
        
    def process_midi_event(self, event):
        """处理MIDI事件"""
        status, data1, data2, data3 = event[0]
        
        # Control Change (CC)
        if status == 0xB0:  # CC on channel 1
            if data1 == 13:  # CC13
                self.update_cc13(data2)
                self.log(f"CC13: {data2}")
        
        # Pitch Bend
        elif status == 0xE0:  # Pitch Bend on channel 1
            value = (data2 << 7) | data1  # 14-bit value
            value_signed = value - 8192   # Convert to -8192 to 8191
            self.update_pitch_bend(value_signed)
            self.log(f"Pitch Bend: {value_signed}")
        
        # Note On
        elif status == 0x90:  # Note On on channel 1
            if data2 > 0:  # velocity > 0
                self.note_on(data1, data2)
                note_name = self.note_names.get(data1, f"#{data1}")
                self.log(f"♪ Note On: {note_name} ({data1}), Velocity: {data2}")
            else:
                self.note_off(data1)
                note_name = self.note_names.get(data1, f"#{data1}")
                self.log(f"♪ Note Off: {note_name} ({data1})")
        
        # Note Off
        elif status == 0x80:  # Note Off on channel 1
            self.note_off(data1)
            note_name = self.note_names.get(data1, f"#{data1}")
            self.log(f"♪ Note Off: {note_name} ({data1})")
        
    def update_cc13(self, value):
        """更新CC13显示"""
        self.cc13_value = value
        self.root.after(0, self._draw_cc13)
    
    def _draw_cc13(self):
        """绘制CC13进度条"""
        canvas = self.cc13_canvas
        canvas.delete("all")
        
        width = 300
        height = 40
        bar_width = int((self.cc13_value / 127) * width)
        
        # 背景
        canvas.create_rectangle(0, 0, width, height, fill='#1b1b1b', outline='')
        
        # 进度条
        if bar_width > 0:
            canvas.create_rectangle(0, 0, bar_width, height, 
                                  fill='#4CAF50', outline='')
        
        # 刻度线
        for i in range(0, 128, 16):
            x = int((i / 127) * width)
            canvas.create_line(x, 0, x, height, fill='#3b3b3b', width=1)
        
        # 边框
        canvas.create_rectangle(0, 0, width-1, height-1, outline='#4CAF50', width=2)
        
        # 更新数值
        self.cc13_label.config(text=str(self.cc13_value))
        
    def update_pitch_bend(self, value):
        """更新Pitch Bend显示"""
        self.pitch_bend_value = value
        self.root.after(0, self._draw_pitch_bend)
        
    def _draw_pitch_bend(self):
        """绘制Pitch Bend进度条"""
        canvas = self.pb_canvas
        canvas.delete("all")
        
        width = 300
        height = 40
        center = width // 2
        
        # 背景
        canvas.create_rectangle(0, 0, width, height, fill='#1b1b1b', outline='')
        
        # 中心线
        canvas.create_line(center, 0, center, height, fill='#5b5b5b', width=2)
        
        # 进度条
        value_scaled = int((self.pitch_bend_value / 8192) * center)
        if value_scaled > 0:
            canvas.create_rectangle(center, 0, center + value_scaled, height,
                                  fill='#2196F3', outline='')
        elif value_scaled < 0:
            canvas.create_rectangle(center + value_scaled, 0, center, height,
                                  fill='#FF5722', outline='')
        
        # 边框
        canvas.create_rectangle(0, 0, width-1, height-1, outline='#2196F3', width=2)
        
        # 更新数值
        self.pb_label.config(text=str(self.pitch_bend_value))
        
    def note_on(self, note, velocity):
        """音符按下"""
        self.active_notes.add(note)
        self.root.after(0, lambda: self._update_note_button(note, True))
        
        # 播放音符
        if note in self.note_freqs:
            threading.Thread(target=self._play_beep, 
                           args=(self.note_freqs[note],), daemon=True).start()
        
    def note_off(self, note):
        """音符释放"""
        if note in self.active_notes:
            self.active_notes.remove(note)
        self.root.after(0, lambda: self._update_note_button(note, False))
        
    def _update_note_button(self, note, active):
        """更新音符按钮状态"""
        if note in self.note_buttons:
            btn = self.note_buttons[note]
            if active:
                btn.config(bg='#FF9800', relief=tk.SUNKEN)
            else:
                btn.config(bg='#5b5b5b', relief=tk.RAISED)
        
    def _play_beep(self, frequency, duration=200):
        """播放Beep音"""
        try:
            winsound.Beep(int(frequency), duration)
        except:
            pass  # 如果播放失败就忽略
        
    def log(self, message):
        """添加日志"""
        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
        log_message = f"[{timestamp}] {message}\n"
        self.root.after(0, lambda: self._append_log(log_message))
        
    def _append_log(self, message):
        """追加日志到文本框"""
        self.log_text.insert(tk.END, message)
        self.log_text.see(tk.END)
        
        # 限制日志行数
        lines = int(self.log_text.index('end-1c').split('.')[0])
        if lines > 100:
            self.log_text.delete('1.0', '2.0')
        
    def on_closing(self):
        """关闭窗口"""
        self.stop_midi()
        pygame.midi.quit()
        self.root.destroy()

def main():
    root = tk.Tk()
    app = MIDITester(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()

if __name__ == "__main__":
    main()

