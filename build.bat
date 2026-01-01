@echo off
echo ========================================
echo    AI聊天助手 - 构建脚本
echo ========================================
echo.

REM 检查是否安装了CMake
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo 错误：未找到CMake，请先安装CMake
    echo 下载地址：https://cmake.org/download/
    pause
    exit /b 1
)

REM 检查是否安装了Visual Studio
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo 警告：未找到Visual Studio编译器，将尝试使用CMake默认配置
)

REM 创建构建目录
if not exist "build" mkdir build
cd build

REM 配置项目
echo 正在配置项目...
cmake .. -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo 错误：CMake配置失败
    pause
    exit /b 1
)

REM 构建项目
echo.
echo 正在构建项目...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo 错误：构建失败
    pause
    exit /b 1
)

echo.
echo ========================================
echo   构建成功！
echo ========================================
echo.
echo 可执行文件位置：build\Release\AIChatApp.exe
echo.
echo 按任意键退出...
pause >nul