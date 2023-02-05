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
 - Post processing pipeline (currently only film grain is implemented)


### Future planned features:
  - Sound component to produce sound with spatial options
  - directional, spot and area lights
  - Shadows
  - Hot reload of assets from disk
  - Customizable post processing shaders
  - Particle system
  - normal, height and bump mapping
  - In-Scene level editor
  - HUD component
  - Multi-texture models

# How to use

First thing to do is to download the code:
```
git clone https://github.com/AsperTheDog/CShard --recursive
```
Notice the `--recursive`. This is important, that way all of the necessary dependencies are properly downloaded.  
In the CShard folder you will find the solution file for Visual Studio 2022.

# Prerequisites

The system needs a GPU capable of running OpenGL 4.6.  
Visual Studio 2022 must be installed.
