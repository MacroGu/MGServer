cmake_minimum_required(VERSION 2.8)
project(MsgPacket)
 
include_directories(
    "${PROJECT_SOURCE_DIR}"
)
 
add_library (MsgPacket
	CommonClass.h
	CommonClass.cpp
)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pthread -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -pthread -std=c++11")
endif()

