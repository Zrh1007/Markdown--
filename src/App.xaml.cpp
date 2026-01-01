#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace AIChatApp;
using namespace AIChatApp::implementation;

namespace winrt::AIChatApp::implementation
{

    void App::InitializeComponent()
    {
        // 空实现，因为我们现在不使用XAML生成器
    }

    /// <summary>
    /// 初始化单一实例应用程序对象。这是执行的创作代码的第一行
    /// ，逻辑上等同于 main() 或 WinMain()。
    /// </summary>
    App::App()
    {
        InitializeComponent();
        Suspending({ this, &App::OnSuspending });

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    /// <summary>
    /// 在应用程序由最终用户正常启动时进行调用。将在启动应用程序
    /// 以打开特定文件等情况下使用。
    /// </summary>
    /// <param name="e">有关启动请求和过程的详细信息。</param>
    void App::OnLaunched(LaunchActivatedEventArgs const& e)
    {
        Frame rootFrame{ nullptr };
        auto content = Window::Current().Content();
        if (content)
        {
            rootFrame = content.try_as<Frame>();
        }

        // 不要在窗口已包含内容时重复应用程序初始化，
        // 只需确保窗口处于活动状态
        if (rootFrame == nullptr)
        {
            // 创建一个 Frame 以用作导航上下文并将其关联到
            // 一个 SuspensionManager 键
            rootFrame = Frame();

            rootFrame.NavigationFailed({ this, &App::OnNavigationFailed });

            if (e.PreviousExecutionState() == ApplicationExecutionState::Terminated)
            {
                // 仅当合适时才还原保存的会话状态，并安排
                // 在还原完成后的最终启动步骤
            }

            if (e.PrelaunchActivated() == false)
            {
                if (rootFrame.Content() == nullptr)
                {
                    // 当导航堆栈尚未还原时，导航到第一页，
                    // 并通过将所需信息作为导航参数传入来配置
                    // 新页面
                    rootFrame.Navigate(xaml_typename<AIChatApp::MainWindow>(), box_value(e.Arguments()));
                }
                // 将框架放在当前窗口中
                Window::Current().Content(rootFrame);
                // 确保当前窗口处于活动状态
                Window::Current().Activate();
            }
        }
        else
        {
            if (e.PrelaunchActivated() == false)
            {
                if (rootFrame.Content() == nullptr)
                {
                    // 当导航堆栈尚未还原时，导航到第一页，
                    // 并通过将所需信息作为导航参数传入来配置
                    // 新页面
                    rootFrame.Navigate(xaml_typename<AIChatApp::MainWindow>(), box_value(e.Arguments()));
                }
                // 确保当前窗口处于活动状态
                Window::Current().Activate();
            }
        }
    }

    /// <summary>
    /// 在将要挂起应用程序执行时调用。  在不知道应用程序
    /// 无需知道应用程序会被终止还是会恢复，
    /// 并让内存内容保持不变。
    /// </summary>
    /// <param name="sender">挂起的请求的源。</param>
    /// <param name="e">有关挂起请求的详细信息。</param>
    void App::OnSuspending(IInspectable const& sender, SuspendingEventArgs const& e)
    {
        auto deferral = e.SuspendingOperation().GetDeferral();
        //TODO: 保存应用程序状态并停止任何后台活动
        deferral.Complete();
    }

    /// <summary>
    /// 在导航到特定页失败时调用
    /// </summary>
    /// <param name="sender">导航失败的框架</param>
    /// <param name="e">有关导航失败的详细信息</param>
    void App::OnNavigationFailed(IInspectable const& sender, NavigationFailedEventArgs const& e)
    {
        throw hresult_error(E_FAIL, hstring(L"未能加载页面 ") + e.SourcePageType().Name);
    }
}