
float3 MatrixTransformPoint(float16 l, float3 r) {
	return (float3) {
		r.x * l.s0 + r.y * l.s4 + r.z * l.s8 + l.sC,
		r.x * l.s1 + r.y * l.s5 + r.z * l.s9 + l.sD,
		r.x * l.s2 + r.y * l.s6 + r.z * l.sA + l.sE,
	};
}

void RayBox(float3 r, float3 o, float3 bmin, float3 bmax, float * enter, float * exit) {
	float3 inv = 1.0f / r;
	float3 t1 = (bmin - o) * inv;
	float3 t2 = (bmax - o) * inv;
	float3 tn = fmin(t1, t2);
	float3 tf = fmax(t1, t2);
	*enter = fmax(tn.x, fmax(tn.y, tn.z));
	*exit = fmin(tf.x, fmin(tf.y, tf.z));
}

float3 BGColor(float3 ray) {
	float t = 1.0f - (1.0f - ray.y) * (1.0f - ray.y);
	return t * (float3){ 0.63f, 0.8f, 1.0f } + (1.0f - t) * (float3){ 1.0f, 1.0f, 1.0f };
}

bool PointInBounds(float3 v, int levelSize) {
	return v.x >= 0 && v.y >= 0 && v.z >= 0 && v.x < levelSize && v.y < 64 && v.z < levelSize;
}

__kernel void trace(int levelSize, __global uchar * distanceField, __global uchar * blocks, __write_only image2d_t texture, int width, int height, float16 camera, __read_only image2d_t terrain, int isUnderWater, float time) {
	int x = get_global_id(0);
	int y = get_global_id(1);
	if (x >= width || y >= height) {
		return;
	}
	float2 uv = (float2){ 1.0f - 2.0f * (float)x / width, 2.0f * (float)y / height - 1.0f };
	uv.x *= (float)width / height;

	float fov = 70.0f;
	float3 origin = MatrixTransformPoint(camera, (float3){ 0.0f, 0.0f, 0.0f });
	float3 ray = normalize(MatrixTransformPoint(camera, (float3){ uv * 0.5f, 0.5f / tanpi(fov / 360.0f) }) - origin);
	float4 fragColor = { BGColor(ray), 1.0f };
	
	float t = 0;
	int i = 0;
	while (PointInBounds(origin + ray * t, levelSize) && i < 1024) {
		int3 voxel = convert_int3(origin + ray * t);
		uchar dist = distanceField[(voxel.y * levelSize + voxel.z) * levelSize + voxel.x];
		if (dist == 0) {
			fragColor.xyz = (float3){ 1.0, 1.0f, 1.0f} * (float)i / 64.0;
			break;
		}
		float enter, exit;
		RayBox(ray, origin, convert_float3(voxel - (dist - 1)), convert_float3(voxel + dist), &enter, &exit);
		if (exit - t < 0.001) {
			exit = t + 0.001;
		}
		t = exit + 0.0001;
		
		i += 1;
		if (i == 1024) {
			fragColor.xyz = (float3){ 1.0, 0.0, 0.0 };
		}
	}
	write_imagef(texture, (int2){ x, y }, (float4){ fragColor.xyz, 1.0f });
}
