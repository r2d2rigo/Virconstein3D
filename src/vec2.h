#ifndef __VEC2__H__
#define __VEC2__H__

#include "defs.h"

struct vec2_f {
	float x;
	float y;
};

struct  vec2_i {
	int x;
	int y;
};


void vec2_zero(vec2_f* v)
{
#ifdef USE_OPTIMIZED_CODE
	asm
	{
		// R0 = 0.000000
		"mov R0, 0.000000"
		// R1 = v->x
		"mov R1, {v}"
		// *R1 = R0
		"mov [R1], R0"
		// *R1 = R0
		"mov [R1 + 1], R0"
	}
#else
	v->x = 0.0;
	v->y = 0.0;
#endif
}

/**
* Adds v2 into v1.
**/
void vec2_add(vec2_f* v1, vec2_f* v2)
{
#ifdef USE_OPTIMIZED_CODE
	asm
	{
		// R0 = v1->x
		"mov R0, {v1}"
		// R1 = v2->x
		"mov R1, {v2}"
		// R2 = *R0
		"mov R2, [R0]"
		// R3 = *R1
		"mov R3, [R1]"
		// R2 += R3
		"fadd R2, R3"
		// *R0 = R2
		"mov [R0], R2"
		// R2 = *R0
		"mov R2, [R0 + 1]"
		// R3 = *R1
		"mov R3, [R1 + 1]"
		// R2 += R3
		"fadd R2, R3"
		// *R0 = R2
		"mov [R0 + 1], R2"
	}
#else
	v1->x += v2->x;
	v1->y += v2->y;
#endif
}

/**
* Subtracts v2 from v1.
**/
void vec2_subtract(vec2_f* v1, vec2_f* v2)
{
#ifdef USE_OPTIMIZED_CODE
	asm
	{
		// R0 = v1->x
		"mov R0, {v1}"
		// R1 = v2->x
		"mov R1, {v2}"
		// R2 = *R0
		"mov R2, [R0]"
		// R3 = *R1
		"mov R3, [R1]"
		// R2 -= R3
		"fsub R2, R3"
		// *R0 = R2
		"mov [R0], R2"
		// R2 = *R0
		"mov R2, [R0 + 1]"
		// R3 = *R1
		"mov R3, [R1 + 1]"
		// R2 -= R3
		"fsub R2, R3"
		// *R0 = R2
		"mov [R0 + 1], R2"
	}
#else
	v1->x -= v2->x;
	v1->y -= v2->y;
#endif
}

/**
* Multiplies v by amount.
**/
void vec2_multiply(vec2_f* v, float amount)
{
#ifdef USE_OPTIMIZED_CODE
	asm
	{
		// R0 = v1->x
		"mov R0, {v}"
		// R1 = v2->x
		"mov R1, {amount}"
		// R2 = *R0
		"mov R2, [R0]"
		"fmul R2, R1"
		"mov [R0], R2"
		"mov R2, [R0 + 1]"
		"fmul R2, R1"
		"mov [R0 + 1], R2"
	}
#else
	v->x *= amount;
	v->y *= amount;
#endif
}

void vec2_copy(vec2_f* dst, vec2_f* src)
{
#ifdef USE_OPTIMIZED_CODE
	asm
	{
		// R0 = dst->x
		"mov R0, {dst}"
		// R1 = src->x
		"mov R1, {src}"
		// R2 = *R1
		"mov R2, [R1]"
		// dst->x = R2
		"mov [R0], R2"
		// R2 = src->y
		"mov R2, [R1 + 1]"
		// dst->y = R2
		"mov [R0 + 1], R2"
	}
#else		
	dst->x = src->x;
	dst->y = src->y;
#endif
}

void vec2_multiply_add(vec2_f* dst, vec2_f* v, float amount)
{
#ifdef USE_OPTIMIZED_CODE
	asm
	{
		"mov R0, {dst}"
		"mov R1, {v}"
		"mov R2, {amount}"
		"mov R3, [R0]"
		"mov R4, [R1]"
		"fmul R4, R2"
		"fadd R3, R4"
		"mov [R0], R3"
		"mov R3, [R0 + 1]"
		"mov R4, [R1 + 1]"
		"fmul R4, R2"
		"fadd R3, R4"
		"mov [R0 + 1], R3"
	}
#else
	dst->x += v->x * amount;
	dst->y += v->y * amount;
#endif
}

float vec2_length(vec2_f* v)
{
	return sqrt((v->x * v->x) + (v->y * v->y));
}

void vec2_normalize(vec2_f* v)
{
	float length = vec2_length(v);

	if (length != 0)
	{
		v->x /= length;
		v->y /= length;
	}
}

void vec2_from_angle(vec2_f* v, float angle)
{
#ifdef USE_OPTIMIZED_CODE
	asm
	{
		"mov R0, {v}"
		"mov R1, {angle}"
		"fadd R1, 1.570796"
		"sin R1"
		"mov [R0], R1"
		"mov R1, {angle}"
		"sin R1"
		"fsgn R1"
		"mov [R0 + 1], R1"
	}
#else
	v->x = cos(angle);
	v->y = -sin(angle);
#endif
}

void vec2_right(vec2_f* right, vec2_f* v)
{
#ifdef USE_OPTIMIZED_CODE
	asm
	{
		"mov R0, {right}"
		"mov R1, {v}"
		"mov R2, [R1+1]"
		"mov [R0], R2"
		"mov R2, [R1]"
		"fsgn R2"
		"mov [R0+1], R2"
	}
#else
	right->x = v->y;
	right->y = -v->x;
#endif
}

#endif