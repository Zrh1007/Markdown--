cmake_minimum_required(VERSION 3.20)

# 项目定义必须在cmake_minimum_required之后
project(AIChatApp LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 添加可执行文件
add_executable(AIChatApp
    src/MainWindow.xaml.cpp
    src/MainWindow.xaml
    src/App.xaml.cpp
    src/App.xaml
    src/pch.h
    src/pch.cpp
    src/SiliconFlowClient.cpp
    src/SiliconFlowClient.h
)

# 包含目录
target_include_directories(AIChatApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 设置预编译头文件
target_precompile_headers(AIChatApp PRIVATE src/pch.h)

# 设置WinUI 3项目属性（适用于Visual Studio）
if(MSVC)
    # 设置Windows SDK版本
    set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0")
    
    # 设置C++/WinRT属性
    target_compile_options(AIChatApp PRIVATE 
        /ZW  # 启用Windows运行时扩展
        /EHsc # 启用C++异常处理
        /utf-8 # 使用UTF-8字符集
        /await # 启用C++协程支持
    )
    
    # 链接Windows运行时库
    target_link_libraries(AIChatApp PRIVATE
        WindowsApp.lib
    )
    
    # 设置XAML文件属性
    set_source_files_properties(src/MainWindow.xaml PROPERTIES
        VS_XAML_TYPE "Page"
    )
    set_source_files_properties(src/App.xaml PROPERTIES
        VS_XAML_TYPE "ApplicationDefinition"
    )
    
    # 设置项目属性（修复字符集冲突）
    set_target_properties(AIChatApp PROPERTIES
        VS_GLOBAL_RootNamespace "AIChatApp"
        VS_GLOBAL_AppContainerApplication "true"
        VS_GLOBAL_ApplicationType "Windows Store"
        VS_GLOBAL_ApplicationTypeRevision "10.0"
        VS_GLOBAL_TargetPlatformVersion "10.0"
        VS_GLOBAL_TargetPlatformMinVersion "10.0.17763.0"
        VS_GLOBAL_WindowsTargetPlatformVersion "10.0"
        VS_GLOBAL_EnableWinRT "true"
        VS_GLOBAL_CharacterSet "Unicode"  # 明确设置为Unicode
    )
endif()

# 安装目标
install(TARGETS AIChatApp
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
)