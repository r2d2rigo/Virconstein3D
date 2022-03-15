#ifndef __RENDER_H__
#define __RENDER_H__

#include "video.h"
#include "string.h"
#include "player.h"
#include "world.h"
#include "physics.h"
#include "Resources.h"

#define	CEILING_COLOR			0xFF151515
#define FLOOR_COLOR				0xFF3C3934

#define WALL_TEXTURE_SIZE		64
#define WALL_TEXTURE_COUNT		8

// #define	CANVAS_WIDTH			(screen_width / 2)
// #define CANVAS_HEIGHT			(screen_height / 2)
#define	CANVAS_WIDTH			((screen_width / 8) * 3)
#define CANVAS_HEIGHT			((screen_height / 8) * 3)
#define CANVAS_X				((screen_width - (CANVAS_WIDTH * CANVAS_SCALE)) / 2)
#define CANVAS_Y				((screen_height - (CANVAS_HEIGHT * CANVAS_SCALE)) / 2)
#define CANVAS_SCALE			2

#define RAY_COUNT				CANVAS_WIDTH
#define FIELD_OF_VIEW			(pi / 2)
#define RAY_ANGLE_DELTA			(FIELD_OF_VIEW / RAY_COUNT)

#define MAX_RENDER_DISTANCE		(WORLD_TILE_SIZE * 50.0)

struct framebuffer_column_s {
	// int x;
	// int region;
	int tile_id;
	float texcoord;
	float slice_height;
};

framebuffer_column_s[RAY_COUNT] framebuffer;

void r_init()
{
	select_texture(pixel);
	define_region(0, 0, 1, 1, 0, 0);

	select_texture(walls);
	define_region_matrix(10, 0, 0, 0, WALL_TEXTURE_SIZE - 1, 0, 0, (WALL_TEXTURE_SIZE * WALL_TEXTURE_COUNT), (WALL_TEXTURE_SIZE * WALL_TEXTURE_COUNT), 0);
}

void r_raycast()
{
	raycast_s raycast_result;
	vec2_f ray_dir;

	int initial_pixel_column = 0;

	if (get_frame_counter() % 2 == 1)
	{
		initial_pixel_column = 1;
	}

	for (int i = initial_pixel_column; i < RAY_COUNT; i += 2)
	{
		float ray_angle = (i - (RAY_COUNT / 2)) * RAY_ANGLE_DELTA;

		vec2_from_angle(&ray_dir, player.rotation + ray_angle);

		p_raycast(&raycast_result, &player.position, &ray_dir, MAX_RENDER_DISTANCE);

		if (!raycast_result.hit)
		{
			continue;
		}

		// int texture_pixel_column;
		float texcoord;

		if (raycast_result.side == SIDE_Y)
		{
			texcoord = (fmod(raycast_result.intersection_point.x, WORLD_TILE_SIZE) / WORLD_TILE_SIZE);
			// texture_pixel_column = (fmod(raycast_result.intersection_point.x, WORLD_TILE_SIZE) / WORLD_TILE_SIZE) * WALL_TEXTURE_SIZE;
		}
		else
		{
			texcoord = (fmod(raycast_result.intersection_point.y, WORLD_TILE_SIZE) / WORLD_TILE_SIZE);
			// texture_pixel_column = (fmod(raycast_result.intersection_point.y, WORLD_TILE_SIZE) / WORLD_TILE_SIZE) * WALL_TEXTURE_SIZE;
		}

		// framebuffer[i].region = 10 + (raycast_result.tile_id * WALL_TEXTURE_SIZE) + texture_pixel_column;
		framebuffer[i].tile_id = raycast_result.tile_id;
		framebuffer[i].texcoord = texcoord;
		// framebuffer[i].region = 10 + (raycast_result.tile_id * WALL_TEXTURE_SIZE) + 64;
		framebuffer[i].slice_height = fmax(2.0, CANVAS_HEIGHT / (raycast_result.distance / WORLD_TILE_SIZE)) / cos(ray_angle);
	}
}

void r_draw_background()
{
	clear_screen(CEILING_COLOR);

	set_drawing_point(CANVAS_X, CANVAS_Y + ((CANVAS_HEIGHT / 2) * CANVAS_SCALE));
	set_drawing_scale(CANVAS_WIDTH * CANVAS_SCALE, (CANVAS_HEIGHT / 2) * CANVAS_SCALE);
	set_multiply_color(FLOOR_COLOR);

	select_texture(pixel);
	select_region(0);
	draw_region_rotozoomed();
}

void r_draw_walls()
{
	int f = get_frame_counter() % (LIGHT_FLICKER_FRAME_LENGTH * strlen(LIGHT_FLICKER_STATES));
	int h = f / LIGHT_FLICKER_FRAME_LENGTH;


	set_multiply_color(color_white);
	select_texture(walls);

	for (int i = 0; i < RAY_COUNT; i++)
	{
		int tile_id = framebuffer[i].tile_id;

		if (tile_id == 41)
		{
			if (LIGHT_FLICKER_STATES[h] == '0')
			{
				tile_id = 32;
			}
			else
			{
				tile_id = 34;
			}
		}

		int slice_region = 10 + (tile_id * WALL_TEXTURE_SIZE) + (framebuffer[i].texcoord * WALL_TEXTURE_SIZE);

		select_region(slice_region);

		set_drawing_scale(1.0 * CANVAS_SCALE, (framebuffer[i].slice_height / WALL_TEXTURE_SIZE) * CANVAS_SCALE);

		draw_region_zoomed_at((i * CANVAS_SCALE) + CANVAS_X, CANVAS_Y + (((CANVAS_HEIGHT - framebuffer[i].slice_height) * 0.5) * CANVAS_SCALE));
	}
}

void r_draw_world()
{
	r_draw_background();

	r_draw_walls();
}

#endif