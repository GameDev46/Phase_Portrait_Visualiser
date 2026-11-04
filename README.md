# Phase Portrait Visualiser

A linear system phase portrait plotter written in C++ using [Raylib](https://www.raylib.com/) and [ImGui](https://github.com/ocornut/imgui)

## Compiling

### Release Build

If you wish to compile your own version from the source code, simply run the following commands in the root directory:

```bash
premake5 gmake
mingw32-make config=release
```

or for a clean build run:

```bash
premake5 gmake
mingw32-make clean config=release
```

To then run the executable use the following command or simply double click the raytracer.exe file in 

```bash
./bin/Release/phase-plotter.exe
```
