cmake_minimum_required(VERSION 2.8)
project(hiredis)
 
include_directories(
    "${PROJECT_SOURCE_DIR}"
)
 
add_library (hiredis
    async.c
	hiredis.c
	net.c
	sds.c
)