import sys
import os
import shutil

ICON_PATH = os.path.realpath(__file__ + '/../../Resources/App/AppIcon.icns')

if len(sys.argv) == 2 or len(sys.argv) == 3:
	path = os.path.realpath(sys.argv[1])
	appPath = path[:path.rfind('/')] + '/MinecraftC.app'
	os.makedirs(appPath + '/Contents/MacOS', exist_ok=True)
	os.makedirs(appPath + '/Contents/Resources', exist_ok=True)
	shutil.copy2(path, appPath + '/Contents/MacOS')
	shutil.copy2(ICON_PATH, appPath + '/Contents/Resources')
	with open(appPath + '/Contents/Info.plist', 'w') as info:
		plist = """
			<?xml version="1.0" encoding="UTF-8"?>
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
				<string>AppIcon</string>
			</dict>
			</plist>
			"""
		info.write(plist)
	if len(sys.argv) == 3:
		os.system('xattr -cr ' + appPath)
		os.system(f'codesign --deep --force -s {sys.argv[2]} --timestamp {appPath}')
else:
	print('usage: python MacOS-App.py <executable> [developer-id]')
