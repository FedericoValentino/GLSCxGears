mkdir build
cd build
cmake ..
cmake --build . --target ShaderCompiler
cd ..

binaryFormat=$(./ShaderCompiler/ShaderCompiler)

sed -i "s/^#define BINARY_FORMAT.*/#define BINARY_FORMAT ${binaryFormat}/" GearsSC/src/main.cpp

mv shader.bin GearsSC/shaders/

cd build
cmake --build . --target Gears_SC

cd ..

cd GearsSC/

./Gears_SC
