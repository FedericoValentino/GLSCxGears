cmake_minimum_required(VERSION 3.30)
project(OPENGL_SC)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(GearsSC/include/glfw-3.3.8)
add_subdirectory(GearsSC/include/glm)

#SHADER_COMPILER Application

add_executable(ShaderCompiler ShaderCompiler/src/glad.c ShaderCompiler/src/main.cpp)

target_include_directories(ShaderCompiler PUBLIC ShaderCompiler/include)

set_target_properties(ShaderCompiler PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/ShaderCompiler")

target_link_libraries(ShaderCompiler glfw)


#GearsSC Application

add_executable(Gears_SC GearsSC/src/glad.c GearsSC/src/main.cpp)

target_include_directories(Gears_SC PUBLIC GearsSC/include)

target_link_libraries(Gears_SC glfw)

set_target_properties(Gears_SC PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/GearsSC")

