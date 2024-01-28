# Game Creation Took-kit (GCTk)

Simple, cross-platform 2D/3D game engine using OpenGL 4.6 with AngelScript bindings.

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
vcpkg install SDL2 GLEW
```

After you've installed the required libraries and setup the tool chain with your IDE, the project should build with no issues.

#### Linux:

Download dependencies with your system's package manager.

###### Apt:
```shell
apt install libsdl2-dev libglew-dev
```
###### Pacman:
```shell
pacman -S sdl2 glew
```
###### Dnf:
```shell
dnf install SDL2-devel glew
```
###### Yum:
```shell
yum install SDL2-devel glew
```

After all dependencies are met, the project should build with no issues.

## Dependencies:
- GLEW
- AngelScript (Version 2.36.1)
- SDL2
- GLM (as submodule)
- FMT lib (as submodule)