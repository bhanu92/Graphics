//
//   simple test of gm
//
//
#include <stdio.h>
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// includes for glm
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

// include glm extensions
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>
using namespace glm;

void wait()
{
        string stuff;
        cout << endl << "Enter any key to continue...";
        cin>>stuff;
        cout << endl;
}

void printMatrix(glm::mat4 aMatrix)
{
        for (int j = 0; j < 4; j++)
        {
                for (int i = 0; i < 4; i++)
                        // notice that glm matrices are column major
                        printf("%6.3f ", aMatrix[i][j]);
                printf("\n");
        }
        printf("\n");
}

int main ()
{
        // ----------------------------------------------------------------------
        //              translate matrix
        // ----------------------------------------------------------------------

        glm::mat4 translateMatrix;
        vec3 t(1.f, 2.f, 3.f);
        translateMatrix = glm::translate(mat4(), t);
        cout << "translate matrix " << endl;
        printMatrix(translateMatrix);
        wait();


        // ----------------------------------------------------------------------
        //              translate a point
        // ----------------------------------------------------------------------

        glm::vec4 aPoint(1., 1., 1., 1.);
        glm::vec4 newPt;
        newPt = translateMatrix * aPoint;
        cout << "old point: " << aPoint.x << ", " << aPoint.y << ", " << aPoint.z << ", " << aPoint.w << endl;
        cout << "New point: " << newPt.x << ", " << newPt.y << ", " << newPt.z << ", "<< newPt.w << endl;
        wait();

        // ----------------------------------------------------------------------
        //              combine (concatenate) translation matrices
        // ----------------------------------------------------------------------
        t = vec3(1., 0., 0.);
        translateMatrix = glm::translate(mat4(), t);
        cout << "translate by x matrix" << endl;
        printMatrix(translateMatrix);
        glm::mat4 translateMatrix2;
        vec3 t2(0.f, 2.f, 3.f);
        translateMatrix2 = glm::translate(translateMatrix, t2);
        cout << "composite translation matrix" << endl;
        printMatrix(translateMatrix2);
        wait();

        // ----------------------------------------------------------------------
        //              rotation matrix
        // ----------------------------------------------------------------------
        vec3 axis;
        axis.x = 0.0f; axis.y = 0.0f; axis.z = 1.0f;    // axis of rotation
        mat4 rotMatrix = rotate(mat4(1.0), 45.f*3.14159f / 180.f, axis);
        cout << "rotation matrix" << endl;
        printMatrix(rotMatrix);
        vec3 myVec(1.f, 1.f, 1.f);
        vec4 newVec;
        newVec = rotMatrix * vec4(myVec, 1.0f);
        cout << "original point" << myVec.x << ", " << myVec.y << ", " << myVec.z << endl;
        cout << "rotated point" << newVec.x << ", " << newVec.y << ", " << newVec.z << endl;
        cout << "length of original vector " << length(myVec) << " length of rotated vector " << length(newVec) << endl;
        wait();

        // ----------------------------------------------------------------------
        //              view matrix
        // ----------------------------------------------------------------------

        glm::vec3 eye(5.0f, 0.0f, 0.0f);
        glm::vec3 center;
        center.x = 0.f;
        center.y = 0.f;
        center.z = 0.f;
        glm::vec3 viewUp( 0.f, 1.f, 0.f);

        glm::mat4 viewMatrix = glm::lookAt( eye, center, viewUp );

        cout << "view matrix " << endl;
        printMatrix(viewMatrix);
        wait();

        vec4 pt(3.f, 0.f, 0.f, 1.0f);
        pt = viewMatrix * pt;
        printf("world coord 3,0,0 becomes %f %f %f\n", pt.x, pt.y, pt.z);
        wait();

        // ----------------------------------------------------------------------
        //              perspective matrix
        // ----------------------------------------------------------------------

        float aspect = .8f;
        float nearDist = 1.f;
        float farDist = 50.f;

        glm::mat4 frustum = glm::frustum( -1.f, 1.f, -aspect, aspect, nearDist, farDist );
        // print frustum
        printf( "\nfrustum\n" );
        printMatrix(frustum);
        wait();

        glm::mat4 perspective = glm::perspective( 45.0f, 1.0f, nearDist, farDist );
        // print perspective
        printf( "\nperspective\n" );
        printMatrix(perspective);
        wait();

        return 0;
}
