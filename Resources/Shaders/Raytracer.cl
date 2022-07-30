#define EPSILON      0.0001f
#define MIN_RAY_STEP 0.001f

#define BlockTypeNone              0
#define BlockTypeStone             1
#define BlockTypeGrass             2
#define BlockTypeDirt              3
#define BlockTypeCobbleStone       4
#define BlockTypeWood              5
#define BlockTypeSapling           6
#define BlockTypeBedrock           7
#define BlockTypeWater             8
#define BlockTypeStillWater        9
#define BlockTypeLava              10
#define BlockTypeStillLava         11
#define BlockTypeSand              12
#define BlockTypeGravel            13
#define BlockTypeGoldOre           14
#define BlockTypeIronOre           15
#define BlockTypeCoalOre           16
#define BlockTypeLog               17
#define BlockTypeLeaves            18
#define BlockTypeSponge            19
#define BlockTypeGlass             20
#define BlockTypeRedWool           21
#define BlockTypeOrangeWool        22
#define BlockTypeYellowWool        23
#define BlockTypeLimeWool          24
#define BlockTypeGreenWool         25
#define BlockTypeAquaGreenWool     26
#define BlockTypeCyanWool          27
#define BlockTypeBlueWool          28
#define BlockTypePurpleWool        29
#define BlockTypeIndigoWool        30
#define BlockTypeVioletWool        31
#define BlockTypeMagentaWool       32
#define BlockTypePinkWool          33
#define BlockTypeBlackWool         34
#define BlockTypeGrayWool          35
#define BlockTypeWhiteWool         36
#define BlockTypeDandelion         37
#define BlockTypeRose              38
#define BlockTypeBrownMushroom     39
#define BlockTypeRedMushroom       40
#define BlockTypeGold              41
#define BlockTypeIron              42
#define BlockTypeDoubleSlab        43
#define BlockTypeSlab              44
#define BlockTypeBrick             45
#define BlockTypeTNT               46
#define BlockTypeBookshelf         47
#define BlockTypeMossyCobbleStone  48
#define BlockTypeObsidian          49
#define BlockTypeCount             50

constant int TextureIDTable[256] = {
	[BlockTypeStone]               = 1,
	[BlockTypeGrass]               = 0,
	[BlockTypeDirt]                = 2,
	[BlockTypeCobbleStone]         = 16,
	[BlockTypeWood]                = 4,
	[BlockTypeSapling]             = 15,
	[BlockTypeBedrock]             = 17,
	[BlockTypeWater]               = 14,
	[BlockTypeStillWater]          = 14,
	[BlockTypeLava]                = 30,
	[BlockTypeStillLava]           = 30,
	[BlockTypeSand]                = 18,
	[BlockTypeGravel]              = 19,
	[BlockTypeGoldOre]             = 32,
	[BlockTypeIronOre]             = 33,
	[BlockTypeCoalOre]             = 34,
	[BlockTypeLog]                 = 20,
	[BlockTypeLeaves]              = 22,
	[BlockTypeSponge]              = 48,
	[BlockTypeGlass]               = 49,
	[BlockTypeRedWool]             = 64,
	[BlockTypeOrangeWool]          = 65,
	[BlockTypeYellowWool]          = 66,
	[BlockTypeLimeWool]            = 67,
	[BlockTypeGreenWool]           = 68,
	[BlockTypeAquaGreenWool]       = 69,
	[BlockTypeCyanWool]            = 70,
	[BlockTypeBlueWool]            = 71,
	[BlockTypePurpleWool]          = 72,
	[BlockTypeIndigoWool]          = 73,
	[BlockTypeVioletWool]          = 74,
	[BlockTypeMagentaWool]         = 75,
	[BlockTypePinkWool]            = 76,
	[BlockTypeBlackWool]           = 77,
	[BlockTypeGrayWool]            = 78,
	[BlockTypeWhiteWool]           = 79,
	[BlockTypeDandelion]           = 13,
	[BlockTypeRose]                = 12,
	[BlockTypeBrownMushroom]       = 29,
	[BlockTypeRedMushroom]         = 28,
	[BlockTypeGold]                = 24,
	[BlockTypeIron]                = 23,
	[BlockTypeDoubleSlab]          = 5,
	[BlockTypeSlab]                = 6,
	[BlockTypeBrick]               = 7,
	[BlockTypeTNT]                 = 8,
	[BlockTypeBookshelf]           = 35,
	[BlockTypeMossyCobbleStone]    = 36,
	[BlockTypeObsidian]            = 37,
};

constant float TileSideBrightness[6] = { 0.5f, 1.0f, 0.8f, 0.8f, 0.6f, 0.6f };

constant float3 TileSideNormal[6] = {
	{ 0.0f, -1.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, -1.0f },
	{ 0.0f, 0.0f, 1.0f, },
	{ -1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f }
};

const sampler_t TerrainSampler = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_REPEAT | CLK_FILTER_NEAREST;

int GetTextureID(uchar tile, int side) {
	if (tile == BlockTypeGrass) { return side == 1 ? 0 : (side == 0 ? 2 : 3); }
	if (tile == BlockTypeLog) { return side == 1 ? 21 : (side == 0 ? 21 : 20); }
	if (tile == BlockTypeSlab || tile == BlockTypeDoubleSlab) { return side <= 1 ? 6 : 5; }
	if (tile == BlockTypeBookshelf) { return side <= 1 ? 4 : 35; }
	if (tile == BlockTypeGold || tile == BlockTypeIron) { return side == 1 ? TextureIDTable[tile] - 16 : (side == 0 ? TextureIDTable[tile] + 16 : TextureIDTable[tile]); }
	if (tile == BlockTypeTNT) { return side == 0 ? 10 : (side == 1 ? 9 : 8); }
	return TextureIDTable[tile];
}

bool HasCrossPlaneCollision(uchar tile) {
	return tile == BlockTypeSapling || tile == BlockTypeDandelion || tile == BlockTypeRose || tile == BlockTypeRedMushroom || tile == BlockTypeBrownMushroom;
}

bool ShouldDiscardTransparency(uchar tile) {
	return tile == BlockTypeLeaves || HasCrossPlaneCollision(tile);
}

float GetTileReflectiveness(uchar tile, float4 color) {
	if (tile == BlockTypeGlass && color.w == 0.0f) { return 0.1f; }
	if (tile == BlockTypeWater || tile == BlockTypeStillWater) { return 0.1f; }
	return 0.0f;
}

void GetTileUV(int3 voxel, float3 dimensions, float3 hit, int * side, float2 * uv) {
	float3 n = hit - convert_float3(voxel);
	if (fabs(n.x) < EPSILON) {
		*uv = (float2){ n.z, 1.0f - n.y };
		*side = 5;
	} else if (fabs(n.x - dimensions.x) < EPSILON) {
		*uv = (float2){ 1.0f - n.z, 1.0 - n.y };
		*side = 4;
	} else if (fabs(n.y) < EPSILON) {
		*uv = n.xz;
		*side = 0;
	} else if (fabs(n.y - dimensions.y) < EPSILON) {
		*uv = n.xz;
		*side = 1;
	} else if (fabs(n.z) < EPSILON) {
		*uv = (float2){ 1.0f - n.x, 1.0f - n.y };
		*side = 3;
	} else if (fabs(n.z - dimensions.z) < EPSILON) {
		*uv = (float2){ n.x, 1.0f - n.y };
		*side = 2;
	} else {
		*uv = (float2){ 0.0f, 0.0f };
		*side = 0;
	}
}

float3 MatrixTransformPoint(float16 l, float3 r) {
	return (float3) {
		r.x * l.s0 + r.y * l.s4 + r.z * l.s8 + l.sC,
		r.x * l.s1 + r.y * l.s5 + r.z * l.s9 + l.sD,
		r.x * l.s2 + r.y * l.s6 + r.z * l.sA + l.sE,
	};
}

bool RayPlaneIntersection(float3 ray, float3 origin, float3 normal, float3 center, float * dist) {
	float d = dot(normal, ray);
	if (fabs(d) > EPSILON) {
		*dist = dot(center - origin, normal) / d;
		return *dist >= 0.0f;
	}
	return false;
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

bool PointInBounds(float3 v, int levelSize) {
	return v.x >= 0.0f && v.y >= 0.0f && v.z >= 0.0f && v.x < (float)levelSize && v.y < 64.0f && v.z < (float)levelSize;
}

float4 WaterColor(float3 hit, __read_only image2d_t terrain) {
	float2 uv = (hit - floor(hit)).xz / 16.0f + (float2){ 224.0f / 256.0f, 0.0f };
	return read_imagef(terrain, TerrainSampler, uv);
}

float4 BedrockColor(float3 hit, __read_only image2d_t terrain) {
	float2 uv = (hit - floor(hit)).xz / 16.0f + (float2){ 16.0f / 256.0f, 16.0f / 256.0f };
	return read_imagef(terrain, TerrainSampler, uv);
}

float3 BGColor(float3 ray) {
	float t = 1.0f - (1.0f - ray.y) * (1.0f - ray.y) * (1.0f - ray.y) * (1.0f - ray.y);
	return t * (float3){ 0.63f, 0.8f, 1.0f } + (1.0f - t) * (float3){ 1.0f, 1.0f, 1.0f };
}

bool RayBlockIntersection(__global uchar * blocks, __read_only image2d_t terrain, float3 ray, float3 origin, int3 voxel, int levelSize, bool inWater, uchar * tile, float * enter, float * exit, float4 * color, float3 * normal) {
	RayBox(ray, origin, convert_float3(voxel), convert_float3(voxel + 1), enter, exit);
	float3 dimensions = { 1.0f, 1.0f, 1.0f };
	*tile = blocks[(voxel.y * levelSize + voxel.z) * levelSize + voxel.x];
	if (*tile == BlockTypeWater || *tile == BlockTypeStillWater) {
		uchar above = blocks[((voxel.y + 1) * levelSize + voxel.z) * levelSize + voxel.x];
		if (above != BlockTypeWater && above != BlockTypeStillWater) {
			dimensions.y -= 0.1f;
			float waterExit;
			if (inWater) {
				RayBox(ray, origin, convert_float3(voxel), convert_float3(voxel) + dimensions, &waterExit, enter);
				float2 uv;
				int side;
				GetTileUV(voxel, dimensions, origin + ray * *enter, &side, &uv);
				if (side != 1) {
					return false;
				}
				if (*enter < waterExit || *enter < 0.0f) {
					return false;
				}
			} else {
				RayBox(ray, origin, convert_float3(voxel), convert_float3(voxel) + dimensions, enter, &waterExit);
				if (waterExit < *enter || waterExit < 0.0f) {
					return false;
				}
			}
		} else if (inWater) {
			return false;
		}
	} else if (*tile == BlockTypeLava || *tile == BlockTypeStillLava) {
		uchar above = blocks[((voxel.y + 1) * levelSize + voxel.z) * levelSize + voxel.x];
		if (above != BlockTypeLava && above != BlockTypeStillLava) {
			dimensions.y -= 0.1f;
			float lavaExit;
			RayBox(ray, origin, convert_float3(voxel), convert_float3(voxel) + dimensions, enter, &lavaExit);
			if (lavaExit < *enter || lavaExit < 0.0f) {
				return false;
			}
		}
	}
	else if (*tile == BlockTypeSlab) {
		dimensions.y -= 0.5f;
		float slabExit;
		RayBox(ray, origin, convert_float3(voxel), convert_float3(voxel) + dimensions, enter, &slabExit);
		if (slabExit < *enter || slabExit < 0.0f) {
			return false;
		}
	} else if (*tile == BlockTypeGlass) {
		int3 prevVoxel = convert_int3(origin + ray * *enter - sign(ray) * EPSILON);
		uchar prevTile = blocks[(prevVoxel.y * levelSize + prevVoxel.z) * levelSize + prevVoxel.x];
		if (prevTile == BlockTypeGlass) {
			return false;
		}
	} else if (HasCrossPlaneCollision(*tile)) {
		float3 base = convert_float3(voxel);
		float dist[2];
		bool intersect[2];
		intersect[0] = RayPlaneIntersection(ray, origin, normalize((float3){ 1.0f, 0.0f, 1.0f }), base + 0.5f, &dist[0]) && dist[0] > *enter && dist[0] < *exit && distance((origin + ray * dist[0]).xz, base.xz + 0.5f) < 0.5f;
		intersect[1] = RayPlaneIntersection(ray, origin, normalize((float3){ 1.0f, 0.0f, -1.0f }), base + 0.5f, &dist[1]) && dist[1] > *enter && dist[1] < *exit && distance((origin + ray * dist[1]).xz, base.xz + 0.5f) < 0.5f;
		
		bool swap = false;
		if (intersect[0] && intersect[1] && dist[1] < dist[0]) {
			float d = dist[0];
			dist[0] = dist[1];
			dist[1] = d;
			swap = true;
		}
		for (int i = 0; i < 2; i++) {
			if (intersect[i]) {
				float3 hit = origin + ray * dist[i];
				if ((!swap && i == 0) || (swap && i == 1)) {
					*normal = normalize((float3){ 1.0f, 0.0f, 1.0f } * (1.0f - hit.z + base.z > hit.x - base.x ? -1.0f : 1.0f));
				} else {
					*normal = normalize((float3){ 1.0f, 0.0f, -1.0f } * (hit.z + base.z > hit.x - base.x ? -1.0f : 1.0f));
				}
				*enter = dist[i] - EPSILON * 10.0f;
				float2 uv;
				if ((!swap && i == 0) || (swap && i == 1)) {
					uv = (float2){ distance(base.xz + (float2){ 0.1464466f, 1.0f - 0.1464466f }, hit.xz), 1.0f - (hit.y - base.y) };
				} else {
					uv = (float2){ 1.0f - distance(base.xz + (float2){ 0.1464466f, 0.1464466f }, hit.xz), 1.0f - (hit.y - base.y) };
				}
				int id = GetTextureID(*tile, 0);
				uv = uv / 16.0f + (float2){ (float)((id % 16) << 4), (float)((id / 16) << 4) } / 256.0f;
				*color = read_imagef(terrain, TerrainSampler, uv);
				if (color->w != 0.0f) {
					return true;
				}
			}
		}
		return false;
	}
	float2 uv;
	int side;
	GetTileUV(voxel, dimensions, origin + ray * *enter, &side, &uv);
	int id = GetTextureID(*tile, side);
	uv = uv / 16.0f + (float2){ (float)((id % 16) << 4), (float)((id / 16) << 4) } / 256.0f;
	*color = read_imagef(terrain, TerrainSampler, uv);
	if (color->w == 0.0f && ShouldDiscardTransparency(*tile)) {
		return false;
	}
	color->xyz *= TileSideBrightness[side];
	*normal = TileSideNormal[side];
	return true;
}

bool RayWorldIntersection(__global uchar * distanceField, __global uchar * blocks, __read_only image2d_t terrain, float3 ray, float3 origin, int levelSize, bool inWater, uchar * tile, float * enter, float * exit, float4 * color, float3 * normal) {
	float t = 0;
	int i = 0;
	while (PointInBounds(origin + ray * t, levelSize) && i < 1024) {
		int3 voxel = convert_int3(origin + ray * t);
		uchar dist = distanceField[(voxel.y * levelSize + voxel.z) * levelSize + voxel.x];
		if (dist > 0) {
			RayBox(ray, origin, convert_float3(voxel - (dist - 1)), convert_float3(voxel + dist), enter, exit);
		} else if (RayBlockIntersection(blocks, terrain, ray, origin, voxel, levelSize, inWater, tile, enter, exit, color, normal)) {
			return true;
		}
		
		if (*exit - t < MIN_RAY_STEP) {
			*exit = t + MIN_RAY_STEP;
		}
		t = *exit + EPSILON;
		
		i += 1;
		if (i == 1024) {
			*color = (float4){ 1.0f, 0.0f, 0.0f, 1.0f };
		}
	}
	RayBox(origin, ray, (float3){ 0.0f, 0.0f, 0.0f }, (float3){ levelSize, 64.0f, levelSize }, enter, exit);
	return false;
}

bool RaySceneIntersection(__global uchar * distanceField, __global uchar * blocks, __read_only image2d_t terrain, float3 ray, float3 origin, int levelSize, bool inWater, uchar * tile, float * enter, float * exit, float4 * color, float3 * normal) {
	if (!RayWorldIntersection(distanceField, blocks, terrain, ray, origin, levelSize, inWater, tile, enter, exit, color, normal)) {
		if (RayPlaneIntersection(ray, origin, (float3){ 0.0f, 1.0f, 0.0f }, (float3){ 0.0f, 32.0f - 0.1f, 0.0f }, enter)) {
			float3 hit = origin + ray * *enter;
			if (hit.x > levelSize - 1.0f || hit.z > levelSize - 1.0f || hit.x < 1.0f || hit.z < 1.0f) {
				*exit = *enter + 1.0f;
				*tile = BlockTypeWater;
				*color = WaterColor(hit, terrain);
				*normal = (float3){ 0.0f, 1.0f, 0.0f };
				return true;
			}
		}
		if (RayPlaneIntersection(ray, origin, (float3){ 0.0f, 1.0f, 0.0f }, (float3){ 0.0f, 0.0f, 0.0f }, enter)) {
			*exit = *enter + 1.0f;
			*tile = BlockTypeBedrock;
			*color = BedrockColor(origin + ray * *enter, terrain);
			*normal = (float3){ 0.0f, 1.0f, 0.0f };
			return true;
		}
		return false;
	} else {
		return true;
	}
}

float3 TraceShadows(__global uchar * distanceField, __global uchar * blocks, __read_only image2d_t terrain, float3 ray, float3 origin, int levelSize, bool inWater, float3 color) {
	float4 shadowColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	uchar tile;
	float enter, exit;
	float waterDist = 0.0f;
	float4 hitColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	float3 normal;
	int i = 0;
	while (hitColor.w < 1.0f && i < 64) {
		if (RaySceneIntersection(distanceField, blocks, terrain, ray, origin, levelSize, inWater, &tile, &enter, &exit, &hitColor, &normal)) {
			if (inWater && !(tile == BlockTypeWater || tile == BlockTypeStillWater)) {
				waterDist += enter;
				shadowColor.w *= (1.0f - min(waterDist / 16.0f, 1.0f));
			}
			shadowColor.xyz += hitColor.xyz * hitColor.w * shadowColor.w;
			shadowColor.w *= 1.0f - hitColor.w;
			
			if (tile == BlockTypeWater || tile == BlockTypeStillWater) {
				if (inWater) {
					waterDist += enter;
				} else {
					exit = enter;
				}
				inWater = !inWater;
			}
			if (exit < MIN_RAY_STEP) {
				exit = MIN_RAY_STEP;
			}
			origin += ray * (exit + EPSILON);
		} else {
			break;
		}
		
		i += 1;
		if (i == 64) {
			shadowColor.xyz = (float3){ 1.0f, 0.0f, 0.0f };
		}
	}
	return color * shadowColor.w + (shadowColor.xyz * shadowColor.w + 0.525f * color * (1.0f - shadowColor.w)) * (1.0f - shadowColor.w);
}

float4 TraceFog(float3 ray, float dist) {
	//float w = 1.0f - exp(-dist / 512.0f );
	float w = clamp(dist / 512.0f, 0.0f, 1.0f);
	float4 fog = (float4){ 1.0f, 1.0f, 1.0f, w };
	return fog;
}

float3 TraceReflections(__global uchar * distanceField, __global uchar * blocks, __read_only image2d_t terrain, float3 ray, float3 origin, int levelSize, bool inWater, float3 lightDir, float3 color) {
	float4 reflectionColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	uchar tile;
	float enter, exit;
	float waterDist = 0.0f;
	float4 hitColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	float3 normal;
	int i = 0;
	while (hitColor.w < 1.0f && i < 64) {
		if (RaySceneIntersection(distanceField, blocks, terrain, ray, origin, levelSize, inWater, &tile, &enter, &exit, &hitColor, &normal)) {
			if (inWater && !(tile == BlockTypeWater || tile == BlockTypeStillWater)) {
				waterDist += enter;
				reflectionColor.w *= (1.0f - min(waterDist / 16.0f, 1.0f));
			}
			hitColor.xyz = TraceShadows(distanceField, blocks, terrain, lightDir, origin + ray * enter + lightDir * EPSILON, levelSize, inWater, hitColor.xyz);
			float4 fog = TraceFog(ray, enter);
			reflectionColor.xyz += fog.xyz * fog.w * reflectionColor.w;
			reflectionColor.w *= 1.0f - fog.w;
			reflectionColor.xyz += hitColor.xyz * hitColor.w * reflectionColor.w;
			reflectionColor.w *= 1.0f - hitColor.w;
			
			if (tile == BlockTypeWater || tile == BlockTypeStillWater) {
				if (inWater) {
					waterDist += enter;
				} else {
					exit = enter;
				}
				inWater = !inWater;
			}
			if (exit < MIN_RAY_STEP) {
				exit = MIN_RAY_STEP;
			}
			origin += ray * (exit + EPSILON);
		} else {
			reflectionColor.xyz += BGColor(ray) * reflectionColor.w;
			break;
		}
		
		i += 1;
		if (i == 64) {
			reflectionColor.xyz = (float3){ 1.0f, 0.0f, 0.0f };
		}
	}
	return reflectionColor.xyz;
}

__kernel void trace(int levelSize, __global uchar * distanceField, __global uchar * blocks, __write_only image2d_t texture, int width, int height, float16 camera, __read_only image2d_t terrain, int isUnderWater, float time) {
	int x = get_global_id(0);
	int y = get_global_id(1);
	if (x >= width || y >= height) {
		return;
	}
	float2 uv = (float2){ 1.0f - 2.0f * (float)x / width, 2.0f * (float)y / height - 1.0f };
	uv.x *= (float)width / height;
	if (isUnderWater) {
		uv.y += sin(uv.x * (10.0 + sin(time)) + time) / (135.0f + 10.0f * sin(time));
	};

	float fov = 70.0f;
	float3 origin = MatrixTransformPoint(camera, (float3){ 0.0f, 0.0f, 0.0f });
	float3 ray = normalize(MatrixTransformPoint(camera, (float3){ uv * 0.5f, 0.5f / tanpi(fov / 360.0f) }) - origin);
	float3 lightDir = normalize((float3){ cos(14.0f / 16.0f * 2.0f * 3.141592653589f), 1.0f, sin(14.0f / 16.0f * 2.0f * 3.141592653589f) });
	bool inWater = isUnderWater;
	float4 fragColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	if (isUnderWater) {
		float4 water = read_imagef(terrain, TerrainSampler, (float2){ x / (float)width, y / (float)height } / 16.0f + (float2){ 224.0f / 256.0f, 0.0f });
		water.w *= 0.375f;
		fragColor.xyz += water.xyz * water.w * fragColor.w;;
		fragColor.w *= 1.0f - water.w;
	}
	
	uchar tile;
	float enter, exit;
	float waterDist = 0.0f;
	float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
	float3 normal;
	int i = 0;
	while (color.w < 1.0f && i < 64) {
		if (RaySceneIntersection(distanceField, blocks, terrain, ray, origin, levelSize, inWater, &tile, &enter, &exit, &color, &normal)) {
			if (inWater && !(tile == BlockTypeWater || tile == BlockTypeStillWater)) {
				waterDist += enter;
				fragColor.w *= (1.0f - min(waterDist / 16.0f, 1.0f));
			}
			color.xyz = TraceShadows(distanceField, blocks, terrain, lightDir, origin + ray * enter + lightDir * EPSILON, levelSize, inWater, color.xyz);
			float4 fog = TraceFog(ray, enter);
			fragColor.xyz += fog.xyz * fog.w * fragColor.w;
			fragColor.w *= 1.0f - fog.w;
			float reflectiveness = GetTileReflectiveness(tile, color);
			if (reflectiveness > 0.0f && !(inWater && (tile == BlockTypeWater || tile == BlockTypeStillWater))) {
				float3 reflectionDir = normalize(ray - 2.0f * dot(ray, normal) * normal);
				float3 rColor = TraceReflections(distanceField, blocks, terrain, reflectionDir, origin + ray * enter + reflectionDir * EPSILON, levelSize, inWater, lightDir, color.xyz);
				fragColor.xyz += rColor * reflectiveness * fragColor.w;
				fragColor.w *= 1.0f - reflectiveness;
			}
			fragColor.xyz += color.xyz * color.w * fragColor.w;
			fragColor.w *= 1.0f - color.w;
			
			if (tile == BlockTypeWater || tile == BlockTypeStillWater) {
				if (inWater) {
					waterDist += enter;
				} else {
					exit = enter;
				}
				inWater = !inWater;
			}
			if (exit < MIN_RAY_STEP) {
				exit = MIN_RAY_STEP;
			}
			origin += ray * (exit + EPSILON);
		} else {
			fragColor.xyz += BGColor(ray) * fragColor.w;
			break;
		}
		
		i += 1;
		if (i == 64) {
			fragColor.xyz = (float3){ 1.0f, 0.0f, 0.0f };
		}
	}
	write_imagef(texture, (int2){ x, y }, (float4){ fragColor.xyz, 1.0f });
}
