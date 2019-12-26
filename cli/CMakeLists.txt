# sokoban.c / cli
project("sokoban-cli" C)
add_executable("${PROJECT_NAME}")


target_sources(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/src/main.c"
)


target_link_libraries(
	"${PROJECT_NAME}"
	PUBLIC
		sokoban::core
	PRIVATE
		sokoban::defaults
)
