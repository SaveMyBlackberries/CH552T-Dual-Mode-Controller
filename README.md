这是一个忠实于你原版格式的英文 README 文件。我保留了所有的标题、列表、表格和代码块结构，并准确地翻译了技术内容。

-----

# CH552T Dual-Mode Controller - HID Joystick + MIDI Controller

**One Board, Two Modes\!**
Freely switch between HID Flight Controller and MIDI Controller modes by selecting a button combination during power-up.

-----

## 📋 Table of Contents

  - [Hardware Connection](https://www.google.com/search?q=%23hardware-connection)
  - [Features](https://www.google.com/search?q=%23features)
  - [Mode Switching](https://www.google.com/search?q=%23mode-switching)
  - [HID Mode Description](https://www.google.com/search?q=%23hid-mode-description)
  - [MIDI Mode Description](https://www.google.com/search?q=%23midi-mode-description)
  - [Compilation and Flashing](https://www.google.com/search?q=%23compilation-and-flashing)
  - [Usage Instructions](https://www.google.com/search?q=%23usage-instructions)
  - [Troubleshooting](https://www.google.com/search?q=%23troubleshooting)

-----

## 🔌 Hardware Connection

### Development Board Information

  - **Model**: CH552T SuperMini USB Development Board
  - **Chip**: CH552T (8051 Core, USB HID Support)
  - **Clock**: 16MHz Internal Oscillator

### Pin Connection Table (Unified Layout)

| Function | Pin | ADC Channel | HID Mode Function | MIDI Mode Function |
|:---------|:----|:------------|:------------------|:-------------------|
| **Knob 1/Pot 1** | P1.4 | AIN1 | Throttle Axis (Z-Axis) | CC13 (Effect Control 1) |
| **Knob 2/Pot 2** | P1.1 | AIN0 | Reverse Thrust Axis (Slider) | Pitch Bend |
| **Button 1** | P3.1 | - | HID Button 1 / **Mode Select** | MIDI Note C4 (60, Middle C) / **Mode Select** |
| **Button 2** | P3.2 | - | HID Button 2 | MIDI Note A4 (69) |
| **Button 3** | P1.6 | - | HID Button 3 | Function Key (+12 Semitones) |
| **Button 4** | P1.7 | - | HID Button 4 | Function Key (-12 Semitones) |
| **LED Indicator** | P3.0 | - | Fast Flash 3 Times | Slow Flash 5 Times |

**Mode Selection Instructions**:

  - **Hold Button 1 on Power-Up** → MIDI Mode (LED flashes slowly 5 times)
  - **No Button Pressed on Power-Up** → HID Mode (LED flashes quickly 3 times)
  - **Hold Button 3 + 4 on Power-Up** → ISP Flashing Mode (LED flashes quickly 10 times)

### Wiring Guide

**Potentiometer/Knob Wiring:**

```
Knob Pins:
┌─────────┐
│  VCC    │ ← 5V or 3.3V
│  Signal  │ ← Connect to P1.1 or P1.4
│  GND    │ ← GND
└─────────┘
```

**Button Wiring:**

  - One end connects to the corresponding pin (P3.1, P3.2, P1.6, P1.7)
  - The other end connects to **GND**
  - Uses internal pull-up, low-level active

-----

## ✨ Features

### Dual-Mode Design

  - **HID Mode**: USB Game Pad (Game Controller)
      - 2 Analog Axes (Z-Axis, Slider)
      - 4 Buttons
      - 100Hz Refresh Rate
      - Driverless, Plug-and-Play
     
  - **MIDI Mode**: USB MIDI Controller
      - CC13 Control (Effect Control 1)
      - Pitch Bend (-8192 to 8191)
      - 2 Note Keys (Middle C4 and A4) + 2 Function Keys
      - Function keys for Transposing $\pm 12$ semitones (One Octave)
      - ADC Calibration Mapping for full knob travel

### Filtering System

  - **16x Average Filtering**: Reduces ADC noise
  - **$50\mu s$ Sampling Interval**: Ensures ADC stability
  - **Response Latency**: $<5ms$

### LED Indicator

  - **HID Mode**: Flashes quickly 3 times on startup
  - **MIDI Mode**: Flashes slowly 5 times on startup
  - **During Operation**: Lights up when any button is pressed

-----

## 🔄 Mode Switching

### Switching Method (Power-Up Button Detection)

```
No Button Pressed + Power-Up → HID Mode
┌────────────────────────────────────────┐
│ 1. Ensure all buttons are released       │
│ 2. Plug in the USB cable                 │
│ 3. LED flashes quickly 3 times → HID Mode Active  │
└────────────────────────────────────────┘

Hold Button 1 + Power-Up → MIDI Mode
┌────────────────────────────────────────┐
│ 1. Hold Button 1 (P3.1)                │
│ 2. Plug in the USB cable                 │
│ 3. Wait for LED to flash slowly 5 times  │
│ 4. Release Button 1 → MIDI Mode Active   │
└────────────────────────────────────────┘

Hold Button 3+4 + Power-Up → ISP Flashing Mode
┌────────────────────────────────────────┐
│ 1. Hold both Button 3 (P1.6) + Button 4 (P1.7)│
│ 2. Plug in the USB cable                 │
│ 3. Wait for LED to flash quickly 10 times│
│ 4. Release buttons → Enters ISP Mode     │
└────────────────────────────────────────┘
```

**Note**: The mode is determined at power-up and cannot be changed during runtime. To switch modes, unplug the USB and re-power the device.

-----

## 🎮 HID Mode Description

### Function Mapping

| Hardware | HID Function | Example Use Case |
|:---------|:-------------|:-----------------|
| Knob 1 (P1.4) | Z-Axis | Microsoft Flight Simulator - Throttle |
| Knob 2 (P1.1) | Slider Axis | Microsoft Flight Simulator - Reverse Thrust |
| Button 1 (P3.1) | Button 1 | AT Disengage (Autothrottle Disconnect) |
| Button 2 (P3.2) | Button 2 | TO/GA (Takeoff/Go-Around) |
| Button 3 (P1.6) | Button 3 | Custom Function |
| Button 4 (P1.7) | Button 4 | Custom Function |

### Windows Configuration

1.  **Open Control Panel**
       - Press `Win+R`, type `joy.cpl`, press Enter

2.  **Calibrate Axes**
       - Select Device → Properties → Settings → Calibrate
       - Calibrate the **Z-Axis** and **Slider Axis** separately

3.  **Test**
       - Check axis and button responses in the Test tab

-----

## 🎹 MIDI Mode Description

### Function Mapping

| Hardware | MIDI Function | MIDI Message | Description |
|:---------|:--------------|:-------------|:------------|
| Knob 1 (P1.4) | CC13 | Control Change 13 | Effect Control 1 (Range 0-127, Calibrated for full travel) |
| Knob 2 (P1.1) | Pitch Bend | Pitch Bend | Range (-8192 to 8191, Calibrated for full travel) |
| Button 1 (P3.1) | Note C4 | Note 60 | Press alone: C4 (Middle C)<br>Button 3 + Button 1: C5 (72)<br>Button 4 + Button 1: C3 (48) |
| Button 2 (P3.2) | Note A4 | Note 69 | Press alone: A4<br>Button 3 + Button 2: A5 (81)<br>Button 4 + Button 2: A3 (57) |
| Button 3 (P1.6) | Function Key | - | While held: Transposes notes $+12$ semitones (One Octave Up) |
| Button 4 (P1.7) | Function Key | - | While held: Transposes notes $-12$ semitones (One Octave Down) |

### Function Key Logic

```
Button 1 Pressed Alone    → Send Note C4 (60, Middle C)
Button 3 + Button 1       → Send Note C5 (72) [+12 Semitones]
Button 4 + Button 1       → Send Note C3 (48) [-12 Semitones]

Button 2 Pressed Alone    → Send Note A4 (69)
Button 3 + Button 2       → Send Note A5 (81) [+12 Semitones]
Button 4 + Button 2       → Send Note A3 (57) [-12 Semitones]
```

### ADC Calibration Technical Details

Due to the physical characteristics of the potentiometers, the actual ADC reading range often does not reach the full theoretical 0-255 range.

**Measured Data for this Project**:

  - **Actual ADC Range**: 0-172 (instead of 0-255)
  - **ADC Center Value**: 86
  - **Corresponding MIDI Range**: CC 0-86 (Uncalibrated)

**Calibration Mapping Algorithm**:

```c
// CC13 Mapping (0-127 Full Travel)
CC13 = adc_value * 127 / 172

// Pitch Bend Mapping (-8192 to +8191 Full Travel)
Pitch Bend = (adc_value - 86) * 8192 / 86
```

**Calibration Effect**:

  - ✅ CC13 Full Travel: 0 $\rightarrow$ 127
  - ✅ Pitch Bend Full Travel: -8192 $\rightarrow$ +8191
  - ✅ Pitch Bend Exact Center: ADC=86 $\rightarrow$ 0

**Note**: If you use potentiometers with different specifications, you may need to modify the mapping parameters in `dual_mode_controller.c`.

-----

### DAW Software Configuration

#### Ableton Live

1.  **Preferences** → **MIDI**
2.  Enable "Dual Mode Controller" in the Input Devices section
3.  Set Track → In From → "Dual Mode Controller"

#### FL Studio

1.  **Options** → **MIDI Settings**
2.  Select "Dual Mode Controller" under Input
3.  Enable "Enable"

#### Cubase

1.  **Studio** → **Studio Setup** → **MIDI Port Setup**
2.  Add "Dual Mode Controller"

-----

## 🔨 Compilation and Flashing

### Compilation Environment Requirements

  - **Compiler**: SDCC (Small Device C Compiler) 4.x
  - **Tool**: `packihx` (Included with SDCC)
  - **System**: Windows (CMD or PowerShell)

### Compilation Steps

1.  **Install SDCC**
       - Download: [https://sdcc.sourceforge.net/](https://sdcc.sourceforge.net/)
       - Ensure `sdcc` is in your system's PATH

2.  **Compile**
       `cmd    cd MIDI_Joystick    compile.bat    `

3.  **Compilation Output**
       `     ===================================    Dual Mode Controller Build Script    ===================================        [1/4] Cleaning old files...    [2/4] Compiling source files...    [3/4] Linking...    [4/4] Converting to HEX format...        ===================================    Build successful!    Output file: dual_mode_controller.hex    ===================================     `

### Flashing Steps

#### Method 1: Software Entry to ISP Mode ⭐ (Recommended, no need to open the case)

1.  **Hold Button 3 and Button 4 simultaneously** (P1.6 + P1.7)
2.  **Keep holding, plug in the USB cable**
3.  **Wait for the LED to flash quickly 10 times** (Indicates ISP Mode entry)
4.  **Release buttons**
5.  Device Manager should show "USB Module"

#### Method 2: Hardware Button Entry to ISP Mode

1.  **Hold the P36 button** (On-board button)
2.  **Plug in the USB cable**
3.  **Release P36**
4.  Device Manager should show "USB Module"

#### Flashing the Firmware

1.  **Use WCH ISP Tool to Flash**
       - Open WCH ISP Tool
       - Chip: CH552
       - File: `dual_mode_controller.hex`
       - Click "Download"

2.  **Verification**
       - Unplug and re-plug the USB
       - No buttons pressed: LED flashes quickly 3 times (HID Mode)
       - Hold Button 1: LED flashes slowly 5 times (MIDI Mode)
       - Hold Button 3+4: LED flashes quickly 10 times (ISP Mode)

-----

## 📖 Usage Instructions

### HID Mode Usage

1.  **Power up with no buttons pressed** $\rightarrow$ Enters HID Mode
2.  LED flashes quickly 3 times for confirmation
3.  Windows automatically recognizes it as a game controller
4.  Open `joy.cpl` for calibration and testing
5.  Configure axes and buttons in your game/simulator

### MIDI Mode Usage

1.  **Hold Button 1 on power-up** $\rightarrow$ Enters MIDI Mode

2.  LED flashes slowly 5 times for confirmation

3.  Windows recognizes it as a MIDI device

4.  **Method A: Use the Test Tool (Recommended)** ⭐
       `bash    # Install dependencies    pip install -r requirements.txt        # Start the test tool    python midi_tester.py    `
       - Automatic device detection
       - Real-time display of CC13 and Pitch Bend
       - Displays note key presses
       - Plays Beep sounds
       - Detailed MIDI log
       
       **For detailed instructions, see**: `MIDI_TESTER_README.md`

5.  **Method B: Use DAW Software**
       - Open your DAW (e.g., Ableton Live)
       - Enable the device in the MIDI settings
       - Use the knobs to control CC and Pitch Bend
       - Use the buttons to play notes

-----

## 🔧 Troubleshooting

### Mode Switching Issues

| Issue | Cause | Solution |
|:------|:------|:----------|
| LED does not flash | Firmware not flashed | Check if flashing was successful |
| Cannot switch modes | Button 1 wiring error | Check P3.1 connection and ensure pull-up is working |
| LED flashes incorrect number of times | Incorrect button state detection | Re-power, ensure correct button timing |

### HID Mode Issues

| Issue | Cause | Solution |
|:------|:------|:----------|
| Axes unresponsive | ADC pins not connected | Check P1.1 and P1.4 wiring |
| Buttons not working | Incorrect pins | Check P3.1, P3.2, P1.6, P1.7 |
| Device not recognized | Driver issue | Re-plug the USB |

### MIDI Mode Issues

| Issue | Cause | Solution |
|:------|:------|:----------|
| DAW unresponsive | Device not enabled | Enable the device in the DAW's MIDI settings |
| Notes not sounding | Velocity is 0 | Check MIDI\_VELOCITY setting in code |
| CC value not changing | Threshold too large | Adjust ADC threshold |

-----

## 🚀 Technical Details

### Code Structure

```
MIDI_Joystick/
├── dual_mode_controller.c   # Main Program (Dual-mode logic)
├── compile.bat               # Compilation Script
├── src/
│   ├── config.h              # Configuration file (Pins, mode definitions)
│   ├── usb_descr.c           # USB Descriptor (4-button HID)
│   ├── usb_composite_simple.c # HID functions
│   ├── usb_hid.c             # HID low-level
│   ├── usb_handler.c         # USB event handling
│   ├── delay.c               # Delay functions
│   ├── ch554.h               # Chip Register definitions
│   ├── gpio.h                # GPIO macros
│   └── system.h              # System functions
└── README.md                 # This document
```

### Main Function Modules

```c
// Mode Detection
uint8_t detect_mode(void)      // Detects P3.4 status on power-up

// LED Indication
void indicate_mode(uint8_t mode) // Flashes LED according to mode

// HID Mode Main Loop
void run_hid_mode(void)        // 2 Knobs + 4 Buttons → USB HID

// MIDI Mode Main Loop
void run_midi_mode(void)       // CC13 + PitchBend + Notes + Function Keys → USB MIDI
```

-----

## 📝 Version History

### v1.0 (2025-11-18)

  - ✅ Initial Release
  - ✅ **Dual-Mode Support**: HID Game Controller + USB MIDI Controller
  - ✅ **HID Mode**: 2 Knobs (Z-Axis/Slider) + 4 Buttons
  - ✅ **MIDI Mode**:
      - CC13 (Effect Control 1) - Full travel 0-127
      - Pitch Bend - Full travel -8192 to +8191
      - 2 Note Keys (C4/A4, Middle C group)
      - 2 Function Keys (Octave Transpose $\pm 12$ Semitones)
      - ADC Calibration Algorithm to solve incomplete potentiometer travel
  - ✅ **Mode Switching**:
      - Button 1 on Power-Up $\rightarrow$ MIDI Mode
      - No Button on Power-Up $\rightarrow$ HID Mode
      - Button 3+4 on Power-Up $\rightarrow$ ISP Flashing Mode
  - ✅ **Performance Optimization**:
      - 16x Average Filtering for noise reduction
      - 100Hz Refresh Rate
      - $<5ms$ Response Latency
  - ✅ **Development Tool**: Python MIDI Test Tool
  - ✅ **Cross-Platform Support**: Windows/macOS/Linux

-----

## 📄 License

This project is based on the MacroPad Plus project and follows its original license.

-----

**Project Completion Date**: 2025-11-18  
**Development Board**: CH552T SuperMini USB  
**Supported Platforms**: Windows 10/11

-----

*Happy Making\! 🎉*