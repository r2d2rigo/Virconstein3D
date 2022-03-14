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
	player.position.x = 4.5 * WORLD_TILE_SIZE;
	player.position.y = 4.5 * WORLD_TILE_SIZE;

	r_init();

	while (true)
	{
		cl_input();

		r_raycast();

		r_draw_world();

		end_frame();
	}
}