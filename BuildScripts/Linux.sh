cd BuildScripts
mkdir -p ../Output
clang \
	../MinecraftC/*.c \
	../MinecraftC/GUI/*.c \
	../MinecraftC/Level/*.c \
	../MinecraftC/Level/Generator/*.c \
	../MinecraftC/Level/Tile/*.c \
	../MinecraftC/Level/Generator/Noise/*.c \
	../MinecraftC/Particle/*.c \
	../MinecraftC/Physics/*.c \
	../MinecraftC/Player/*.c \
	../MinecraftC/Render/*.c \
	../MinecraftC/Render/Texture/*.c \
	../MinecraftC/Utilities/*.c \
	-lm -lSDL2 -lGL -lGLU \
	-o ../Output/MinecraftC
