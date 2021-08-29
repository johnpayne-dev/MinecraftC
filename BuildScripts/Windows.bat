mkdir Output
cd BuildScripts
clang ^
	../MinecraftC/*.c ^
	../MinecraftC/GUI/*.c ^
	../MinecraftC/Level/*.c ^
	../MinecraftC/Level/Generator/*.c ^
	../MinecraftC/Level/Tile/*.c ^
	../MinecraftC/Level/Generator/Noise/*.c ^
	../MinecraftC/Particle/*.c ^
	../MinecraftC/Physics/*.c ^
	../MinecraftC/Player/*.c ^
	../MinecraftC/Render/*.c ^
	../MinecraftC/Render/Texture/*.c ^
	../MinecraftC/Utilities/*.c ^
	../Resources/MinecraftC.res ^
	-I ../Include ^
	-L ../Libraries ^
	-D _CRT_SECURE_NO_WARNINGS ^
	-std=c11 ^
	-lShell32.lib ^
	-lSetupapi.lib ^
	-lOle32.lib ^
	-lUser32.lib ^
	-lGdi32.lib ^
	-lWinmm.lib ^
	-lOleAut32.lib ^
	-lImm32.lib ^
	-lVersion.lib ^
	-lAdvapi32.lib ^
	-lShcore.lib ^
	-lSDL2main.lib ^
	-lSDL2-static.lib ^
	-lOpengl32.lib ^
	-lGlu32.lib ^
	-Xlinker /subsystem:windows ^
	-Xlinker /NODEFAULTLIB:MSVCRT ^
	-o ../Output/MinecraftC.exe
