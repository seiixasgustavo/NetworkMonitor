cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

project(NetworkMonitor)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conaninfo.txt)
	list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
endif()	

# Packages --------

find_package(Boost 1.74 REQUIRED COMPONENTS system unit_test_framework)
find_package(CURL REQUIRED)
find_package(Filesystem REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(OpenSSL REQUIRED)

# Packages END ----

# Enable Testing -------

enable_testing()

# Enable Testing END ---

# Enable Vim Autocomplete -------

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

# Enable Vim Autocomplete END ---


# STATIC LIBRARY -------

set(LIB_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/file-downloader.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/transport-network.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/websocket-client.cpp"
)
add_library(NetworkMonitor STATIC ${LIB_SOURCES})

target_compile_features(NetworkMonitor
	PUBLIC
		cxx_std_17
)
target_link_libraries(NetworkMonitor
	PUBLIC
		Boost::Boost
		OpenSSL::OpenSSL
        nlohmann_json::nlohmann_json
		std::filesystem
	PRIVATE
		CURL::CURL
)
target_include_directories(NetworkMonitor
  	PUBLIC
		inc
)

# STATIC LIBRARY END ---

# Tests Configuration

set(TESTS_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/file-downloader.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/transport-network.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/websocket-client.cpp"
)
add_executable(NetworkMonitorTests ${TESTS_SOURCES})

target_compile_features(NetworkMonitorTests
	PRIVATE
      		cxx_std_17	
)
target_compile_definitions(NetworkMonitorTests
  	PRIVATE
        TEST_DATA="${CMAKE_CURRENT_SOURCE_DIR}/tests/test-data"
		TESTS_CACERT_PEM="${CMAKE_CURRENT_SOURCE_DIR}/tests/cacert.pem"
        TESTS_NETWORK_LAYOUT_JSON="${CMAKE_CURRENT_SOURCE_DIR}/tests/network-layout.json"
)
target_link_libraries(NetworkMonitorTests
  	PRIVATE
		NetworkMonitor
		Boost::Boost
        OpenSSL::OpenSSL
    	std::filesystem
)

add_test(
  	NAME NetworkMonitorTests 
  	COMMAND $<TARGET_FILE:NetworkMonitorTests>
)

set_tests_properties(NetworkMonitorTests PROPERTIES 
  	PASS_REGULAR_EXPRESSION ".*No errors detected"
)
