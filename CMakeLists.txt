cmake_minimum_required(VERSION 3.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(MinecraftC)

include(SDL2.cmake)

set(MINECRAFTC_SOURCES
	MinecraftC/GUI/BlockSelectScreen.c
	MinecraftC/GUI/BlockSelectScreen.h
	MinecraftC/GUI/Button.c
	MinecraftC/GUI/Button.h
	MinecraftC/GUI/ChatInputScreen.c
	MinecraftC/GUI/ChatInputScreen.h
	MinecraftC/GUI/ControlsScreen.c
	MinecraftC/GUI/ControlsScreen.h
	MinecraftC/GUI/ErrorScreen.c
	MinecraftC/GUI/ErrorScreen.h
	MinecraftC/GUI/FontRenderer.c
	MinecraftC/GUI/FontRenderer.h
	MinecraftC/GUI/GenerateLevelScreen.c
	MinecraftC/GUI/GenerateLevelScreen.h
	MinecraftC/GUI/GUIScreen.c
	MinecraftC/GUI/GUIScreen.h
	MinecraftC/GUI/HUDScreen.c
	MinecraftC/GUI/HUDScreen.h
	MinecraftC/GUI/LevelNameScreen.c
	MinecraftC/GUI/LevelNameScreen.h
	MinecraftC/GUI/LoadLevelScreen.c
	MinecraftC/GUI/LoadLevelScreen.h
	MinecraftC/GUI/OptionsScreen.c
	MinecraftC/GUI/OptionsScreen.h
	MinecraftC/GUI/PauseScreen.c
	MinecraftC/GUI/PauseScreen.h
	MinecraftC/GUI/SaveLevelScreen.c
	MinecraftC/GUI/SaveLevelScreen.h
	MinecraftC/GUI/Screen.c
	MinecraftC/GUI/Screen.h
	MinecraftC/Level/Generator/Noise/CombinedNoise.c
	MinecraftC/Level/Generator/Noise/CombinedNoise.h
	MinecraftC/Level/Generator/Noise/Noise.c
	MinecraftC/Level/Generator/Noise/Noise.h
	MinecraftC/Level/Generator/Noise/OctaveNoise.c
	MinecraftC/Level/Generator/Noise/OctaveNoise.h
	MinecraftC/Level/Generator/Noise/PerlinNoise.c
	MinecraftC/Level/Generator/Noise/PerlinNoise.h
	MinecraftC/Level/Generator/LevelGenerator.c
	MinecraftC/Level/Generator/LevelGenerator.h
	MinecraftC/Level/Tile/Block.c
	MinecraftC/Level/Tile/Block.h
	MinecraftC/Level/Tile/BookshelfBlock.c
	MinecraftC/Level/Tile/BookshelfBlock.h
	MinecraftC/Level/Tile/FlowerBlock.c
	MinecraftC/Level/Tile/FlowerBlock.h
	MinecraftC/Level/Tile/GlassBlock.c
	MinecraftC/Level/Tile/GlassBlock.h
	MinecraftC/Level/Tile/GrassBlock.c
	MinecraftC/Level/Tile/GrassBlock.h
	MinecraftC/Level/Tile/LeavesBlock.c
	MinecraftC/Level/Tile/LeavesBlock.h
	MinecraftC/Level/Tile/LiquidBlock.c
	MinecraftC/Level/Tile/LiquidBlock.h
	MinecraftC/Level/Tile/LogBlock.c
	MinecraftC/Level/Tile/LogBlock.h
	MinecraftC/Level/Tile/MetalBlock.c
	MinecraftC/Level/Tile/MetalBlock.h
	MinecraftC/Level/Tile/MushroomBlock.c
	MinecraftC/Level/Tile/MushroomBlock.h
	MinecraftC/Level/Tile/SandBlock.c
	MinecraftC/Level/Tile/SandBlock.h
	MinecraftC/Level/Tile/SaplingBlock.c
	MinecraftC/Level/Tile/SaplingBlock.h
	MinecraftC/Level/Tile/SlabBlock.c
	MinecraftC/Level/Tile/SlabBlock.h
	MinecraftC/Level/Tile/SpongeBlock.c
	MinecraftC/Level/Tile/SpongeBlock.h
	MinecraftC/Level/Tile/StillLiquidBlock.c
	MinecraftC/Level/Tile/StillLiquidBlock.h
	MinecraftC/Level/Tile/TileSound.c
	MinecraftC/Level/Tile/TileSound.h
	MinecraftC/Level/Tile/TNTBlock.c
	MinecraftC/Level/Tile/TNTBlock.h
	MinecraftC/Level/Level.c
	MinecraftC/Level/Level.h
	MinecraftC/Level/NextTickListEntry.h
	MinecraftC/Particle/PrimedTNT.c
	MinecraftC/Particle/PrimedTNT.h
	MinecraftC/Particle/Particle.c
	MinecraftC/Particle/Particle.h
	MinecraftC/Particle/ParticleManager.c
	MinecraftC/Particle/ParticleManager.h
	MinecraftC/Particle/SmokeParticle.c
	MinecraftC/Particle/SmokeParticle.h
	MinecraftC/Particle/TerrainParticle.c
	MinecraftC/Particle/TerrainParticle.h
	MinecraftC/Particle/WaterDropParticle.c
	MinecraftC/Particle/WaterDropParticle.h
	MinecraftC/Physics/AABB.c
	MinecraftC/Physics/AABB.h
	MinecraftC/Physics/Vector3D.h
	MinecraftC/Player/InputHandler.c
	MinecraftC/Player/InputHandler.h
	MinecraftC/Player/Inventory.c
	MinecraftC/Player/Inventory.h
	MinecraftC/Player/Player.c
	MinecraftC/Player/Player.h
	MinecraftC/Player/PlayerAI.c
	MinecraftC/Player/PlayerAI.h
	MinecraftC/Render/Texture/AnimatedTexture.c
	MinecraftC/Render/Texture/AnimatedTexture.h
	MinecraftC/Render/Texture/LavaTexture.c
	MinecraftC/Render/Texture/LavaTexture.h
	MinecraftC/Render/Texture/WaterTexture.c
	MinecraftC/Render/Texture/WaterTexture.h
	MinecraftC/Render/Chunk.c
	MinecraftC/Render/Chunk.h
	MinecraftC/Render/Frustum.c
	MinecraftC/Render/Frustum.h
	MinecraftC/Render/HeldBlock.h
	MinecraftC/Render/LevelRenderer.c
	MinecraftC/Render/LevelRenderer.h
	MinecraftC/Render/Renderer.c
	MinecraftC/Render/Renderer.h
	MinecraftC/Render/ShapeRenderer.c
	MinecraftC/Render/ShapeRenderer.h
	MinecraftC/Render/TextureManager.c
	MinecraftC/Render/TextureManager.h
	MinecraftC/Sound/SoundManager.c
	MinecraftC/Sound/SoundManager.h
	MinecraftC/Utilities/List.c
	MinecraftC/Utilities/List.h
	MinecraftC/Utilities/Log.c
	MinecraftC/Utilities/Log.h
	MinecraftC/Utilities/OpenGL.h
	MinecraftC/Utilities/Random.c
	MinecraftC/Utilities/Random.h
	MinecraftC/Utilities/SinTable.c
	MinecraftC/Utilities/SinTable.h
	MinecraftC/Utilities/String.c
	MinecraftC/Utilities/String.h
	MinecraftC/Utilities/Time.h
	MinecraftC/ChatLine.c
	MinecraftC/ChatLine.h
	MinecraftC/Entity.c
	MinecraftC/Entity.h
	MinecraftC/GameSettings.c
	MinecraftC/GameSettings.h
	MinecraftC/Minecraft.c
	MinecraftC/Minecraft.h
	MinecraftC/MovingObjectPosition.h
	MinecraftC/ProgressBarDisplay.c
	MinecraftC/ProgressBarDisplay.h
	MinecraftC/SessionData.c
	MinecraftC/SessionData.h
	MinecraftC/Timer.c
	MinecraftC/Timer.h
	)

if(MSVC)
	set(RELEASE_FLAGS "/O2")
	set(DEBUG_FLAGS "/Od")
	set(LINKER_FLAGS "/subsystem:windows")
else()
	set(RELEASE_FLAGS "-Ofast")
	set(DEBUG_FLAGS "-O0")
endif()
set(FLAGS "$<IF:$<CONFIG:Debug>,${DEBUG_FLAGS},${RELEASE_FLAGS}>")

add_definitions(
	-DSDL_dynapi_h_
	-DSDL_DYNAMIC_API=0
	-DSDL_VIDEO_OPENGL_EGL=0
	-DSDL_VIDEO_RENDER_OGL_ES2=0
	)

if(APPLE)
	set(SOURCES
		${MINECRAFTC_SOURCES}
		${SDL2_COMMON_SOURCES}
		${SDL2_APPLE_SOURCES}
		)
	set(FLAGS ${FLAGS} "-fobjc-arc" "-Wno-deprecated")
	set(LIBRARIES
		"-framework AudioToolbox"
		"-framework Carbon"
		"-framework Cocoa"
		"-framework CoreAudio"
		"-framework CoreFoundation"
		"-framework CoreHaptics"
		"-framework CoreVideo"
		"-framework ForceFeedback"
		"-framework Foundation"
		"-framework GameController"
		"-framework IOKit"
		"-framework Metal"
		"-framework OpenGL"
		)
elseif(WIN32)
	set(SOURCES
		${MINECRAFTC_SOURCES}
		${SDL2_COMMON_SOURCES}
		${SDL2_WINDOWS_SOURCES}
		Resources/App/AppIcon.rc
		)
	set(LIBRARIES
		opengl32
		glu32
		Setupapi
		Winmm
		Version
		Imm32
		)
elseif(UNIX)
	set(SOURCES ${MINECRAFTC_SOURCES})
	set(LIBRARIES
		m
		SDL2
		GL
		GLU
		)
endif()

add_executable(MinecraftC ${SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
target_compile_options(MinecraftC PRIVATE ${FLAGS})
if(MSVC)
	target_link_options(MinecraftC PRIVATE ${LINKER_FLAGS})
endif()
target_include_directories(MinecraftC PRIVATE
	External/SDL/include
	External/cute_headers
	External/stb
	)
target_link_libraries(MinecraftC ${LIBRARIES})
