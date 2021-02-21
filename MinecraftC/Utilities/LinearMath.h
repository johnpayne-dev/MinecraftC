#pragma once
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

typedef float __attribute__((ext_vector_type(4))) float4;
typedef float __attribute__((ext_vector_type(3))) float3;
typedef float __attribute__((ext_vector_type(2))) float2;
typedef int32_t __attribute__((ext_vector_type(2))) int2;
typedef int32_t __attribute__((ext_vector_type(3))) int3;
typedef int32_t __attribute__((ext_vector_type(4))) int4;

#define true 1
#define false 0
extern const float pi;
#define rad (pi / 180.0)
#define deg (180.0 / pi)
#define zero2f (float2){ 0.0, 0.0 }
#define one2f (float2){ 1.0, 1.0 }
#define up2f (float2){ 0.0, 1.0 }
#define right2f (float2){ 1.0, 0.0 }
#define zero3f (float3){ 0.0, 0.0, 0.0 }
#define one3f (float3){ 1.0, 1.0, 1.0 }
#define up3f (float3){ 0.0, 1.0, 0.0 }
#define right3f (float3){ 1.0, 0.0, 0.0 }
#define forward3f (float3){ 0.0, 0.0, 1.0 }
#define zero4f (float4){ 0.0, 0.0, 0.0, 0.0 }
#define one4f (float4){ 1.0, 1.0, 1.0, 1.0 }

static inline int3 int3f(float3 v) { return (int3){ (int)v.x, (int)v.y, (int)v.z }; }
static inline float3 float3i(int3 v) { return (float3){ v.x, v.y, v.z }; }
static inline float2 float2i(int2 v) { return (float2){ v.x, v.y }; }

float dot2f(float2 v1, float2 v2);

float length2f(float2 v);

float angle2f(float2 v1, float2 v2);

float distance2f(float2 v1, float2 v2);

float sqdistance2f(float2 v1, float2 v2);

float2 normalize2f(float2 v);

float3 cross3f(float3 v1, float3 v2);

float dot3f(float3 v1, float3 v2);

float length3f(float3 v);

float angle3f(float3 v1, float3 v2);

float distance3f(float3 v1, float3 v2);

float sqdistance3f(float3 v1, float3 v2);

float3 normalize3f(float3 v);

float3 normal3f(float3 A, float3 B, float3 C);

float dot4f(float4 v1, float4 v2);

float length4f(float4 v);

float distance4f(float4 v1, float4 v2);

float sqdistance4f(float4 v1, float4 v2);

float4 normalize4f(float4 v);

typedef struct Matrix4x4
{
	float M00, M10, M20, M30;
	float M01, M11, M21, M31;
	float M02, M12, M22, M32;
	float M03, M13, M23, M33;
} Matrix4x4;

#define Matrix4x4Identity (Matrix4x4){ 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 }

Matrix4x4 Matrix4x4Multiply(Matrix4x4 l, Matrix4x4 r);

float4 Matrix4x4MultiplyFloat4(Matrix4x4 l, float4 r);

float3 Matrix4x4MultiplyFloat3(Matrix4x4 l, float3 r);

Matrix4x4 Matrix4x4FromTranslate(float3 v);

Matrix4x4 Matrix4x4FromScale(float3 v);

Matrix4x4 Matrix4x4FromAxisAngle(float3 v, float a);

Matrix4x4 Matrix4x4FromEulerAngles(float3 v);

Matrix4x4 Matrix4x4FromPerspective(float fov, float aspect, float near, float far);

Matrix4x4 Matrix4x4FromLookAt(float3 position, float3 target, float3 up);

Matrix4x4 Matrix4x4FromDirection(float3 direction, float3 up);

typedef unsigned char __attribute__((ext_vector_type(4))) Color;

#define ColorBlack (Color){ 0x00, 0x00, 0x00, 0xff }
#define ColorWhite (Color){ 0xff, 0xff, 0xff, 0xff }
#define ColorRed (Color){ 0xff, 0x00, 0x00, 0xff }
#define ColorGreen (Color){ 0x00, 0xff, 0x00, 0xff }
#define ColorBlue (Color){ 0x00, 0x00, 0xff, 0xff }

Color ColorFromHex(unsigned int hex);

Color ColorFromFloat4(float4 rgba);

float4 ColorToFloat4(Color color);
