# MinecraftC - A Minecraft Classic Port to C

### Features:
- True to the original version (0.0.30a)
- Implemented fully in C using SDL2 and OpenGL 1.1
- Works on Windows, Linux, and MacOS (with more to come)

## Screenshots
<img width="856" alt="Screen Shot 2021-08-16 at 6 22 31 PM" src="https://user-images.githubusercontent.com/79012575/129637145-c0145174-ab00-4121-8055-05d2dc206af1.png">
<img width="861" alt="Screen Shot 2021-08-16 at 6 24 26 PM" src="https://user-images.githubusercontent.com/79012575/129637151-b7a36271-3842-4af2-82c9-5466f46c016c.png">
<img width="858" alt="Screen Shot 2021-08-16 at 6 28 46 PM" src="https://user-images.githubusercontent.com/79012575/129637157-e6727c40-7fd6-40be-9562-bfaa5b7b3f68.png">


## Downloads
- [Windows (64 bit)](https://github.com/johnpayne-dev/MinecraftC/releases/download/v1.0/MinecraftC-Windows-64.zip)
- [MacOS (ARM)](https://github.com/johnpayne-dev/MinecraftC/releases/download/v1.0/MinecraftC-MacOS-ARM.zip)
- [MacOS (x86)](https://github.com/johnpayne-dev/MinecraftC/releases/download/v1.0/MinecraftC-MacOS-x86.zip)
- [Linux](https://github.com/johnpayne-dev/MinecraftC/releases/download/v1.0/MinecraftC-Linux.zip)

## Planned features
- Sound effects and music (v1.1)
- Saving and loading levels (v1.1)
- Support for Emscripten, iOS, and Android (v1.2)
- Server support (v1.3)
- Raytracing mode (v1.4)

## Compiling
### Requirements:
- LLVM/Clang (the project uses clang-specific extensions).
- SDL2 library and headers, which are provided in the corresponding [`External.zip`](https://github.com/johnpayne-dev/MinecraftC/releases/tag/v1.0).
- If you're using Linux, grab the library with this command instead: `sudo apt-get install libsdl2-dev`
### Instructions:
1. Clone or download the repository and rename top directory to `"MinecraftC/"` if it's not already
2. (Windows, MacOS) Drag the `"Libraries"` and `"Include"` folders from `External.zip` into `"MinecraftC/"` 
3. Open the terminal in `"MinecraftC/"`
4. Run the following command, according to your OS:
   - Windows: `./BuildScripts/Windows.bat`
   - MacOS: `sh BuildScripts/MacOS.sh`
   - Linux: `sh BuildScripts/Linux.sh`
5. The binary should have been exported to `"MinecraftC/Output"`
6. (MacOS) Optionally, you may sign the app by running `sh ./BuildScripts/MacOS-Sign.sh <DeveloperID>`
   - You can find your developer id by running `security find-identity`

## Modifying Resources
If you're creating a mod and want to change or add any `.png` resouces, then follow these steps to embed the files into `.h` files:

1. Put all `.png` files that you're modifying or adding into `"MinecraftC/Tools/Input"` (you'll see that the original resources already exist there)
2. Run `python3 PNGEmbedder.py`
   - You will need to run `pip3 install pillow` if you do not already have PIL installed
3. All `.png` files in all subdirectories should have been automatically converted to `.h` files and placed into `MinecraftC/Resources/`

## Known Issues
- Known bugs are listed [here](https://github.com/johnpayne-dev/MinecraftC/releases/tag/v1.0).
- There is no survival mode, it was initially implemented in the beta stage of this project however removed for a few different reasons:
  - Very buggy and slowed down development,
  - Not compatible with the beta raytracing mode,
  - By removing it, this project avoids issues with the EULA by making the game, in essence, a mod.


## Discord
Find any bugs? Want to show off something cool? Join our [discord server](https://discord.gg/2BVnXdU5rB).
