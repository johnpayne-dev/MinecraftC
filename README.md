# MinecraftC - A Minecraft Classic Port to C

### Features:
- True to the original version (0.0.30a)
- Saving and loading levels
- Original music and sounds
- Implemented fully in C using SDL2 and OpenGL 1.1
- Works on Windows, Linux, and MacOS
- CMake build system

## Screenshots
<img width="856" alt="Screen Shot 2021-08-16 at 6 22 31 PM" src="https://user-images.githubusercontent.com/79012575/129637145-c0145174-ab00-4121-8055-05d2dc206af1.png">
<img width="861" alt="Screen Shot 2021-08-16 at 6 24 26 PM" src="https://user-images.githubusercontent.com/79012575/129637151-b7a36271-3842-4af2-82c9-5466f46c016c.png">
<img width="858" alt="Screen Shot 2021-08-16 at 6 28 46 PM" src="https://user-images.githubusercontent.com/79012575/129637157-e6727c40-7fd6-40be-9562-bfaa5b7b3f68.png">

## Download
- [MinecraftC Releases](https://github.com/johnpayne-dev/MinecraftC/releases)

## Planned features
- Raytracing mode
- (Possibly) A modding system
- (Possibly) Support for mobile and web platforms

## Building

### Requirements:
- `git`, `cmake` and a compiler of your choice

### Windows instructions for Visual Studio:
1. Run the following commands:
```sh
git clone --recursive https://github.com/johnpayne-dev/MinecraftC.git
cd MinecraftC
mkdir Build
cd Build
cmake -G"Visual Studio 17" .. # replace "Visual Studio 17" with your version
```
2. You should now have `MinecraftC.sln` in your directory that you can open with Visual Studio and build

### MacOS instructions for Xcode:
1. Run the following commands:
```sh
git clone --recursive https://github.com/johnpayne-dev/MinecraftC.git
cd MinecraftC
mkdir Build && cd Build
cmake -GXcode ..
```
2. You should now have `MinecraftC.xcodeproj` in your directory that you open with Xcode and build

3. (optional) If you would like it in a .app format, you can run this command to convert your executable:
```sh
python3 Scripts/MacOS-App.py <path-to-executable> [developer-id]
```
Providing the developer id will sign the .app, which you can find by running `security find-identity`

### Linux instructions (currently untested, I have no access to a linux machine at the moment):
1. If you don't have SDL2 already, run these commands:
```sh
sudo apt-get update
sudo apt-get install libsdl2-dev
```
2. Run the following commands:
```sh
git clone https://github.com/johnpayne-dev/MinecraftC.git
cd MinecraftC
mkdir Build && cd Build
cmake ..
make
```

### Modifying Resources
If you're creating a mod and want to change or add any `.png` or `.ogg` resouces, then run this script to update the resource headers:
```sh
python3 Scripts/EmbedResources.py
```
It will recursively go through all `.png` and `.ogg` files in `Resources/`, and convert them to `.h`

## Known Issues
- There is no survival mode, it was removed for simplicity and compatibility with raytracing (not released yet)
- A few sounds have the incorrect pitch, I'm waiting for cute_sound.h to implement pitch shifting (which is planned)
- Load file and save file are not implemented
