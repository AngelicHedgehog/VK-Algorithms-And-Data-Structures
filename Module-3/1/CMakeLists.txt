cmake_minimum_required(VERSION 3.9)

set(project_name Graphs)
project(${project_name})

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wextra -Werror -Wpedantic)

set(main_executable ${PROJECT_SOURCE_DIR}/src/main.cpp)

set(include_files
    ${PROJECT_SOURCE_DIR}/include/TestGraphs.hpp
)
set(include_locations
    ${PROJECT_SOURCE_DIR}/include
)
set(source 
    ${PROJECT_SOURCE_DIR}/src/TestGraphs.cpp
)

set(libs_names GraphsLib)
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/Graphs)

include_directories(${include_locations})

add_executable(${project_name} ${main_executable} ${source})

target_link_libraries(${project_name} ${libs_names})

add_custom_target(EXE_TARGET DEPENDS ${project_name})
