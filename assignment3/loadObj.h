////////////////////////////////////////////////////////////////////
//	loadObj.h
////////////////////////////////////////////////////////////////////

#ifndef LOADOBJ_H
#define LOADOBJ_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

#include "vgl.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

typedef struct {
        char name[32];  // identifier, i.e. "red"
        vec3 ambient;   // ambient color, r,g,b
        vec3 diffuse;   // diffuse color, r,g,b
        vec3 specular;  // specular color, r,g,b
        float shininess;        // specular power
        char diffuseTexMap[128];        // file name of diffuse color texture map
        char bumpTexMap[128];           // file name of bump map
} colorValues;

class three {
public:
// constructor with three parameters
three(float f1, float f2, float f3) {
        x = f1;
        y = f2;
        z = f3;
}
// constructor with no parameters
three() {
};
// member data
float x;
float y;
float z;
};


// View description
typedef struct {
        vec3 eye;
        vec3 center;
        vec3 viewup;
}viewProperties;

// Object description
typedef struct {
        string objPath;
        string shaderType;
        vec3 t;
        float rx = 0.0f, ry = 0.0f, rz = 0.0f;
        vec3 s;
        mat4 modelMatrix;
} objectProperties;

// light description
typedef struct {
        int isEnabled = 0;  // is this light active
        int isLocal = 0;    // true for point light source, false for directional light
        int isSpot = 0;             // true for spotlight
        vec3 ambient;   // light's contribution to ambient light (r,g,b)
        vec3 color;             // color of light (r,g,b)
        vec3 position;  // location of local light source, direction to light source otherwise
        vec3 halfVector;
        vec3 coneDirection;             // spotlight attributes
        float spotCosCutoff = 0.0f;
        float spotExponent = 0.0f;
        float constantAttenuation = 0.0f;      // local light attenuation coefficients
        float linearAttenuation = 0.0f;
        float quadraticAttenuation = 0.0f;
}lightProperties;


// Structure for the entire scene information
typedef struct {
        viewProperties view;
        vector<lightProperties> light;
        vector<objectProperties> object;
}sceneInfo;


typedef enum { BLINN_PHONG, COOK_TORRANCE, TOON } shaderType;

typedef struct {
        // geometric information
        GLuint VAO;                             // vertex array object for this object
        GLuint VAOsize;                 // number of primitives (triangles) for this object
        float bounds[6];                // min,max X, Y, Z for this object
        mat4 modelMatrix;               // composite modeling matrix
        // lighting information
        shaderType shader;
        // material information, e.g. lighting parameters and texture maps
        char materialName[32];  // identifier of this entry in the material file
        vec3 Ka;                                // ambient color for this object
        vec3 Kd;                                // diffuse color for this object
        vec3 Ks;                                // specular color for this object
        float n;                                // exponent for dot(N,H) or shininess
        char diffuseTex[128];   // file name of diffuse texture map
        int diffuseTexMapID;     // texture unit ID
        char bumpMap[128];              // file name of bump map
        int bumpMapTexID;               // texture unit ID
} objInfo;

// prototype for loadObjFile
int loadObjFile(char *fileName, objInfo **theObjects, int *numObjects);
// in: fileName - name of OBJ file to read and load
// out: theObjects - array of objInfo records describing the objects within the OBJ file
// out: the number of objInfo records in theObjects array
// memory for theObjects array is allocated and managed within loadObjFile function

sceneInfo sceneParse(string file);

#endif  //LOADOBJ_H
