#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include <algorithm> // For std::copy
#include <iterator>  // For std::begin, std::end


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"


#define timeDT std::chrono::steady_clock::time_point

using namespace glm;
using namespace std;

const char *getError()
{
    const char *errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW()
{
    if (!glfwInit())
    {
        throw getError();
    }
}

inline GLFWwindow *setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    GLFWwindow *window;                                            // (In the accompanying source code, this variable is global for simplicity)
    window = glfwCreateWindow(1000, 1000, "Experiment", NULL, NULL);
    if (window == NULL)
    {
        cout << getError() << endl;
        glfwTerminate();
        throw "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n";
    }
    glfwMakeContextCurrent(window); // Initialize GLAD
    if (!gladLoadGL())
    {
        glfwTerminate();
        throw "Failed to initialize GLAD\n";
    }
    return window;
}

double aFPS()
{
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    double detlaTime = currentTime - lastTime;
    cout << "FPS: " << 1 / detlaTime << endl;
    lastTime = currentTime;
    return detlaTime;
}

int main()
{
    GLFWwindow *window;
    try
    {
        window = setUp();
    }
    catch (const char *e)
    {
        cout << e << endl;
        throw;
    }

    glClearColor(0.2, 0.2, 0.2, 0.2);
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_NEAREST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint programID = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");

    timeDT lastChanged = chrono::steady_clock::now();
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    double lastTime;
    lastTime = glfwGetTime();

    GLfloat vertices[18]; // 6 vertices * 3 components (x, y, z)
    GLfloat colors[24];   // 6 vertices * 4 components (r, g, b, a)

    // Initialize vertices
    const GLfloat initialVertices[18] = {
        0,0.1,0.1,
        -0.1,0,0.1,
        0.1,0,0.1,
        0,0.2,-0.1,
        -0.2,-0.1,-0.1,
        0.2,-0.1,-0.1,
    };
    std::copy(std::begin(initialVertices), std::end(initialVertices), std::begin(vertices));

    float alpha = 0.8;

    // Initialize colors
    const GLfloat initialColors[24] = {
        1, 0, 0, 1,
        0, 1, 0, 1,
        1, 0, 1, 1,
        0, 0, 0, alpha,
        0, 0, 0, alpha,
        0, 0, 0, alpha};
    std::copy(std::begin(initialColors), std::end(initialColors), std::begin(colors));

    int half = 9;

    do
    {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
            4,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );
        glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            mat4x4 translation = mat4x4(0.0f);
            translation[0].x = 1;
            translation[1].y = 1;
            translation[2].z = 1;
            translation[3].w = 1;
            translation[0].w = 0.01;

            for (int i = 0; i < half; i = i + 3)
            {
                vec4 point = vec4(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
                vec4 res = transpose(translation) * point;
                vertices[i] = res.x;
                vertices[i + 1] = res.y;
                vertices[i + 2] = res.z;
            }

            translation[0].w = -0.01;

            for (int i = 0; i < half; i = i + 3)
            {
                vec4 point = vec4(vertices[i + half], vertices[i + 1 + half], vertices[i + 2 + half], 1.0f);
                vec4 res = transpose(translation) * point;
                vertices[i + half] = res.x;
                vertices[i + 1 + half] = res.y;
                vertices[i + 2 + half] = res.z;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            mat4x4 translation = mat4x4(0.0f);
            translation[0].x = 1;
            translation[1].y = 1;
            translation[2].z = 1;
            translation[3].w = 1;
            translation[0].w = -0.01;

            for (int i = 0; i < half; i = i + 3)
            {
                vec4 point = vec4(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
                vec4 res = transpose(translation) * point;
                vertices[i] = res.x;
                vertices[i + 1] = res.y;
                vertices[i + 2] = res.z;
            }

            translation[0].w = 0.01;

            for (int i = 0; i < half; i = i + 3)
            {
                vec4 point = vec4(vertices[i + half], vertices[i + 1 + half], vertices[i + 2 + half], 1.0f);
                vec4 res = transpose(translation) * point;
                vertices[i + half] = res.x;
                vertices[i + 1 + half] = res.y;
                vertices[i + 2 + half] = res.z;
            }
        }

        lastTime = currentTime;
        cout << "FPS: " << 1 / deltaTime << endl;

    } while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);
}
