#Create the build directory
mkdir build
cd build

#Run Cmake to configure the project
cmake ..

#Build the ShaderCompiler application
cmake --build . --target ShaderCompiler
cd ..

#Run the ShaderCompiler application and save its output
#The output will be the binaryFormat used by the shaders
binaryFormat=$(./ShaderCompiler/ShaderCompiler)

#Update the binary format in the source code of the main application
sed -i "s/^#define BINARY_FORMAT.*/#define BINARY_FORMAT ${binaryFormat}/" GearsSC/src/main.cpp

#Move the compiled shaders to the main application shaders folder
mv shader.bin GearsSC/shaders/

cd build

#Build the Gears_SC application
cmake --build . --target Gears_SC

cd ..

#IMPORTANT cd into the main application directory
cd GearsSC/

#Run the application
./Gears_SC
