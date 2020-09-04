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

float cube_vertices[] = {
    // float3 position, float3 normal
    // front
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    // back
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    // left
    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
    // right
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
    // bottom
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
    // top
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};

glm::vec3 cubePosition = glm::vec3(0.0f, -0.4f, -3.0f);
glm::vec3 lightCubePosition = glm::vec3(2.0f, 1.0f, -1.0f);

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
int windowW, windowH;

float deltaTime = 0.0f;
float lastTime = 0.0f;

// ---- CAMERA ----
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH/2.0f, lastY = SCR_HEIGHT/2.0f;
bool isFirstMouseUpdate = true;

int main()
{
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
    
    // tell OpenGL to render onto the window
    // the viewport could be smaller than the actual window.
    // a window could have other viewports as well.
    // (lower-left corner, width, height)
    glfwGetFramebufferSize(window, &windowW, &windowH); // needed for retina display
    glViewport(0, 0, windowW, windowH);

    // Setup callbakcs
    // update viewport on window size change
    glfwSetFramebufferSizeCallback(window, didChangeSize);
    glfwSetCursorPosCallback(window, didChangeMousePosition);
    glfwSetScrollCallback(window, didChangeScrollValue);

    glEnable(GL_DEPTH_TEST);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader basicShader("/Users/alexnascimento/repos/Graphics_APIs/OpenGL_Render/basic_vertex.vs", "/Users/alexnascimento/repos/Graphics_APIs/OpenGL_Render/basic_fragment.fs");
    Shader lightShader("/Users/alexnascimento/repos/Graphics_APIs/OpenGL_Render/light_vertex.vs", "/Users/alexnascimento/repos/Graphics_APIs/OpenGL_Render/light_fragment.fs");
    // ---- PERMANENT SETUP ----

    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    
    ModelLoader *model = new ModelLoader("/Users/alexnascimento/repos/Graphics_APIs/Shared/cube.obj");
    vector<float> suzanneVertices = model->getVertices();
//    for(float f : suzanneVertices)
//        std::cout << "f = " << f << std::endl;
    unsigned int suzanneVAO;
    glGenVertexArrays(1, &suzanneVAO);
    glBindVertexArray(suzanneVAO);

    unsigned int suzanneVBO;
    glGenBuffers(1, &suzanneVBO);

    glBindBuffer(GL_ARRAY_BUFFER, suzanneVBO);
    glBufferData(GL_ARRAY_BUFFER, suzanneVertices.size() * sizeof(float), &suzanneVertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    basicShader.use();
    basicShader.setFloat3("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    basicShader.setFloat3("material.diffuse", glm::vec3(1.0f, 0.6f, 0.31f));
    basicShader.setFloat3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    basicShader.setFloat("material.shininess", 128.0f);
    basicShader.setFloat3("lightColor", glm::vec3(1.0f));
    basicShader.setFloat3("lightPos", lightCubePosition);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // ---- RENDER LOOP ----
    while(!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
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

        glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        
        basicShader.use();
        glm::mat4 suzanneModel = glm::mat4(1.0f);
        suzanneModel = glm::translate(suzanneModel, glm::vec3(0.0f,0.0f,-3.0f));
        basicShader.setMat4("model", suzanneModel);
        
        glBindVertexArray(suzanneVAO);
        glDrawArrays(GL_TRIANGLES, 0, suzanneVertices.size()/6);

        lightShader.use();
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightCubePosition);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        lightShader.setMat4("model", lightModel);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        
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
// ###  GLFW CALLBACKS  ###
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
