cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(NetworkMonitor)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conaninfo.txt)
	list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
endif()	
find_package(Boost 1.74 REQUIRED COMPONENTS system)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

set(LIB_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/websocket-client.cpp"
)
add_library(NetworkMonitor STATIC ${LIB_SOURCES})

target_compile_features(NetworkMonitor
	PUBLIC
		cxx_std_14
)
target_link_libraries(NetworkMonitor
	PUBLIC
		Boost::Boost
)
target_include_directories(NetworkMonitor
  	PUBLIC
		inc
)


set(TESTS_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/websocket-client.cpp"
)
add_executable(NetworkMonitorTests ${TESTS_SOURCES})

target_compile_features(NetworkMonitorTests
	PRIVATE
      		cxx_std_14	
)
target_link_libraries(NetworkMonitorTests
  	PRIVATE
		NetworkMonitor
		Boost::Boost
)
