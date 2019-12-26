# sokoban
cmake_minimum_required(VERSION 3.14)
project("sokoban")


enable_testing()


add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/cli")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/core")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/defaults")
