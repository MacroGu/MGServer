cmake_minimum_required(VERSION 2.8.9)
project(server)


include_directories(./ ./base ../ ../websocketpp ../wepoll/include)
link_directories(../libs/mysql)

set(SOURCE_FILES_PREFIX base)
set(BASE_FILES 
	singleton.h
	protocol.h
	${SOURCE_FILES_PREFIX}/defines.h
	${SOURCE_FILES_PREFIX}/ServerLog.h	
    ${SOURCE_FILES_PREFIX}/ServerLog.cpp
	${SOURCE_FILES_PREFIX}/ServerConf.h
	${SOURCE_FILES_PREFIX}/ServerConf.cpp
	${SOURCE_FILES_PREFIX}/WorkerThread.h
	${SOURCE_FILES_PREFIX}/WorkerThread.cpp
	${SOURCE_FILES_PREFIX}/EpollSocket.h
	${SOURCE_FILES_PREFIX}/EpollSocket.cpp
	${SOURCE_FILES_PREFIX}/RedisHandle.h
	${SOURCE_FILES_PREFIX}/RedisHandle.cpp
	${SOURCE_FILES_PREFIX}/MysqlHandle.h
	${SOURCE_FILES_PREFIX}/MysqlHandle.cpp
	${SOURCE_FILES_PREFIX}/GameServer.h
	${SOURCE_FILES_PREFIX}/GameServer.cpp
	${SOURCE_FILES_PREFIX}/GameSocketWatcher.h
	${SOURCE_FILES_PREFIX}/GameSocketWatcher.cpp	
	${SOURCE_FILES_PREFIX}/Interface.h
	${SOURCE_FILES_PREFIX}/Interface.cpp	
	main.cpp
)

set(SOURCE_FILES_GAMELOGIC gamelogic)
set(GAMELOGIC_FILES 
	${SOURCE_FILES_GAMELOGIC}/MsgDistribute.h
	${SOURCE_FILES_GAMELOGIC}/MsgDistribute.cpp
)

source_group(base FILES ${BASE_FILES} )
source_group(gamelogic FILES ${GAMELOGIC_FILES} )

# Target
add_executable(server
    ${BASE_FILES}
	${GAMELOGIC_FILES}
)
if(MSVC)
	target_link_libraries(server tinyxml hiredis wepoll libmysql ws2_32)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS -D_SPDLOG_ACTIVE_LEVEL)
else()
	target_link_libraries(server tinyxml hiredis mysqlclient)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pthread -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -pthread -std=c++11")
endif()
