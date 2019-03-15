cmake_minimum_required(VERSION 2.8)
project(tinyxml)
 
include_directories(
    "${PROJECT_SOURCE_DIR}"
)
 
add_library (tinyxml
    	tinystr.cpp
	tinyxml.cpp
	tinyxmlerror.cpp
	tinyxmlparser.cpp
)