cmake_minimum_required(VERSION 3.0.2)

project(filesystem)

set(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(${PROJECT_NAME}
	filesystem_pick_extension.cpp
	)

	
