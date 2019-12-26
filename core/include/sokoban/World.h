#ifndef HEADER_SOKOBAN_WORLD
#define HEADER_SOKOBAN_WORLD

#include <sokoban/Tile.h>
#include <stdbool.h>
#include <stdint.h>

struct Sokoban_World;

/* World coordinates
 *
 * (0,0) | (1,0) | (2,0)              up
 * ------+-------+------              ↑
 * (0,1) | (1,1) | (2,1)    left ← player  → right
 * ------+-------+------             ↓
 * (0,2) | (1,2) | (2,2)           down
 */
struct Sokoban_World_Position {
	int32_t x;
	int32_t y;
};

struct Sokoban_World* Sokoban_World_Create(uint32_t width, uint32_t height, uint32_t targets);
void Sokoban_World_Destroy(struct Sokoban_World* world);

bool Sokoban_World_Valid(struct Sokoban_World const* world);
uint32_t Sokoban_World_Width(struct Sokoban_World const* world);
uint32_t Sokoban_World_Height(struct Sokoban_World const* world);

struct Sokoban_World_Position Sokoban_World_Player(struct Sokoban_World const* world);

uint32_t Sokoban_World_Targets(struct Sokoban_World const* world);
struct Sokoban_World_Position Sokoban_World_Target(struct Sokoban_World const* world, uint32_t target);
bool Sokoban_World_IsTarget(struct Sokoban_World const* world, uint32_t x, uint32_t y);
void Sokoban_World_SetTarget(struct Sokoban_World* world, uint32_t target, uint32_t x, uint32_t y);

enum Sokoban_Tile Sokoban_World_Tile(struct Sokoban_World const* world, uint32_t x, uint32_t y);
void Sokoban_World_SetTile(struct Sokoban_World* world, uint32_t x, uint32_t y, enum Sokoban_Tile tile);

#endif
