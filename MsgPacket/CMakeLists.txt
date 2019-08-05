cmake_minimum_required(VERSION 2.8)
project(MsgPacket)
 
include_directories(
    "${PROJECT_SOURCE_DIR}"
)
 
add_library (MsgPacket
	CommonClass.h
	CommonClass.cpp
)