cmake_minimum_required(VERSION 3.15)
project(Lab_4_Reader)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab_4_Reader main.cpp)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Lab_4_Reader Threads::Threads)