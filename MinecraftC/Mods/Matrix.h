#pragma once
#if MINECRAFTC_MODS

typedef struct Matrix4x4 {
	float m00, m10, m20, m30;
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;
} Matrix4x4;

#define Matrix4x4Identity (Matrix4x4){ 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 }

static inline Matrix4x4 Matrix4x4Multiply(Matrix4x4 l, Matrix4x4 r) {
	return (Matrix4x4){
		r.m00 * l.m00 + r.m10 * l.m01 + r.m20 * l.m02 + r.m30 * l.m03,
		r.m00 * l.m10 + r.m10 * l.m11 + r.m20 * l.m12 + r.m30 * l.m13,
		r.m00 * l.m20 + r.m10 * l.m21 + r.m20 * l.m22 + r.m30 * l.m23,
		r.m00 * l.m30 + r.m10 * l.m31 + r.m20 * l.m32 + r.m30 * l.m33,
		r.m01 * l.m00 + r.m11 * l.m01 + r.m21 * l.m02 + r.m31 * l.m03,
		r.m01 * l.m10 + r.m11 * l.m11 + r.m21 * l.m12 + r.m31 * l.m13,
		r.m01 * l.m20 + r.m11 * l.m21 + r.m21 * l.m22 + r.m31 * l.m23,
		r.m01 * l.m30 + r.m11 * l.m31 + r.m21 * l.m32 + r.m31 * l.m33,
		r.m02 * l.m00 + r.m12 * l.m01 + r.m22 * l.m02 + r.m32 * l.m03,
		r.m02 * l.m10 + r.m12 * l.m11 + r.m22 * l.m12 + r.m32 * l.m13,
		r.m02 * l.m20 + r.m12 * l.m21 + r.m22 * l.m22 + r.m32 * l.m23,
		r.m02 * l.m30 + r.m12 * l.m31 + r.m22 * l.m32 + r.m32 * l.m33,
		r.m03 * l.m00 + r.m13 * l.m01 + r.m23 * l.m02 + r.m33 * l.m03,
		r.m03 * l.m10 + r.m13 * l.m11 + r.m23 * l.m12 + r.m33 * l.m13,
		r.m03 * l.m20 + r.m13 * l.m21 + r.m23 * l.m22 + r.m33 * l.m23,
		r.m03 * l.m30 + r.m13 * l.m31 + r.m23 * l.m32 + r.m33 * l.m33,
	};
}

static inline Matrix4x4 Matrix4x4FromTranslate(float vx, float vy, float vz) {
	return (Matrix4x4){ 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, vx, vy, vz, 1.0 };
}

static inline Matrix4x4 Matrix4x4FromAxisAngle(float vx, float vy, float vz, float a) {
	float c = cosf(a), c2 = 1.0 - cosf(a), s = sinf(a), xx = vx * vx, xy = vx * vy, xz = vx * vz, yy = vy * vy, yz = vy * vz, zz = vz * vz;
	return (Matrix4x4){
		(xx + (yy + zz) * c), (xy * c2 + vz * s), (xz * c2 - vy * s), 0.0f,
		(xy * c2 - vz * s), (yy + (xx + zz) * c), (yz * c2 + vx * s), 0.0f,
		(xz * c2 + vy * s), (yz * c2 - vx * s), (zz + (xx + yy) * c), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

static inline Matrix4x4 Matrix4x4FromEulerAngles(float vx, float vy, float vz) {
	Matrix4x4 matrix = Matrix4x4Identity;
	matrix = Matrix4x4Multiply(Matrix4x4FromAxisAngle(0.0, 0.0, 1.0, vz), matrix);
	matrix = Matrix4x4Multiply(Matrix4x4FromAxisAngle(1.0, 0.0, 0.0, vy), matrix);
	matrix = Matrix4x4Multiply(Matrix4x4FromAxisAngle(0.0, 1.0, 0.0, vx), matrix);
	return matrix;
}

#endif
