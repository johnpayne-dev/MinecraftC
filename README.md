# MinecraftC - A Raytraced Minecraft Classic Port to C

### Features:
- True to the original version (0.0.30a)
- Implemented fully in C using SDL2 and OpenGL 1.1
- Original music and sounds
- A modded and vanilla version
- Works on Windows, Linux, and MacOS
- CMake build system
#### Raytracing:
- Uses an 8-bit distance field as an acceleration structure
- In some cases, performance is better than the original
  - 60+ fps at 1080p on the integrated graphics cards I've tested
- To enable raytracing, download the modded binary and enable it in the mods screen under options

## Screenshots
<img width="1912" alt="Screen Shot 2022-07-31 at 12 57 49 PM" src="https://user-images.githubusercontent.com/79012575/182039578-991a0473-fc20-484f-b425-9e36cbfb77cc.png">
<img width="1912" alt="Screen Shot 2022-07-31 at 12 57 54 PM" src="https://user-images.githubusercontent.com/79012575/182039619-02a4b955-4bc4-4f63-bec2-4aafd52cebf2.png">
<img width="1912" alt="Screen Shot 2022-07-31 at 12 43 33 PM" src="https://user-images.githubusercontent.com/79012575/182039653-7ead1d2b-229e-4991-8110-586ef7a27411.png">
<img width="1912" alt="Screen Shot 2022-07-31 at 12 43 39 PM" src="https://user-images.githubusercontent.com/79012575/182039837-0b171b95-3799-4715-ab3c-cfea5414adaf.png">

## Downloads
- There are two versions, modded and vanilla, where vanilla doesn't have the option to turn on mods
- [Windows 32 bit](https://github.com/johnpayne-dev/MinecraftC/releases)
- [Windows 64 bit](https://github.com/johnpayne-dev/MinecraftC/releases)
- [MacOS x86_64](https://github.com/johnpayne-dev/MinecraftC/releases)
- [MacOS ARM](https://github.com/johnpayne-dev/MinecraftC/releases)

## Building
Note: if you would like to build the vanilla version, remove `-DMINECRAFTC_MODS=1` from the cmake command
### Requirements:
- `git`
- `cmake`
- A compiler of your choice

### Windows instructions for Visual Studio:
1. Run the following commands:
```sh
git clone --recursive https://github.com/johnpayne-dev/MinecraftC.git
cd MinecraftC
mkdir Build
cd Build
cmake -DMINECRAFTC_MODS=1 -G"Visual Studio 17" .. # replace "Visual Studio 17" with your version
```
2. You should now have `MinecraftC.sln` in your directory that you can open with Visual Studio and build

### MacOS instructions for Xcode:
1. Run the following commands:
```sh
git clone --recursive https://github.com/johnpayne-dev/MinecraftC.git
cd MinecraftC
mkdir Build && cd Build
cmake -DMINECRAFTC_MODS -GXcode ..
```
2. You should now have `MinecraftC.xcodeproj` in your directory that you open with Xcode and build

3. (optional) If you would like it in a .app format, you can run this command to convert your executable:
```
python3 Scripts/MacOS-App.py <path-to-executable> [developer-id]
```
Providing the developer id will sign the .app, which you can find by running `security find-identity`

### Linux instructions (untested):
1. If you don't have SDL2 already, run these commands:
```sh
sudo apt-get update
sudo apt-get install libsdl2-dev
```
2. Run the following commands:
```sh
git clone --recursive https://github.com/johnpayne-dev/MinecraftC.git
cd MinecraftC
mkdir Build && cd Build
cmake -DMINECRAFTC_MODS=1 ..
make
```

### Modifying Resources
If you're creating a mod and want to change or add any `.png` or `.ogg` resouces, then run this script to update the resource headers:
```sh
python3 Scripts/EmbedResources.py
```
It will recursively go through all `.png` and `.ogg` files in `Resources/`, and convert them to `.h`

## Known Issues
- There is no survival mode, it was removed for simplicity and compatibility with raytracing
- A few sounds have the incorrect pitch, I'm waiting for cute_sound.h to implement pitch shifting
- Load file and save file are not implemented
- If raytracing doesn't turn on, then something is going wrong with OpenCL, report an issue and I can help troubleshoot
