from PIL import Image
import os
import sys

RESOURCE_PATH = os.path.realpath(__file__ + '/../../Resources')

def embed_png(filePath, outPath):
	cName = filePath[len(RESOURCE_PATH) + 1:-4].replace(os.sep, '_')
	
	image = Image.open(filePath).convert('RGBA')
	pixels = image.load()
	width, height = image.size
	
	output = open(outPath, 'w')
	output.write(f'static const unsigned int Resource_{cName}_Width = {width};\n\n')
	output.write(f'static const unsigned int Resource_{cName}_Height = {height};\n\n')
	output.write(f'static const unsigned char Resource_{cName}_RGBA[] = {{')
	for y in range(height):
		for x in range(width):
			if (y * width + x) % 4 == 0: output.write('\n\t')
			r, g, b, a = pixels[x, y]
			output.write('0x%0.2X, ' % r)
			output.write('0x%0.2X, ' % g)
			output.write('0x%0.2X, ' % b)
			output.write('0x%0.2X, ' % a)
	output.write('\n};\n')
	output.close()
	
	image.close()

def embed_file(filePath, outPath):
	input, output = open(filePath, 'rb'), open(outPath, 'w')
	cName = filePath[len(RESOURCE_PATH) + 1:filePath.rfind('.')].replace(os.sep, '_')
	output.write(f'static const unsigned char Resource_{cName}[] = {{')
	for i, byte in enumerate(input.read()):
		if i % 16 == 0: output.write('\n\t')
		output.write('0x{0:02x}, '.format(byte))
	output.write('0x00\n};\n')
	input.close()
	output.close()

for subdir, dirs, files in os.walk(RESOURCE_PATH):
	for fileName in files:
		filePath = subdir + os.sep + fileName
		if filePath.endswith('.png'):
			embed_png(filePath, filePath[:-4] + '.h')
		elif filePath.endswith('.ogg'):
			embed_file(filePath, filePath[:-4] + '.h')
		elif filePath.endswith('.cl'):
			embed_file(filePath, filePath[:-3] + '.h')
