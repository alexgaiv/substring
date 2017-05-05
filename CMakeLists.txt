cmake_minimum_required(VERSION 2.6)
project(substring)

include_directories("3rdparty/mathgl/include")

add_library(substring STATIC substring.cpp substring.h)
add_executable(substring_test main.cpp)
target_link_libraries(substring_test substring)
target_link_libraries(substring_test "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/mathgl/lib/mgl.lib")

add_custom_command(TARGET substring_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/3rdparty/mathgl/bin"
		$<TARGET_FILE_DIR:substring_test>)
			
add_custom_command(TARGET substring_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/sample.txt"
		"${CMAKE_CURRENT_BINARY_DIR}")

# disable mathgl warnings
if (MSVC)
    add_definitions("/wd4251" "/wd4800" "/wd4190")
endif()