cmake_minimum_required(VERSION 3.30)
project(OPENGL_SC)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Application/include/glfw-3.3.8)

#OPENGL_SC Application

add_executable(OPENGL_SC Application/src/glad.c Application/src/main.cpp)

target_include_directories(OPENGL_SC PUBLIC Application/include)

target_link_libraries(OPENGL_SC glfw)

set_target_properties(OPENGL_SC PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Application")

#SHADER_COMPILER Application

add_executable(ShaderCompiler ShaderCompiler/src/glad.c ShaderCompiler/src/main.cpp)

target_include_directories(ShaderCompiler PUBLIC ShaderCompiler/include)

set_target_properties(ShaderCompiler PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/ShaderCompiler")

target_link_libraries(ShaderCompiler glfw)

