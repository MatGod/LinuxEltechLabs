cmake_minimum_required(VERSION 3.15)
project(Lab_7)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab_7 main.cpp)
add_executable(Lab_7_1 Lab7_1/main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Lab_7 Threads::Threads)