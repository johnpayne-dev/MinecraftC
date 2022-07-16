#pragma once

#define Vector3DNull (float3){ INFINITY, INFINITY, INFINITY }

typedef struct Vec3D {
	float x, y, z;
} Vec3D;

static inline float3 Vector3DGetXIntersection(float3 v0, float3 v1, float s) {
	float3 d = v1 - v0;
	s = (s - v0.x) / d.x;
	return d.x * d.x < 1.0E-7 ? Vector3DNull : (s >= 0.0 && s <= 1.0 ? v0 + d * s : Vector3DNull);
}

static inline float3 Vector3DGetYIntersection(float3 v0, float3 v1, float s) {
	float3 d = v1 - v0;
	s = (s - v0.y) / d.y;
	return d.y * d.y < 1.0E-7 ? Vector3DNull : (s >= 0.0 && s <= 1.0 ? v0 + d * s : Vector3DNull);
}

static inline float3 Vector3DGetZIntersection(float3 v0, float3 v1, float s) {
	float3 d = v1 - v0;
	s = (s - v0.z) / d.z;
	return d.z * d.z < 1.0E-7 ? Vector3DNull : (s >= 0.0 && s <= 1.0 ? v0 + d * s : Vector3DNull);
}

static inline bool Vector3DIsNull(float3 v) { return v.x == INFINITY && v.y == INFINITY && v.z == INFINITY; }
