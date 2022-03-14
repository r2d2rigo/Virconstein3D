#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "math.h"
#include "vec2.h"
#include "world.h"

#define SIDE_X			0
#define SIDE_Y			1

struct raycast_s {
	bool hit;
	int tile_id;
	int side;
	float distance;
	vec2_f intersection_point;
};

void p_raycast(raycast_s* result, vec2_f* origin, vec2_f* direction, float max_distance)
{
	vec2_f ray_step_size, ray_length_1d;
	vec2_i step, map_check;

	result->hit = false;
	result->tile_id = 0;
	result->side = 0;
	result->distance = 0;
	vec2_zero(&result->intersection_point);

	if (direction->x == 0)
	{
		ray_step_size.x = 0.000001;
	}
	else
	{
		ray_step_size.x = sqrt(WORLD_TILE_SIZE + pow(direction->y / direction->x, 2.0));
	}

	if (direction->y == 0)
	{
		ray_step_size.y = 0.000001;
	}
	else
	{
		ray_step_size.y = sqrt(WORLD_TILE_SIZE + pow(direction->x / direction->y, 2.0));
	}

	map_check.x = floor(origin->x);
	map_check.y = floor(origin->y);

	if (direction->x < 0)
	{
		step.x = -1;
		ray_length_1d.x = (origin->x - map_check.x) * ray_step_size.x;
	}
	else
	{
		step.x = 1;
		ray_length_1d.x = ((map_check.x + WORLD_TILE_SIZE) - origin->x) * ray_step_size.x;
	}

	if (direction->y < 0)
	{
		step.y = -1;
		ray_length_1d.y = (origin->y - map_check.y) * ray_step_size.y;
	}
	else
	{
		step.y = 1;
		ray_length_1d.y = ((map_check.y + WORLD_TILE_SIZE) - origin->y) * ray_step_size.y;
	}

	int tile_found = 0;
	int side_hit = -1;
	float distance = 0.0;

	while (tile_found <= 0 && distance < max_distance)
	{
		if (ray_length_1d.x < ray_length_1d.y)
		{
			map_check.x += step.x;
			distance = ray_length_1d.x;
			ray_length_1d.x += ray_step_size.x;
			side_hit = SIDE_X;
		}
		else
		{
			map_check.y += step.y;
			distance = ray_length_1d.y;
			ray_length_1d.y += ray_step_size.y;
			side_hit = SIDE_Y;
		}

		tile_found = level1[map_check.x][map_check.y];

		if (tile_found > 0)
		{
			result->hit = true;
			result->distance = distance;
			result->side = side_hit;
			result->tile_id = tile_found;
			vec2_copy(&result->intersection_point, origin);
			vec2_multiply_add(&result->intersection_point, direction, distance);

			return;
		}
	}
}

#endif