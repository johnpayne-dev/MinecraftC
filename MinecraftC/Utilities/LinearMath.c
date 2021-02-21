#include "LinearMath.h"

const float pi = 3.14159265358979323846;

float dot2f(float2 v1, float2 v2) { return v1.x * v2.x + v1.y * v2.y; }

float length2f(float2 v) { return sqrtf(v.x * v.x + v.y * v.y); }

float angle2f(float2 v1, float2 v2) { return acosf(dot2f(v1, v2) / (length2f(v1) * length2f(v2))); }

float distance2f(float2 v1, float2 v2) { return length2f(v2 - v1); }

float sqdistance2f(float2 v1, float2 v2) { return dot2f(v2 - v1, v2 - v1); }

float2 normalize2f(float2 v) { return v / length2f(v); }

float3 cross3f(float3 v1, float3 v2) { return (float3){ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x }; }

float dot3f(float3 v1, float3 v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

float length3f(float3 v) { return sqrtf(dot3f(v, v)); }

float angle3f(float3 v1, float3 v2) { return acosf(dot3f(v1, v2) / (length3f(v1) * length3f(v2))); }

float distance3f(float3 v1, float3 v2) { return length3f(v2 - v1); }

float sqdistance3f(float3 v1, float3 v2) { return dot3f(v2 - v1, v2 - v1); }

float3 normalize3f(float3 v) { return v / length3f(v); }

float3 normal3f(float3 A, float3 B, float3 C) { return cross3f(B - A, C - A); }

float dot4f(float4 v1, float4 v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

float length4f(float4 v) { return sqrtf(dot4f(v, v)); }

float distance4f(float4 v1, float4 v2) { return length4f(v2 - v1); }

float sqdistance4f(float4 v1, float4 v2) { return dot4f(v2 - v1, v2 - v1); }

float4 normalize4f(float4 v) { return v / length4f(v); }

Matrix4x4 Matrix4x4Multiply(Matrix4x4 l, Matrix4x4 r)
{
	return (Matrix4x4)
	{
		r.M00 * l.M00 + r.M10 * l.M01 + r.M20 * l.M02 + r.M30 * l.M03,
		r.M00 * l.M10 + r.M10 * l.M11 + r.M20 * l.M12 + r.M30 * l.M13,
		r.M00 * l.M20 + r.M10 * l.M21 + r.M20 * l.M22 + r.M30 * l.M23,
		r.M00 * l.M30 + r.M10 * l.M31 + r.M20 * l.M32 + r.M30 * l.M33,
		r.M01 * l.M00 + r.M11 * l.M01 + r.M21 * l.M02 + r.M31 * l.M03,
		r.M01 * l.M10 + r.M11 * l.M11 + r.M21 * l.M12 + r.M31 * l.M13,
		r.M01 * l.M20 + r.M11 * l.M21 + r.M21 * l.M22 + r.M31 * l.M23,
		r.M01 * l.M30 + r.M11 * l.M31 + r.M21 * l.M32 + r.M31 * l.M33,
		r.M02 * l.M00 + r.M12 * l.M01 + r.M22 * l.M02 + r.M32 * l.M03,
		r.M02 * l.M10 + r.M12 * l.M11 + r.M22 * l.M12 + r.M32 * l.M13,
		r.M02 * l.M20 + r.M12 * l.M21 + r.M22 * l.M22 + r.M32 * l.M23,
		r.M02 * l.M30 + r.M12 * l.M31 + r.M22 * l.M32 + r.M32 * l.M33,
		r.M03 * l.M00 + r.M13 * l.M01 + r.M23 * l.M02 + r.M33 * l.M03,
		r.M03 * l.M10 + r.M13 * l.M11 + r.M23 * l.M12 + r.M33 * l.M13,
		r.M03 * l.M20 + r.M13 * l.M21 + r.M23 * l.M22 + r.M33 * l.M23,
		r.M03 * l.M30 + r.M13 * l.M31 + r.M23 * l.M32 + r.M33 * l.M33,
	};
}

float4 Matrix4x4MultiplyFloat4(Matrix4x4 l, float4 r)
{
	return (float4)
	{
		r.x * l.M00 + r.y * l.M01 + r.z * l.M02 + r.w * l.M03,
		r.x * l.M10 + r.y * l.M11 + r.z * l.M12 + r.w * l.M13,
		r.x * l.M20 + r.y * l.M21 + r.z * l.M22 + r.w * l.M23,
		r.x * l.M30 + r.y * l.M31 + r.z * l.M32 + r.w * l.M33,
	};
}

float3 Matrix4x4MultiplyFloat3(Matrix4x4 l, float3 r)
{
	return (float3)
	{
		r.x * l.M00 + r.y * l.M01 + r.z * l.M02 + l.M03,
		r.x * l.M10 + r.y * l.M11 + r.z * l.M12 + l.M13,
		r.x * l.M20 + r.y * l.M21 + r.z * l.M22 + l.M23,
	};
}

Matrix4x4 Matrix4x4FromTranslate(float3 v)
{
	return (Matrix4x4) { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, v.x, v.y, v.z, 1.0 };
}

Matrix4x4 Matrix4x4FromScale(float3 v)
{
	return (Matrix4x4) { v.x, 0.0, 0.0, 0.0, 0.0, v.y, 0.0, 0.0, 0.0, 0.0, v.z, 0.0, 0.0, 0.0, 0.0, 1.0 };
}

Matrix4x4 Matrix4x4FromAxisAngle(float3 v, float a)
{
	v = normalize3f(v);
	float c = cosf(a), c2 = 1.0 - cosf(a), s = sinf(a), xx = v.x * v.x, xy = v.x * v.y, xz = v.x * v.z, yy = v.y * v.y, yz = v.y * v.z, zz = v.z * v.z;
	return (Matrix4x4)
	{
		(xx + (yy + zz) * c), (xy * c2 + v.z * s), (xz * c2 - v.y * s), 0.0f,
		(xy * c2 - v.z * s), (yy + (xx + zz) * c), (yz * c2 + v.x * s), 0.0f,
		(xz * c2 + v.y * s), (yz * c2 - v.x * s), (zz + (xx + yy) * c), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4x4 Matrix4x4FromEulerAngles(float3 v)
{
	Matrix4x4 matrix = Matrix4x4Identity;
	matrix = Matrix4x4Multiply(Matrix4x4FromAxisAngle((float3){ 0.0, 0.0, 1.0 }, v.z), matrix);
	matrix = Matrix4x4Multiply(Matrix4x4FromAxisAngle((float3){ 1.0, 0.0, 0.0 }, v.y), matrix);
	matrix = Matrix4x4Multiply(Matrix4x4FromAxisAngle((float3){ 0.0, 1.0, 0.0 }, v.x), matrix);
	return matrix;
}

Matrix4x4 Matrix4x4FromPerspective(float fov, float aspect, float near, float far)
{
	float f = 1.0f / tan(0.5f * fov);
	return (Matrix4x4)
	{
		f / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, -f, 0.0f, 0.0f,
		0.0f, 0.0f, -far / (near - far), 1.0f,
		0.0f, 0.0f, (near * far) / (near - far), 0.0f
	};
}

Matrix4x4 Matrix4x4FromLookAt(float3 position, float3 target, float3 up)
{
	float3 f = normalize3f(target - position);
	float3 r = normalize3f(cross3f(up, f));
	float3 u = normalize3f(cross3f(f, r));
	return (Matrix4x4){ r.x, u.x, f.x, 0.0, r.y, u.y, f.y, 0.0, r.z, u.z, f.z, 0.0, -dot3f(r, position), -dot3f(u, position), -dot3f(f, position), 1.0 };
}

Matrix4x4 Matrix4x4FromDirection(float3 direction, float3 up)
{
	float3 f = normalize3f(direction);
	float3 r = normalize3f(cross3f(f, up));
	float3 u = normalize3f(cross3f(f, r));
	return (Matrix4x4){ r.x, u.x, f.x, 0.0, r.y, u.y, f.y, 0.0, r.z, u.z, f.z, 0.0, 0.0, 0.0, 0.0, 1.0 };
}

Color ColorFromHex(unsigned int hex)
{
	return (Color){ (hex & 0xff000000) >> 24, (hex & 0x00ff0000) >> 16, (hex & 0x0000ff00) >> 8, (hex & 0x000000ff) >> 0 };
}

Color ColorFromFloat4(float4 rgba)
{
	rgba *= 255.0;
	return (Color){ (unsigned char)rgba.x, (unsigned char)rgba.y, (unsigned char)rgba.z, (unsigned char)rgba.w, };
}

float4 ColorToFloat4(Color color)
{
	return (float4){ color.r, color.g, color.b, color.a } / 256.0;
}
