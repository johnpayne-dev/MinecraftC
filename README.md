# MinecraftC - A 0.0.30a implementation in C

## Features
- True to the original version
- Completed creative mode
- Implemented fully in C using SDL2 and OpenGL 1.1

## Screenshots
***
<img width="855" alt="Screen Shot 2021-03-10 at 2 09 46 PM" src="https://user-images.githubusercontent.com/79012575/110685071-e5d0f700-81ab-11eb-90c7-8ac8aa8d4283.png">
<img width="858" alt="Screen Shot 2021-03-10 at 2 16 12 PM" src="https://user-images.githubusercontent.com/79012575/110685245-11ec7800-81ac-11eb-9081-c0ebc11f23d9.png">

## Downloads
***
- [Windows (32 bit)]()
- [Windows (64 bit)]()
- [MacOS (ARM)]()
- [MacOS (x86)]()
- [Linux]()

## Planned features
***
- Sound effects and music (v1.1)
- Saving and loading levels (v1.1)
- Support for Emscripten, iOS, and Android (v1.2)
- Server support (v1.3)
- Raytracing mode (v1.4)

## Compiling
***
### Requirements:
- LLVM/Clang (the project uses clang-specific extensions).
- SDL2 library and headers, which are provided [here]().
### Instructions:
1. Clone or download the repository
2. Drag the `"Libraries"` and `"Include"` folders from the download above into `"MinecraftC/"`
3. Open the terminal in `"MinecraftC/"`
4. Run the following command, according to your OS:
   - Windows: `./BuildScripts/Windows.bat`
   - MacOS: `sh BuildScripts/MacOS.sh`
   - Linux: `sh BuildScripts/Linux.sh`
5. The binary should have been exported to `"MinecraftC/Output"`

## Modifying Resources
***
If you're creating a mod and want to change or add any `.png` resouces, then follow these steps to embed the files into `.h` files:

1. Put all `.png` files that you're modifying or adding into `"MinecraftC/Tools/Input"` (you'll see that the original resources already exist there)
2. Run `python3 PNGEmbedder.py`
   - You will need to run `pip3 install pillow` if you do not already have PIL installed
3. All `.png` files in all subdirectories should have been automatically converted to `.h` files and placed into `MinecraftC/Resources/`

## Discord
***
Find any bugs? Want to show off something cool? Join our [discord server](https://discord.gg/2BVnXdU5rB).
