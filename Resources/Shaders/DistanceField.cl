
__kernel void iterate(__global uchar * distanceField, uchar iteration, int levelSize) {
	int x = get_global_id(0);
	int y = get_global_id(1);
	int z = get_global_id(2);
	uchar d = distanceField[(y * levelSize + z) * levelSize + x];
	if (d != iteration) {
		return;
	}
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				if (y + j < 0 || y + j >= 64 || x + i < 0 || x + i >= levelSize || z + k < 0 || z + k >= levelSize) {
					continue;
				}
				int n = ((y + j) * levelSize + z + k) * levelSize + x + i;
				if (d + 1 < distanceField[n]) {
					distanceField[n] = d + 1;
				}
			}
		}
	}
}
