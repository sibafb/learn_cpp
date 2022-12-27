cmake_minimum_required(VERSION 3.0.2)

project(LibrariesSandbox)

set(CMAKE_CXX_FLAGS "-std=c++17")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_subdirectory(filesystem)
add_subdirectory(future)
add_subdirectory(thread)
add_subdirectory(threadClass)
add_subdirectory(threadTimeOut)
