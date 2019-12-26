# sokoban / defaults
cmake_minimum_required(VERSION 3.5)


project("sokoban-defaults" C)
add_library("${PROJECT_NAME}" INTERFACE)
add_library("sokoban::defaults" ALIAS "${PROJECT_NAME}")


target_compile_options(
	"${PROJECT_NAME}"
	INTERFACE
		-std=c18
		-Wall
		-Wextra
		-Werror
		-pedantic
)

