#ifndef __PLAYER__H__
#define __PLAYER__H__

#include "vec2.h"
#include "math.h"
#include "input.h"

#define PLAYER_MOVE_SPEED			0.05			// Units / frame
#define PLAYER_ROTATE_SPEED			(pi / 128)		// Radians / frame

struct player_s {
	vec2_f position;
	float rotation;
};

player_s player;


void cl_input()
{
	vec2_f player_forward;

	select_gamepad(0);

	if (gamepad_left() > 0)
	{
		player.rotation -= PLAYER_ROTATE_SPEED;
	}
	else if (gamepad_right() > 0)
	{
		player.rotation += PLAYER_ROTATE_SPEED;
	}
	
	if (gamepad_up() > 0)
	{
		vec2_from_angle(&player_forward, player.rotation);
		vec2_multiply(&player_forward, PLAYER_MOVE_SPEED);

		vec2_add(&player.position, &player_forward);
	}
	else if (gamepad_down() > 0)
	{
		vec2_from_angle(&player_forward, player.rotation);
		vec2_multiply(&player_forward, -PLAYER_MOVE_SPEED);

		vec2_add(&player.position, &player_forward);
	}
}

#endif