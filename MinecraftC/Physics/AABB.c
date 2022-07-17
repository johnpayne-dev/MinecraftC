#include "AABB.h"
#include "Vector3D.h"

static float epsilon = 0.0;

AABB AABBExpand(AABB c, float x, float y, float z) {
	c.x0 += x < 0.0 ? x : 0.0;
	c.y0 += y < 0.0 ? y : 0.0;
	c.z0 += z < 0.0 ? z : 0.0;
	c.x1 += x > 0.0 ? x : 0.0;
	c.y1 += y > 0.0 ? y : 0.0;
	c.z1 += z > 0.0 ? z : 0.0;
	return c;
}

AABB AABBGrow(AABB c, float x, float y, float z) {
	return (AABB){ .x0 = c.x0 - x, .y0 = c.y0 - y, .z0 = c.z0 - z, .x1 = c.x1 + x, .y1 = c.y1 + y, .z1 = c.z1 + z };
}

AABB AABBMove(AABB c, float x, float y, float z) {
	return (AABB){ .x0 = c.x0 + x, .y0 = c.y0 + y, .z0 = c.z0 + z, .x1 = c.x1 + x, .y1 = c.y1 + y, .z1 = c.z1 + z };
}

float AABBClipXCollide(AABB c0, AABB c1, float xa) {
	if (c1.y1 > c0.y0 && c1.y0 < c0.y1) {
		if (c1.z1 > c0.z0 && c1.z0 < c0.z1) {
			float max = c0.x0 - c1.x1 - epsilon;
			if (xa > 0.0 && c1.x1 <= c0.x0 && max < xa) { xa = max; }
			max = c0.x1 - c1.x0 + epsilon;
			if (xa < 0.0 && c1.x0 >= c0.x1 && max > xa) { xa = max; }
			return xa;
		} else { return xa; }
	} else { return xa; }
}

float AABBClipYCollide(AABB c0, AABB c1, float ya) {
	if (c1.x1 > c0.x0 && c1.x0 < c0.x1) {
		if (c1.z1 > c0.z0 && c1.z0 < c0.z1) {
			float max = c0.y0 - c1.y1 - epsilon;
			if (ya > 0.0 && c1.y1 <= c0.y0 && max < ya) { ya = max; }
			max = c0.y1 - c1.y0 + epsilon;
			if (ya < 0.0 && c1.y0 >= c0.y1 && max > ya) { ya = max; }
			return ya;
		} else { return ya; }
	} else { return ya; }
}

float AABBClipZCollide(AABB c0, AABB c1, float za) {
	if (c1.x1 > c0.x0 && c1.x0 < c0.x1) {
		if (c1.y1 > c0.y0 && c1.y0 < c0.y1) {
			float max = c0.z0 - c1.z1 - epsilon;
			if (za > 0.0 && c1.z1 <= c0.z0 && max < za) { za = max; }
			max = c0.z1 - c1.z0 + epsilon;
			if (za < 0.0 && c1.z0 >= c0.z1 && max > za) { za = max; }
			return za;
		} else { return za; }
	} else { return za; }
}

bool AABBIntersects(AABB c0, AABB c1) {
	return c1.x1 > c0.x0 && c1.x0 < c0.x1 ? (c1.y1 > c0.y0 && c1.y0 < c0.y1 ? c1.z1 > c0.z0 && c1.z0 < c0.z1 : false) : false;
}

bool AABBIntersectsInner(AABB c0, AABB c1) {
	return c1.x1 >= c0.x0 && c1.x0 <= c0.x1 ? (c1.y1 >= c0.y0 && c1.y0 <= c0.y1 ? c1.z1 >= c0.z0 && c1.z0 <= c0.z1 : false) : false;
}

bool AABBContainsPoint(AABB c, Vector3D p) {
	return p.x > c.x0 && p.x < c.x1 ? (p.y > c.y0 && p.y < c.y1 ? p.z > c.z0 && p.z < c.z1 : false) : false;
}

float AABBGetSize(AABB c) {
	return (c.x1 - c.x0 + c.y1 - c.y0 + c.z1 - c.z0) / 3.0;
}

AABB AABBShrink(AABB c, float x, float y, float z) {
	c.x0 -= x < 0.0 ? x : 0.0;
	c.y0 -= y < 0.0 ? y : 0.0;
	c.z0 -= z < 0.0 ? z : 0.0;
	c.x1 -= x > 0.0 ? x : 0.0;
	c.y1 -= y > 0.0 ? y : 0.0;
	c.z1 -= z > 0.0 ? z : 0.0;
	return c;
}

static bool XIntersects(AABB c, Vector3D v) {
	return Vector3DIsNull(v) ? false : v.y >= c.y0 && v.y <= c.y1 && v.z >= c.z0 && v.z <= c.z1;
}

static bool YIntersects(AABB c, Vector3D v) {
	return Vector3DIsNull(v) ? false : v.x >= c.x0 && v.x <= c.x1 && v.z >= c.z0 && v.z <= c.z1;
}

static bool ZIntersects(AABB c, Vector3D v) {
	return Vector3DIsNull(v) ? false : v.x >= c.x0 && v.x <= c.x1 && v.y >= c.y0 && v.y <= c.y1;
}

MovingObjectPosition AABBClip(AABB c, Vector3D v1, Vector3D v2) {
	Vector3D f0 = Vector3DGetXIntersection(v1, v2, c.x0);
	Vector3D f1 = Vector3DGetXIntersection(v1, v2, c.x1);
	Vector3D f2 = Vector3DGetYIntersection(v1, v2, c.y0);
	Vector3D f3 = Vector3DGetYIntersection(v1, v2, c.y1);
	Vector3D f4 = Vector3DGetZIntersection(v1, v2, c.z0);
	Vector3D f5 = Vector3DGetZIntersection(v1, v2, c.z1);
	if (!XIntersects(c, f0)) { f0 = Vector3DNull; }
	if (!XIntersects(c, f1)) { f1 = Vector3DNull; }
	if (!YIntersects(c, f2)) { f2 = Vector3DNull; }
	if (!YIntersects(c, f3)) { f3 = Vector3DNull; }
	if (!ZIntersects(c, f4)) { f4 = Vector3DNull; }
	if (!ZIntersects(c, f5)) { f5 = Vector3DNull; }
	Vector3D f6 = Vector3DNull;
	if (!Vector3DIsNull(f0)) { f6 = f0; }
	if (!Vector3DIsNull(f1) && (Vector3DIsNull(f6) || Vector3DSqDistance(v1, f1) < Vector3DSqDistance(v1, f6))) { f6 = f1; }
	if (!Vector3DIsNull(f2) && (Vector3DIsNull(f6) || Vector3DSqDistance(v1, f2) < Vector3DSqDistance(v1, f6))) { f6 = f2; }
	if (!Vector3DIsNull(f3) && (Vector3DIsNull(f6) || Vector3DSqDistance(v1, f3) < Vector3DSqDistance(v1, f6))) { f6 = f3; }
	if (!Vector3DIsNull(f4) && (Vector3DIsNull(f6) || Vector3DSqDistance(v1, f4) < Vector3DSqDistance(v1, f6))) { f6 = f4; }
	if (!Vector3DIsNull(f5) && (Vector3DIsNull(f6) || Vector3DSqDistance(v1, f5) < Vector3DSqDistance(v1, f6))) { f6 = f5; }
	if (Vector3DIsNull(f6)) { return (MovingObjectPosition){ .null = true }; }
	int face = -1;
	if (f6.x == f0.x && f6.y == f0.y && f6.z == f0.z) { face = 4; }
	if (f6.x == f1.x && f6.y == f1.y && f6.z == f1.z) { face = 5; }
	if (f6.x == f2.x && f6.y == f2.y && f6.z == f2.z) { face = 0; }
	if (f6.x == f3.x && f6.y == f3.y && f6.z == f3.z) { face = 1; }
	if (f6.x == f4.x && f6.y == f4.y && f6.z == f4.z) { face = 2; }
	if (f6.x == f5.x && f6.y == f5.y && f6.z == f5.z) { face = 3; }
	return (MovingObjectPosition){ .face = face, .vector = f6 };
}

bool AABBIsNull(AABB c) {
	return c.x0 == 0.0 && c.y0 == 0.0 && c.z0 == 0.0 && c.x1 == 0.0 && c.y1 == 0.0 && c.z1 == 0.0;
}

