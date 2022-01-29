make:
        mkdir -pv ../Output
        cd MinecraftC; \
                clang -lm -lSDL2 -lGL -lGLU -Wall \
                *.c GUI/*.c Level/*.c Level/Generator/*.c \
                Level/Tile/*.c Level/Generator/Noise/*.c \
                Particle/*.c Physics/*.c Player/*.c Render/*.c \
                Render/Texture/*.c Utilities/*.c \
                -o ../Output/MinecraftC
