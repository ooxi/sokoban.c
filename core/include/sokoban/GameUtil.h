#ifndef HEADER_SOKOBAN_GAME_UTIL
#define HEADER_SOKOBAN_GAME_UTIL

#include <sokoban/Tile.h>
#include <sokoban/World.h>
#include <stdint.h>

/**
 * Accesses a tile in an indefinite world. Whenever a tile is accessed, which is
 * outside of the world coordinates, a {@code SOKOBAN_TILE_WALL} is returned.
 *
 * @param world Real world
 * @param x X-coodrinate
 * @param y Y-coordinate
 *
 * @return Tile in world, as long as coordinates are inside world
 */
enum Sokoban_Tile Sokoban_GameUtil_SafeGetTile(struct Sokoban_World const* world, int32_t x, int32_t y);
void Sokoban_GameUtil_SafeSetTile(struct Sokoban_World* world, int32_t x, int32_t y, enum Sokoban_Tile tile);

#endif
