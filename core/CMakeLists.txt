# sokoban / core
project("sokoban-core" C)
add_library("${PROJECT_NAME}" STATIC)
add_library("sokoban::core" ALIAS "${PROJECT_NAME}")


target_sources(
	"${PROJECT_NAME}"
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/src/Game.c"
		"${CMAKE_CURRENT_LIST_DIR}/src/GameUtil.c"
		"${CMAKE_CURRENT_LIST_DIR}/src/Movement.c"
		"${CMAKE_CURRENT_LIST_DIR}/src/Tile.c"
		"${CMAKE_CURRENT_LIST_DIR}/src/World.c"
)


target_include_directories(
	"${PROJECT_NAME}"
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/include"
)


target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		sokoban::defaults
)

