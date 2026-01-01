#include "pch.h"
#include "MainWindow.xaml.h"
#include "SiliconFlowClient.h"
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::System;

namespace winrt::AIChatApp::implementation
{
    // 消息数据结构
    struct MessageData
    {
        hstring Sender;
        hstring Content;
        hstring Timestamp;
        hstring MessageType; // "user" 或 "ai"
    };

    void MainWindow::InitializeComponent()
    {
        // 空实现，因为我们现在不使用XAML生成器
    }

    MainWindow::MainWindow()
    {
        InitializeComponent();
        
        // 初始化SiliconFlow客户端
        m_siliconFlowClient = std::make_unique<SiliconFlowClient>();
        
        // 设置默认模型
        ModelComboBox().SelectedIndex(0);
        
        // 添加欢迎消息
        AddWelcomeMessage();
    }

    void MainWindow::AddWelcomeMessage()
    {
        MessageData welcomeMessage;
        welcomeMessage.Sender = L"AI助手";
        welcomeMessage.Content = L"欢迎使用AI聊天助手！请在上方输入您的SiliconFlow API密钥，然后开始对话。";
        welcomeMessage.Timestamp = GetCurrentTime();
        welcomeMessage.MessageType = L"ai";
        
        auto messageVector = winrt::single_threaded_observable_vector<IInspectable>();
        messageVector.Append(winrt::box_value(welcomeMessage));
        MessagesItemsControl().ItemsSource(messageVector);
    }

    hstring MainWindow::GetCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::tm tm;
        localtime_s(&tm, &time_t);
        
        std::wstringstream ss;
        ss << std::setw(2) << std::setfill(L'0') << tm.tm_hour << L":" 
           << std::setw(2) << std::setfill(L'0') << tm.tm_min;
        return hstring(ss.str());
    }

    void MainWindow::AddMessage(const hstring& sender, const hstring& content, const hstring& messageType)
    {
        MessageData message;
        message.Sender = sender;
        message.Content = content;
        message.Timestamp = GetCurrentTime();
        message.MessageType = messageType;
        
        auto itemsSource = MessagesItemsControl().ItemsSource().as<IObservableVector<IInspectable>>();
        if (!itemsSource)
        {
            itemsSource = winrt::single_threaded_observable_vector<IInspectable>();
            MessagesItemsControl().ItemsSource(itemsSource);
        }
        
        itemsSource.Append(winrt::box_value(message));
        
        // 滚动到底部
        Dispatcher().RunAsync(CoreDispatcherPriority::Normal, [this]()
        {
            MessageScrollViewer().ChangeView(nullptr, MessageScrollViewer().ScrollableHeight(), nullptr);
        });
    }

    void MainWindow::UpdateAIMessage(const hstring& newContent)
    {
        auto itemsSource = MessagesItemsControl().ItemsSource().as<IObservableVector<IInspectable>>();
        if (itemsSource && itemsSource.Size() > 0)
        {
            auto lastMessage = itemsSource.GetAt(itemsSource.Size() - 1);
            try
            {
                MessageData messageData = winrt::unbox_value<MessageData>(lastMessage);
                if (messageData.MessageType == L"ai")
                {
                    messageData.Content = newContent;
                    itemsSource.SetAt(itemsSource.Size() - 1, winrt::box_value(messageData));
                }
            }
            catch (const winrt::hresult_error&)
            {
                // 如果无法转换，忽略错误
            }
        }
    }

    void MainWindow::ApiKeyTextBox_TextChanged(IInspectable const& sender, TextChangedEventArgs const& e)
    {
        auto textBox = sender.as<TextBox>();
        if (textBox && m_siliconFlowClient)
        {
            m_siliconFlowClient->SetApiKey(textBox.Text().c_str());
        }
    }

    void MainWindow::ModelComboBox_SelectionChanged(IInspectable const& sender, SelectionChangedEventArgs const& e)
    {
        auto comboBox = sender.as<ComboBox>();
        if (comboBox && m_siliconFlowClient)
        {
            auto selectedItem = comboBox.SelectedItem().as<ComboBoxItem>();
            if (selectedItem)
            {
                hstring modelName = selectedItem.Content().as<hstring>();
                
                // 映射模型名称到实际模型ID
                if (modelName == L"DeepSeek-V2.5")
                {
                    m_siliconFlowClient->SetModel(L"deepseek-ai/DeepSeek-V2.5");
                }
                else if (modelName == L"DeepSeek-V2")
                {
                    m_siliconFlowClient->SetModel(L"deepseek-ai/DeepSeek-V2");
                }
                else
                {
                    m_siliconFlowClient->SetModel(L"deepseek-ai/DeepSeek-V2.5");
                }
            }
        }
    }

    void MainWindow::SendButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        SendMessage();
    }

    void MainWindow::MessageTextBox_KeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e)
    {
        if (e.Key() == VirtualKey::Enter)
        {
            // 检查是否按下了Ctrl键（发送消息）或Shift键（换行）
            auto coreWindow = CoreWindow::GetForCurrentThread();
            auto ctrlState = coreWindow.GetKeyState(VirtualKey::Control);
            auto shiftState = coreWindow.GetKeyState(VirtualKey::Shift);
            
            if ((ctrlState & CoreVirtualKeyStates::Down) == CoreVirtualKeyStates::Down)
            {
                // Ctrl+Enter: 发送消息
                e.Handled(true);
                SendMessage();
            }
            else if ((shiftState & CoreVirtualKeyStates::Down) != CoreVirtualKeyStates::Down)
            {
                // Enter（没有Shift）: 发送消息
                e.Handled(true);
                SendMessage();
            }
            // Shift+Enter: 允许换行（不处理事件）
        }
    }

    void MainWindow::SendMessage()
    {
        hstring messageText = MessageTextBox().Text();
        
        if (messageText.empty())
        {
            return;
        }
        
        // 添加用户消息
        AddMessage(L"您", messageText, L"user");
        
        // 清空输入框
        MessageTextBox().Text(L"");
        
        // 添加AI消息占位符
        AddMessage(L"AI助手", L"思考中...", L"ai");
        
        // 显示加载指示器
        LoadingOverlay().Visibility(Visibility::Visible);
        SendButton().IsEnabled(false);
        
        // 异步发送消息到AI
        SendMessageToAIAsync(messageText);
    }

    fire_and_forget MainWindow::SendMessageToAIAsync(hstring message)
    {
        auto lifetime = get_strong();
        
        try
        {
            std::wstring fullResponse;
            
            // 发送消息并处理流式响应
            hstring response = co_await m_siliconFlowClient->SendChatMessageAsync(
                message.c_str(),
                [this, &fullResponse](const std::wstring& chunk)
                {
                    fullResponse += chunk;
                    
                    // 在UI线程中更新消息
                    Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [this, fullResponse]()
                    {
                        UpdateAIMessage(fullResponse.c_str());
                    });
                }
            );
            
            // 隐藏加载指示器
        Dispatcher().RunAsync(CoreDispatcherPriority::Normal, [this]()
        {
            LoadingOverlay().Visibility(Visibility::Collapsed);
            SendButton().IsEnabled(true);
        });
    }
    catch (const hresult_error& ex)
    {
        // 错误处理
        Dispatcher().RunAsync(CoreDispatcherPriority::Normal, [this, ex]()
        {
            LoadingOverlay().Visibility(Visibility::Collapsed);
            SendButton().IsEnabled(true);
            UpdateAIMessage(L"错误：" + ex.message());
        });
    }
    }
}