#ifndef __VEC2__H__
#define __VEC2__H__

struct vec2_f {
	float x;
	float y;
};

struct  vec2_i {
	int x;
	int y;
};

/**
* Adds v2 into v1.
**/
void vec2_add(vec2_f* v1, vec2_f* v2)
{
	v1->x += v2->x;
	v1->y += v2->y;
}

/**
* Subtracts v2 from v1.
**/
void vec2_subtract(vec2_f* v1, vec2_f* v2)
{
	v1->x -= v2->x;
	v1->y -= v2->y;
}

/**
* Multiplies v by amount.
**/
void vec2_multiply(vec2_f* v, float amount)
{
	v->x *= amount;
	v->y *= amount;
}

void vec2_copy(vec2_f* dst, vec2_f* src)
{
	dst->x = src->x;
	dst->y = src->y;
}

float vec2_length(vec2_f* v)
{
	return sqrt((v->x * v->x) + (v->y * v->y));
}

void vec2_normalize(vec2_f* v)
{
	float length = vec2_length(v);

	v->x /= length;
	v->y /= length;
}

void vec2_from_angle(vec2_f* v, float angle)
{
	v->x = cos(angle);
	v->y = sin(angle);
}

#endif