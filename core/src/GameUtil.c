#include <sokoban/GameUtil.h>

#include <sokoban/Tile.h>
#include <sokoban/World.h>



enum Sokoban_Tile Sokoban_GameUtil_SafeGetTile(
			struct Sokoban_World const* world,
			int32_t x,
			int32_t y
		) {

	int64_t const width = Sokoban_World_Width(world);
	int64_t const height = Sokoban_World_Height(world);

	if (x < 0) {
		return SOKOBAN_TILE_WALL;
	}
	if (y < 0) {
		return SOKOBAN_TILE_WALL;
	}
	if (x >= width) {
		return SOKOBAN_TILE_WALL;
	}
	if (y >= height) {
		return SOKOBAN_TILE_WALL;
	}

	return Sokoban_World_Tile(world, x, y);
}



void Sokoban_GameUtil_SafeSetTile(
			struct Sokoban_World* world,
			int32_t x,
			int32_t y,
			enum Sokoban_Tile tile
		) {

	int64_t const width = Sokoban_World_Width(world);
	int64_t const height = Sokoban_World_Height(world);

	if (x < 0) {
		return /*nop*/;
	}
	if (y < 0) {
		return /*nop*/;
	}
	if (x >= width) {
		return /*nop*/;
	}
	if (y >= height) {
		return /*nop*/;
	}

	Sokoban_World_SetTile(world, x, y, tile);
}
