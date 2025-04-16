#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BINARY_FORMAT 36385

typedef struct {
    //Vertex Data
    std::vector<glm::vec3> FrontFace;
    unsigned int FrontFaceVBO;
    std::vector<glm::vec3> FrontTeethFace;
    unsigned int FrontTeethFaceVBO;
    std::vector<glm::vec3> BackFace;
    unsigned int BackFaceVBO;
    std::vector<glm::vec3> BackTeethFace;
    unsigned int BackTeethFaceVBO;
    std::vector<glm::vec3> OutFaceTeeth;
    unsigned int OutFaceVBO;
    std::vector<glm::vec3> InsideRadius;
    unsigned int InsideRadiusVBO;

    glm::mat4 model;
    glm::mat3 normalMatrix;
    glm::vec3 position;
    float angle;
    glm::vec4 color;

} gearVertices;

gearVertices gear1;
gearVertices gear2;
gearVertices gear3;

//Global Variables
GLFWwindow* window;
GLint width = 300;
GLint height = 300;

GLuint program;

glm::mat4 view;
glm::mat4 projection;

GLfloat globalAngle = 0.0f;

float lastTime = 0.0f;  // Store the last time (in seconds)
float dt = 0.0f;        // Store delta time

void update()
{
    float currentTime = glfwGetTime();
    dt = currentTime - lastTime;
    lastTime = currentTime;
}


GLuint createProgram(const char* filename) {
    GLuint program = glCreateProgram();

    std::ifstream file(filename, std::ios::binary);
    std::vector<uint8_t> binary((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    GLenum binaryFormat = BINARY_FORMAT;
    glProgramBinary(program, binaryFormat, binary.data(), binary.size());
    file.close();
    return program;
}

static void gear(gearVertices &stGear, GLfloat inner_radius, GLfloat outer_radius, GLfloat width, GLint teeth, GLfloat tooth_depth)
{
    GLint i = 0;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;

    r0 = inner_radius;
    r1 = outer_radius - tooth_depth / 2.0;
    r2 = outer_radius + tooth_depth / 2.0;

    da = 2.0 * M_PI / teeth / 4.0;

    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);

    /* draw front face */
    angle = i * 2.0 * M_PI / teeth;


    glm::vec3 v1 = glm::vec3(r0 * cos(angle), r0 * sin(angle), width * 0.5);
    glm::vec3 v2 = glm::vec3(r1 * cos(angle), r1 * sin(angle), width * 0.5);

    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;
        glm::vec3 v3 = glm::vec3(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),width * 0.5);
        GLfloat nextAngle = (i + 1) * 2.0 * M_PI / teeth;
        glm::vec3 v4 = glm::vec3(r0 * cos(nextAngle), r0 * sin(nextAngle), width * 0.5);
        glm::vec3 v5 = glm::vec3(r1 * cos(nextAngle), r1 * sin(nextAngle), width * 0.5);

        //triangle 1
        stGear.FrontFace.push_back(v1);
        stGear.FrontFace.push_back(normal);
        stGear.FrontFace.push_back(v2);
        stGear.FrontFace.push_back(normal);
        stGear.FrontFace.push_back(v3);
        stGear.FrontFace.push_back(normal);

        //triangle 2
        stGear.FrontFace.push_back(v1);
        stGear.FrontFace.push_back(normal);
        stGear.FrontFace.push_back(v3);
        stGear.FrontFace.push_back(normal);
        stGear.FrontFace.push_back(v5);
        stGear.FrontFace.push_back(normal);

        //triangle 3
        stGear.FrontFace.push_back(v1);
        stGear.FrontFace.push_back(normal);
        stGear.FrontFace.push_back(v5);
        stGear.FrontFace.push_back(normal);
        stGear.FrontFace.push_back(v4);
        stGear.FrontFace.push_back(normal);

        v1 = v4;
        v2 = v5;
    }

    /* draw front sides of teeth */
    da = 2.0 * M_PI / teeth / 4.0;
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;

        glm::vec3 v1 = glm::vec3(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glm::vec3 v2 = glm::vec3(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
        glm::vec3 v3 = glm::vec3(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
        glm::vec3 v4 = glm::vec3(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);

        //triangle 1
        stGear.FrontTeethFace.push_back(v1);
        stGear.FrontTeethFace.push_back(normal);
        stGear.FrontTeethFace.push_back(v2);
        stGear.FrontTeethFace.push_back(normal);
        stGear.FrontTeethFace.push_back(v3);
        stGear.FrontTeethFace.push_back(normal);

        //triangle 2
        stGear.FrontTeethFace.push_back(v1);
        stGear.FrontTeethFace.push_back(normal);
        stGear.FrontTeethFace.push_back(v3);
        stGear.FrontTeethFace.push_back(normal);
        stGear.FrontTeethFace.push_back(v4);
        stGear.FrontTeethFace.push_back(normal);
    }

    normal = glm::vec3(0.0, 0.0, -1.0);


    /* draw back face */
    angle = i * 2.0 * M_PI / teeth;
    v1 = glm::vec3(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    v2 = glm::vec3(r1 * cos(angle), r1 * sin(angle), -width * 0.5);

    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;
        glm::vec3 v3 = glm::vec3(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),-width * 0.5);
        GLfloat nextAngle = (i + 1) * 2.0 * M_PI / teeth;
        glm::vec3 v4 = glm::vec3(r0 * cos(nextAngle), r0 * sin(nextAngle), -width * 0.5);
        glm::vec3 v5 = glm::vec3(r1 * cos(nextAngle), r1 * sin(nextAngle), -width * 0.5);

        //triangle 1
        stGear.BackFace.push_back(v1);
        stGear.BackFace.push_back(normal);
        stGear.BackFace.push_back(v2);
        stGear.BackFace.push_back(normal);
        stGear.BackFace.push_back(v3);
        stGear.BackFace.push_back(normal);

        //triangle 2
        stGear.BackFace.push_back(v1);
        stGear.BackFace.push_back(normal);
        stGear.BackFace.push_back(v3);
        stGear.BackFace.push_back(normal);
        stGear.BackFace.push_back(v5);
        stGear.BackFace.push_back(normal);

        //triangle 3
        stGear.BackFace.push_back(v1);
        stGear.BackFace.push_back(normal);
        stGear.BackFace.push_back(v5);
        stGear.BackFace.push_back(normal);
        stGear.BackFace.push_back(v4);
        stGear.BackFace.push_back(normal);

        v1 = v4;
        v2 = v5;
    }

    /* draw back sides of teeth */
    da = 2.0 * M_PI / teeth / 4.0;
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;

        glm::vec3 v1 = glm::vec3(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
        glm::vec3 v2 = glm::vec3(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
        glm::vec3 v3 = glm::vec3(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
        glm::vec3 v4 = glm::vec3(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);

        //triangle 1
        stGear.BackTeethFace.push_back(v1);
        stGear.BackTeethFace.push_back(normal);
        stGear.BackTeethFace.push_back(v2);
        stGear.BackTeethFace.push_back(normal);
        stGear.BackTeethFace.push_back(v3);
        stGear.BackTeethFace.push_back(normal);

        //triangle 2
        stGear.BackTeethFace.push_back(v1);
        stGear.BackTeethFace.push_back(normal);
        stGear.BackTeethFace.push_back(v3);
        stGear.BackTeethFace.push_back(normal);
        stGear.BackTeethFace.push_back(v4);
        stGear.BackTeethFace.push_back(normal);
    }

    /* draw outward faces of teeth */
    i = 0;
    for (i = 0; i < teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;
        GLfloat nextAngle = (i + 1) * 2.0 * M_PI / teeth;
        glm::vec3 v1 = glm::vec3(r1 * cos(angle), r1 * sin(angle), width * 0.5);
        glm::vec3 v2 = glm::vec3(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
        glm::vec3 v3 = glm::vec3(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
        glm::vec3 v4 = glm::vec3(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
        glm::vec3 v5 = glm::vec3(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
        glm::vec3 v6 = glm::vec3(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
        glm::vec3 v7 = glm::vec3(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
        glm::vec3 v8 = glm::vec3(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
        glm::vec3 v9 = glm::vec3(r1 * cos(nextAngle), r1 * sin(nextAngle), width * 0.5);
        glm::vec3 v10 = glm::vec3(r1 * cos(nextAngle), r1 * sin(nextAngle), -width * 0.5);





        //face 1
        u = r2 * cos(angle + da) - r1 * cos(angle);
        v = r2 * sin(angle + da) - r1 * sin(angle);
        len = sqrt(u * u + v * v);
        u /= len;
        v /= len;
        normal = glm::vec3(v, -u, 0.0);

        stGear.OutFaceTeeth.push_back(v1);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v2);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v3);
        stGear.OutFaceTeeth.push_back(normal);

        stGear.OutFaceTeeth.push_back(v2);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v3);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v4);
        stGear.OutFaceTeeth.push_back(normal);

        //face 2
        normal = glm::vec3(cos(angle), sin(angle), 0.0);

        stGear.OutFaceTeeth.push_back(v3);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v4);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v5);
        stGear.OutFaceTeeth.push_back(normal);

        stGear.OutFaceTeeth.push_back(v4);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v5);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v6);
        stGear.OutFaceTeeth.push_back(normal);

        //face 3

        u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
        v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
        normal = glm::vec3(v, -u, 0.0);

        stGear.OutFaceTeeth.push_back(v5);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v6);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v7);
        stGear.OutFaceTeeth.push_back(normal);

        stGear.OutFaceTeeth.push_back(v6);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v7);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v8);
        stGear.OutFaceTeeth.push_back(normal);

        //face 4

        u = r2 * cos(nextAngle + da) - r1 * cos(nextAngle);
        v = r2 * sin(nextAngle + da) - r1 * sin(nextAngle);
        len = sqrt(u * u + v * v);
        u /= len;
        v /= len;
        normal = glm::vec3(v, -u, 0.0);

        stGear.OutFaceTeeth.push_back(v7);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v8);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v9);
        stGear.OutFaceTeeth.push_back(normal);

        stGear.OutFaceTeeth.push_back(v8);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v9);
        stGear.OutFaceTeeth.push_back(normal);
        stGear.OutFaceTeeth.push_back(v10);
        stGear.OutFaceTeeth.push_back(normal);

    }

    /* draw inside radius cylinder */
    for (i = 0; i <= teeth; i++) {
        angle = i * 2.0 * M_PI / teeth;
        glm::vec3 v1 = glm::vec3(r0 * cos(angle), r0 * sin(angle), width * 0.5);
        glm::vec3 v2 = glm::vec3(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
        GLfloat nextAngle = (i + 1) * 2.0 * M_PI / teeth;

        glm::vec3 v3 = glm::vec3(r0 * cos(nextAngle), r0 * sin(nextAngle), width * 0.5);
        glm::vec3 v4 = glm::vec3(r0 * cos(nextAngle), r0 * sin(nextAngle), -width * 0.5);

        normal = glm::vec3(-cos(angle), -sin(angle), 0.0);

        stGear.InsideRadius.push_back(v1);
        stGear.InsideRadius.push_back(normal);
        stGear.InsideRadius.push_back(v2);
        stGear.InsideRadius.push_back(normal);
        stGear.InsideRadius.push_back(v3);
        stGear.InsideRadius.push_back(normal);

        stGear.InsideRadius.push_back(v2);
        stGear.InsideRadius.push_back(normal);
        stGear.InsideRadius.push_back(v3);
        stGear.InsideRadius.push_back(normal);
        stGear.InsideRadius.push_back(v4);
        stGear.InsideRadius.push_back(normal);
    }
}

void initGear(gearVertices& stGear, GLfloat inner_radius, GLfloat outer_radius, GLfloat width, GLint teeth, GLfloat tooth_depth)
{
    gear(stGear, inner_radius, outer_radius, width, teeth, tooth_depth);

    //Vertex data
    glGenBuffers(1, &stGear.FrontFaceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, stGear.FrontFaceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * stGear.FrontFace.size(), &stGear.FrontFace[0].x, GL_STATIC_DRAW);

    glGenBuffers(1, &stGear.FrontTeethFaceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, stGear.FrontTeethFaceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * stGear.FrontTeethFace.size(), &stGear.FrontTeethFace[0].x, GL_STATIC_DRAW);

    glGenBuffers(1, &stGear.BackFaceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, stGear.BackFaceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * stGear.BackFace.size(), &stGear.BackFace[0].x, GL_STATIC_DRAW);

    glGenBuffers(1, &stGear.BackTeethFaceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, stGear.BackTeethFaceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * stGear.BackTeethFace.size(), &stGear.BackTeethFace[0].x, GL_STATIC_DRAW);

    glGenBuffers(1, &stGear.OutFaceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, stGear.OutFaceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * stGear.OutFaceTeeth.size(), &stGear.OutFaceTeeth[0].x, GL_STATIC_DRAW);

    glGenBuffers(1, &stGear.InsideRadiusVBO);
    glBindBuffer(GL_ARRAY_BUFFER, stGear.InsideRadiusVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * stGear.InsideRadius.size(), &stGear.InsideRadius[0].x, GL_STATIC_DRAW);

    stGear.model = glm::rotate(glm::mat4(1), glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0))
            * glm::rotate(glm::mat4(1), glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0))
            * glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0))
            * glm::translate(glm::mat4(1.0f), stGear.position);

    stGear.normalMatrix = glm::transpose(glm::inverse(glm::mat3(stGear.model)));
}

void init()
{

    gear1.position = glm::vec3(-3.0, -2.0, 0.0);
    gear2.position = glm::vec3(3.1, -2.0, 0.0);
    gear3.position = glm::vec3(-3.1, 4.2, 0.0);
    gear1.color = glm::vec4(0.8, 0.1, 0.0, 1.0);
    gear2.color = glm::vec4(0.0, 0.8, 0.2, 1.0);
    gear3.color = glm::vec4(0.2, 0.2, 1.0, 1.0);

    initGear(gear1, 1.0, 4.0, 1.0, 20, 0.7);
    initGear(gear2, 0.5, 2.0, 2.0, 10, 0.7);
    initGear(gear3, 1.3, 2.0, 0.5, 10, 0.7);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    view = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, -40.0f));

    projection = glm::frustum(-1.0f, 1.0f, -1.0f , 1.0f, 5.0f ,60.0f);

    program = createProgram("shaders/shader.bin");
}


void drawGear(gearVertices &stGear)
{
    glUseProgram(program);

    GLint positionLoc = glGetAttribLocation(program, "aPos");
    GLint positionNormalLoc = glGetAttribLocation(program, "aNorm");

    int modelLoc = glGetUniformLocation(program, "model");
    stGear.model = glm::rotate(glm::mat4(1), glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0))
                   * glm::rotate(glm::mat4(1), glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0))
                   * glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0))
                   * glm::translate(glm::mat4(1.0f), stGear.position);
    stGear.model = glm::rotate(stGear.model, glm::radians(stGear.angle), glm::vec3(0.0, 0.0, 1.0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(stGear.model));

    int viewLoc = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projLoc = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    int normMatrLoc = glGetUniformLocation(program, "normalMatrix");
    stGear.normalMatrix = glm::transpose(glm::inverse(glm::mat3(stGear.model)));
    glUniformMatrix3fv(normMatrLoc, 1, GL_FALSE, glm::value_ptr(stGear.normalMatrix));

    int colorLoc = glGetUniformLocation(program, "color");
    glUniform4fv(colorLoc, 1, glm::value_ptr(stGear.color));

    glBindBuffer(GL_ARRAY_BUFFER, stGear.FrontFaceVBO);
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(positionNormalLoc);
    glVertexAttribPointer(positionNormalLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glDrawArrays(GL_TRIANGLES, 0, stGear.FrontFace.size());


    glBindBuffer(GL_ARRAY_BUFFER, stGear.FrontTeethFaceVBO);
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(positionNormalLoc);
    glVertexAttribPointer(positionNormalLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glDrawArrays(GL_TRIANGLES, 0, stGear.FrontTeethFace.size());


    glBindBuffer(GL_ARRAY_BUFFER, stGear.BackFaceVBO);
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(positionNormalLoc);
    glVertexAttribPointer(positionNormalLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glDrawArrays(GL_TRIANGLES, 0, stGear.BackFace.size());

    glBindBuffer(GL_ARRAY_BUFFER, stGear.BackTeethFaceVBO);
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(positionNormalLoc);
    glVertexAttribPointer(positionNormalLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glDrawArrays(GL_TRIANGLES, 0, stGear.BackTeethFace.size());

    glBindBuffer(GL_ARRAY_BUFFER, stGear.OutFaceVBO);
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(positionNormalLoc);
    glVertexAttribPointer(positionNormalLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glDrawArrays(GL_TRIANGLES, 0, stGear.OutFaceTeeth.size());


    glBindBuffer(GL_ARRAY_BUFFER, stGear.InsideRadiusVBO);
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(positionNormalLoc);
    glVertexAttribPointer(positionNormalLoc, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glDrawArrays(GL_TRIANGLES, 0, stGear.InsideRadius.size());

}

void drawObject()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gear1.angle = globalAngle;
    gear2.angle = -2.0 * globalAngle - 9.0;
    gear3.angle = -2.0 * globalAngle - 25.0;

    drawGear(gear1);
    drawGear(gear2);
    drawGear(gear3);
}

void draw()
{
    float start, end;
    unsigned long long count = 0;

    start = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        update();
        globalAngle += 70.0f * dt;  /* 70 degrees per second */
        if (globalAngle > 3600.0)
            globalAngle -= 3600.0;

        drawObject();

        glfwSwapBuffers(window);

        glfwPollEvents();

        count++;

        end = glfwGetTime();

        float elapsed_time = end - start;

        if(elapsed_time >= 5.0)
        {
            std::cout<<"Generated "<<count<<"frames in 5 seconds"<<std::endl;
            start = glfwGetTime();
            count = 0;
        }
    }
}


int main() {
    if(!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(width, height, "Gears SC", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLSC2Loader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to load OpenGL SC functions\n");
        glfwTerminate();
        return -1;
    }


    init();

    printf("Gear1 using %zu vertices\n", gear1.FrontTeethFace.size() +
                                            gear1.FrontFace.size() +
                                            gear1.BackFace.size() +
                                            gear1.BackTeethFace.size() +
                                            gear1.OutFaceTeeth.size() +
                                            gear1.InsideRadius.size());
    printf("Gear2 using %zu vertices\n", gear2.FrontTeethFace.size() +
                                       gear2.FrontFace.size() +
                                       gear2.BackFace.size() +
                                       gear2.BackTeethFace.size() +
                                       gear2.OutFaceTeeth.size() +
                                       gear2.InsideRadius.size());
    printf("Gear3 using %zu vertices\n", gear3.FrontTeethFace.size() +
                                       gear3.FrontFace.size() +
                                       gear3.BackFace.size() +
                                       gear3.BackTeethFace.size() +
                                       gear3.OutFaceTeeth.size() +
                                       gear3.InsideRadius.size());

    draw();


    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

