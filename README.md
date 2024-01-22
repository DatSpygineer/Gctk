# Game Creation Took-kit (GCTk)

Simple, cross-platform 2D/3D game engine using OpenGL 4.6 with Lua bindings.

This project is split into four subprojects:
- GctkMath -> Basic math library.
- GctkGtex -> GCTk texture format library.
- Gctk -> Game engine core library.
- StrLib -> String library used for the game engine.

## How to build:
#### Windows:
On Windows packages are loaded using VCPKG.<br/>
> To use VCPKG with CMAKE see: https://learn.microsoft.com/en-us/vcpkg/users/buildsystems/cmake-integration

First make sure you have SDL2, GLEW and Lua libraries installed.
```shell
vcpkg install SDL2 GLEW lua
```

After you've installed the required libraries and setup the tool chain with your IDE, the project should build with no issues.

#### Linux:
TODO

## Dependencies:
- GLEW
- Lua 5.4 or newer
- SDL2
- GLM (as submodule)
- FMT lib (as submodule)