cmake_minimum_required(VERSION 2.8)
project(client)


# Add the source in project root directory
aux_source_directory(. DIRSRCS)
# Add header file include directories
include_directories(./ ../ ../websocketpp)
# Target
add_executable(client ${DIRSRCS})

if(CMAKE_COMPILER_IS_GNUCXX)
	    add_definitions(-std=c++11)
endif()
