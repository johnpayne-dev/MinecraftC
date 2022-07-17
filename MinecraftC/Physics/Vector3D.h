#pragma once
#include <math.h>

#define Vector3DNull (Vector3D){ INFINITY, INFINITY, INFINITY }

typedef struct Vector3D {
	float x, y, z;
} Vector3D;

static inline Vector3D Vector3DAdd(Vector3D a, Vector3D b) {
	return (Vector3D){ a.x + b.x, a.y + b.y, a.z + b.z };
}

static inline Vector3D Vector3DSubtract(Vector3D a, Vector3D b) {
	return (Vector3D){ a.x - b.x, a.y - b.y, a.z - b.z };
}

static inline float Vector3DLength(Vector3D a) {
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

static inline float Vector3DSqDistance(Vector3D a, Vector3D b) {
	return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.x - a.x);
}

static inline Vector3D Vector3DNormalize(Vector3D a) {
	float len = Vector3DLength(a);
	return (Vector3D){ a.x / len, a.y / len, a.z / len };
}

static inline Vector3D Vector3DGetXIntersection(Vector3D v0, Vector3D v1, float s) {
	Vector3D d = Vector3DSubtract(v1, v0);
	s = (s - v0.x) / d.x;
	return d.x * d.x < 1.0E-7 ? Vector3DNull : (s >= 0.0 && s <= 1.0 ? (Vector3D){ v0.x + d.x * s, v0.y + d.y * s, v0.z + d.z * s } : Vector3DNull);
}

static inline Vector3D Vector3DGetYIntersection(Vector3D v0, Vector3D v1, float s) {
	Vector3D d = Vector3DSubtract(v1, v0);
	s = (s - v0.y) / d.y;
	return d.y * d.y < 1.0E-7 ? Vector3DNull : (s >= 0.0 && s <= 1.0 ? (Vector3D){ v0.x + d.x * s, v0.y + d.y * s, v0.z + d.z * s } : Vector3DNull);
}

static inline Vector3D Vector3DGetZIntersection(Vector3D v0, Vector3D v1, float s) {
	Vector3D d = Vector3DSubtract(v1, v0);
	s = (s - v0.z) / d.z;
	return d.z * d.z < 1.0E-7 ? Vector3DNull : (s >= 0.0 && s <= 1.0 ? (Vector3D){ v0.x + d.x * s, v0.y + d.y * s, v0.z + d.z * s } : Vector3DNull);
}

static inline bool Vector3DIsNull(Vector3D v) {
	return v.x == INFINITY && v.y == INFINITY && v.z == INFINITY;
}
