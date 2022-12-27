cmake_minimum_required(VERSION 3.0.2)

project(thread)

set(CMAKE_CXX_FLAGS "-std=c++17")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}
    thread.cpp
	)

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

