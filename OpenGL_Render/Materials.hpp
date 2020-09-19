//
//  Materials.hpp
//  OpenGL_Render
//
//  Created by Alex Nascimento on 18/09/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

#ifndef Materials_hpp
#define Materials_hpp

#include <stdio.h>
#include <vector>
#include "glm.hpp"

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};
// Material values from: http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html
Material gold {
    glm::vec3(0.2125,    0.1275,     0.054),
    glm::vec3(0.75164,   0.60648,    0.22648),
    glm::vec3(0.628281,  0.555802,   0.366065),
    51.2
};
Material bronze {
    glm::vec3(0.2125,    0.1275,     0.054),
    glm::vec3(0.714,     0.4284,     0.18144),
    glm::vec3(0.393548,  0.271906,   0.166721),
    25.6
};
Material cyanPlastic {
    glm::vec3(0.0,        0.1,        0.06),
    glm::vec3(0.0,        0.50980392, 0.50980392),
    glm::vec3(0.50196078, 0.50196078, 0.50196078),
    32.0
};
Material redRubber {
    glm::vec3(0.05,       0.0,        0.0),
    glm::vec3(0.5,        0.4,        0.4),
    glm::vec3(0.7,        0.04,       0.04),
    10.0
};
Material greenRubber {
    glm::vec3(0.0,        0.05,       0.0),
    glm::vec3(0.4,        0.5,        0.4),
    glm::vec3(0.04,       0.7,        0.04),
    10.0
};
vector<Material> materials = { gold, bronze, cyanPlastic, redRubber, greenRubber };
#endif /* Materials_hpp */
