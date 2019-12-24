cmake_minimum_required(VERSION 3.15)
project(Lab_4)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lab_4 main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Lab_4 Threads::Threads)