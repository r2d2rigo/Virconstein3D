#ifndef __WORLD_H__
#define __WORLD_H__

#define WORLD_TILE_SIZE				10.0
#define WORLD_TILE_WIDTH			100
#define WORLD_TILE_HEIGHT			100

embedded int[WORLD_TILE_WIDTH][WORLD_TILE_HEIGHT] level1 = "obj\\level1.vmap";
embedded int[WORLD_TILE_WIDTH][WORLD_TILE_HEIGHT] level1_objects = "obj\\level1_objects.vmap";

#define LIGHT_FLICKER_FRAME_LENGTH	10
#define LIGHT_FLICKER_STATES		"1101010001011111010"

#endif