//
//  Scenes.hpp
//  OpenGL_Render
//
//  Created by Alex Nascimento on 18/09/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

#ifndef Scenes_hpp
#define Scenes_hpp

#include <stdio.h>
#include <vector>
#include "glm.hpp"

std::vector<glm::vec3> genCylinder(const float radius, const float angleDelta, const float depthDelta, const int posCount) {
    std::vector<glm::vec3> result;
    for (int i = 0; i < posCount; i++) {
        float theta = i * angleDelta;
        float x = sin(theta) * radius;
        float y = cos(theta) * radius;
        float z = i * -depthDelta;
        result.push_back(glm::vec3(x,y,z));
    }
    return result;
}

#if defined SCENE1
std::vector<glm::vec3> scene = {
    glm::vec3(0,-1,0)
};
#elif defined SCENE2
std::vector<glm::vec3> scene = {
    glm::vec3(0,-1,0),
    glm::vec3(8,0,-7),
    glm::vec3(-3,1,10),
    glm::vec3(2,-6,1),
    glm::vec3(-10,-7,-20)
};
#elif defined SCENE3
std::vector<glm::vec3> scene = {
    glm::vec3(0,-1,0),     // gold
    glm::vec3(8,0,-7),     // bronze
    glm::vec3(-3,1,10),    // cyan
    glm::vec3(2,-6,1),     // red
    glm::vec3(-10,-7,-20), // green
    glm::vec3(8,-9,-4),   // gold
    glm::vec3(5,4,-3),     // bronze
    glm::vec3(17,-8,-24),     // cyan
    glm::vec3(4,3,-43),     // red
    glm::vec3(5,-5,-14),   // green
    glm::vec3(18,9,-19),     // gold
    glm::vec3(-3,5,-27),  // bronze
    glm::vec3(14,6,-33),     // cyan
    glm::vec3(-9,-3,-6),     // red
    glm::vec3(12,11,-13),  // green
    glm::vec3(-9,-11,-9),   // gold
    glm::vec3(-7,-6,1),    // bronze
    glm::vec3(-9,-3,-30),  // cyan
    glm::vec3(13,3,-9),     // red
    glm::vec3(0,8,-17)     // green
};
#elif defined SCENE4
std::vector<glm::vec3> scene  = genCylinder(16, 1, 0.4, 1000);
#elif defined SCENE5
std::vector<glm::vec3> scene  = genCylinder(16, 1, 0.4, 10000);
#endif // SCENE#

#endif /* Scenes_hpp */
