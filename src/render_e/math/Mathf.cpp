/*
 *  RenderE
 *
 *  Created by Morten Nobel-Jørgensen ( http://www.nobel-joergnesen.com/ ) 
 *  License: LGPL 3.0 ( http://www.gnu.org/licenses/lgpl-3.0.txt )
 */

#include "Mathf.h"

#include <ctime>
#include <iostream>
#include <cstdlib>
#include <glm/gtc/quaternion.hpp>

namespace render_e {

const float Mathf::PI = 3.1415927f;
const float Mathf::PI_INVERSE = 1.0f/3.1415927f;
const float Mathf::DEGREE_TO_RADIAN = PI*2/360.0f;
const float Mathf::RADIAN_TO_DEGREE = 3600.0f/PI*2;

const float Mathf::EPSILON = 1.0e-5f;
bool Mathf::IsRandomInitialized = false;

void Mathf::SetFromEuler(float pitch, float yaw, float roll, glm::quat &rotation){
    // todo find glm replacement
    float cX, cY, cZ, sX, sY, sZ, cYcZ, sYsZ, cYsZ, sYcZ;

    pitch *= 0.5f;
    yaw *= 0.5f;
    roll *= 0.5f;

    cX = cos(pitch);
    cY = cos(yaw);
    cZ = cos(roll);

    sX = sin(pitch);
    sY = sin(yaw);
    sZ = sin(roll);

    cYcZ = cY * cZ;
    sYsZ = sY * sZ;
    cYsZ = cY * sZ;
    sYcZ = sY * cZ;

    rotation.w = cX * cYcZ + sX * sYsZ;
    rotation.x = sX * cYcZ - cX * sYsZ;
    rotation.y = cX * sYcZ + sX * cYsZ;
    rotation.z = cX * cYsZ - sX * sYcZ;
    // todo implement
}

// Returns true if point on triangle plane lies inside triangle (3D version)
// Assumes triangle is not degenerate
bool Mathf::IsPointInTriangle( const glm::vec3& point, const glm::vec3& P0,
                              const glm::vec3& P1, const glm::vec3& P2 ) {

    glm::vec3 v0 = P1 - P0;
    glm::vec3 v1 = P2 - P1;
    glm::vec3 n = glm::cross(v0,v1);

    glm::vec3 wTest = glm::cross(v0,point - P0);
    if ( glm::dot(wTest,n) < 0.0f ) {
        return false;
    }

    wTest = glm::cross(v1,point - P1);
    if ( glm::dot(wTest,n) < 0.0f ) {
        return false;
    }

    glm::vec3 v2 = P0 - P2;
    wTest = glm::cross(v2,point - P2);
    if ( glm::dot(wTest,n) < 0.0f ) {
        return false;
    }

    return true;
}

float Mathf::Rand(){
    if (!Mathf::IsRandomInitialized) {
        srand(static_cast<unsigned int>(time(0)));
        Mathf::IsRandomInitialized = true;
    }
    return rand()/(float)RAND_MAX;
}

void Mathf::PrintVec4(glm::vec4 v){
    for (int x = 0; x < 4; x++) {
        std::cout<<v[x]<<" ";
    }
    std::cout<<std::endl;
}

void Mathf::PrintMat4(glm::mat4 m){
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            std::cout<<m[x][y]<<" ";
        }
        std::cout<<std::endl;
    }
}
}
