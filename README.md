# AI聊天助手 - WinUI C++应用

这是一个基于WinUI 3的C++ AI聊天应用程序，使用SiliconFlow API进行对话。

## 功能特性

- 🎨 现代化的WinUI 3界面
- 💬 实时流式聊天体验
- 🔑 支持SiliconFlow API密钥配置
- 🤖 支持多种AI模型选择
- ⌨️ 支持快捷键操作（Enter发送，Shift+Enter换行）
- ⚡ 异步网络请求，界面不卡顿

## 系统要求

- Windows 10 版本 1809 (Build 17763) 或更高版本
- Visual Studio 2022 或更高版本
- Windows SDK 版本 10.0.19041.0 或更高版本
- C++/WinRT 支持

## 构建说明

### 方法一：使用CMake（推荐）

1. 确保已安装CMake和Visual Studio 2022
2. 打开命令提示符或PowerShell，导航到项目目录
3. 运行以下命令：

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### 方法二：使用Visual Studio

1. 打开Visual Studio 2022
2. 选择"打开本地文件夹"
3. 导航到项目目录
4. 等待CMake配置完成
5. 按F5构建并运行

## 使用方法

1. **获取API密钥**：
   - 访问 [SiliconFlow官网](https://siliconflow.cn)
   - 注册账号并获取API密钥

2. **配置应用**：
   - 运行应用程序
   - 在顶部输入框中输入您的SiliconFlow API密钥
   - 选择要使用的AI模型（默认DeepSeek-V2.5）

3. **开始聊天**：
   - 在底部输入框中输入消息
   - 按Enter键或点击"发送"按钮
   - 使用Shift+Enter进行换行

## 项目结构

```
src/
├── App.xaml/.cpp/.h          # 应用程序入口和资源
├── MainWindow.xaml/.cpp/.h   # 主窗口界面和逻辑
├── SiliconFlowClient.cpp/.h  # SiliconFlow API客户端
└── pch.h/.cpp                # 预编译头文件
```

## API配置

应用使用SiliconFlow API，支持以下模型：
- `deepseek-ai/DeepSeek-V2.5`
- `deepseek-ai/DeepSeek-V2`
- 其他兼容模型

API端点：`https://api.siliconflow.cn/v1`

## 快捷键

- **Enter**: 发送消息
- **Shift + Enter**: 在输入框中换行
- **Ctrl + Enter**: 发送消息（备用）

## 技术栈

- **UI框架**: WinUI 3 (C++/WinRT)
- **网络库**: Windows.Web.Http
- **JSON处理**: Windows.Data.Json
- **异步编程**: C++/WinRT协程
- **构建工具**: CMake

## 注意事项

1. 请妥善保管您的API密钥，不要泄露给他人
2. 应用需要网络连接才能正常工作
3. 响应时间取决于网络状况和API服务器负载
4. 建议在稳定的网络环境下使用

## 故障排除

### 常见问题

1. **应用无法启动**
   - 确保系统满足最低要求
   - 检查Visual C++运行库是否安装

2. **API调用失败**
   - 验证API密钥是否正确
   - 检查网络连接
   - 确认API服务状态

3. **界面显示异常**
   - 确保Windows版本支持WinUI 3
   - 更新显卡驱动程序

### 调试模式

在Debug模式下运行可以查看详细的错误信息：

```bash
cmake --build . --config Debug
```

## 许可证

本项目仅供学习和演示使用。请遵守SiliconFlow API的使用条款。

## 贡献

欢迎提交Issue和Pull Request来改进这个项目。