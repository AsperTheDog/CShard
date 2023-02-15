# CShard
Reimplementation in C++ of the closed source Shard engine for the course Introduction To Game Engine Architecture

### The engine offers the following functionality:
 - Basic object system that can have components:
    - Camera component, represents a camera in the game
    - Model component, represents a 3D model with a texture
    - Script component, represents a lua script that can be executed on demand
    - Collider component, represents a collision testing element that will detect collisions with other colliders
    - Background component, represents a skybox
    - Light component, represents a point light source
 - A fully functioning GUI that lets you hot import meshes, textures and scripts
 - Post processing pipeline (While the number, types and order of the effects are customizable, right now they are simply fixed functions)
 - A complete LUA interface to make scripts with hot reload (even while the game is executing) and the ability to modify parameters of almost everything


### Future planned features:
  - Sound component to produce sound with spatial options
  - directional, spot and area lights
  - Shadows
  - Hot reload of assets from disk
  - Removing assets from project
  - Loading screen, preventing the app from freezing
  - Customizable post processing shaders
  - Particle system
  - normal, height and bump mapping
  - In-Scene level editor
  - HUD component
  - Multi-texture models
  - Temporary savestate to revert when the game finishes executing (scene must be saved when the player clicks "start" and restored when they click "stop")
  - Input mapping modification from LUA scripts
  - Creation and destruction of objects and components from LUA scripts
  - WIKI

# How to use

First thing to do is to download the code:
```
git clone https://github.com/AsperTheDog/CShard --recursive
```
Notice the `--recursive`. This is important, that way all of the necessary dependencies are properly downloaded.  
In the CShard folder you will find the solution file for Visual Studio 2022.

**If the project fails because SDL2.dll is required, simply copy it from CShard/dependencies/sdl/lib and paste it beside the project executable (e.g. CShard/x64/release for 64bit release build)**

# Prerequisites

The system needs a GPU capable of running OpenGL 4.6.  
Visual Studio 2022 must be installed.

# Screenshots
![image](https://github.com/AsperTheDog/CShard/blob/main/wiki/example1.png?raw=true)
