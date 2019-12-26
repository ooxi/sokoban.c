#include <sokoban/Game.h>
#include <sokoban/Movement.h>
#include <sokoban/Tile.h>
#include <sokoban/World.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



static enum Sokoban_Tile Sokoban_Tile_Parse(char c) {
	enum Sokoban_Tile const ERROR = SOKOBAN_TILE_PLAYER;

	switch (c) {
		case '#':	return SOKOBAN_TILE_WALL;
		case ' ':	return SOKOBAN_TILE_FLOOR;
		case '@':	return SOKOBAN_TILE_PLAYER;
		case '$':	return SOKOBAN_TILE_CRATE;

		/* @todo Recognize target
		 */
		case '.':	return SOKOBAN_TILE_FLOOR;
	}

	return ERROR;
}



static struct Sokoban_Game* Sokoban_Game_Parse(
			uint32_t const width,
			uint32_t const height,
			char const* level
		) {

	/* Count number of targest
	 */
	uint32_t numberOfTargets = 0;

	for (uint32_t idx = 0; idx < width * height; ++ idx) {
		if ('.' == level[idx]) {
			numberOfTargets += 1;
		}
	}


	struct Sokoban_Game* game = Sokoban_Game_Create(width, height, numberOfTargets);
	struct Sokoban_World* world = Sokoban_Game_World(game);

	uint32_t nextTarget = 0;

	for (uint32_t x = 0; x < width; ++x) {
		for (uint32_t y = 0; y < height; ++y) {
			char const c = level[(y * width) + x];

			/* Target found
			 */
			if ('.' == c) {
				Sokoban_World_SetTile(world, x, y, SOKOBAN_TILE_FLOOR);
				Sokoban_World_SetTarget(world, nextTarget, x, y);
				nextTarget += 1;

			} else {
				enum Sokoban_Tile const tile = Sokoban_Tile_Parse(c);
				Sokoban_World_SetTile(world, x, y, tile);
			}
		}
	}

	return game;
}



static void Sokoban_Tile_Print(enum Sokoban_Tile const tile, FILE* to) {
	switch (tile) {
		case SOKOBAN_TILE_WALL: {
			fprintf(to, "##");
		} break;

		case SOKOBAN_TILE_FLOOR: {
			fprintf(to, "  ");
		} break;

		case SOKOBAN_TILE_PLAYER: {
			fprintf(to, "@@");
		} break;

		case SOKOBAN_TILE_CRATE: {
			fprintf(to, "$$");
		} break;
	}
}



static void Sokoban_World_Print(struct Sokoban_World const* world, FILE* to) {
	uint32_t const width = Sokoban_World_Width(world);
	uint32_t const height = Sokoban_World_Height(world);


	for (uint32_t y = 0; y < height; ++y) {
		for (uint32_t x = 0; x < width; ++x) {
			enum Sokoban_Tile tile = Sokoban_World_Tile(world, x, y);
			bool isTarget = Sokoban_World_IsTarget(world, x, y);

			if ((SOKOBAN_TILE_FLOOR == tile) && isTarget) {
				fprintf(stdout, "..");
			} else {
				Sokoban_Tile_Print(tile, to);
			}
		}
		fprintf(to, "\n");
	}
}



int main(void) {

	/* @see https://www.janko.at/Spiele/Sokoban/GarciaAlberto.txt
	 */
	uint32_t const WIDTH = 8;
	uint32_t const HEIGHT = 13;

	char const* LEVEL = ""
		" ###### "
		"##    # "
		"#  ## ##"
		"# #..  #"
		"#   . @#"
		"# $## ##"
		"## #  # "
		"## $ ## "
		"#  $##  "
		"#   #   "
		"#   #   "
		"#  ##   "
		"####    ";


	/* Parse game
	 */
	struct Sokoban_Game* game = Sokoban_Game_Parse(WIDTH, HEIGHT, LEVEL);

	if (!Sokoban_World_Valid(Sokoban_Game_World(game))) {
		Sokoban_Game_Destroy(game);
		return EXIT_FAILURE;
	}


	/* Run game
	 */
	while (!Sokoban_Game_Won(game)) {
		Sokoban_World_Print(Sokoban_Game_World(game), stdout);

		/* @see https://stackoverflow.com/a/1799024
		 */
		system("/bin/stty raw");
		char c = fgetc(stdin);
		system("/bin/stty cooked");

		switch (c) {
			case 'w': {
				Sokoban_Game_Move(game, SOKOBAN_MOVEMENT_UP);
			} break;

			case 's': {
				Sokoban_Game_Move(game, SOKOBAN_MOVEMENT_DOWN);
			} break;

			case 'a': {
				Sokoban_Game_Move(game, SOKOBAN_MOVEMENT_LEFT);
			} break;

			case 'd': {
				Sokoban_Game_Move(game, SOKOBAN_MOVEMENT_RIGHT);
			} break;

			case 'x': {
				Sokoban_Game_Destroy(game);
				return EXIT_FAILURE;
			} break;

			default: {
				fprintf(stderr, ""
					"\n"
					"Unrecognized input %c!\n"
					"\tw - Up\n"
					"\ta - Left\n"
					"\ts - Down\n"
					"\td - Right\n"
					"\tx - Exit\n"
				, c);
			} break;
		}

		fprintf(stdout, "\n\n");
	}


	/* Print final screen after game is won
	 */
	Sokoban_World_Print(Sokoban_Game_World(game), stdout);
	fprintf(stdout, "You won!\n");


	/* Game finished successfully
	 */
	Sokoban_Game_Destroy(game);
	return EXIT_SUCCESS;
}
