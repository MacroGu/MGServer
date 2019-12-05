cmake_minimum_required(VERSION 2.8)
project(client)


# Add the source in project root directory
aux_source_directory(. DIRSRCS)
# Add header file include directories
include_directories(./ ../)

FILE(GLOB GENPROTO_FILES "../server/genproto/*.cc" "../server/genproto/*.h")
source_group(genproto FILES ${GENPROTO_FILES} )


# Target
add_executable(client 
	${GENPROTO_FILES}
	${DIRSRCS})

target_link_libraries(client tinyxml protobuf ws2_32 )

if(CMAKE_COMPILER_IS_GNUCXX)
	    add_definitions(-std=c++11)
endif()
