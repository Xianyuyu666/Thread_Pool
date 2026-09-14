cmake_minimum_required(VERSION 3.16)
#项目名称
project(threadpool)
#定义全局C++标准为C++11
set(CMAKE_CXX_STANDARD 11)
#强制要求C++11，不允许编译器降级
set(CXX_STANDARD_REQUIRED ON)
#禁用GNU扩展，代码可移植
set(CMAKE_CXX_EXTENSIONS OFF)
#添加编译警告，排查Bug
add_compile_options(-Wall -Wextra -Wpedantic)
add_executable(main src/main.cpp)
