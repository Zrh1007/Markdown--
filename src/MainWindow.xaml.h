#pragma once

#include "pch.h"
#include "SiliconFlowClient.h"

namespace winrt::AIChatApp::implementation
{
    struct MainWindow : winrt::Windows::UI::Xaml::Controls::PageT<MainWindow>
    {
        MainWindow();

        // UI控件
        TextBox ApiKeyTextBox() { return m_apiKeyTextBox; }
        ComboBox ModelComboBox() { return m_modelComboBox; }
        ScrollViewer MessageScrollViewer() { return m_messageScrollViewer; }
        ItemsControl MessagesItemsControl() { return m_messagesItemsControl; }
        TextBox MessageTextBox() { return m_messageTextBox; }
        Button SendButton() { return m_sendButton; }
        Border LoadingOverlay() { return m_loadingOverlay; }

        // 事件处理程序
        void ApiKeyTextBox_TextChanged(IInspectable const& sender, Controls::TextChangedEventArgs const& e);
        void ModelComboBox_SelectionChanged(IInspectable const& sender, Controls::SelectionChangedEventArgs const& e);
        void SendButton_Click(IInspectable const& sender, RoutedEventArgs const& e);
        void MessageTextBox_KeyDown(IInspectable const& sender, Input::KeyRoutedEventArgs const& e);

        void InitializeComponent();

    private:
        // UI控件
        TextBox m_apiKeyTextBox{ nullptr };
        ComboBox m_modelComboBox{ nullptr };
        ScrollViewer m_messageScrollViewer{ nullptr };
        ItemsControl m_messagesItemsControl{ nullptr };
        TextBox m_messageTextBox{ nullptr };
        Button m_sendButton{ nullptr };
        Border m_loadingOverlay{ nullptr };

        // SiliconFlow客户端
        std::unique_ptr<SiliconFlowClient> m_siliconFlowClient;

        // 辅助方法
        void AddWelcomeMessage();
        void AddMessage(hstring const& sender, hstring const& content, hstring const& messageType);
        void UpdateAIMessage(hstring const& newContent);
        hstring GetCurrentTime();
        void SendMessage();
        IAsyncAction SendMessageToAIAsync(hstring message);
    };
}

namespace winrt::AIChatApp::factory_implementation
{
    struct MainWindow : winrt::Windows::UI::Xaml::Controls::PageT<MainWindow, implementation::MainWindow>
    {
    };
}