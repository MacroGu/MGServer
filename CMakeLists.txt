cmake_minimum_required(VERSION 2.8.9)
project(MGServer)
set(CMAKE_VERBOSE_MAKEFILE OFF)
# Add the source in project root directory
aux_source_directory(. DIRSRCS)
# Add header file include directories
include_directories(./ ./include ./tinyxml ./server ./client ./MsgPacket ./include/mysql)
if(MSVC)
	include_directories(hiredis/win)
else()
	include_directories(hiredis/linux)
endif()
# Add block directories
add_subdirectory(tinyxml)
add_subdirectory(libuv)
add_subdirectory(MsgPacket)
add_subdirectory(server)



if(MSVC)
	add_subdirectory(client)
	add_subdirectory(hiredis/win)
else()
	add_subdirectory(hiredis/linux)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-std=c++11)
endif()
