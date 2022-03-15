#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "math.h"
#include "vec2.h"
#include "world.h"

#define SIDE_X			0
#define SIDE_Y			1

struct raycast_s {
	int hit;
	int tile_id;
	int side;
	float distance;
	vec2_f intersection_point;
};

void raycast_clear(raycast_s* raycast)
{
#ifdef USE_OPTIMIZED_CODE
	asm
	{
		"mov R0, {raycast}"
		"mov R1, 0"
		"mov R2, -1"
		"mov [R0], R1"							// raycast->hit = 0;
		"mov [R0+1], R2"						// raycast->tile_id = -1;
		"mov [R0+2], R2"						// raycast->side = -1;
		"mov [R0+3], R1"						// raycast->distance = 0.0;
	}
#else
	raycast->hit = 0;
	raycast->tile_id = -1;
	raycast->side = -1;
	raycast->distance = 0.0;
#endif

	vec2_zero(&raycast->intersection_point);
}

bool p_raymarch(raycast_s* result, vec2_f* ray_step_size, vec2_f* ray_length_1d, vec2_i* step, vec2_i* map_check, float max_distance)
{
	int tile_found = 0;							// [BP - 1]
	int side_hit = -1;							// [BP - 2]
	float distance = 0.0;						// [BP - 3]

#ifdef USE_OPTIMIZED_CODE
	asm
	{
		// Function parameters
		"mov R4, {result}"						// R4 <- result
		"mov R5, {ray_step_size}"				// R5 <- ray_step_size
		"mov R6, {ray_length_1d}"				// R6 <- ray_length_1d
		"mov R7, {step}"						// R7 <- step
		"mov R8, {map_check}"					// R8 <- map_check
		"mov R9, {max_distance}"				// R9 <- max_distance

"__p_raymarch_loop:"
		"mov R0, {distance}"
		"fge R0, R9"							// if (distance >= max_distance)
		"jt R0, __p_raymarch_return_false"		// return false;

		"mov R0, [R6]"
		"mov R1, [R6+1]"
		"fge R0, R1"							// if (ray_length_1d->x < ray_length_1d->y)
		"jt R0, __p_raymarch_length_true"

		"mov R0, [R8]"
		"mov R1, [R7]"
		"iadd R0, R1"
		"mov [R8], R0"							// map_check->x += step->x;

		"mov R0, [R6]"
		"mov R1, 10.0"
		"fmul R0, R1"
		"mov [BP-3], R0"						// distance = ray_length_1d->x * WORLD_TILE_SIZE;

		"mov R0, [R6]"
		"mov R1, [R5]"
		"fadd R0, R1"
		"mov [R6], R0"							// ray_length_1d->x += ray_step_size->x;

		"mov R0, 0"
		"mov [BP-2], R0"						// side_hit = SIDE_X;

		"jmp __p_raymarch_length_end"

"__p_raymarch_length_true:"						// else
		"mov R0, [R8+1]"
		"mov R1, [R7+1]"
		"iadd R0, R1"
		"mov [R8+1], R0"						// map_check->y += step->y;

		"mov R0, [R6+1]"
		"mov R1, 10.0"
		"fmul R0, R1"
		"mov [BP-3], R0"						// distance = ray_length_1d->y * WORLD_TILE_SIZE;

		"mov R0, [R6+1]"
		"mov R1, [R5+1]"
		"fadd R0, R1"
		"mov [R6+1], R0"						// ray_length_1d->y += ray_step_size->y;

		"mov R0, 1"
		"mov [BP-2], R0"						// side_hit = SIDE_Y;

"__p_raymarch_length_end:"
	}

	tile_found = level1[map_check->x][map_check->y];

	asm
	{
		"mov R0, {tile_found}"
		"ile R0, 0"						// if (tile_found <= 0)
		"jt R0, __p_raymarch_loop"		// goto __p_raymarch_loop

		"mov R0, 1"
		"mov [R4], R0"					// result->hit = TRUE
		"mov R0, {tile_found}"
		"mov [R4+1], R0"				// result->tile_id = tile_found;
		"mov R0, {side_hit}"
		"mov [R4+2], R0"				// result->side = side_hit;
		"mov R0, {distance}"
		"mov [R4+3], R0"				// result->distance = distance;
		"mov R0, 1"						
		"jmp __p_raymarch_end"			// return true;

"__p_raymarch_return_false:"
		"mov R0, 0"						// return false;

"__p_raymarch_end:"
	}
#else
	while (tile_found <= 0 && distance < max_distance)
	{
		if (ray_length_1d->x < ray_length_1d->y)
		{
			map_check->x += step->x;
			distance = ray_length_1d->x * WORLD_TILE_SIZE;
			ray_length_1d->x += ray_step_size->x;
			side_hit = SIDE_X;
		}
		else
		{
			map_check->y += step->y;
			distance = ray_length_1d->y * WORLD_TILE_SIZE;
			ray_length_1d->y += ray_step_size->y;
			side_hit = SIDE_Y;
		}

		tile_found = level1[map_check->x][map_check->y];

		if (tile_found > 0)
		{
			result->hit = true;
			result->distance = distance;
			result->side = side_hit;
			result->tile_id = tile_found;

			return true;
		}
	}

	return false;
#endif
}

void p_raycast(raycast_s* result, vec2_f* origin, vec2_f* direction, float max_distance)
{
	vec2_f ray_step_size, ray_length_1d;
	vec2_i step, map_check;

	raycast_clear(result);
	//result->hit = false;
	//result->tile_id = 0;
	//result->side = 0;
	//result->distance = 0;
	//vec2_zero(&result->intersection_point);

	if (direction->x == 0)
	{
		ray_step_size.x = 0.000001;
	}
	else
	{
		ray_step_size.x = sqrt(1 + pow(direction->y / direction->x, 2.0));
	}

	if (direction->y == 0)
	{
		ray_step_size.y = 0.000001;
	}
	else
	{
		ray_step_size.y = sqrt(1 + pow(direction->x / direction->y, 2.0));
	}

	map_check.x = floor(origin->x / WORLD_TILE_SIZE);
	map_check.y = floor(origin->y / WORLD_TILE_SIZE);

	if (direction->x < 0)
	{
		step.x = -1;
		ray_length_1d.x = ((origin->x / WORLD_TILE_SIZE) - map_check.x) * ray_step_size.x;
	}
	else
	{
		step.x = 1;
		ray_length_1d.x = ((map_check.x + 1) - (origin->x / WORLD_TILE_SIZE)) * ray_step_size.x;
	}

	if (direction->y < 0)
	{
		step.y = -1;
		ray_length_1d.y = ((origin->y / WORLD_TILE_SIZE) - map_check.y) * ray_step_size.y;
	}
	else
	{
		step.y = 1;
		ray_length_1d.y = ((map_check.y + 1) - (origin->y / WORLD_TILE_SIZE)) * ray_step_size.y;
	}

	if (p_raymarch(result, &ray_step_size, &ray_length_1d, &step, &map_check, max_distance))
	{
		vec2_copy(&result->intersection_point, origin);
		vec2_multiply_add(&result->intersection_point, direction, result->distance);
	}
}

#endif