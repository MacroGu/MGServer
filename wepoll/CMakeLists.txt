cmake_minimum_required(VERSION 2.8)

 
# 必要的变量。方便后面引用代码位置。
project(wepoll)
SET(PROJECT_ROOT_PATH ".")

include_directories(
    "${PROJECT_SOURCE_DIR}"
)
 
add_library (wepoll
	${PROJECT_ROOT_PATH}/src/afd.c
	${PROJECT_ROOT_PATH}/src/api.c
	${PROJECT_ROOT_PATH}/src/error.c
	${PROJECT_ROOT_PATH}/src/init.c
	${PROJECT_ROOT_PATH}/src/nt.c
	${PROJECT_ROOT_PATH}/src/poll-group.c
	${PROJECT_ROOT_PATH}/src/port.c
	${PROJECT_ROOT_PATH}/src/queue.c
	${PROJECT_ROOT_PATH}/src/reflock.c
	${PROJECT_ROOT_PATH}/src/sock.c
	${PROJECT_ROOT_PATH}/src/thread-safe-tree.c
	${PROJECT_ROOT_PATH}/src/tree.c
	${PROJECT_ROOT_PATH}/src/ws.c
)