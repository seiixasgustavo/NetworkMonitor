cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(NetworkMonitor)

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conaninfo.txt)
	list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
endif()	
find_package(Boost 1.74 REQUIRED COMPONENTS system)

set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_executable(NetworkMonitor ${SOURCES})
target_compile_features(NetworkMonitor
	PRIVATE
		cxx_std_11
)
target_link_libraries(NetworkMonitor
	PRIVATE
		Boost::Boost
)
