//
//  main.cpp
//  OpenGL
//
//  Created by Alex Nascimento on 13/08/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "matrix_transform.hpp"
#include "type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "ModelLoader.hpp"

std::vector<glm::vec3> objPositions = {
    glm::vec3(0,0,-5),
    glm::vec3(8,0,-7),
    glm::vec3(-8,2,0),
    glm::vec3(0,-6,1),
    glm::vec3(-1,-2,4)
};

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

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
int windowW, windowH;

float deltaTime = 0.0f;
float lastTime = 0.0f;

// ---- CAMERA ----
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 20.0f);

int main()
{
    // ---- INITIALIZATION ----
    
    glfwInit();

    // set OpenGL version to 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewInit();
    
    glfwGetFramebufferSize(window, &windowW, &windowH); // needed for retina display
    glViewport(0, 0, windowW, windowH);
    
    
    // ---- PERMANENT SETUP ----
    
    glEnable(GL_DEPTH_TEST);

    Shader basicShader("OpenGL_Render/basic_vertex.vs", "OpenGL_Render/basic_fragment.fs");
    
    ModelLoader *modelLoader = new ModelLoader("Shared/teapot.obj");
    vector<float> modelVertices = modelLoader->getVertexData();
    
    unsigned int teapotVAO;
    glGenVertexArrays(1, &teapotVAO);
    glBindVertexArray(teapotVAO);

    unsigned int teapotVBO;
    glGenBuffers(1, &teapotVBO);

    glBindBuffer(GL_ARRAY_BUFFER, teapotVBO);
    glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(float), &modelVertices[0], GL_STATIC_DRAW);

    int stride = modelLoader->getVertexLength();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) (4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    basicShader.use();
    basicShader.setFloat3("lightDirection", glm::vec3(-1.0f));
    basicShader.setFloat3("lightColor", glm::vec3(1.0f));
    
    // ---- RENDER LOOP ----
    
    while(!glfwWindowShouldClose(window))
    {
        // Clear Background
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, -cameraPosition);
        basicShader.setMat4("view", view);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)windowW/(float)windowH, 0.1f, 100.0f);
        basicShader.setMat4("projection", projection);
        basicShader.setFloat3("cameraPos", cameraPosition);
        
        glBindVertexArray(teapotVAO);
        for (int i = 0; i < objPositions.size(); i++)
        {
            int imat = i%materials.size();
            basicShader.setFloat3("material.ambient", materials[imat].ambient);
            basicShader.setFloat3("material.diffuse", materials[imat].diffuse);
            basicShader.setFloat3("material.specular", materials[imat].specular);
            basicShader.setFloat("material.shininess", materials[imat].shininess);
            
            glm::mat4 teapotModel = glm::mat4(1.0f);
            teapotModel = glm::translate(teapotModel, objPositions[i]);
            basicShader.setMat4("model", teapotModel);
            glDrawArrays(GL_TRIANGLES, 0, modelVertices.size()/6);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

#endif // MAIN_H_INCLUDED
