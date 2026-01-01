#pragma once
#include "pch.h"

namespace winrt::AIChatApp::implementation
{
    class SiliconFlowClient
    {
    public:
        SiliconFlowClient();
        
        // 设置API密钥
        void SetApiKey(const std::wstring& apiKey);
        
        // 发送聊天消息并获取流式响应
        winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> SendChatMessageAsync(
            const std::wstring& message,
            const std::function<void(const std::wstring&)>& onChunkReceived = nullptr);
        
        // 设置模型
        void SetModel(const std::wstring& model);
        
    private:
        std::wstring m_apiKey;
        std::wstring m_baseUrl = L"https://api.siliconflow.cn/v1";
        std::wstring m_model = L"deepseek-ai/DeepSeek-V2.5";
        
        winrt::Windows::Web::Http::HttpClient m_httpClient;
        
        // 构建请求JSON
        winrt::hstring BuildRequestJson(const std::wstring& message);
        
        // 处理流式响应
        winrt::Windows::Foundation::IAsyncOperation<winrt::hstring> ProcessStreamResponseAsync(
            winrt::Windows::Web::Http::HttpResponseMessage response,
            const std::function<void(const std::wstring&)>& onChunkReceived);
    };
}