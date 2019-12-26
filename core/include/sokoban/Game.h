#ifndef HEADER_SOKOBAN_GAME
#define HEADER_SOKOBAN_GAME

#include <sokoban/Movement.h>
#include <sokoban/World.h>
#include <stdbool.h>
#include <stdint.h>

struct Sokoban_Game;

struct Sokoban_Game* Sokoban_Game_Create(uint32_t width, uint32_t height, uint32_t crates);
void Sokoban_Game_Destroy(struct Sokoban_Game* game);

/**
 * @param game self
 * @return Current view of game world
 */
struct Sokoban_World* Sokoban_Game_World(struct Sokoban_Game* game);

/**
 * @param game self
 * @return true iff. all crates have been moved to their target
 */
bool Sokoban_Game_Won(struct Sokoban_Game const* game);

/**
 * @param game self
 * @return Number of moves it took to reach the current state from the start
 */
uint32_t Sokoban_Game_Moves(struct Sokoban_Game const* game);

/**
 * @param game self
 * @param movement Next movement to execute
 */
void Sokoban_Game_Move(struct Sokoban_Game* game, enum Sokoban_Movement movement);

#endif
