mkdir Output
cd BuildScripts
clang-cl ^
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
	/I ../Include ^
	/D _CRT_SECURE_NO_WARNINGS ^
	/link ^
		/LIBPATH:../Libraries ^
		/SUBSYSTEM:WINDOWS ^
		Shell32.lib ^
		Setupapi.lib ^
		Ole32.lib ^
		User32.lib ^
		Gdi32.lib ^
		Winmm.lib ^
		OleAut32.lib ^
		Imm32.lib ^
		Version.lib ^
		Advapi32.lib ^
		Shcore.lib ^
		SDL2main.lib ^
		SDL2-static.lib ^
		Opengl32.lib ^
		Glu32.lib ^
		../Resources/MinecraftC.res ^
		/NODEFAULTLIB:MSVCRT ^
		/OUT:../Output/MinecraftC.exe
