#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>


const char* vertexShaderSource =
        "#version 100\n"  // OpenGL SC uses GLSL ES 1.00
        "attribute vec3 aPos;                                                     \n"
        "attribute vec3 aNorm;                                                    \n"
        "uniform mat4 model;                                                      \n"
        "uniform mat4 view;                                                       \n"
        "uniform mat4 projection;                                                 \n"
        "uniform mat3 normalMatrix;                                               \n"
        "varying vec3 Normal;                                                     \n"
        "varying vec3 FragPos;                                                    \n"
        "void main() {                                                            \n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);            \n"
        "   FragPos = vec3(model * vec4(aPos, 1.0));                              \n"
        "   Normal = normalize(normalMatrix * aNorm);                             \n"
        "}                                                                        \n";

// Fragment shader source code
const char* fragmentShaderSource =
        "#version 100\n"  // OpenGL SC uses GLSL ES 1.00
        "precision mediump float;                                                 \n"
        "varying vec3 Normal;                                                     \n"
        "varying vec3 FragPos;                                                    \n"
        "uniform vec4 color;                                                      \n"
        "void main() {                                                            \n"
        "   vec3 ambient = vec3(0.3, 0.3, 0.3);                                   \n"
        "   vec3 lightPos = vec3(5.0, 5.0, 10.0);                                 \n"
        "   vec3 norm = normalize(Normal);                                        \n"
        "   vec3 lightDir = normalize(lightPos - FragPos);                        \n"
        "   float diff = max(dot(norm, lightDir), 0.0);                           \n"
        "   vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);                            \n"
        "   vec3 result = (ambient + diffuse) * vec3(color);                      \n"
        "   gl_FragColor = vec4(result, 1.0);                                     \n"

        "}\n";

// Function to compile shaders
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Check for compilation errors
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            printf("Error compiling shader:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

// Function to link shaders into a program
GLuint createProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            printf("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

    GLFWwindow* window = glfwCreateWindow(640, 480, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    GLuint program = createProgram(vertexShaderSource, fragmentShaderSource);

    GLint binaryLength = 0;
    glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &binaryLength);

    std::vector<uint8_t> binary(binaryLength);
    GLenum binaryFormat;

    glGetProgramBinary(program, binaryLength, nullptr, &binaryFormat, binary.data());
    std::cout<<binaryFormat<<std::endl;
// Save binary to file
    std::ofstream file("shader.bin", std::ios::binary);
    file.write(reinterpret_cast<const char*>(binary.data()), binary.size());
    file.close();

}