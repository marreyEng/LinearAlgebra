cmake_minimum_required(VERSION 4.2)

project(qwe)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(PROJECT_IS_TOP_LEVEL)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink 
        "${CMAKE_BINARY_DIR}/compile_commands.json" 
        "${CMAKE_SOURCE_DIR}/compile_commands.json"
    )
endif()

add_executable(exec)

target_sources(exec
PRIVATE
    src/main.cpp
)

target_link_libraries(exec
PRIVATE
    pthread
)