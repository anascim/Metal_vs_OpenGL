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

void processInput(GLFWwindow *window);
void didChangeSize(GLFWwindow* window, int width, int height);
void didChangeMousePosition(GLFWwindow* window, double xPos, double yPos);
void didChangeScrollValue(GLFWwindow* window, double xOffset, double yOffset);

glm::vec3 objPositions[] = {
    glm::vec3(0,0,-5),
    glm::vec3(8,0,-7),
    glm::vec3(-8,2,0),
    glm::vec3(0,-6,1),
    glm::vec3(-1,-2,4)
};

glm::vec3 cubePosition = glm::vec3(0.0f, -0.4f, -3.0f);

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
int windowW, windowH;

float deltaTime = 0.0f;
float lastTime = 0.0f;

// ---- CAMERA ----
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
float lastX = SCR_WIDTH/2.0f, lastY = SCR_HEIGHT/2.0f;
bool isFirstMouseUpdate = true;

int main()
{
    // #######################
    // ::  INITIALIZATION   ::
    // #######################
    
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

    // ---- SETUP CALLBACKS ----
    glfwSetFramebufferSizeCallback(window, didChangeSize);
    glfwSetCursorPosCallback(window, didChangeMousePosition);
    glfwSetScrollCallback(window, didChangeScrollValue);

    glEnable(GL_DEPTH_TEST);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader basicShader("OpenGL_Render/basic_vertex.vs", "OpenGL_Render/basic_fragment.fs");
    
    // ---- PERMANENT SETUP ----
    ModelLoader *modelLoader = new ModelLoader("Shared/teapot_triangulated.obj");
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
    basicShader.setFloat3("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    basicShader.setFloat3("material.diffuse", glm::vec3(1.0f, 0.6f, 0.31f));
    basicShader.setFloat3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    basicShader.setFloat("material.shininess", 128.0f);
    basicShader.setFloat3("lightColor", glm::vec3(1.0f));
    basicShader.setFloat3("lightDirection", glm::vec3(-1.0f,-1.0f,-1.0f));

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // ###################
    // ::  RENDER LOOP  ::
    // ###################
    
    while(!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window);

        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        basicShader.use(); // program must be used before updating its uniforms
        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, cubePosition);
        basicShader.setMat4("model", cubeModel);
        
        glm::mat4 view = camera.GetViewMatrix();
        basicShader.setMat4("view", view);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Fov), (float)windowW/(float)windowH, 0.1f, 100.0f);
        basicShader.setMat4("projection", projection);
        basicShader.setFloat3("cameraPos", camera.Position);
        
        glBindVertexArray(teapotVAO);
        for (int i = 0; i < 5; i++)
        {
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

    int hMove = 0;
    int vMove = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        vMove += 1;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        hMove -= 1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        vMove -= 1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        hMove += 1;

    camera.ProcessMovement(hMove, vMove, deltaTime);
}

// ########################
// ::   GLFW CALLBACKS   ::
// ########################

void didChangeSize(GLFWwindow* window, int width, int height)
{
    windowW = width;
    windowH = height;
    glViewport(0, 0, width, height);
}

void didChangeMousePosition(GLFWwindow* window, double xPos, double yPos)
{
    if (isFirstMouseUpdate)
    {
        lastX = xPos;
        lastY = yPos;
        isFirstMouseUpdate = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // reversed since y coordinates range from bottom to top
    lastX = xPos;
    lastY = yPos;

    camera.ProcessRotation(xOffset, yOffset);
}

void didChangeScrollValue(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.ProcessScroll((float)yOffset);
}

#endif // MAIN_H_INCLUDED
