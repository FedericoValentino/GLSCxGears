#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <fstream>
#include <vector>


const char* vertexShaderSource =
        "#version 100\n"  // OpenGL SC uses GLSL ES 1.00
        "attribute vec4 aPosition;    \n"
        "void main() {                \n"
        "   gl_Position = aPosition;  \n"
        "}                            \n";

const char* fragmentShaderSource =
        "#version 100\n"  // OpenGL SC uses GLSL ES 1.00
        "precision mediump float;     \n"
        "void main() {                \n"
        "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
        "}                            \n";


GLuint createProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint program = glCreateProgram();


    std::ifstream file("shaders/shader.bin", std::ios::binary);
    std::vector<uint8_t> binary((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());


    GLenum binaryFormat = 36385;
    glProgramBinary(program, binaryFormat, binary.data(), binary.size());
    file.close();
    return program;
}

int main() {
    if(!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OPENGL SC", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLSC2Loader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to load OpenGL SC functions\n");
        glfwTerminate();
        return -1;
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    GLuint program = createProgram(vertexShaderSource, fragmentShaderSource);
    glUseProgram(program);

    // Define the triangle vertices
    GLfloat vertices[] = {
            0.0f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
    };

    // Create a vertex buffer object (VBO)
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint positionLoc = glGetAttribLocation(program, "aPosition");
    glEnableVertexAttribArray(positionLoc);

    // Set up the vertex attribute pointer
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Set the clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

