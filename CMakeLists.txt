cmake_minimum_required(VERSION 2.8.9)
project(MGServer)
set(CMAKE_VERBOSE_MAKEFILE OFF)
# Add the source in project root directory
aux_source_directory(. DIRSRCS)
# Add header file include directories
include_directories(./ ./include ./tinyxml ./server ./client ./wepoll/include ./wepoll/src ./wepoll/src/regular ./wepoll/src/combined)
# Add block directories
add_subdirectory(tinyxml)
add_subdirectory(client)
add_subdirectory(server)

if(MSVC)
	add_subdirectory(wepoll)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions(-std=c++11)
endif()
