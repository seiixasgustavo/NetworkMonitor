cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(NetworkMonitor)

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conaninfo.txt)
	list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
endif()	
find_package(Boost 1.74 REQUIRED COMPONENTS system)

set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

add_executable(NetworkMonitor ${SOURCES})
target_compile_features(NetworkMonitor
	PRIVATE
		cxx_std_11
)
target_link_libraries(NetworkMonitor
	PRIVATE
		Boost::Boost
)
