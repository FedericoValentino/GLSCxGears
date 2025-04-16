Repository for the Embedded Systems Project for A.Y. 2024/2025

# Gears SC

Gears SC is a port of the classic glxgears demo to OpenGL SC (Safety Critical), which is a subset of OpenGL tailored for safety-critical systems. This version avoids heap allocations and shader compilation at runtime to comply with OpenGL SC constraints.

# Structure of the repository

* `GearsSC` contains the source code for the port of glxgears;
* `ShaderCompiler` contains the source code for the ShaderCompiler, the application responsible for creating the shaders binary container for the main application;
* `Embedded_Systems_OpenGL_SC.pdf` is a report on the main differences between OpenGL, OpenGL ES and OpenGL SC and of the challenges encountered during the porting process.

## Requirements

This project is self-contained and includes all necessary libraries. The only external tool required is:

- `CMake` version 3.28 or higher

The project compiles on both linux and windows given that the appropriate compilers are installed on the machine.

# Building
Firstly, clone the repository:
```
git clone https://github.com/FedericoValentino/GLSCxGears.git
```

Then enter the repository with:
```
cd GLSCxGears
```

Finally, to build and run the application launch:
* Linux:
```
./Compile&Run.sh
```
* Windows:
```
Compile&Run.bat <PathToGCC> <PathToG++>
```

# Running the built application

To run the built application first enter the repository directory, then:
```
cd GearsSC
```

And finally, run:
```
./Gears_SC
```

# Known problems
## Black screen when launching the application
Check that the file `GearsSC/shaders/shaders.bin` exists. If it does check that the application is being launched in the `GearsSC` directory.

If both options have been checked and the problem persists start again with a clean repository.

