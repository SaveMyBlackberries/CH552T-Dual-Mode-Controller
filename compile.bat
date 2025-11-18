@echo off
setlocal

echo ===================================
echo Dual Mode Controller Build Script
echo HID Joystick + MIDI Controller
echo ===================================

REM SDCC compiler flags
set CFLAGS=-mmcs51 --model-small --xram-size 0x0300 --xram-loc 0x0100 --code-size 0x3800 -Isrc -DF_CPU=16000000

REM Source files
set SRC_FILES=dual_mode_controller.c src\usb_composite_simple.c src\delay.c src\usb_handler.c src\usb_descr.c src\usb_descr_midi.c src\usb_hid.c src\usb_midi.c

echo.
echo [1/4] Cleaning old files...
del /F /Q *.rel *.ihx *.hex *.bin *.lk *.map *.mem *.lst *.rst *.sym *.asm *.adb > nul 2>&1
del /F /Q src\*.rel src\*.ihx src\*.hex src\*.bin src\*.lk src\*.map src\*.mem src\*.lst src\*.rst src\*.sym src\*.asm src\*.adb > nul 2>&1

echo [2/4] Compiling source files...
for %%f in (%SRC_FILES%) do (
    echo   - Compiling %%f...
    sdcc -c %CFLAGS% %%f
    if errorlevel 1 (
        echo Compilation failed: %%f
        pause
        exit /b 1
    )
)

echo [3/4] Linking...
sdcc usb_composite_simple.rel delay.rel usb_handler.rel usb_descr.rel usb_descr_midi.rel usb_hid.rel usb_midi.rel dual_mode_controller.rel %CFLAGS% -o dual_mode_controller.ihx
if errorlevel 1 (
    echo Linking failed!
    pause
    exit /b 1
)

echo [4/4] Converting to HEX format...
packihx dual_mode_controller.ihx > dual_mode_controller.hex
if errorlevel 1 (
    echo Conversion failed!
    pause
    exit /b 1
)

echo.
echo ===================================
echo Build successful!
echo Output file: dual_mode_controller.hex
echo Ready for WCH ISP Tool flashing
echo ===================================
echo.
echo Mode Selection:
echo - Power on WITHOUT holding any button = HID Mode (LED blinks 3 times)
echo - Power on WHILE holding Button 1     = MIDI Mode (LED blinks 5 times)
echo - Power on WHILE holding Button 3+4   = ISP Mode (LED blinks 10 times)
echo.
pause


