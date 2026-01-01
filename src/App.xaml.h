#pragma once

#include "pch.h"

namespace winrt::AIChatApp::implementation
{
    struct App : winrt::Windows::UI::Xaml::ApplicationT<App>
    {
        App();

        void OnLaunched(LaunchActivatedEventArgs const& args);
        void OnSuspending(IInspectable const& sender, SuspendingEventArgs const& args);
        void OnNavigationFailed(IInspectable const& sender, Navigation::NavigationFailedEventArgs const& args);

        void InitializeComponent();

    private:
        // 转换器类声明（已移除，在App.xaml中直接定义样式）
    };
}

namespace winrt::AIChatApp::factory_implementation
{
    struct App : winrt::Windows::UI::Xaml::ApplicationT<App, implementation::App>
    {
    };
}