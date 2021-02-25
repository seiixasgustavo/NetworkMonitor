cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(NetworkMonitor)

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conaninfo.txt)
	list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
endif()	

set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_executable(NetworkMonitor ${SOURCES})
target_compile_features(NetworkMonitor
	PRIVATE
		cxx_std_11
)
