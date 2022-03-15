#ifndef __PLAYER__H__
#define __PLAYER__H__

#include "vec2.h"
#include "math.h"
#include "input.h"
#include "physics.h"
#include "rect.h"
#include "misc.h"

#define PLAYER_BBOX_SIZE			7.5
#define PLAYER_MOVE_SPEED			0.5				// Units / frame
#define PLAYER_ROTATE_SPEED			(pi / 128)		// Radians / frame

struct player_s {
	vec2_f position;
	float rotation;
};

player_s player;

void cl_push_back()
{
	vec2_i player_tile, left_tile, top_tile, right_tile, bottom_tile;

	// TODO: clean up
	vec2_i_set(&player_tile, floor(player.position.x / WORLD_TILE_SIZE),  floor(player.position.y / WORLD_TILE_SIZE));
	vec2_i_set(&left_tile, player_tile.x - 1, player_tile.y);
	vec2_i_set(&top_tile, player_tile.x, player_tile.y - 1);
	vec2_i_set(&right_tile, player_tile.x + 1, player_tile.y);
	vec2_i_set(&bottom_tile, player_tile.x, player_tile.y + 1);

	if (level1[left_tile.x][left_tile.y] > 0)
	{
		if (player.position.x - (PLAYER_BBOX_SIZE * 0.5) < ((left_tile.x + 1) * WORLD_TILE_SIZE))
		{
			player.position.x = ((left_tile.x + 1) * WORLD_TILE_SIZE) + (PLAYER_BBOX_SIZE * 0.5);
		}
	}

	if (level1[top_tile.x][top_tile.y] > 0)
	{
		if (player.position.y - (PLAYER_BBOX_SIZE * 0.5) < ((top_tile.y + 1) * WORLD_TILE_SIZE))
		{
			player.position.y = ((top_tile.y + 1) * WORLD_TILE_SIZE) + (PLAYER_BBOX_SIZE * 0.5);
		}
	}

	if (level1[right_tile.x][right_tile.y] > 0)
	{
		if (player.position.x + (PLAYER_BBOX_SIZE * 0.5) > ((right_tile.x) * WORLD_TILE_SIZE))
		{
			player.position.x = ((right_tile.x) * WORLD_TILE_SIZE) - (PLAYER_BBOX_SIZE * 0.5);
		}
	}

	if (level1[bottom_tile.x][bottom_tile.y] > 0)
	{
		if (player.position.y + (PLAYER_BBOX_SIZE * 0.5) > ((bottom_tile.y) * WORLD_TILE_SIZE))
		{
			player.position.y = ((bottom_tile.y) * WORLD_TILE_SIZE) - (PLAYER_BBOX_SIZE * 0.5);
		}
	}
}

void cl_input()
{
	vec2_f move_direction, player_forward, player_right;

	select_gamepad(0);

	vec2_zero(&move_direction);
	vec2_from_angle(&player_forward, player.rotation);
	vec2_right(&player_right, &player_forward);

	bool is_strafing = gamepad_button_b() > 0;

	if (gamepad_left() > 0)
	{
		if (is_strafing)
		{
			vec2_multiply_add(&move_direction, &player_right, -1);
		}
		else
		{
			player.rotation -= PLAYER_ROTATE_SPEED;
		}
	}
	else if (gamepad_right() > 0)
	{
		if (is_strafing)
		{
			vec2_multiply_add(&move_direction, &player_right, 1);
		}
		else
		{
			player.rotation += PLAYER_ROTATE_SPEED;
		}
	}

	if (gamepad_up() > 0)
	{
		vec2_multiply_add(&move_direction, &player_forward, 1);
	}
	else if (gamepad_down() > 0)
	{
		vec2_multiply_add(&move_direction, &player_forward, -1);
	}

	vec2_normalize(&move_direction);
	vec2_multiply_add(&player.position, &move_direction, PLAYER_MOVE_SPEED);

	cl_push_back();
}


#endif