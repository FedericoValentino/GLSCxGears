@echo off

mkdir build
cd build

cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER="C:\MinGW\bin\gcc.exe" -DCMAKE_CXX_COMPILER="C:\MinGW\bin\g++.exe" ..

cmake --build . --target ShaderCompiler

cd ..

for /f "delims=" %%i in ('.\ShaderCompiler\ShaderCompiler') do set binaryFormat=%%i

powershell -Command "(Get-Content GearsSC\src\main.cpp) -replace '^#define BINARY_FORMAT.*', '#define BINARY_FORMAT %binaryFormat%' | Set-Content GearsSC\src\main.cpp"

mkdir GearsSC\shaders\
move shader.bin GearsSC\shaders\

cd build

cmake --build . --target Gears_SC

cd ..

cd GearsSC
.\Gears_SC
