// viewer.cpp : CSC 433/533 assignment 3 - Spring 2017.
//	based on the 9th edition of the OpenGL Programming Guide (red book)
//

#include "vgl.h"
#include "LoadShaders.h"
#include "loadObj.h"
#include <SOIL/SOIL.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>
#include <cstring>

#include <glm/gtx/string_cast.hpp>
using namespace glm;

using namespace std;

sceneInfo sceneData;
int numObjs;
objInfo* objinfo;
vector<objInfo> objData;
viewProperties initView;
vector<lightProperties> initLight;
mat4 viewMatrix;
GLuint program1, program2, program;
GLfloat nearVal = 1.7f, farVal = 10000.0f;
bool wireFrame = false;
bool north = false, south = false, west = false, east = false;
bool top = false, down = false;
bool rotateViewClock = false, rotateViewCounterClock = false;
bool rotateGazeClock = false, rotateGazeCounterClock = false;
int initWidth = 800, initHeight = 800, currWidth, currHeight;
int texIDs = 0;

void lightInformation();
// Function for responding to user inputs
void cbfun(GLFWwindow* window, int key, int scancode, int action, int mods){

        vec3 gazeVector = sceneData.view.center - sceneData.view.eye; // Gaze Vector
        vec3 gaze = gazeVector/length(gazeVector); // GazeVector/Magnitude(W-Axis)

        vec3 uAxis = cross(sceneData.view.viewup, -gaze); // u-axis
        vec3 u = uAxis/length(uAxis);

        vec3 vAxis = cross(-gazeVector, uAxis); // v-axis

        if(action == GLFW_PRESS || action != GLFW_RELEASE) {
                if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
                        exit(EXIT_SUCCESS);
                }

                else{
                        switch (key) {

                        case GLFW_KEY_P:
                                sceneData.view.eye = initView.eye;
                                sceneData.view.center = initView.center;
                                sceneData.view.viewup = initView.viewup;
                                viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                break;

                        case GLFW_KEY_O:
                                if(wireFrame)
                                        wireFrame = false;
                                else
                                        wireFrame = true;
                                break;

                        case GLFW_KEY_W:
                        case GLFW_KEY_UP:
                                if(north == true) {
                                        sceneData.view.eye += 1.5f * gaze;
                                        sceneData.view.center += 1.5f *gaze;
                                        viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                        north = false;
                                }
                                else
                                        north = true;
                                break;

                        case GLFW_KEY_S:
                        case GLFW_KEY_DOWN:
                                if(south == true) {
                                        sceneData.view.eye -= 2.0f * gaze;
                                        sceneData.view.center -= 2.0f * gaze;
                                        viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                        south = false;
                                }
                                else
                                        south = true;
                                break;

                        case GLFW_KEY_A:
                        case GLFW_KEY_LEFT:
                                if(east == true) {
                                        sceneData.view.eye -= 2.0f * u;
                                        sceneData.view.center -= 2.0f * u;
                                        viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                        east = false;
                                }
                                else
                                        east = true;
                                break;

                        case GLFW_KEY_D:
                        case GLFW_KEY_RIGHT:
                                if(west == true) {
                                        sceneData.view.eye += 2.0f * u;
                                        sceneData.view.center += 2.0f * u;
                                        viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                        west = false;
                                }
                                else
                                        west = true;
                                break;

                        case GLFW_KEY_R:
                                if(top == true) {
                                        sceneData.view.eye -= 0.5f * sceneData.view.viewup;
                                        sceneData.view.center -= 0.5f * sceneData.view.viewup;
                                        viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                        top = false;
                                }
                                else
                                        top = true;
                                break;

                        case GLFW_KEY_T:
                                if(down == true) {
                                        sceneData.view.eye += 0.5f * sceneData.view.viewup;
                                        sceneData.view.center += 0.5f * sceneData.view.viewup;
                                        viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                        down = false;
                                }
                                else
                                        down = true;
                                break;

                        case GLFW_KEY_Z:
                                if(rotateViewClock == true) {
                                        mat4 matrix = rotate(mat4(1.0f), -(1.0f*3.14f)/180.0f, sceneData.view.viewup);
                                        sceneData.view.eye = vec3(matrix * vec4(sceneData.view.eye, 1.0f));
                                        sceneData.view.center = vec3(matrix * vec4(sceneData.view.center, 1.0f));
                                        viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                        rotateViewClock = false;
                                }
                                else
                                        rotateViewClock = true;
                                break;

                        case GLFW_KEY_X:
                                if(rotateViewCounterClock == true) {
                                        mat4 matrix = rotate(mat4(1.0f), (1.0f*3.14f)/180.0f, sceneData.view.viewup);
                                        sceneData.view.eye = vec3(matrix * vec4(sceneData.view.eye, 1.0f));
                                        sceneData.view.center = vec3(matrix * vec4(sceneData.view.center, 1.0f));
                                        viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                        rotateViewCounterClock = false;
                                }
                                else
                                        rotateViewCounterClock = true;
                                break;

                        case GLFW_KEY_C:
                                if(rotateGazeClock == true) {
                                        mat4 matrix = rotate(mat4(1.0f), (1.0f*3.14f)/180.0f, gaze);
                                        sceneData.view.viewup = vec3(matrix * vec4(sceneData.view.viewup, 1.0f));
                                        viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                        rotateGazeClock = false;
                                }
                                else
                                        rotateGazeClock = true;
                                break;

                        case GLFW_KEY_V:
                                if(rotateGazeCounterClock == true) {
                                        mat4 matrix = rotate(mat4(1.0f), -(1.0f*3.14f)/180.0f, gaze);
                                        sceneData.view.viewup = vec3(matrix * vec4(sceneData.view.viewup, 1.0f));
                                        viewMatrix = lookAt(sceneData.view.eye, sceneData.view.center, sceneData.view.viewup);
                                        rotateGazeCounterClock = false;
                                }
                                else
                                        rotateGazeCounterClock = true;
                                break;

                        default:
                                return;
                        }
                }
        }
}

//Function for setting the window size
void windowCallback(GLFWwindow* window, int w, int h){
        float x = (w - initWidth)/2;
        float y = (w - initHeight)/2;
        glViewport(x, y, w, h);
}


//----------------------------------------------------------------------------
//
// init
//

void init(string fileName) {

        sceneData = sceneParse(fileName);


        cout << "Size of light vector from scene " << sceneData.light.size() << "\n"
             << "Size of object vector from scene " << sceneData.object.size() << endl;
        /*
           cout << "**********Scene Data************" << endl;
           cout << "Eye: " << to_string(sceneData.view.eye) << endl;
           cout << "Center: " << to_string(sceneData.view.center) << endl;
           cout << "viewUp: " << to_string(sceneData.view.viewup) << endl;
           for(int i = 0; i < sceneData.light.size(); i++) {
                cout << "********light number " << i << "*********"<< endl;
                cout << "isEnabled" << sceneData.light.at(i).isEnabled << endl;
                cout << "isLocal" << sceneData.light.at(i).isLocal << endl;
                cout << "isSpot" << sceneData.light.at(i).isSpot << endl;
                cout << "ambient" << to_string(sceneData.light.at(i).ambient) << endl;
                cout << "color" << to_string(sceneData.light.at(i).color) << endl;
                cout << "position" << to_string(sceneData.light.at(i).position) << endl;
                cout << "halfVector" << to_string(sceneData.light.at(i).halfVector) << endl;
                cout << "coneDirection" << to_string(sceneData.light.at(i).coneDirection) << endl;
                cout << "spotCosCutoff" << sceneData.light.at(i).spotCosCutoff << endl;
                cout << "spotExponent" << sceneData.light.at(i).spotExponent << endl;
                cout << "constantAttenuation" << sceneData.light.at(i).constantAttenuation << endl;
                cout << "linearAttenuation" << sceneData.light.at(i).linearAttenuation << endl;
                cout << "quadraticAttenuation" << sceneData.light.at(i).quadraticAttenuation << endl;
           }
           for (int i = 0; i < sceneData.object.size(); i++) {
                cout << "********object number " << i << "*********"<< endl;
                cout << "Object path "<< sceneData.object.at(i).objPath << endl;
                cout << "shaderPath" << sceneData.object.at(i).shaderType << endl;
                cout << "t vector " << to_string(sceneData.object.at(i).t) << endl;
                cout << "s vector " << to_string(sceneData.object.at(i).s) << endl;
                cout << "rx value " << sceneData.object.at(i).rx << endl;
                cout << "ry value " << sceneData.object.at(i).ry << endl;
                cout << "rz value " << sceneData.object.at(i).rz << endl;
                cout << "Model Mat" << to_string(sceneData.object.at(i).modelMatrix) << endl;
           }

         */

        for(int i = 0; i < sceneData.object.size(); i++) {
                string objFilePath = sceneData.object.at(i).objPath;
                //cout << objFilePath <<" " << objFilePath.length()<<endl;
                char* file = &objFilePath[0];
                //char *file = (char*) calloc(1, sizeof(objFilePath));
                //for(int i = 0; i < objFilePath.length(); i++)
                //        *(file + (i*sizeof(char))) = objFilePath[i];
                cout << "Object file path " << file << endl;
                loadObjFile(file, &objinfo, &numObjs);
                //cout << "Number of objects returned from OBJ file: " << numObjs << endl;
                for(int j = 0; j < numObjs; j++) {
                        objinfo[j].modelMatrix = sceneData.object.at(i).modelMatrix;
                        if(sceneData.object.at(i).shaderType.compare("BLINN_PHONG") == 0)
                                objinfo[j].shader = BLINN_PHONG;

                        else if(sceneData.object.at(i).shaderType.compare("TOON") == 0)
                                objinfo[j].shader = TOON;
                        if(strcmp(objinfo[j].diffuseTex, "") != 0) {
                                objinfo[j].diffuseTexMapID = texIDs;
                                string imagePath = "";
                                string objPath = sceneData.object.at(0).objPath;
                                int pos = 0;
                                for(pos = objFilePath.length() - 1; pos > 0; pos--) {
                                        if(objFilePath[pos] == '/') {
                                                //cout << pos << endl;
                                                break;
                                        }
                                }
                                imagePath = objFilePath.substr(0,pos+1) + objinfo[j].diffuseTex;
                                char* path = new char[imagePath.length() + 1];
                                strcpy(path, imagePath.c_str());
                                int channels, height, width;
                                cout << "Image Path: " << path << endl;
                                unsigned char* image = SOIL_load_image(path, &width, &height,
                                                                       &channels, SOIL_LOAD_AUTO);

                                if(image == NULL) {
                                        cerr << "Failed Image Load" << endl;
                                        exit(EXIT_FAILURE);
                                }

                                // Flipping image values around yaxis
                                for (int j = 0; j * 2 < height; ++j)
                                {
                                        int index1 = j * width * channels;
                                        int index2 = (height - 1 - j) * width * channels;
                                        for (int i = width * channels; i > 0; --i)
                                        {
                                                unsigned char temp = image[index1];
                                                image[index1] = image[index2];
                                                image[index2] = temp;
                                                ++index1;
                                                ++index2;
                                        }
                                }

                                GLuint m_texture;
                                glGenTextures(1, &m_texture);
                                glActiveTexture(GL_TEXTURE0 + texIDs);
                                glBindTexture(GL_TEXTURE_2D, m_texture);

                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                                //cout << m_texture << endl;

                                if (channels == 3)
                                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                                                     GL_UNSIGNED_BYTE, image);
                                else if (channels == 4) {
                                        //image = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_RGBA);
                                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
                                }

                                texIDs++;
                                //cout << texIDs << endl;
                                SOIL_free_image_data(image);
                        }


                        objData.push_back(objinfo[j]);

                }
        }
        cout << "Obj data vector size: "<< objData.size() << endl;
        initView = sceneData.view;
        initLight = sceneData.light;
        //cout << to_string(initView.eye) << to_string(initView.center) << to_string(initView.viewup) << endl;
        viewMatrix = lookAt(initView.eye, initView.center, initView.viewup);
        //cout << to_string(viewMatrix) << endl;
        ShaderInfo shaders1[] = {{GL_VERTEX_SHADER, "toon.vert"},
                                 {GL_FRAGMENT_SHADER, "toon.frag"},
                                 {GL_NONE, NULL}};

        program1  = LoadShaders(shaders1);
        if(program1 == NULL) {
                cerr << "Failed shader load" << endl;
                exit(EXIT_FAILURE);
        }

        ShaderInfo shaders2[]  = {{GL_VERTEX_SHADER, "BlinnPhong.vert"},
                                  {GL_FRAGMENT_SHADER, "BlinnPhong.frag"},
                                  {GL_NONE, NULL}};

        program2  = LoadShaders(shaders2);
        if(program2 == NULL) {
                cerr << "Failed shader load" << endl;
                exit(EXIT_FAILURE);
        }

        return;
}

void objectInformation(){
        for (int i = 0; i < objData.size(); i++) {

                if(objData.at(i).shader == BLINN_PHONG)
                        program = program2;
                else if(objData.at(i).shader == TOON)
                        program = program1;

                glUseProgram(program);
                lightInformation();

                if(strcmp(objData.at(i).diffuseTex, "") != 0) {

                        //glActiveTexture(GL_TEXTURE0 + objData.at(i).diffuseTexMapID);
                        //glBindTexture(GL_TEXTURE_2D, objData.at(i).diffuseTexMapID);

                        GLuint texLocation1 = glGetUniformLocation(program, "texID");
                        glUniform1i(texLocation1, objData.at(i).diffuseTexMapID);

                        GLuint texLocation2 = glGetUniformLocation(program, "isTextureMapping");
                        glUniform1i(texLocation2, true);

                }
                else{
                        GLuint texLocation2 = glGetUniformLocation(program, "isTextureMapping");
                        glUniform1i(texLocation2, false);
                }

                mat4 modelMat = objData.at(i).modelMatrix;
                //cout << "Model Matrix in the display: " << to_string(modelMat) << endl;
                mat4 mvMatrix = viewMatrix * modelMat;
                //cout << "MV Matrix in the display: " << to_string(mvMatrix) << endl;
                mat3 normalMatrix = transpose(inverse((mat3(mvMatrix))));
                //cout << "NormalMatrix in the display: " << to_string(normalMatrix) << endl;
                float x = 1.0f, y = 1.0f;
                float aspectRatio = float(currWidth) / float(currHeight);
                if(aspectRatio > 1)
                        x = x * aspectRatio;
                else
                        y = x / aspectRatio;

                //mat4 projectionMatrix = perspective(radians(70.0f), aspectRatio, nearVal, farVal);
                mat4 projectionMatrix = frustum(-x, x,
                                                -y, y,
                                                nearVal, farVal);
                //cout << "Projection Matrix in the display: " << to_string(projectionMatrix) << endl;
                mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMat;
                //cout << "MVP Matrix in the display: " << to_string(mvpMatrix) << endl;
                GLint locate;

                locate = glGetUniformLocation(program, "MVPMatrix");
                glUniformMatrix4fv(locate, 1, GL_FALSE, &mvpMatrix[0][0]);

                locate = glGetUniformLocation(program, "MVMatrix");
                glUniformMatrix4fv(locate, 1, GL_FALSE, &mvMatrix[0][0]);

                locate = glGetUniformLocation(program, "NormalMatrix");
                glUniformMatrix3fv(locate, 1, GL_FALSE, &normalMatrix[0][0]);

                locate = glGetUniformLocation(program, "ambient");
                glUniform3fv(locate, 1, &objData.at(i).Ka[0]);
                //cout <<"Ka: " << to_string(objData.at(i).Ka) << endl;

                locate = glGetUniformLocation(program, "diffuse");
                glUniform3fv(locate, 1, &objData.at(i).Kd[0]);
                //cout <<"Kd: " << to_string(objData.at(i).Kd) << endl;

                locate = glGetUniformLocation(program, "specular");
                glUniform3fv(locate, 1, &objData.at(i).Ks[0]);
                //cout <<"Ks: " << to_string(objData.at(i).Ks) << endl;

                locate = glGetUniformLocation(program, "shininess");
                glUniform1f(locate, objData.at(i).n);

                glBindVertexArray(objData.at(i).VAO);
                glDrawArrays(GL_TRIANGLES, 0, objData.at(i).VAOsize);

                //GLuint texLocation2 = glGetUniformLocation(program, "isTextureMapping");
                //glUniform1i(texLocation2, false);

                //////////////////////////////////////////////////////////////////
                // For texture Mapping
                //////////////////////////////////////////////////////////////////


        }
}

void lightInformation(){
        for(int i = 0; i < sceneData.light.size(); i++) {

                sceneData.light.at(i).isEnabled = 1;
                char buf[64];
                GLint location;
                vec3 ambient = sceneData.light.at(i).ambient;
                vec3 color = sceneData.light.at(i).color;
                vec3 position = sceneData.light.at(i).position;
                if(sceneData.light.at(i).isLocal == 1) {
                        vec4 pos = viewMatrix * (vec4(position, 1.0f)); //changing world coordinates to eye coordinates
                        pos = pos/pos.w;
                        position = vec3(pos);
                        //position = vec3(viewMatrix * vec4(position, 1.0f));
                }
                else{
                        position = mat3(viewMatrix) * position;
                        //position = vec3(viewMatrix * vec4(position, 0.0f));
                }
                //cout << to_string(position) << endl;
                vec3 halfVector = sceneData.light.at(i).halfVector;
                vec3 coneDirection = sceneData.light.at(i).coneDirection;
                coneDirection = mat3(viewMatrix) * coneDirection;
                /*if(sceneData.light.at(i).isLocal == 1) {
                        vec4 pos = viewMatrix * (vec4(coneDirection, 1.0f)); //changing world coordinates to eye coordinates
                        pos = pos/pos.w;
                        coneDirection = vec3(pos);
                   }
                   else{
                        coneDirection = mat3(viewMatrix) * coneDirection;
                   }*/

                sprintf(buf, "Lights[%1d].isEnabled", i );
                location = glGetUniformLocation(program, buf);
                glUniform1i(location, sceneData.light.at(i).isEnabled);

                sprintf(buf, "Lights[%1d].isLocal", i );
                location = glGetUniformLocation(program, buf);
                glUniform1i(location, sceneData.light.at(i).isLocal);

                sprintf(buf, "Lights[%1d].isSpot", i );
                location = glGetUniformLocation(program, buf);
                glUniform1i(location, sceneData.light.at(i).isSpot);

                sprintf(buf, "Lights[%1d].ambient", i );
                location = glGetUniformLocation(program, buf);
                glUniform3fv(location, 1,  &ambient[0]);

                sprintf(buf, "Lights[%1d].color", i );
                location = glGetUniformLocation(program, buf);
                glUniform3fv(location, 1,  &color[0]);

                sprintf(buf, "Lights[%1d].position", i );
                location = glGetUniformLocation(program, buf);
                glUniform3fv(location, 1,  &position[0]);

                sprintf(buf, "Lights[%1d].halfVector", i );
                location = glGetUniformLocation(program, buf);
                glUniform3fv(location, 1,  &halfVector[0]);

                sprintf(buf, "Lights[%1d].coneDirection", i );
                location = glGetUniformLocation(program, buf);
                glUniform3fv(location, 1,  &coneDirection[0]);

                sprintf(buf, "Lights[%1d].spotCosCutoff", i );
                location = glGetUniformLocation(program, buf);
                glUniform1f(location, sceneData.light.at(i).spotCosCutoff );

                sprintf(buf, "Lights[%1d].spotExponent", i );
                location = glGetUniformLocation(program, buf);
                glUniform1f(location, sceneData.light.at(i).spotExponent );

                sprintf(buf, "Lights[%1d].constantAttenuation", i );
                location = glGetUniformLocation(program, buf);
                glUniform1f(location, sceneData.light.at(i).constantAttenuation);

                sprintf(buf, "Lights[%1d].linearAttenuation", i );
                location = glGetUniformLocation(program, buf);
                glUniform1f(location, sceneData.light.at(i).linearAttenuation);

                sprintf(buf, "Lights[%1d].quadraticAttenuation", i );
                location = glGetUniformLocation(program, buf);
                glUniform1f(location, sceneData.light.at(i).quadraticAttenuation);

        }

}


//----------------------------------------------------------------------------
//
// display
//

void display(void) {
        //sleep(3);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glClearDepthf(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        static const float initColor[] = { 0.f, .25f, .3f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, initColor);
        //glUseProgram(program1);
        //glUseProgram(program2);
        glEnable(GL_TEXTURE_2D);

        if (wireFrame) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glLineWidth(5.0f);
        }
        else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        objectInformation();
        glViewport(0, 0, currWidth, currHeight);
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

        if(argc != 2) {
                cerr << "Entered Wrong arguments" << endl;
                exit(EXIT_FAILURE);
        }

        glfwInit();

        //#ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                       3); // 4.1 latest version of OpenGL for OSX 10.9 Mavericks
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //#endif

        GLFWwindow *window = glfwCreateWindow(800, 800, "Viewer", NULL, NULL);

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

        //float lineWidth[2];
        //glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);
        //cout << *lineWidth << endl;
        string fileName = argv[1];

        init(fileName);

        glfwSetKeyCallback(window, cbfun);

        while (!glfwWindowShouldClose(window)) {
                glfwGetWindowSize(window, &currWidth, &currHeight);
                //glfwSetWindowSizeCallback(window, windowCallback);
                display();
                glfwSwapBuffers(window);
                glfwPollEvents();
        }

        glfwDestroyWindow(window);

        glfwTerminate();

}
