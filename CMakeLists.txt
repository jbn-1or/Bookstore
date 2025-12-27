# 最低CMake版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(BookstoreManagementSystem)

# 设置C++标准（支持C++11及以上特性）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定头文件目录（include文件夹存放所有.hpp声明）
include_directories(include)

# 收集所有源文件
set(SOURCES
    main.cpp  # 程序入口
    src/User.cpp
    src/Book.cpp
    src/Transaction.cpp
    src/Log.cpp
    src/Parser.cpp
    src/Storage.cpp
    src/Utils.cpp
)

# 设置可执行文件输出目录（统一输出到bin文件夹）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# 确保输出目录存在
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# 生成可执行文件，名称为code
add_executable(code ${SOURCES})