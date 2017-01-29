// assignment1.cpp : CSC 433/533 assignment 1 - Spring 2017.
//	based on the 9th edition of the OpenGL Programming Guide (red book)
//
#include "vgl.h"
#include "LoadShaders.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

#define PI 3.14

using namespace std;

enum VAO_IDs { Triangles, NumVAOs  };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition, vColor };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint colorBuffer[1];

const GLuint NumVertices = 9;
GLuint program, circleVertices, heartVertices;
GLfloat radius = 0.6f;
GLuint circleSteps = 51, heartSteps = 101;

struct vertexData {
        GLfloat color[4];
        GLfloat vertex[4];
};


// Function that generates coordinates for the rendering of circle

vector<vertexData> circle(GLfloat radius, GLuint steps ) {

        vector<vertexData> circleVector;
        GLfloat circleFragment = 360.0f/steps;
        vertexData data;

        for(GLfloat angle = 0.0f; angle <= 360.0f; angle+=circleFragment) {
                GLfloat x = radius * cos((angle * PI) / 180.0f);
                GLfloat y = radius * sin((angle * PI) / 180.0f);
                data.color[0] = 0.0f; data.color[1] = 0.0f;
                data.color[2] = 1.0f; data.color[3] = 1.0f;
                data.vertex[0] = x; data.vertex[1] = y;
                data.vertex[2] = 0.0f; data.vertex[3] = 0.0f;
                circleVector.push_back(data);
        }

        return circleVector;
}

// Functions that generates coodinates for the rendering of heart

vector<vertexData> heart(GLuint steps){

        vector<vertexData> heartVector;
        GLfloat heartFragment = 360.0f/steps;
        vertexData data;

        for(GLfloat angle = 0.0f; angle <= 360.0f; angle+=heartFragment) {

                GLfloat x = (16.0f/17.0f) * sin((angle * PI) / 180.0f) *
                            sin((angle * PI) / 180.0f) * sin((angle * PI) / 180.0f);
                GLfloat y = (1.0f/17.0f) * (13 * cos((angle * PI) / 180.0f) -
                                            5 * cos(2 * ((angle * PI) / 180.0f))
                                            - 2 * cos(3 * ((angle * PI) / 180.0f))
                                            - cos(4*((angle * PI) / 180.0f)));

                data.color[0] = 1.0f; data.color[1] = 0.0f;
                data.color[2] = 0.0f; data.color[3] = 1.0f;
                data.vertex[0] = x; data.vertex[1] = y;
                data.vertex[2] = 0.0f; data.vertex[3] = 0.0f;
                heartVector.push_back(data);
        }

        return heartVector;
}


//----------------------------------------------------------------------------
//
// init
//

void init(void) {

        vertexData triangleVertices[NumVertices] = {
                {{0.0f, 0.0f, 1.0f, 1.0f}, {-0.90f, -0.90f}},   //triangle 1
                {{0.0f, 0.0f, 1.0f, 1.0f}, {0.85f, -0.90f}},
                {{0.0f, 0.0f, 1.0f, 1.0f}, {-0.90f, 0.85f}},
                {{0.0f, 0.0f, 1.0f, 1.0f}, {0.90f, -0.85f}},    //triangle2
                {{0.0f, 0.0f, 1.0f, 1.0f}, {0.90f, 0.90f}},
                {{0.0f, 0.0f, 1.0f, 1.0f}, {-0.85f, 0.90f}},
                {{1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}},       //colored triangle
                {{0.0f, 1.0f, 0.0f, 1.0f}, {0.7f, -0.35f}},
                {{0.0f, 0.0f, 1.0f, 1.0f}, {-0.75f, -0.35f}}
        };

        vector<vertexData> allVertices;
        for(int i = 0; i < NumVertices; i++) {
                allVertices.push_back(triangleVertices[i]);
        }

        vector<vertexData> Circle = circle(radius, circleSteps);
        circleVertices = Circle.size();

        for(int i = 0; i < circleVertices; i++) {
                allVertices.push_back(Circle.at(i));
        }


        vector<vertexData> Heart = heart(heartSteps);
        heartVertices = Heart.size();

        for(int i = 0; i < heartVertices; i++) {
                allVertices.push_back(Heart.at(i));
        }

        GLuint numVertices = allVertices.size();
        cout << numVertices << endl;
        vertexData vertexArray[numVertices];
        for(int i = 0; i < numVertices; i++) {
                for(int j = 0; j < 4; j++) {
                        vertexArray[i].color[j] = allVertices[i].color[j];
                        vertexArray[i].vertex[j] = allVertices[i].vertex[j];
                        //cout << vertexArray[i].color[j]<<","<<vertexArray[i].vertex[j] << endl;
                }
        }

        glGenVertexArrays(NumVAOs, VAOs);
        glBindVertexArray(VAOs[Triangles]);
        glGenBuffers(NumBuffers, Buffers);
        glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

        ShaderInfo shaders[] = {{GL_VERTEX_SHADER, "triangles.vert"},
                                {GL_FRAGMENT_SHADER, "triangles.frag"},
                                {GL_NONE, NULL}};

        program = LoadShaders(shaders);
        if (program == NULL) {
                cerr << "Failed shader1 load" << endl;
        }
        glUseProgram(program);

        glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(0));
        glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(vertexArray[0].color)));
        glEnableVertexAttribArray(vColor);
        glEnableVertexAttribArray(vPosition);

}

//----------------------------------------------------------------------------
//
// display
//

void display(void) {
        static const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};

        glClearBufferfv(GL_COLOR, 0, black);
        glUseProgram(program);
        glBindVertexArray(VAOs[Triangles]);
        //glDrawArrays(GL_TRIANGLES, 0, 9);
        glDrawArrays(GL_TRIANGLE_FAN, NumVertices, circleVertices);
        glDrawArrays(GL_LINE_STRIP, circleVertices+NumVertices, heartVertices);

}


//----------------------------------------------------------------------------
//
// main
//

#undef _WIN32
#ifdef _WIN32
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
        glfwInit();

        //#ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                       3); // 4.1 latest version of OpenGL for OSX 10.9 Mavericks
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //#endif

        GLFWwindow *window = glfwCreateWindow(800, 800, "Assignment1", NULL, NULL);

        glfwMakeContextCurrent(window);

        gl3wInit();

#define CHECK_VERSION
#ifdef CHECK_VERSION
        // code from OpenGL 4 Shading Language cookbook, second edition
        const GLubyte *renderer = glGetString(GL_RENDERER);
        const GLubyte *vendor = glGetString(GL_VENDOR);
        const GLubyte *version = glGetString(GL_VERSION);
        const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        cout << "GL Vendor            :" << vendor << endl;
        cout << "GL Renderer          :" << renderer << endl;
        cout << "GL Version (string)  :" << version << endl;
        cout << "GL Version (integer) :" << major << " " << minor << endl;
        cout << "GLSL Version         :" << glslVersion << endl;
        cout << "major version: " << major << "  minor version: " << minor << endl;
#endif

        init();

        while (!glfwWindowShouldClose(window)) {
                display();
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glfwDestroyWindow(window);

        glfwTerminate();
}
