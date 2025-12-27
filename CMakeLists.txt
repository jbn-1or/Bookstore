# 指定CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(BookStore)

# 设置C++标准（11/14/17/20）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制要求指定的C++标准，否则报错

# 包含头文件目录
include_directories(include)

# 收集源文件：src目录下所有.cpp + 根目录main.cpp
# 若源文件结构不同，可修改路径（如仅根目录：file(GLOB SOURCES "*.cpp")）
file(GLOB SOURCES 
    "src/*.cpp"
    "main.cpp"
)

# 生成可执行文件
add_executable(code ${SOURCES})

