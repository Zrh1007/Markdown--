#include "pch.h"
#include "SiliconFlowClient.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace Windows::Storage::Streams;
using namespace Windows::Data::Json;
using namespace std;

namespace winrt::AIChatApp::implementation
{
    SiliconFlowClient::SiliconFlowClient()
    {
        // 配置HTTP客户端
        m_httpClient.DefaultRequestHeaders().Append(L"Content-Type", L"application/json");
    }
    
    void SiliconFlowClient::SetApiKey(const std::wstring& apiKey)
    {
        m_apiKey = apiKey;
        
        // 更新HTTP客户端头信息
        m_httpClient.DefaultRequestHeaders().Remove(L"Authorization");
        if (!m_apiKey.empty())
        {
            std::wstring authHeader = L"Bearer " + m_apiKey;
            m_httpClient.DefaultRequestHeaders().Append(L"Authorization", authHeader);
        }
    }
    
    void SiliconFlowClient::SetModel(const std::wstring& model)
    {
        m_model = model;
    }
    
    hstring SiliconFlowClient::BuildRequestJson(const std::wstring& message)
    {
        JsonObject requestJson;
        requestJson.Insert(L"model", JsonValue::CreateStringValue(winrt::to_hstring(m_model)));
        requestJson.Insert(L"stream", JsonValue::CreateBooleanValue(true));
        
        JsonArray messagesArray;
        JsonObject userMessage;
        userMessage.Insert(L"role", JsonValue::CreateStringValue(L"user"));
        userMessage.Insert(L"content", JsonValue::CreateStringValue(winrt::to_hstring(message)));
        messagesArray.Append(userMessage);
        
        requestJson.Insert(L"messages", messagesArray);
        
        return requestJson.Stringify();
    }
    
    IAsyncOperation<hstring> SiliconFlowClient::SendChatMessageAsync(
        const std::wstring& message,
        const std::function<void(const std::wstring&)>& onChunkReceived)
    {
        if (m_apiKey.empty())
        {
            co_return L"错误：请先设置API密钥";
        }
        
        try
        {
            // 构建请求
            hstring requestBody = BuildRequestJson(message);
            Uri uri(winrt::to_hstring(m_baseUrl + L"/chat/completions"));
            
            HttpRequestMessage request;
            request.Method(HttpMethod::Post());
            request.RequestUri(uri);
            
            HttpStringContent content(requestBody, UnicodeEncoding::Utf8, L"application/json");
            request.Content(content);
            
            // 发送请求
            HttpResponseMessage response = co_await m_httpClient.SendRequestAsync(request, HttpCompletionOption::ResponseHeadersRead);
            
            if (response.IsSuccessStatusCode())
            {
                co_return co_await ProcessStreamResponseAsync(response, onChunkReceived);
            }
            else
            {
                co_return L"错误：HTTP请求失败，状态码：" + to_wstring(response.StatusCode());
            }
        }
        catch (const hresult_error& ex)
        {
            co_return L"错误：" + ex.message();
        }
        catch (const std::exception& ex)
        {
            co_return L"错误：" + winrt::to_hstring(ex.what());
        }
    }
    
    IAsyncOperation<hstring> SiliconFlowClient::ProcessStreamResponseAsync(
        HttpResponseMessage response,
        const std::function<void(const std::wstring&)>& onChunkReceived)
    {
        std::wstring fullResponse;
        
        auto inputStream = co_await response.Content().ReadAsInputStreamAsync();
        DataReader dataReader(inputStream);
        
        // 设置缓冲区大小
        const uint32_t bufferSize = 4096;
        dataReader.InputStreamOptions(InputStreamOptions::Partial);
        
        while (true)
        {
            // 读取数据
            co_await dataReader.LoadAsync(bufferSize);
            
            if (dataReader.UnconsumedBufferLength() == 0)
            {
                break;
            }
            
            // 读取字符串
            hstring chunk = dataReader.ReadString(dataReader.UnconsumedBufferLength());
            std::wstring chunkStr = std::wstring(chunk.c_str());
            
            // 处理流式数据（SSE格式）
            size_t pos = 0;
            while ((pos = chunkStr.find(L"data: ", pos)) != std::wstring::npos)
            {
                pos += 6; // 跳过 "data: "
                
                size_t endPos = chunkStr.find(L"\n\n", pos);
                if (endPos == std::wstring::npos)
                {
                    endPos = chunkStr.length();
                }
                
                std::wstring dataLine = chunkStr.substr(pos, endPos - pos);
                
                if (dataLine == L"[DONE]")
                {
                    break;
                }
                
                try
                {
                    JsonObject jsonObject = JsonObject::Parse(dataLine);
                    
                    if (jsonObject.HasKey(L"choices") && jsonObject.GetNamedArray(L"choices").Size() > 0)
                    {
                        JsonObject choice = jsonObject.GetNamedArray(L"choices").GetObjectAt(0);
                        
                        if (choice.HasKey(L"delta"))
                        {
                            JsonObject delta = choice.GetNamedObject(L"delta");
                            
                            if (delta.HasKey(L"content"))
                            {
                                hstring contentHstring = delta.GetNamedString(L"content");
                                std::wstring content = std::wstring(contentHstring.c_str());
                                if (!content.empty())
                                {
                                    fullResponse += content;
                                    if (onChunkReceived)
                                    {
                                        onChunkReceived(content);
                                    }
                                }
                            }
                            
                            if (delta.HasKey(L"reasoning_content"))
                            {
                                hstring reasoningHstring = delta.GetNamedString(L"reasoning_content");
                                std::wstring reasoning = std::wstring(reasoningHstring.c_str());
                                if (!reasoning.empty())
                                {
                                    fullResponse += reasoning;
                                    if (onChunkReceived)
                                    {
                                        onChunkReceived(reasoning);
                                    }
                                }
                            }
                        }
                    }
                }
                catch (const hresult_error&)
                {
                    // JSON解析失败，跳过这一行
                }
                
                pos = endPos;
            }
        }
        
        dataReader.Close();
        co_return winrt::to_hstring(fullResponse);
    }
}