#ifndef __RECT_H__
#define __RECT_H__

struct rect_f {
	float left;
	float top;
	float right;
	float bottom;
};

void rect_set(rect_f* rect, int left, int top, int right, int bottom)
{
	rect->left = left;
	rect->top = top;
	rect->right = right;
	rect->bottom = bottom;
}

bool rect_intersects(rect_f* r1, rect_f* r2)
{
	return !((r1->left < r2->right) &&
		(r1->right > r2->left) &&
		(r1->top > r2->bottom) &&
		(r1->bottom < r2->top));
}

#endif
