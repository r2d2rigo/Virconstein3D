#ifndef __PLAYER__H__
#define __PLAYER__H__

#include "vec2.h"

struct player_s {
	vec2_f position;
	float rotation;
};

player_s player;

#endif