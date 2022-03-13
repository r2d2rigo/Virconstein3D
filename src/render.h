#ifndef __RENDER_H__
#define __RENDER_H__

#include "video.h"
#include "player.h"
#include "world.h"
#include "Resources.h"

#define	CEILING_COLOR			0xFFC2B094
#define FLOOR_COLOR				0xFF573C33

#define WALL_TEXTURE_SIZE		64
#define WALL_TEXTURE_COUNT		3

#define RAY_COUNT				(screen_width / 4)
#define FIELD_OF_VIEW			(pi / 2)
#define RAY_ANGLE_DELTA			(FIELD_OF_VIEW / RAY_COUNT)
#define SLICE_PIXEL_WIDTH		(screen_width / RAY_COUNT)

void r_init()
{
	select_texture(pixel);
	define_region(0, 0, 1, 1, 0, 0);

	select_texture(walls);
	define_region_matrix(10, 0, 0, 0, WALL_TEXTURE_SIZE, 0, 0, (WALL_TEXTURE_SIZE * WALL_TEXTURE_COUNT), 1, 0);
}

void r_draw_background()
{
	clear_screen(CEILING_COLOR);

	set_drawing_point(0, screen_height / 2);
	set_drawing_scale(screen_width, screen_height / 2);
	set_multiply_color(FLOOR_COLOR);

	select_texture(pixel);
	select_region(0);
	draw_region_rotozoomed();
}

void r_draw_world()
{
	vec2_f ray_start, ray_dir, ray_step_size; 
	vec2_f ray_length_1d;
	vec2_i step, map_check;
	vec2_f intersection;

	r_draw_background();

	set_multiply_color(color_white);

	for (int i = 0; i < RAY_COUNT; i++)
	{
		float ray_angle = (i - (RAY_COUNT / 2)) * RAY_ANGLE_DELTA;

		vec2_copy(&ray_start, &player.position);
		vec2_from_angle(&ray_dir, player.rotation + ray_angle);

		if (ray_dir.x == 0)
		{
			ray_step_size.x = 0.00001;
		}
		else
		{
			ray_step_size.x = sqrt(WORLD_TILE_SIZE + (ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x));
		}

		if (ray_dir.y == 0)
		{
			ray_step_size.y = 0.00001;
		}
		else
		{
			ray_step_size.y = sqrt(WORLD_TILE_SIZE + (ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y));
		}

		map_check.x = floor(ray_start.x);
		map_check.y = floor(ray_start.y);

		if (ray_dir.x < 0)
		{
			step.x = -1;
			ray_length_1d.x = (ray_start.x - map_check.x) * ray_step_size.x;
		}
		else
		{
			step.x = 1;
			ray_length_1d.x = ((map_check.x + WORLD_TILE_SIZE) - ray_start.x) * ray_step_size.x;
		}

		if (ray_dir.y < 0)
		{
			step.y = -1;
			ray_length_1d.y = (ray_start.y - map_check.y) * ray_step_size.y;
		}
		else
		{
			step.y = 1;
			ray_length_1d.y = ((map_check.y + WORLD_TILE_SIZE) - ray_start.y) * ray_step_size.y;
		}

		int tile_found = 0;
		int side_hit = -1;
		float max_distance = WORLD_TILE_SIZE * 50.0;
		float distance = 0.0;

		while (tile_found <= 0 && distance < max_distance)
		{
			if (ray_length_1d.x < ray_length_1d.y)
			{
				map_check.x += step.x;
				distance = ray_length_1d.x;
				ray_length_1d.x += ray_step_size.x;
				side_hit = 0;
			}
			else
			{
				map_check.y += step.y;
				distance = ray_length_1d.y;
				ray_length_1d.y += ray_step_size.y;
				side_hit = 1;
			}

			int tile_x = map_check.x;
			int tile_y = map_check.y;

			if (tile_x >= 0 && tile_x < 10 && tile_y >= 0 && tile_y < 10)
			{
				tile_found = level1[tile_x][tile_y];
			}

			if (tile_found > 0)
			{
				vec2_copy(&intersection, &ray_dir);
				vec2_multiply(&intersection, distance);
				vec2_add(&intersection, &ray_start);
			}
		}

		if (tile_found <= 0)
		{
			continue;
		}

		int texture_pixel_column;
		
		if (side_hit == 1)
		{
			texture_pixel_column = fmod(intersection.x, WORLD_TILE_SIZE) * WALL_TEXTURE_SIZE;
		}
		else
		{
			texture_pixel_column = fmod(intersection.y, WORLD_TILE_SIZE) * WALL_TEXTURE_SIZE;
		}

		select_texture(walls);
		select_region(10 + (tile_found * WALL_TEXTURE_SIZE) + texture_pixel_column);

		float slice_height = (screen_height / distance) / cos(ray_angle);

		set_drawing_scale(SLICE_PIXEL_WIDTH, slice_height / WALL_TEXTURE_SIZE);

		draw_region_zoomed_at(SLICE_PIXEL_WIDTH * i, (screen_height - slice_height) * 0.5);
	}
}

#endif