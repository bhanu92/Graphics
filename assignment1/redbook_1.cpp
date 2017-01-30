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
//#include <cstdbool>

#define PI 3.14

using namespace std;

enum VAO_IDs { Triangles, NumVAOs  };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition1,vPosition2, vPosition3, vPosition4, vColor };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

GLuint colorVAOs[1];
GLuint colorBuffer[1];

GLuint circleVAOs[1];
GLuint circleBuffer[1];

GLuint heartVAOs[1];
GLuint heartBuffer[1];

const GLuint NumVertices = 6;
GLuint program1, program2, program3, program4;
GLuint circleVertices, heartVertices;
GLfloat radius;
GLuint circleSteps, heartSteps;
GLfloat red = 0.0f, green = 0.0f, blue = 0.0f;
bool twoTriangles = true, coloredTriangle = false, circle = false, heart = false, wireFrame = false;
bool defaultColor = true;


struct vertexData {
        GLfloat color[4];
        GLfloat vertex[4];
};


// Function that reads the users inputs for the triangles and the curves.

void readInputs(){

        cout << "Enter the color values accordingly" << endl;
        cout << "Enter the value for Red: ";
        cin >> red;
        cout << "\nEnter the value for Green: ";
        cin >> green;
        cout << "\nEnter the value for Blue: ";
        cin >> blue;

        if(!((red>=0 && red<=1) && (green>=0 && green<=1) && (blue>=0 && blue<=1))) {
                cout << "Entered out of bound values. Enter in between <0,1>";
                return;
        }
        else
                defaultColor = false;
}

// Function for circle settings

void circleInputs(){

        cout << "\nEnter the value of Radius of circle(0,1): ";
        cin >> radius;
        cout << "\nEnter the number of steps: ";
        cin >>  circleSteps;

        if(!(radius <= 1 && radius >=0 && circleSteps > 0)) {
                cout << "Entered out of bound values. Enter radius in between <0,1>";
                return;
        }
        else
                circle = true;
        return;
}

// Function that generates coordinates for the rendering of circle

vector<vertexData> circleVec(GLfloat rad, GLuint step ) {

        vector<vertexData> circleVector;
        GLfloat circleFragment = 360.0f/step;
        vertexData data;

        for(GLfloat angle = 0.0f; angle <= 360.0f; angle+=circleFragment) {
                GLfloat x = rad * cos((angle * PI) / 180.0f);
                GLfloat y = rad * sin((angle * PI) / 180.0f);
                data.color[0] = 0.0f; data.color[1] = 0.0f;
                data.color[2] = 1.0f; data.color[3] = 1.0f;
                data.vertex[0] = x; data.vertex[1] = y;
                data.vertex[2] = 0.0f; data.vertex[3] = 0.0f;
                circleVector.push_back(data);
        }

        return circleVector;
}

// Function for rendering circle

void renderCircle(GLfloat radius, GLuint steps){
        int count = 0;
        vector<vertexData> circleData = circleVec(radius, steps);
        circleVertices = circleData.size();
        vertexData circleVert[circleVertices];
        for(int i = 0; i < circleVertices; i++) {
                for(int j = 0; j < 4; j++) {
                        circleVert[i].color[j] = circleData[i].color[j];
                        circleVert[i].vertex[j] = circleData[i].vertex[j];

                        cout << circleVert[i].color[j] << "   " << circleVert[i].vertex[j] <<endl;
                }
                count++;
        }
        cout << count << endl;
        ShaderInfo shaders3[] = {{GL_VERTEX_SHADER, "circle.vert"},
                                 {GL_FRAGMENT_SHADER, "circle.frag"},
                                 {GL_NONE, NULL}};

        program3  = LoadShaders(shaders3);
        if(program3 == NULL) {
                cerr << "Failed shader3 load" << endl;
        }
        glUseProgram(program3);
        if(defaultColor) {
                GLfloat circleColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
                GLint locatn = glGetUniformLocation(program3, "inColor");
                glUniform4fv(locatn, 1, circleColor);
        }

        glGenVertexArrays(1, circleVAOs);
        glGenBuffers(1, circleBuffer);
        glBindVertexArray(circleVAOs[0]);
        glBindBuffer(GL_ARRAY_BUFFER, circleBuffer[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circleVert), circleVert, GL_STATIC_DRAW);

        //glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(0));
        glVertexAttribPointer(vPosition3, 4, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(circleVert[0].color)));
        //glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(vPosition3);

}

// Functions that generates coodinates for the rendering of heart

vector<vertexData> heartVec(GLuint steps){

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

// Function for rendering the heart

void renderHeart(GLuint step){
        int count = 0;
        vector<vertexData> heartData = heartVec(step);
        heartVertices = heartData.size();
        vertexData heartVert[heartVertices];
        for(int i = 0; i < heartVertices; i++) {
                for(int j = 0; j < 4; j++) {
                        heartVert[i].color[j] = heartData[i].color[j];
                        heartVert[i].vertex[j] = heartData[i].vertex[j];

                        cout << heartVert[i].color[j] << "   " << heartVert[i].vertex[j] <<endl;
                }
                count++;
        }
        cout << count << endl;
        ShaderInfo shaders4[] = {{GL_VERTEX_SHADER, "heart.vert"},
                                 {GL_FRAGMENT_SHADER, "heart.frag"},
                                 {GL_NONE, NULL}};

        program4  = LoadShaders(shaders4);
        if(program4 == NULL) {
                cerr << "Failed shader4 load" << endl;
        }
        glUseProgram(program4);
        if(defaultColor) {
                GLfloat heartColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
                GLint locatn = glGetUniformLocation(program4, "inColor");
                glUniform4fv(locatn, 1, heartColor);
        }

        glGenVertexArrays(1, heartVAOs);
        glGenBuffers(1, heartBuffer);
        glBindVertexArray(heartVAOs[0]);
        glBindBuffer(GL_ARRAY_BUFFER, heartBuffer[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(heartVert), heartVert, GL_STATIC_DRAW);

        //glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(0));
        glVertexAttribPointer(vPosition4, 4, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(heartVert[0].color)));
        //glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(vPosition4);

}

// Function for responding to user inputs

void cbfun(GLFWwindow* window, int key, int scancode, int action, int mods){

        if(action == GLFW_PRESS) {
                if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) { exit(EXIT_SUCCESS); }
                else{
                        switch (key) {

                        case GLFW_KEY_C:
                                readInputs();
                                break;

                        case GLFW_KEY_S:
                                wireFrame = false;
                                break;

                        case GLFW_KEY_W:
                                wireFrame = true;
                                break;

                        case GLFW_KEY_G:
                                circleInputs();
                                renderCircle(radius, circleSteps);
                                break;

                        case GLFW_KEY_H:
                                cout << "Enter the number of steps for heart curve: ";
                                cin >> heartSteps;
                                if(heartSteps <= 0) {
                                        cout << "Enter a vaild input for the steps ( >0 )" << endl;
                                        return;
                                }
                                renderHeart(heartSteps);
                                heart = true;
                                break;

                        case GLFW_KEY_X:
                                if(twoTriangles) twoTriangles = false;
                                else twoTriangles = true;
                                break;

                        case GLFW_KEY_Y:
                                if(coloredTriangle) coloredTriangle = false;
                                else coloredTriangle = true;
                                break;

                        case GLFW_KEY_Z:
                                if(circle) circle = false;
                                else circle = true;
                                break;

                        case GLFW_KEY_A:
                                if(heart) heart = false;
                                else heart = true;
                                break;

                        default:
                                return;
                        }
                }
        }
}


void colorTriangle(){

        vertexData Triangle[3] = {
                {{1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}},       //colored triangle
                {{0.0f, 1.0f, 0.0f, 1.0f}, {0.7f, -0.35f}},
                {{0.0f, 0.0f, 1.0f, 1.0f}, {-0.75f, -0.35f}}
        };


        ShaderInfo shaders2[] = {{GL_VERTEX_SHADER, "colorTriangle.vert"},
                                 {GL_FRAGMENT_SHADER, "colorTriangle.frag"},
                                 {GL_NONE, NULL}};

        program2  = LoadShaders(shaders2);
        if(program2 == NULL) {
                cerr << "Failed shader2 load" << endl;
        }
        glUseProgram(program2);

        glGenVertexArrays(1, colorVAOs);
        glBindVertexArray(colorVAOs[0]);
        glGenBuffers(1, colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);
        glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(0));
        glVertexAttribPointer(vPosition2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), BUFFER_OFFSET(sizeof(Triangle[0].color)));
        glEnableVertexAttribArray(vPosition2);
        glEnableVertexAttribArray(vColor);

}


//----------------------------------------------------------------------------
//
// init
//

void init(void) {

        //VAO and VBO for the initial two triangles.

        GLfloat triangle1[NumVertices][2] = {
                {-0.90f, -0.90f}, {0.85f, -0.90f}, {-0.90f, 0.85f}, // Triangle 1
                {0.90f, -0.85f},  {0.90f, 0.90f},  {-0.85f, 0.90f}// Triangle 2
        };

        glGenVertexArrays(NumVAOs, VAOs);
        glBindVertexArray(VAOs[Triangles]);

        // glCreateBuffers(NumBuffers, Buffers);		//OpenGL 4.5 method
        glGenBuffers(NumBuffers, Buffers);
        // glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);
        // OpenGL 4.5 method
        glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]); // ArrayBuffer is 0
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
        glVertexAttribPointer(vPosition1, 2, GL_FLOAT, GL_FALSE, 0,
                              BUFFER_OFFSET(0)); // vPosition is 0
        glEnableVertexAttribArray(vPosition1);

        ShaderInfo shaders1[] = {{GL_VERTEX_SHADER, "triangles.vert"},
                                 {GL_FRAGMENT_SHADER, "triangles.frag"},
                                 {GL_NONE, NULL}};

        program1 = LoadShaders(shaders1);
        if (program1 == NULL) {
                cerr << "Failed shader1 load" << endl;
        }
        glUseProgram(program1);
        if(defaultColor) {
                GLfloat trianglesColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f};
                GLint locate = glGetUniformLocation(program1, "inColor");
                glUniform4fv(locate, 1, trianglesColor);
        }
        // VAO anf VBO for colored triangles
        //colorTriangle();

}

//----------------------------------------------------------------------------
//
// display
//

void display(void) {
        static const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
        GLfloat color[4] = {red, green, blue, 1.0f};
        if (wireFrame) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glLineWidth(5.0f);
        }
        else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if(twoTriangles) {
                glUseProgram(program1);
                if(!defaultColor) {
                        GLint locate = glGetUniformLocation(program1, "inColor");
                        glUniform4fv(locate, 1, color);
                }
                glBindVertexArray(VAOs[Triangles]);
                glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
                glEnableVertexAttribArray(vPosition1);
                glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        if(coloredTriangle) {
                glUseProgram(program2);
                glBindVertexArray(colorVAOs[0]);
                glBindBuffer(GL_ARRAY_BUFFER, colorBuffer[0]);
                glEnableVertexAttribArray(vPosition2);
                glEnableVertexAttribArray(vColor);
                glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        if(circle) {
                glUseProgram(program3);
                if(!defaultColor) {
                        GLint locate = glGetUniformLocation(program3, "inColor");
                        glUniform4fv(locate, 1, color);
                }
                glBindVertexArray(circleVAOs[0]);
                glBindBuffer(GL_ARRAY_BUFFER, circleBuffer[0]);
                glEnableVertexAttribArray(vPosition3);
                glDrawArrays(GL_TRIANGLE_FAN, 0, circleSteps);
        }

        if(heart) {
                glUseProgram(program4);
                if(!defaultColor) {
                        GLint locate = glGetUniformLocation(program3, "inColor");
                        glUniform4fv(locate, 1, color);
                }
                glBindVertexArray(heartVAOs[0]);
                glBindBuffer(GL_ARRAY_BUFFER, heartBuffer[0]);
                glEnableVertexAttribArray(vPosition3);
                glDrawArrays(GL_LINE_STRIP, 0, heartSteps);
        }
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

        glfwSetKeyCallback(window, cbfun);

        while (!glfwWindowShouldClose(window)) {
                display();
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glfwDestroyWindow(window);

        glfwTerminate();
}
