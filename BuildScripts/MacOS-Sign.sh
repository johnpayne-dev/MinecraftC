xattr -cr Output/MinecraftC.app
codesign -s "$1" Output/MinecraftC.app/Contents/MacOS/MinecraftC
if [[ $# == 3 ]]; then
	editto -c -k --keepParent Output/MinecraftC.app Output/MinecraftC-MacOS.zip
	xcrun altool --notarize-app --primary-bundle-id "$2" -u "$3" --file Output/MinecraftC-MacOS.zip
	xcrun stapler staple Output/MinecraftC.app
fi
