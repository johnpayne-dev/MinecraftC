cd BuildScripts
mkdir -p ../Output/MinecraftC.app/Contents/MacOS
mkdir ../Output/MinecraftC.app/Contents/Resources
cp ../Resources/Icons/MacOS.icns ../Output/MinecraftC.app/Contents/Resources
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
	-I ../Include \
	-L ../Libraries/MacOS/ARM \
	-D GL_SILENCE_DEPRECATION \
	-framework AppKit \
	-framework AudioToolbox \
	-framework Carbon \
	-framework CoreAudio \
	-framework CoreGraphics \
	-framework CoreVideo \
	-framework ForceFeedback \
	-framework IOKit \
	-framework Metal \
	-framework OpenGL \
	-lSDL2 \
	-o ../Output/MinecraftC.app/Contents/MacOS/MinecraftC

echo >../Output/MinecraftC.app/Contents/Info.plist \
'<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleDevelopmentRegion</key>
    <string>en</string>
    <key>CFBundleExecutable</key>
    <string>MinecraftC</string>
    <key>CFBundleIdentifier</key>
    <string>com.NotMojang.MinecraftC</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleName</key>
    <string>MinecraftC</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>CFBundleVersion</key>
    <string>1</string>
    <key>CFBundleIconFile</key>
    <string>MacOS</string>
</dict>
</plist>'
