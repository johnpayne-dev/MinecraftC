from PIL import Image
import os
import sys

RESOURCE_PATH = os.path.realpath(__file__ + '/../../Resources')

def export(filePath, outPath):
	cName = filePath[len(RESOURCE_PATH) + 1:-4].replace('/', '_')
	
	image = Image.open(filePath).convert('RGBA')
	pixels = image.load()
	width, height = image.size
	
	output = open(outPath, 'w')
	output.write('static unsigned int Resource_' + cName + '_Width = ' + str(width) + ';\n\n')
	output.write('static unsigned int Resource_' + cName + '_Height = ' + str(height) + ';\n\n')
	output.write('static unsigned char Resource_' + cName + '_RGBA[] = \n')
	output.write('{')
	for y in range(height):
		for x in range(width):
			if (x * width + y) % 4 == 0: output.write('\n\t')
			r, g, b, a = pixels[x, y]
			output.write('0x%0.2X, ' % r)
			output.write('0x%0.2X, ' % g)
			output.write('0x%0.2X, ' % b)
			output.write('0x%0.2X, ' % a)
	output.write('\n};\n')
	output.close()
	
	image.close()

for subdir, dirs, files in os.walk(RESOURCE_PATH):
	for fileName in files:
		filePath = subdir + os.sep + fileName
		if filePath.endswith('.png'):
			export(filePath, filePath[:-4] + '.h')
