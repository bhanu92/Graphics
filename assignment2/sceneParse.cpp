// Code to parse the entire specified scene file
// Reads the entire file word by word and stores into a string vector.

#include "loadObj.h"

#include <istream>
#include <sstream>
#include <algorithm>

#define PI 3.14

sceneInfo sceneParse(string sceneFile){

        sceneInfo scene;
        cout << "Scene File Name: " << sceneFile << endl;

        vector<string> tokens;
        string token;

        ifstream file;
        file.open(sceneFile);
        if(!file.is_open()) {
                cout << "Cannot open Scene file" << endl;
                exit(EXIT_FAILURE);
        }

        while(file >> token)
                tokens.push_back(token);

        // Adding NULL values at the end of the vector
        // As the scenefiles are of varying size
        for(int i = 0; i < 32; i++)
                tokens.push_back("");

        for (int i = 0; i < tokens.size(); i++) {

                if(tokens.at(i).compare("view") == 0) {

                        scene.view.eye.x =  stof(tokens.at(i+2));
                        scene.view.eye.y =  stof(tokens.at(i+3));
                        scene.view.eye.z =  stof(tokens.at(i+4));

                        scene.view.center.x =  stof(tokens.at(i+6));
                        scene.view.center.y =  stof(tokens.at(i+7));
                        scene.view.center.z =  stof(tokens.at(i+8));

                        scene.view.viewup.x =  stof(tokens.at(i+10));
                        scene.view.viewup.y =  stof(tokens.at(i+11));
                        scene.view.viewup.z =  stof(tokens.at(i+12));

                        continue;

                }

                else if(tokens.at(i).compare("light") == 0) {

                        lightProperties sceneLight;

                        if(tokens.at(i+2).compare("directional") == 0)
                                sceneLight.isLocal = 0;

                        else if(tokens.at(i+2).compare("local") == 0)
                                sceneLight.isLocal = 1;

                        else if(tokens.at(i+2).compare("spot") == 0) {
                                sceneLight.isSpot = 1;
                                sceneLight.isLocal = 1;
                        }

                        if(tokens.at(i+3).compare("ambient") == 0) {
                                sceneLight.ambient.x = stof(tokens.at(i+4));
                                sceneLight.ambient.y = stof(tokens.at(i+5));
                                sceneLight.ambient.z = stof(tokens.at(i+6));
                        }

                        if(tokens.at(i+7).compare("color") == 0) {
                                sceneLight.color.x = stof(tokens.at(i+8));
                                sceneLight.color.y = stof(tokens.at(i+9));
                                sceneLight.color.z = stof(tokens.at(i+10));
                        }

                        if(tokens.at(i+11).compare("position") == 0) {
                                sceneLight.position.x = stof(tokens.at(i+12));
                                sceneLight.position.y = stof(tokens.at(i+13));
                                sceneLight.position.z = stof(tokens.at(i+14));
                        }

                        if(tokens.at(i+15).compare("constAtt") == 0)
                                sceneLight.constantAttenuation = stof(tokens.at(i+16));

                        if(tokens.at(i+17).compare("linearAtt") == 0)
                                sceneLight.linearAttenuation = stof(tokens.at(i+18));

                        if(tokens.at(i+19).compare("quadAtt") == 0)
                                sceneLight.quadraticAttenuation = stof(tokens.at(i+20));

                        if(tokens.at(i+21).compare("coneDirection") == 0) {
                                sceneLight.coneDirection.x = stof(tokens.at(i+22));
                                sceneLight.coneDirection.y = stof(tokens.at(i+23));
                                sceneLight.coneDirection.z = stof(tokens.at(i+24));
                        }

                        if(tokens.at(i+25).compare("spotCosCutoff") == 0)
                                sceneLight.spotCosCutoff = stof(tokens.at(i+26));

                        if(tokens.at(i+27).compare("spotExponent") == 0)
                                sceneLight.spotExponent = stof(tokens.at(i+28));

                        //End of reading one light information
                        //Stuffing the above info into the light vector
                        scene.light.push_back(sceneLight);

                        continue;

                }

                else if(tokens.at(i).compare("object") == 0) {

                        objectProperties sceneObject;
                        mat4 modMat(1.0f);
                        bool translation = false, scaling = false;
                        bool rotX = false, rotY = false, rotZ = false;

                        sceneObject.objPath = tokens.at(i+1);
                        sceneObject.shaderType = tokens.at(i+3);

                        for(int j = i+4; j < tokens.size(); j++) {

                                if(tokens.at(j).compare("object") == 0)
                                        break;

                                if(tokens.at(j).compare("t") == 0) {
                                        sceneObject.t.x = stof(tokens.at(j+1));
                                        sceneObject.t.y = stof(tokens.at(j+2));
                                        sceneObject.t.z = stof(tokens.at(j+3));
                                        modMat = translate(modMat, sceneObject.t) * modMat;
                                        //translation = true;
                                }

                                if(tokens.at(j).compare("s") == 0) {
                                        sceneObject.s.x = stof(tokens.at(j+1));
                                        sceneObject.s.y = stof(tokens.at(j+2));
                                        sceneObject.s.z = stof(tokens.at(j+3));
                                        modMat = scale(modMat, sceneObject.s) * modMat;
                                        //scaling = true;
                                }

                                if(tokens.at(j).compare("rx") == 0) {
                                        sceneObject.rx = stof(tokens.at(j+1));
                                        vec3 xaxis(1, 0, 0);
                                        modMat = rotate(modMat, float((sceneObject.rx * PI)/180.0f), xaxis) * modMat;
                                        //rotX = true;
                                }

                                if(tokens.at(j).compare("ry") == 0) {
                                        sceneObject.ry = stof(tokens.at(j+1));
                                        vec3 yaxis(0, 1, 0);
                                        modMat = rotate(modMat, float((sceneObject.ry * PI)/180.0f), yaxis) * modMat;
                                        //rotY = true;
                                }

                                if(tokens.at(j).compare("rz") == 0) {
                                        sceneObject.rz = stof(tokens.at(j+1));
                                        vec3 zaxis(0, 0, 1);
                                        modMat = rotate(modMat, float((sceneObject.rz * PI)/180.0f), zaxis) * modMat;
                                        //rotZ = true;
                                }
                        }

                        sceneObject.modelMatrix = modMat;

                        /*
                           // This order is verrryyyy important
                           if(rotX) {
                                vec3 xaxis(1, 0, 0);
                                modMat = modMat * rotate(modMat, float((sceneObject.rx * PI)/180.0f), xaxis);
                           }

                           if(rotY) {
                                vec3 yaxis(0, 1, 0);
                                modMat = modMat * rotate(modMat, float((sceneObject.ry * PI)/180.0f), yaxis);
                           }

                           if(rotZ) {
                                vec3 zaxis(0, 0, 1);
                                modMat = modMat * rotate(modMat, float((sceneObject.rz * PI)/180.0f), zaxis);
                           }

                           if(translation) {
                                modMat = modMat * translate(modMat, sceneObject.t);
                           }

                           if(scaling) {
                                modMat = modMat * scale(modMat, sceneObject.s);
                           }

                           sceneObject.modelMatrix = modMat;


                           //rotation
                           if(tokens.at(i+4).compare("rx") == 0) {
                                sceneObject.rx = stof(tokens.at(i+5));
                                vec3 xaxis(1, 0, 0);
                                modMat = modMat * rotate(modMat, float((sceneObject.rx * PI)/180.0f), xaxis);
                                sceneObject.modelMatrix = modMat;
                           }

                           //translation
                           else if(tokens.at(i+4).compare("t") == 0) {
                                sceneObject.t.x = stof(tokens.at(i+5));
                                sceneObject.t.y = stof(tokens.at(i+6));
                                sceneObject.t.z = stof(tokens.at(i+7));
                                modMat = modMat * translate(modMat, sceneObject.t);
                                sceneObject.modelMatrix = modMat;
                           }

                           //scaling
                           else if(tokens.at(i+4).compare("s") == 0) {
                                sceneObject.s.x = stof(tokens.at(i+5));
                                sceneObject.s.y = stof(tokens.at(i+6));
                                sceneObject.s.z = stof(tokens.at(i+7));
                                modMat = modMat * scale(modMat, sceneObject.s);
                                sceneObject.modelMatrix = modMat;
                           }
                         */

                        //End of reading one object description
                        //Stuffing the above info into the object vector
                        scene.object.push_back(sceneObject);

                }

        }

        return scene;
}
