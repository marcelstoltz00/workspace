#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <thread>
#include <random>
#include <chrono>
#include "Square.h"
#include "Circle.h"
#include "Triangle.h"
#include "Vector.h"
#include "Matrix.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"

using namespace glm;
using namespace std;

enum type_of_object // only selected objects of a type can be transformed
{
    obj_DECOR = 0,
    obj_BALL = 1,
    obj_OBSTACLE_A = 2,
    obj_OBSTACLE_B = 3,
    obj_HOLE = 4,
    obj_OBSTACLE_C = 5  
};

const char *getError()
{
    const char *errorDescription;
    glfwGetError(&errorDescription);
    return errorDescription;
}

inline void startUpGLFW()
{
    glewExperimental = true;
    if (!glfwInit())
    {
        throw getError();
    }
}

inline void startUpGLEW()
{
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw getError();
    }
}
struct golfObject
{
    string id;        // what is this object->human readable name
    int type;         // what is this object in terms of attributes
    Shape<3> *shape;  // point to the geometric source
    vec3 baseColor;   // normal render colour
    vec3 pastelColor; // colour shown when selected
    bool selected;    // is this object selected?

    // Transform state (cumulative).
    vec2 position;     // w//a/s/d position changes
    vec2 scale;        //+ and - behaviour changes
    float rotationDeg; // q and e behaviour changes

    // cache for easy acess to transformed vertices for rendering
    vector<float> fillVertices;
    vector<float> wireVertices;

    golfObject() // safe defaults
        : type(obj_DECOR), shape(NULL), baseColor(1.0f, 1.0f, 1.0f),
          pastelColor(1.0f, 1.0f, 1.0f), selected(false),
          position(0.0f, 0.0f), scale(1.0f, 1.0f), rotationDeg(0.0f)
    {
    }
};
vector<golfObject *> objects;
golfObject *selectedObject = NULL;
bool WF = false;
double lastTime = 0.0;

vec3 toPastel(const vec3 &c)
{
    return c * 0.5f + vec3(1.0f, 1.0f, 1.0f) * 0.5f;
}

golfObject *findNextOfType(int type)
{
    if (objects.empty())
        return NULL;

    int startIndex = -1;
    if (selectedObject != NULL)
    {
        for (size_t i = 0; i < objects.size(); i++)
        {
            if (objects[i] == selectedObject)
            {
                startIndex = (int)i;
                break;
            }
        }
    }

    for (size_t step = 1; step <= objects.size(); step++)
    {
        int idx = (startIndex + (int)step) % (int)objects.size();
        if (objects[idx] != NULL && objects[idx]->type == type)
            return objects[idx];
    }

    return NULL;
}

void handleSelectionKeys(GLFWwindow *window)
{
    static bool key1Down = false;
    static bool key2Down = false;
    static bool key3Down = false;
    static bool key4Down = false;
    static bool key5down = false;
    static bool key0Down = false;

    bool key1Pressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
    bool key2Pressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
    bool key3Pressed = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;
    bool key4Pressed = glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;
    bool key0Pressed = glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS;
    bool key5Pressed = glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS;

    if (key1Pressed && !key1Down)
        selectedObject = findNextOfType(obj_BALL);
    if (key2Pressed && !key2Down)
        selectedObject = findNextOfType(obj_OBSTACLE_A);
    if (key3Pressed && !key3Down)
        selectedObject = findNextOfType(obj_OBSTACLE_B);
    if (key5Pressed && !key5down)
        selectedObject = findNextOfType(obj_OBSTACLE_C);
    if (key4Pressed && !key4Down)
        selectedObject = findNextOfType(obj_HOLE);
    if (key0Pressed && !key0Down)
        selectedObject = NULL;

    key1Down = key1Pressed;
    key2Down = key2Pressed;
    key3Down = key3Pressed;
    key4Down = key4Pressed;
    key0Down = key0Pressed;
    key5down = key5Pressed;
}

void handleTransformKeys(GLFWwindow *window)
{
    if (selectedObject == NULL)
        return;

    const float moveStep = 0.01f;
    const float scaleStep = 0.01f;
    const float rotateStep = 2.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        selectedObject->position.y += moveStep;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        selectedObject->position.y -= moveStep;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        selectedObject->position.x -= moveStep;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        selectedObject->position.x += moveStep;

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {
        selectedObject->scale.x += scaleStep;
        selectedObject->scale.y += scaleStep;
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {
        selectedObject->scale.x = std::max(0.05f, selectedObject->scale.x - scaleStep);
        selectedObject->scale.y = std::max(0.05f, selectedObject->scale.y - scaleStep);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        selectedObject->rotationDeg += rotateStep;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        selectedObject->rotationDeg -= rotateStep;
}

void handleWireframeToggle(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        double now = glfwGetTime();
        if (now - lastTime > 0.2)
        {
            WF = !WF;
            lastTime = now;
        }
    }
}

void drawObjects(GLuint progID, GLuint clrLOC, GLuint aspectLOC, float aspect, GLuint VAO, GLuint VBO)
{
    glUseProgram(progID);
    glUniform1f(aspectLOC, aspect);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (size_t i = 0; i < objects.size(); i++)
    {
        golfObject *obj = objects[i];
        if (obj == NULL || obj->shape == NULL)
            continue;

        // gets the raw vertices from the various shapes we've already coded in prac 1
        float *rawVerts = obj->shape->getPoints();
        int numPoints = obj->shape->getNumPoints();
        if (numPoints == 0)
            continue;

        float centerX = 0.0f;
        float centerY = 0.0f;
        for (int j = 0; j < numPoints; j++)
        {
            centerX += rawVerts[j * 3 + 0];
            centerY += rawVerts[j * 3 + 1];
        }
        centerX /= (float)numPoints;
        centerY /= (float)numPoints;

        float radians = obj->rotationDeg * 3.14159265f / 180.0f;
        float cosine = cosf(radians);
        float sine = sinf(radians);

        // Copy transformed shape vertices for filled rendering.
        obj->fillVertices.clear();
        for (int j = 0; j < numPoints; j++)
        {
            float x = rawVerts[j * 3 + 0];
            float y = rawVerts[j * 3 + 1];
            float z = rawVerts[j * 3 + 2];

            float localX = x - centerX;
            float localY = y - centerY;

            localX *= obj->scale.x;
            localY *= obj->scale.y;

            float rotatedX = localX * cosine - localY * sine;
            float rotatedY = localX * sine + localY * cosine;

            obj->fillVertices.push_back(centerX + rotatedX + obj->position.x);
            obj->fillVertices.push_back(centerY + rotatedY + obj->position.y);
            obj->fillVertices.push_back(z);
        }

        // Build explicit line segments for wireframe rendering.
        obj->wireVertices.clear();
        for (int j = 0; j < numPoints; j++)
        {
            int next = (j + 1) % numPoints;
            for (int k = 0; k < 3; k++)
                obj->wireVertices.push_back(obj->fillVertices[j * 3 + k]);
            for (int k = 0; k < 3; k++)
                obj->wireVertices.push_back(obj->fillVertices[next * 3 + k]);
        }

        vector<float> &activeVertices = WF ? obj->wireVertices : obj->fillVertices;

        // Upload active geometry to GPU.
        glBufferData(GL_ARRAY_BUFFER, activeVertices.size() * sizeof(float),
                     activeVertices.data(), GL_DYNAMIC_DRAW);

        // Tell OpenGL how to interpret the vertex data.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Use a strong outline colour in wireframe mode so edges are easy to read.
        vec3 c = WF ? vec3(0.05f, 0.05f, 0.05f)
                    : (obj->selected ? obj->pastelColor : obj->baseColor);
        glUniform3f(clrLOC, c.x, c.y, c.z);

        if (WF)
        {
            glLineWidth(2.0f);
            glDrawArrays(GL_LINES, 0, (GLsizei)obj->wireVertices.size() / 3);
            glLineWidth(1.0f);
        }
        else
        {
            glDrawArrays(GL_TRIANGLE_FAN, 0, numPoints);
        }

        delete[] rawVerts;
    }
}

void cleanupScene()
{
    for (size_t i = 0; i < objects.size(); i++)
    {
        if (objects[i] != NULL)
        {
            delete objects[i]->shape;
            delete objects[i];
        }
    }
    objects.clear();
}

void buildscene()
{
    // Concrete floor: almost full screen (leave tiny window border)
    golfObject *concrete = new golfObject();
    concrete->id = "concrete_floor";
    concrete->type = obj_DECOR;
    concrete->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 1.95f, 2.90f);
    concrete->baseColor = vec3(0.70f, 0.70f, 0.70f);
    concrete->pastelColor = toPastel(concrete->baseColor);
    objects.push_back(concrete);

    // One large border rectangle between concrete and grass
    golfObject *border = new golfObject();
    border->id = "course_border";
    border->type = obj_DECOR;
    border->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 1.80f, 2.65f);
    border->baseColor = vec3(0.28f, 0.17f, 0.09f);
    border->pastelColor = toPastel(border->baseColor);
    objects.push_back(border);

    // Inner grass patch (slightly smaller than concrete)
    golfObject *grass = new golfObject();
    grass->id = "grass_patch";
    grass->type = obj_DECOR;
    grass->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 1.65f, 2.45f);
    grass->baseColor = vec3(0.20f, 0.62f, 0.26f);
    grass->pastelColor = toPastel(grass->baseColor);
    objects.push_back(grass);



    // Start area
    golfObject *start = new golfObject();
    start->id = "start_area";
    start->type = obj_DECOR;
    start->shape = new Square<3>(Vector<3>({-0.72f, -0.72f, 0.0f}), 0.16f, 0.22f);
    start->baseColor = vec3(0.50f, 0.12f, 0.18f);
    start->pastelColor = toPastel(start->baseColor);
    objects.push_back(start);

    // River through middle
    golfObject *river = new golfObject();
    river->id = "river";
    river->type = obj_DECOR;
    river->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 0.20f, 2.45f);
    river->baseColor = vec3(0.15f, 0.42f, 0.86f);
    river->pastelColor = toPastel(river->baseColor);
    objects.push_back(river);

    // Left divider wall: 3/4 of grass height, near left third
    golfObject *wallLeft = new golfObject();
    wallLeft->id = "wall_left";
    wallLeft->type = obj_DECOR;
    wallLeft->shape = new Square<3>(Vector<3>({-0.42f, -0.21f, 0.0f}), 1.24f, 0.07f);
    wallLeft->baseColor = vec3(0.28f, 0.17f, 0.09f);
    wallLeft->pastelColor = toPastel(wallLeft->baseColor);
    objects.push_back(wallLeft);

    // Right divider wall: 3/4 of grass height, near right third
    golfObject *wallRight = new golfObject();
    wallRight->id = "wall_right";
    wallRight->type = obj_DECOR;
    wallRight->shape = new Square<3>(Vector<3>({0.42f, 0.21f, 0.0f}), 1.24f, 0.07f);
    wallRight->baseColor = vec3(0.28f, 0.17f, 0.09f);
    wallRight->pastelColor = toPastel(wallRight->baseColor);
    objects.push_back(wallRight);

    // Bridge 1 (left crossing)
    golfObject *bridge1 = new golfObject();
    bridge1->id = "bridge_1";
    bridge1->type = obj_DECOR;
    bridge1->shape = new Square<3>(Vector<3>({-0.9f, 0.0f, 0.0f}), 0.20f, 0.1f);
    bridge1->baseColor = vec3(0.72f, 0.52f, 0.30f);
    bridge1->pastelColor = toPastel(bridge1->baseColor);
    objects.push_back(bridge1);

    // Bridge 2 (middle crossing)
    golfObject *bridge2 = new golfObject();
    bridge2->id = "bridge_2";
    bridge2->type = obj_DECOR;
    bridge2->shape = new Square<3>(Vector<3>({0.00f, 0.0f, 0.0f}), 0.20f, 0.21f);
    bridge2->baseColor = vec3(0.72f, 0.52f, 0.30f);
    bridge2->pastelColor = toPastel(bridge2->baseColor);
    objects.push_back(bridge2);

    // Bridge 3 (thin rectangular crossing)
    golfObject *bridgeSkew = new golfObject();
    bridgeSkew->id = "bridge_skew";
    bridgeSkew->type = obj_DECOR;
    bridgeSkew->shape = new Square<3>(Vector<3>({0.8f, 0.0f, 0.0f}), 0.20f, 0.05f);
    bridgeSkew->baseColor = vec3(0.70f, 0.50f, 0.28f);
    bridgeSkew->pastelColor = toPastel(bridgeSkew->baseColor);
    objects.push_back(bridgeSkew);

    // Ball (white)
    golfObject *ball = new golfObject();
    ball->id = "ball";
    ball->type = obj_BALL;
    ball->shape = new Circle<3>(Vector<3>({-0.72f, -0.72f, 0.0f}), 0.025f, 60);
    ball->baseColor = vec3(1.0f, 1.0f, 1.0f);
    ball->pastelColor =  vec3(0.0f,0.0f,0.0f);
    objects.push_back(ball);

    // Hole (dark)
    golfObject *hole = new golfObject();
    hole->id = "hole";
    hole->type = obj_HOLE;
    hole->shape = new Circle<3>(Vector<3>({0.72f, 0.72f, 0.0f}), 0.035f, 60);
    hole->baseColor = vec3(0.10f, 0.10f, 0.10f);
    hole->pastelColor = toPastel(hole->baseColor);
    objects.push_back(hole);

    // Selectable obstacle type A (squares)
    golfObject *obsA1 = new golfObject();
    obsA1->id = "obstacle_a_1";
    obsA1->type = obj_OBSTACLE_A;
    obsA1->shape = new Square<3>(Vector<3>({0.8f, -0.45f, 0.0f}), 0.12f, 0.12f);
    obsA1->baseColor = vec3(0.62f, 0.40f, 0.22f);
    obsA1->pastelColor = toPastel(obsA1->baseColor);
    objects.push_back(obsA1);

     golfObject *obsA3 = new golfObject();
    obsA3->id = "obstacle_a_3";
    obsA3->type = obj_OBSTACLE_A;
    obsA3->shape = new Square<3>(Vector<3>({0.6f, -0.45f, 0.0f}), 0.12f, 0.12f);
    obsA3->baseColor = vec3(0.62f, 0.40f, 0.22f);
    obsA3->pastelColor = toPastel(obsA3->baseColor);
    objects.push_back(obsA3);

        // Selectable obstacle type A (squares)
    golfObject *obsA2 = new golfObject();
    obsA2->id = "obstacle_a_2";
    obsA2->type = obj_OBSTACLE_A;
    obsA2->shape = new Square<3>(Vector<3>({1.0f, -0.45f, 0.0f}), 0.12f, 0.12f);
    obsA2->baseColor = vec3(0.62f, 0.40f, 0.22f);
    obsA2->pastelColor = toPastel(obsA2->baseColor);
    objects.push_back(obsA2);

    // Selectable obstacle type B (9 mini triangles clustered between the walls)
    golfObject *obsB1 = new golfObject();
    obsB1->id = "obstacle_b_1";
    obsB1->type = obj_OBSTACLE_B;
    obsB1->shape = new Triangle<3>(
        Vector<3>({-0.24f, -0.19f, 0.0f}),
        Vector<3>({-0.20f, -0.25f, 0.0f}),
        Vector<3>({-0.28f, -0.25f, 0.0f}));
    obsB1->baseColor = vec3(0.92f, 0.46f, 0.14f);
    obsB1->pastelColor = toPastel(obsB1->baseColor);
    objects.push_back(obsB1);

    golfObject *obsB2 = new golfObject();
    obsB2->id = "obstacle_b_2";
    obsB2->type = obj_OBSTACLE_B;
    obsB2->shape = new Triangle<3>(
        Vector<3>({-0.10f, -0.17f, 0.0f}),
        Vector<3>({-0.06f, -0.22f, 0.0f}),
        Vector<3>({-0.14f, -0.22f, 0.0f}));
    obsB2->baseColor = vec3(0.92f, 0.46f, 0.14f);
    obsB2->pastelColor = toPastel(obsB2->baseColor);
    objects.push_back(obsB2);

    golfObject *obsB3 = new golfObject();
    obsB3->id = "obstacle_b_3";
    obsB3->type = obj_OBSTACLE_B;
    obsB3->shape = new Triangle<3>(
        Vector<3>({0.03f, -0.20f, 0.0f}),
        Vector<3>({0.07f, -0.27f, 0.0f}),
        Vector<3>({-0.01f, -0.27f, 0.0f}));
    obsB3->baseColor = vec3(0.92f, 0.46f, 0.14f);
    obsB3->pastelColor = toPastel(obsB3->baseColor);
    objects.push_back(obsB3);

    golfObject *obsB4 = new golfObject();
    obsB4->id = "obstacle_b_4";
    obsB4->type = obj_OBSTACLE_B;
    obsB4->shape = new Triangle<3>(
        Vector<3>({0.18f, -0.18f, 0.0f}),
        Vector<3>({0.22f, -0.24f, 0.0f}),
        Vector<3>({0.14f, -0.24f, 0.0f}));
    obsB4->baseColor = vec3(0.92f, 0.46f, 0.14f);
    obsB4->pastelColor = toPastel(obsB4->baseColor);
    objects.push_back(obsB4);

    golfObject *obsB5 = new golfObject();
    obsB5->id = "obstacle_b_5";
    obsB5->type = obj_OBSTACLE_B;
    obsB5->shape = new Triangle<3>(
        Vector<3>({-0.18f, -0.33f, 0.0f}),
        Vector<3>({-0.13f, -0.40f, 0.0f}),
        Vector<3>({-0.23f, -0.40f, 0.0f}));
    obsB5->baseColor = vec3(0.92f, 0.46f, 0.14f);
    obsB5->pastelColor = toPastel(obsB5->baseColor);
    objects.push_back(obsB5);

    golfObject *obsB6 = new golfObject();
    obsB6->id = "obstacle_b_6";
    obsB6->type = obj_OBSTACLE_B;
    obsB6->shape = new Triangle<3>(
        Vector<3>({-0.02f, -0.36f, 0.0f}),
        Vector<3>({0.02f, -0.42f, 0.0f}),
        Vector<3>({-0.06f, -0.42f, 0.0f}));
    obsB6->baseColor = vec3(0.92f, 0.46f, 0.14f);
    obsB6->pastelColor = toPastel(obsB6->baseColor);
    objects.push_back(obsB6);

    golfObject *obsB7 = new golfObject();
    obsB7->id = "obstacle_b_7";
    obsB7->type = obj_OBSTACLE_B;
    obsB7->shape = new Triangle<3>(
        Vector<3>({0.14f, -0.34f, 0.0f}),
        Vector<3>({0.18f, -0.39f, 0.0f}),
        Vector<3>({0.10f, -0.39f, 0.0f}));
    obsB7->baseColor = vec3(0.92f, 0.46f, 0.14f);
    obsB7->pastelColor = toPastel(obsB7->baseColor);
    objects.push_back(obsB7);

    golfObject *obsB8 = new golfObject();
    obsB8->id = "obstacle_b_8";
    obsB8->type = obj_OBSTACLE_B;
    obsB8->shape = new Triangle<3>(
        Vector<3>({-0.11f, -0.50f, 0.0f}),
        Vector<3>({-0.07f, -0.55f, 0.0f}),
        Vector<3>({-0.15f, -0.55f, 0.0f}));
    obsB8->baseColor = vec3(0.92f, 0.46f, 0.14f);
    obsB8->pastelColor = toPastel(obsB8->baseColor);
    objects.push_back(obsB8);

    golfObject *obsB9 = new golfObject();
    obsB9->id = "obstacle_b_9";
    obsB9->type = obj_OBSTACLE_B;
    obsB9->shape = new Triangle<3>(
        Vector<3>({0.09f, -0.49f, 0.0f}),
        Vector<3>({0.13f, -0.56f, 0.0f}),
        Vector<3>({0.05f, -0.56f, 0.0f}));
    obsB9->baseColor = vec3(0.92f, 0.46f, 0.14f);
    obsB9->pastelColor = toPastel(obsB9->baseColor);
    objects.push_back(obsB9);

    // Third unique obstacle combo (low-poly circle)
    golfObject *obsC1 = new golfObject();
    obsC1->id = "obstacle_c_1";
    obsC1->type = obj_OBSTACLE_C;
    obsC1->shape = new Circle<3>(Vector<3>({-0.62f, 0.58f, 0.0f}), 0.06f, 8);
    obsC1->baseColor = vec3(0.56f, 0.24f, 0.48f);
    obsC1->pastelColor = toPastel(obsC1->baseColor);
    objects.push_back(obsC1);
}

inline GLFWwindow *setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    GLFWwindow *window;
    window = glfwCreateWindow(1280, 800, "U24566552 COS344 Practical 2", NULL, NULL);
    if (window == NULL)
    {
        cout << getError() << endl;
        glfwTerminate();
        throw "Failed to open GLFW window.";
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    startUpGLEW();
    return window;
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

    cout << "Window created successfully. Press ESC or close window to exit." << endl;

    GLuint progID = LoadShaders("simple.vert", "simple.frag"); // reads both shader files, compiles them and links them to create a shader program -> returns the id of the shader program->this program gets used to render the objects
    GLuint clrLOC = glGetUniformLocation(progID, "inColor");   // essentially fast lookup for the memory location of the inColor uniform variable in the shader program -> allows us to set the color for rendering objects by passing values to this location before drawing calls
    GLuint aspectLOC = glGetUniformLocation(progID, "uAspect");

    GLuint VAO = 0; // Vertex Array Object -> describes vertex data layout for rendering -> the instructuons to read the bucket (layout description of vertex data)
    GLuint VBO = 0; // Vertex Buffer Object -> stores raw vertex data in gpu memory for fast access -> takes in the shapes vertices (bucket if vertex numbers)(vertex data)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);

    buildscene();

    glClearColor(0.5f, 0.0f, 1.0f, 1.0f);

    int frameCount = 0;
    do
    {

        int fbW = 1, fbH = 1;
        glfwGetFramebufferSize(window, &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);
        float aspect = (fbH != 0) ? (float)fbW / (float)fbH : 1.0f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

        // inputs from keyboard get logged here
        handleSelectionKeys(window);
        handleTransformKeys(window);
        handleWireframeToggle(window);

        // Sync exact selected object to object flags.
        for (size_t i = 0; i < objects.size(); i++)
        {
            golfObject *obj = objects[i];
            if (obj != NULL)
            {
                obj->selected = (obj == selectedObject);
            }
        }

        // pass allthe objects to render
        drawObjects(progID, clrLOC, aspectLOC, aspect, VAO, VBO);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        frameCount++;
        if (frameCount == 1)
        {
            cout << "Rendering started..." << endl;
        }

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    cout << "Closing window. Rendered " << frameCount << " frames." << endl;

    // make clean ayy
    cleanupScene();
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(progID);
    glfwTerminate();

    return 0;
}
