@echo off
echo ===================================
echo   MIDI控制器测试工具启动器
echo ===================================
echo.

REM 检查Python是否安装
python --version >nul 2>&1
if errorlevel 1 (
    echo [错误] 未检测到Python！
    echo 请先安装Python: https://www.python.org/downloads/
    echo.
    pause
    exit /b 1
)

REM 检查pygame是否安装
python -c "import pygame" >nul 2>&1
if errorlevel 1 (
    echo [警告] 未检测到pygame库
    echo 正在安装依赖...
    echo.
    pip install -r requirements.txt
    if errorlevel 1 (
        echo.
        echo [错误] 依赖安装失败！
        echo 请手动运行: pip install pygame
        echo.
        pause
        exit /b 1
    )
    echo.
    echo [成功] 依赖安装完成！
    echo.
)

echo 启动MIDI测试工具...
echo.
python midi_tester.py

if errorlevel 1 (
    echo.
    echo [错误] 程序运行出错
    pause
)

