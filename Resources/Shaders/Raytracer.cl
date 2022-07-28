
float3 MatrixTransformPoint(float16 l, float3 r) {
	return (float3) {
		r.x * l.s0 + r.y * l.s4 + r.z * l.s8 + l.sC,
		r.x * l.s1 + r.y * l.s5 + r.z * l.s9 + l.sD,
		r.x * l.s2 + r.y * l.s6 + r.z * l.sA + l.sE,
	};
}

void RayBox(float3 r, float3 o, float3 bmin, float3 bmax, float * enter, float * exit) {
	float3 inv = 1.0 / r;
	float3 t1 = (bmin - o) * inv;
	float3 t2 = (bmax - o) * inv;
	float3 tn = fmin(t1, t2);
	float3 tf = fmax(t1, t2);
	*enter = fmax(tn.x, fmax(tn.y, tn.z));
	*exit = fmin(tf.x, fmin(tf.y, tf.z));
}

bool RayBoxIntersection(float3 r, float3 o, float3 bmin, float3 bmax, float * dist) {
	float n, f;
	RayBox(r, o, bmin, bmax, &n, &f);
	*dist = n;
	return f > n && f > 0.0;
}

float3 BoxNormal(float3 hit, float3 bmin, float3 bmax) {
	return normalize(round((hit - (bmin + bmax) / 2.0) / (fabs(bmin - bmax)) * 1.0001));
}

bool RayTreeIntersection(__global uchar * octree, __global uchar * blocks, float3 ray, float3 origin, float size, float depth, float3 * hit, uchar * tile, float3 * normal) {
	float dist;
	if (!RayBoxIntersection(ray, origin, (float3){ 0.0, 0.0, 0.0 }, (float3){ 1.0, 1.0, 1.0 } * size, &dist)) {
		*hit = ray * dist + origin;
		//*tile = 255;
		return false;
	}
	if (dist > depth) {
		*hit = ray * dist + origin;
		*tile = 254;
		return false;
	}

	*hit = origin + max(dist, 0.0) * ray;
	float3 base = { 0.0, 0.0, 0.0 };
	float mid = size / 2.0;
	int level = 0;
	int offset = 0;
	while (level < 8) {
		uchar mask = octree[(int)((pow(8.0, float(level)) - 1.0) / 7.0) + offset];
		uint q = (hit->x > base.x + mid) + 2 * (hit->y > base.y + mid) + 4 * (hit->z > base.z + mid);
		base += mid * convert_float3(((uint3){ q, q, q } >> (uint3){ 0, 1, 2 }) & 1);

		if (((mask >> q) & 1) == 0) {
			float enter, exit;
			RayBox(ray, *hit, base, base + mid, &enter, &exit);
			//if (exit - enter < 0.05) { *tile = 253; break; }
			*hit += exit * ray + sign(ray) * size * 0.000001;

			if (hit->x >= size || hit->y >= size || hit->z >= size || hit->x <= 0.0 || hit->y <= 0.0 || hit->z <= 0.0) {
				return false;
			}

			base = (float3){ 0.0, 0.0, 0.0 };
			mid = size / 2.0;
			offset = 0;
			level = 0;
			continue;
		}

		offset = 8 * offset + int(q);

		if (level == 7) {
			*hit -= 0.000001 * sign(ray) * size;
			*normal = BoxNormal(*hit, base, base + mid);
			int3 v = convert_int3(base);
			if (v.x >= 0 && v.y >= 0 && v.z >= 0 && v.x < 256 && v.y < 64 && v.z < 256) {
				*tile = blocks[(v.y * 256 + v.z) * 256 + 256];
				return true;
			} else {
				*tile = 0;
				return false;
			}
		}

		mid /= 2.0;
		level++;
	}
	return false;
}

float3 BGColor(float3 ray) {
	return (float3){ 0.6, 0.8, 1.0 };
	//return ray;
	//return (float3){ 0.0, 0.0, 0.0 };
}

__kernel void trace(uint treeDepth, __global uchar * octree, __global uchar * blocks, __write_only image2d_t texture, float16 camera) {
	int x = get_global_id(0);
	int y = get_global_id(1);
	int width = get_global_size(0);
	int height = get_global_size(1);
	float2 uv = (float2){ 1.0 - 2.0 * (float)x / width, 2.0 * (float)y / height - 1.0 };
	uv.x *= (float)width / height;

	float fov = 70.0;
	float3 origin = MatrixTransformPoint(camera, (float3){ 0.0, 0.0, 0.0 });
	float3 ray = normalize(MatrixTransformPoint(camera, (float3){ uv * 0.5, 0.5 / tanpi(fov / 360.0) }) - origin);

	float4 finalColor = { BGColor(ray), 1.0 };
	float depth = 1.0 / 0.0;
	float3 lightPos = { 50.0, 50.0, -200.0 };

	float3 hit, normal;
	uchar tile;
	if (RayTreeIntersection(octree, blocks, ray, origin, 256.0, depth, &hit, &tile, &normal) && distance(hit, origin) < depth) {
		//finalColor.xyz = (float3){ 1.0, 1.0, 1.0 };
		depth = distance(hit, origin);
		float3 lightDir = normalize(lightPos - hit);
		float diff = max(dot(lightDir, normal), -1.0);
		finalColor.xyz = (float3){ 1.0, 1.0, 1.0 } * (diff * 0.25 + 0.75);

		/*float3 shadowHit;
		if (RayTreeIntersection(octree, blocks, lightDir, hit + 0.001 * lightDir, 256.0, distance(hit, lightPos), &shadowHit, &tile, &normal))
		{
			finalColor.xyz *= 0.7;
		}*/

		float w = clamp(depth / 200.0, 0.0, 1.0);
		finalColor.xyz = finalColor.xyz * (1.0 - w) + BGColor(ray) * w;
	}
	if (tile == 255) { finalColor.xyz = (float3){ 1.0, 0.0, 0.0 }; }
	if (tile == 254) { finalColor.xyz = (float3){ 0.0, 1.0, 0.0 }; }
	if (tile == 253) { finalColor.xyz = (float3){ 0.0, 0.0, 0.0 }; }
	write_imagef(texture, (int2){ x, y }, finalColor);
}
