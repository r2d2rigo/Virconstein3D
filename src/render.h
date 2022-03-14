#ifndef __RENDER_H__
#define __RENDER_H__

#include "video.h"
#include "player.h"
#include "world.h"
#include "physics.h"
#include "Resources.h"

#define	CEILING_COLOR			0xFFC2B094
#define FLOOR_COLOR				0xFF573C33

#define WALL_TEXTURE_SIZE		64
#define WALL_TEXTURE_COUNT		5

#define	CANVAS_WIDTH			(screen_width / 2)
#define CANVAS_HEIGHT			(screen_height - 120)
#define CANVAS_X				((screen_width - CANVAS_WIDTH) / 2)
#define CANVAS_Y				0

#define RAY_COUNT				CANVAS_WIDTH
#define FIELD_OF_VIEW			(pi / 4)
#define RAY_ANGLE_DELTA			(FIELD_OF_VIEW / RAY_COUNT)

#define MAX_RENDER_DISTANCE		(WORLD_TILE_SIZE * 10.0)

struct framebuffer_column_s {
	int x;
	int region;
	float slice_height;
};

framebuffer_column_s[RAY_COUNT] framebuffer;

void r_init()
{
	select_texture(pixel);
	define_region(0, 0, 1, 1, 0, 0);

	select_texture(walls);
	define_region_matrix(10, 0, 0, 0, WALL_TEXTURE_SIZE, 0, 0, (WALL_TEXTURE_SIZE * WALL_TEXTURE_COUNT), 1, 0);
}

void r_raycast()
{
	raycast_s raycast_result;
	vec2_f ray_dir;

	int initial_pixel_column = 0;

	if (get_frame_counter() % 2 == 0)
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

		int texture_pixel_column;

		if (raycast_result.side == SIDE_Y)
		{
			texture_pixel_column = fmod(raycast_result.intersection_point.x, WORLD_TILE_SIZE) * WALL_TEXTURE_SIZE;
		}
		else
		{
			texture_pixel_column = fmod(raycast_result.intersection_point.y, WORLD_TILE_SIZE) * WALL_TEXTURE_SIZE;
		}

		// framebuffer[i].x = i;
		framebuffer[i].region = 10 + (raycast_result.tile_id * WALL_TEXTURE_SIZE) + texture_pixel_column;
		framebuffer[i].slice_height = fmax(20.0, CANVAS_HEIGHT / raycast_result.distance) / cos(ray_angle);
	}
}

void r_draw_background()
{
	clear_screen(CEILING_COLOR);

	set_drawing_point(CANVAS_X, CANVAS_HEIGHT / 2);
	set_drawing_scale(CANVAS_WIDTH, CANVAS_HEIGHT / 2);
	set_multiply_color(FLOOR_COLOR);

	select_texture(pixel);
	select_region(0);
	draw_region_rotozoomed();
}

void r_draw_walls()
{
	set_multiply_color(color_white);
	select_texture(walls);

	for (int i = 0; i < RAY_COUNT; i++)
	{
		select_region(framebuffer[i].region);

		set_drawing_scale(1.0, framebuffer[i].slice_height / WALL_TEXTURE_SIZE);

		draw_region_zoomed_at(CANVAS_X + i, (CANVAS_HEIGHT - framebuffer[i].slice_height) * 0.5);
	}
}

void r_draw_world()
{
	r_draw_background();

	r_draw_walls();
}

#endif