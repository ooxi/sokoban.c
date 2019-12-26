#include <sokoban/World.h>

#include <sokoban/Tile.h>
#include <stdint.h>
#include <stdio.h>	// TODO REMOVE
#include <stdlib.h>



struct Sokoban_World {
	uint32_t width;
	uint32_t height;
	enum Sokoban_Tile* tile;

	uint32_t targets;
	struct Sokoban_World_Position* target;
};



struct Sokoban_World* Sokoban_World_Create(
			uint32_t width,
			uint32_t height,
			uint32_t targets
		) {

	struct Sokoban_World* world = malloc(sizeof(struct Sokoban_World));

	world->width = width;
	world->height = height;
	world->tile = calloc(width * height, sizeof(enum Sokoban_Tile));

	for (uint32_t x = 0; x < width; ++x) {
		for (uint32_t y = 0; y < height; ++y) {
			Sokoban_World_SetTile(world, x, y, SOKOBAN_TILE_WALL);
		}
	}

	world->targets = targets;
	world->target = calloc(targets, sizeof(struct Sokoban_World_Position));

	for (uint32_t target = 0; target < targets; ++target) {
		world->target[target].x = -1;
		world->target[target].y = -1;
	}

	return world;
}



void Sokoban_World_Destroy(struct Sokoban_World* world) {
	free(world->tile);
	free(world->target);

	world->width = 0;
	world->height = 0;
	world->tile = 0;

	world->targets = 0;
	world->target = 0;

	free(world);
}



bool Sokoban_World_Valid(struct Sokoban_World const* world) {
	fprintf(stderr, "TODO: Sokoban_World_Valid\n");
	(void)world;
	return true;
}



uint32_t Sokoban_World_Width(struct Sokoban_World const* world) {
	return world->width;
}



uint32_t Sokoban_World_Height(struct Sokoban_World const* world) {
	return world->height;
}



struct Sokoban_World_Position Sokoban_World_Player(struct Sokoban_World const* world) {
	struct Sokoban_World_Position const ERROR = {
		.x = -1,
		.y = -1,
	};

	uint32_t const width = Sokoban_World_Width(world);
	uint32_t const height = Sokoban_World_Height(world);

	for (uint32_t x = 0; x < width; ++x) {
		for (uint32_t y = 0; y < height; ++y) {
			enum Sokoban_Tile tile = Sokoban_World_Tile(world, x, y);

			if (SOKOBAN_TILE_PLAYER == tile) {
				struct Sokoban_World_Position player = {
					.x = x,
					.y = y,
				};

				return player;
			}
		}
	}


	return ERROR;
}



uint32_t Sokoban_World_Targets(struct Sokoban_World const* world) {
	return world->targets;
}



struct Sokoban_World_Position Sokoban_World_Target(
			struct Sokoban_World const* world,
			uint32_t target
		) {

	struct Sokoban_World_Position const ERROR = {
		.x = -1,
		.y = -1,
	};

	uint32_t const targets = Sokoban_World_Targets(world);

	if (target >= targets) {
		return ERROR;
	}

	return world->target[target];

}



bool Sokoban_World_IsTarget(
			struct Sokoban_World const* world,
			uint32_t x,
			uint32_t y
		) {

	struct Sokoban_World_Position test = {
		.x = x,
		.y = y,
	};

	uint32_t const targets = Sokoban_World_Targets(world);

	for (uint32_t target = 0; target < targets; ++target) {
		struct Sokoban_World_Position position = Sokoban_World_Target(world, target);

		if ((position.x == test.x) && (position.y == test.y)) {
			return true;
		}
	}

	return false;
}



void Sokoban_World_SetTarget(
			struct Sokoban_World* world,
			uint32_t target,
			uint32_t x,
			uint32_t y
		) {

	uint32_t const width = Sokoban_World_Width(world);
	uint32_t const height = Sokoban_World_Height(world);
	uint32_t const targets = Sokoban_World_Targets(world);

	if (x >= width) {
		return /* TODO ERROR */;
	}
	if (y >= height) {
		return /* TODO ERROR */;
	}
	if (target >= targets) {
		return /* TODO ERROR */;
	}

	world->target[target].x = x;
	world->target[target].y = y;
}



enum Sokoban_Tile Sokoban_World_Tile(struct Sokoban_World const* world, uint32_t x, uint32_t y) {
	enum Sokoban_Tile const ERROR = SOKOBAN_TILE_PLAYER;

	uint32_t const width = Sokoban_World_Width(world);
	uint32_t const height = Sokoban_World_Height(world);

	if (x >= width) {
		return ERROR;
	}
	if (y >= height) {
		return ERROR;
	}

	return world->tile[(y * width) + x];
}



void Sokoban_World_SetTile(
			struct Sokoban_World* world,
			uint32_t x,
			uint32_t y,
			enum Sokoban_Tile tile
		) {


	uint32_t const width = Sokoban_World_Width(world);
	uint32_t const height = Sokoban_World_Height(world);

	if (x >= width) {
		// TODO ERROR;
		return;
	}
	if (y >= height) {
		// TODO: ERROR
		return;
	}

	world->tile[(y * width) + x] = tile;
}
