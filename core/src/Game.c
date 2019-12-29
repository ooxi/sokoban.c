#include <sokoban/Game.h>

#include <sokoban/GameUtil.h>
#include <sokoban/Tile.h>
#include <sokoban/Movement.h>
#include <sokoban/World.h>
#include <stdint.h>
#include <stdio.h>	// TODO REMOVE
#include <stdlib.h>



struct Sokoban_Game {

	/* Owned by {@code struct Sokoban_Game}
	 */
	struct Sokoban_World* world;

	uint32_t moves;
};



struct Sokoban_Game_Tileset {
	enum Sokoban_Tile a;
	enum Sokoban_Tile b;
	enum Sokoban_Tile c;
};

static struct Sokoban_Game_Tileset Sokoban_Game_Transition(struct Sokoban_Game_Tileset before);



struct Sokoban_Game* Sokoban_Game_Create(uint32_t width, uint32_t height, uint32_t crates) {
	struct Sokoban_Game* game = malloc(sizeof(struct Sokoban_Game));

	game->world = Sokoban_World_Create(width, height, crates);
	game->moves = 0;

	return game;
}



void Sokoban_Game_Destroy(struct Sokoban_Game* game) {
	Sokoban_World_Destroy(game->world);

	game->world = 0;
	game->moves = 0;

	free(game);
}



struct Sokoban_World* Sokoban_Game_World(struct Sokoban_Game* game) {
	return game->world;
}



bool Sokoban_Game_Won(struct Sokoban_Game const* game) {
	uint32_t const targets = Sokoban_World_Targets(game->world);

	for (uint32_t target = 0; target < targets; ++target) {
		struct Sokoban_World_Position position = Sokoban_World_Target(game->world, target);
		enum Sokoban_Tile tile = Sokoban_World_Tile(game->world, position.x, position.y);

		/* If position of target is not filled with a crate, the game
		 * is not won
		 */
		if (SOKOBAN_TILE_CRATE != tile) {
			return false;
		}
	}

	/* No target without a crate, thus game is won
	 */
	return true;
}



uint32_t Sokoban_Game_Moves(struct Sokoban_Game const* game) {
	return game->moves;
}



/**
 * Executes main game logic.
 *
 * The result of a movement depends on at most three tiles. Assuming the player
 * is moved from left to right, the tile set are
 *
 *    +---+   +---+   +---+
 *    | A | → | B | → | C |
 *    +---+   +---+   +---+
 *
 * Depending on the contents of B and C the player A can move forward and maybe
 * even push a crate.
 *
 * Moreover, movements in all directions are rotationally equivalent.
 *
 * @param game self
 * @param movement Movement to execute
 */
void Sokoban_Game_Move(struct Sokoban_Game* game, enum Sokoban_Movement movement) {

	/* The position of B and C depend of movement
	 *
	 * @see struct Sokoban_World_Position
	 */
	struct Sokoban_World_Position a = Sokoban_World_Player(game->world);
	struct Sokoban_World_Position b;
	struct Sokoban_World_Position c;

	switch (movement) {
		case SOKOBAN_MOVEMENT_UP: {
			b.x = c.x = a.x;
			b.y = a.y - 1;
			c.y = a.y - 2;
		} break;

		case SOKOBAN_MOVEMENT_DOWN: {
			b.x = c.x = a.x;
			b.y = a.y + 1;
			c.y = a.y + 2;
		} break;

		case SOKOBAN_MOVEMENT_LEFT: {
			b.y = c.y = a.y;
			b.x = a.x - 1;
			c.x = a.x - 2;
		} break;

		case SOKOBAN_MOVEMENT_RIGHT: {
			b.y = c.y = a.y;
			b.x = a.x + 1;
			c.x = a.x + 2;
		} break;
	}

	struct Sokoban_Game_Tileset before = {
		.a = Sokoban_GameUtil_SafeGetTile(game->world, a.x, a.y),
		.b = Sokoban_GameUtil_SafeGetTile(game->world, b.x, b.y),
		.c = Sokoban_GameUtil_SafeGetTile(game->world, c.x, c.y),
	};

	struct Sokoban_Game_Tileset after = Sokoban_Game_Transition(before);

	Sokoban_GameUtil_SafeSetTile(game->world, a.x, a.y, after.a);
	Sokoban_GameUtil_SafeSetTile(game->world, b.x, b.y, after.b);
	Sokoban_GameUtil_SafeSetTile(game->world, c.x, c.y, after.c);
}



static struct Sokoban_Game_Tileset Sokoban_Game_Transition(struct Sokoban_Game_Tileset before) {
	struct Sokoban_Game_Tileset const ERROR = {
		.a = SOKOBAN_TILE_PLAYER,
		.b = SOKOBAN_TILE_PLAYER,
		.c = SOKOBAN_TILE_PLAYER,
	};


	/* Player must always be on position A
	 */
	if (SOKOBAN_TILE_PLAYER != before.a) {
		return ERROR;
	}


	/* Action depending on tile B
	 */
	switch (before.b) {

		/* Player cannot move into wall or push wall out of the way
		 */
		case SOKOBAN_TILE_WALL: {
			return before;
		} break;

		/* Player can move into an empty floor, no problem at all
		 */
		case SOKOBAN_TILE_FLOOR: {
			struct Sokoban_Game_Tileset const after = {
				.a = SOKOBAN_TILE_FLOOR,
				.b = SOKOBAN_TILE_PLAYER,
				.c = before.c,
			};
			return after;
		} break;

		/* There must not be two players on the same world
		 */
		case SOKOBAN_TILE_PLAYER: {
			return ERROR;
		} break;

		/* If the player can push a create depends on the tile _behind_
		 * the create (C)
		 */
		case SOKOBAN_TILE_CRATE: {
			switch (before.c) {

				/* Player cannot push a crate into a wall
				 */
				case SOKOBAN_TILE_WALL: {
					return before;
				} break;

				/* Player can push a create into an empty floor
				 */
				case SOKOBAN_TILE_FLOOR: {
					struct Sokoban_Game_Tileset const after = {
						.a = SOKOBAN_TILE_FLOOR,
						.b = SOKOBAN_TILE_PLAYER,
						.c = SOKOBAN_TILE_CRATE,
					};
					return after;
				} break;

				/* There must only be one player in the world
				 */
				case SOKOBAN_TILE_PLAYER: {
					return ERROR;
				} break;

				/* Player cannot push a create into another crate
				 */
				case SOKOBAN_TILE_CRATE: {
					return before;
				} break;
			}
		} break;
	}

	/* All other inputs are erroneous
	 */
	return ERROR;
}
