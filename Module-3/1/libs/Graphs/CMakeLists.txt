cmake_minimum_required(VERSION 3.9)

set(project_name GraphsLib)
project(${project_name})

set(CMAKE_CXX_STANDARD 20)

set(include_files
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ArcGraph/ArcGraph.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/IGraph/IGraph.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ListGraph/ListGraph.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/MatrixGraph/MatrixGraph.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SetGraph/SetGraph.hpp
)

set(include_locations
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ArcGraph
    ${CMAKE_CURRENT_SOURCE_DIR}/include/IGraph
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ListGraph
    ${CMAKE_CURRENT_SOURCE_DIR}/include/MatrixGraph
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SetGraph
)

set(source
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ArcGraph/ArcGraph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ListGraph/ListGraph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MatrixGraph/MatrixGraph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SetGraph/SetGraph.cpp
)

add_library(${project_name} STATIC ${source})
target_include_directories(${project_name} PUBLIC ${include_locations})
