#pragma once
#include "Structures.h"
struct CatmullRomSegment {
	Vector3 a;
	Vector3 b;
	Vector3 c;
	Vector3 d;
};

static class Math
{
public:
	static float radians(float degrees);

	static float degrees(float radians);

	//Source: http://paulbourke.net/miscellaneous/interpolation/
	static float LinearInterpolate(float y1, float y2, float alpha);

	static float CosineInterpolate(float y1, float y2, float alpha);

	static float CubicInterpolate(float y0, float y1, float y2, float y3, float alpha);

	static float HermiteInterpolate(float y0, float y1, float y2, float y3, float alpha, float tension, float bias);

	static float distance(Vector3 a, Vector3 b);

	static Vector3 CatmullRomInterpolate(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4, float alpha, float tension);

	static Vector3 GetForwardPosition(float Distance, Vector3 pos, Vector3 rotation);

	static Vector3 LookAt(Vector3 current, Vector3 target);
};

