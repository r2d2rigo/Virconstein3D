// include Vircon libraries
#include "time.h"
#include "video.h"
#include "audio.h"
#include "math.h"
#include "input.h"
#include "misc.h"
#include "string.h"
#include "vec2.h"
#include "render.h"
#include "world.h"
#include "player.h"

void main(void)
{
	for (int y = 0; y < WORLD_TILE_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_TILE_WIDTH; x++)
		{
			if (level1_objects[x][y] == 40)
			{
				player.position.x = x * WORLD_TILE_SIZE + 0.5;
				player.position.y = y * WORLD_TILE_SIZE + 0.5;

				break;
			}
		}
	}

	r_init();

	while (true)
	{
		cl_input();

		r_raycast();

		r_draw_world();

		end_frame();
	}
}