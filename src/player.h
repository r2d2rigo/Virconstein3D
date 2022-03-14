#ifndef __PLAYER__H__
#define __PLAYER__H__

#include "vec2.h"
#include "math.h"
#include "input.h"
#include "physics.h"

#define PLAYER_MOVE_SPEED			0.05			// Units / frame
#define PLAYER_ROTATE_SPEED			(pi / 128)		// Radians / frame

struct player_s {
	vec2_f position;
	float rotation;
};

player_s player;


void cl_input()
{
	vec2_f move_direction, player_forward, player_right;
	raycast_s move_raycast;

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
}

#endif