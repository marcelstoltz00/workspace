#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
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
    obj_HOLE = 4
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
int select = -1;
bool WF = false;
double lastTime = 0.0;

vec3 toPastel(const vec3 &c)
{
    return c * 0.5f + vec3(1.0f, 1.0f, 1.0f) * 0.5f;
}

void handleSelectionKeys(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        select = obj_BALL;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        select = obj_OBSTACLE_A;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        select = obj_OBSTACLE_B;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        select = obj_HOLE;
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        select = -1;
}

void handleTransformKeys(GLFWwindow *window)
{
    // TODO: Apply transforms to selected object only.
    //  W/S/A/D -> translation
    //  +/-     -> scale around object center
    //  Q/E     -> rotation around object center
    // Keep transformations cumulative.
    (void)window;
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

        //  copy these vertices into the objects own vector without changing the OG shapes data
        obj->fillVertices.clear();
        for (int j = 0; j < numPoints * 3; j++)
            obj->fillVertices.push_back(rawVerts[j]);

        // upload to GPU(but haha mac has integrated)
        glBufferData(GL_ARRAY_BUFFER, obj->fillVertices.size() * sizeof(float),
                     obj->fillVertices.data(), GL_DYNAMIC_DRAW);

        // how do we read the vbo and how do we send it to the shader->without this shader won't receive any data aka for every vertex you draw extract 3 floats from vbo and pass them to the shader as inposition
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // set the actual colour
        vec3 c = (obj->selected ? obj->pastelColor : obj->baseColor);
        glUniform3f(clrLOC, c.x, c.y, c.z);
        glDrawArrays(WF ? GL_LINE_LOOP : GL_TRIANGLE_FAN, 0, numPoints);
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

    // Inner grass patch (slightly smaller than concrete)
    golfObject *grass = new golfObject();
    grass->id = "grass_patch";
    grass->type = obj_DECOR;
    grass->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 1.65f, 2.45f);
    grass->baseColor = vec3(0.20f, 0.62f, 0.26f);
    grass->pastelColor = toPastel(grass->baseColor);
    objects.push_back(grass);

    // 9 small dark-brown border rectangles between grass and concrete
    golfObject *b1 = new golfObject();
    b1->id = "border_1";
    b1->type = obj_DECOR;
    b1->shape = new Square<3>(Vector<3>({-1.10f, 0.86f, 0.0f}), 0.10f, 0.20f);
    b1->baseColor = vec3(0.28f, 0.17f, 0.09f);
    b1->pastelColor = toPastel(b1->baseColor);
    objects.push_back(b1);

    golfObject *b2 = new golfObject();
    b2->id = "border_2";
    b2->type = obj_DECOR;
    b2->shape = new Square<3>(Vector<3>({0.0f, 0.86f, 0.0f}), 0.10f, 0.20f);
    b2->baseColor = vec3(0.28f, 0.17f, 0.09f);
    b2->pastelColor = toPastel(b2->baseColor);
    objects.push_back(b2);

    golfObject *b3 = new golfObject();
    b3->id = "border_3";
    b3->type = obj_DECOR;
    b3->shape = new Square<3>(Vector<3>({1.10f, 0.86f, 0.0f}), 0.10f, 0.20f);
    b3->baseColor = vec3(0.28f, 0.17f, 0.09f);
    b3->pastelColor = toPastel(b3->baseColor);
    objects.push_back(b3);

    golfObject *b4 = new golfObject();
    b4->id = "border_4";
    b4->type = obj_DECOR;
    b4->shape = new Square<3>(Vector<3>({-1.26f, 0.0f, 0.0f}), 0.18f, 0.10f);
    b4->baseColor = vec3(0.28f, 0.17f, 0.09f);
    b4->pastelColor = toPastel(b4->baseColor);
    objects.push_back(b4);

    golfObject *b5 = new golfObject();
    b5->id = "border_5";
    b5->type = obj_DECOR;
    b5->shape = new Square<3>(Vector<3>({1.26f, 0.0f, 0.0f}), 0.18f, 0.10f);
    b5->baseColor = vec3(0.28f, 0.17f, 0.09f);
    b5->pastelColor = toPastel(b5->baseColor);
    objects.push_back(b5);

    golfObject *b6 = new golfObject();
    b6->id = "border_6";
    b6->type = obj_DECOR;
    b6->shape = new Square<3>(Vector<3>({-1.10f, -0.86f, 0.0f}), 0.10f, 0.20f);
    b6->baseColor = vec3(0.28f, 0.17f, 0.09f);
    b6->pastelColor = toPastel(b6->baseColor);
    objects.push_back(b6);

    golfObject *b7 = new golfObject();
    b7->id = "border_7";
    b7->type = obj_DECOR;
    b7->shape = new Square<3>(Vector<3>({0.0f, -0.86f, 0.0f}), 0.10f, 0.20f);
    b7->baseColor = vec3(0.28f, 0.17f, 0.09f);
    b7->pastelColor = toPastel(b7->baseColor);
    objects.push_back(b7);

    golfObject *b8 = new golfObject();
    b8->id = "border_8";
    b8->type = obj_DECOR;
    b8->shape = new Square<3>(Vector<3>({1.10f, -0.86f, 0.0f}), 0.10f, 0.20f);
    b8->baseColor = vec3(0.28f, 0.17f, 0.09f);
    b8->pastelColor = toPastel(b8->baseColor);
    objects.push_back(b8);

    golfObject *b9 = new golfObject();
    b9->id = "border_9";
    b9->type = obj_DECOR;
    b9->shape = new Square<3>(Vector<3>({0.55f, 0.86f, 0.0f}), 0.10f, 0.20f);
    b9->baseColor = vec3(0.28f, 0.17f, 0.09f);
    b9->pastelColor = toPastel(b9->baseColor);
    objects.push_back(b9);

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
    river->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 0.20f, 2.30f);
    river->baseColor = vec3(0.15f, 0.42f, 0.86f);
    river->pastelColor = toPastel(river->baseColor);
    objects.push_back(river);

    // Left wall: rises 3/4 of floor height near 1/3 of base
    golfObject *wallLeft = new golfObject();
    wallLeft->id = "wall_left";
    wallLeft->type = obj_DECOR;
    wallLeft->shape = new Square<3>(Vector<3>({-0.30f, -0.225f, 0.0f}), 1.35f, 0.06f);
    wallLeft->baseColor = vec3(0.28f, 0.17f, 0.09f);
    wallLeft->pastelColor = toPastel(wallLeft->baseColor);
    objects.push_back(wallLeft);

    // Right wall: drops 3/4 of floor height near 2/3 of base
    golfObject *wallRight = new golfObject();
    wallRight->id = "wall_right";
    wallRight->type = obj_DECOR;
    wallRight->shape = new Square<3>(Vector<3>({0.30f, 0.225f, 0.0f}), 1.35f, 0.06f);
    wallRight->baseColor = vec3(0.28f, 0.17f, 0.09f);
    wallRight->pastelColor = toPastel(wallRight->baseColor);
    objects.push_back(wallRight);

    // Bridge 1 (left crossing)
    golfObject *bridge1 = new golfObject();
    bridge1->id = "bridge_1";
    bridge1->type = obj_DECOR;
    bridge1->shape = new Square<3>(Vector<3>({-0.62f, 0.0f, 0.0f}), 0.20f, 0.12f);
    bridge1->baseColor = vec3(0.72f, 0.52f, 0.30f);
    bridge1->pastelColor = toPastel(bridge1->baseColor);
    objects.push_back(bridge1);

    // Bridge 2 (middle crossing)
    golfObject *bridge2 = new golfObject();
    bridge2->id = "bridge_2";
    bridge2->type = obj_DECOR;
    bridge2->shape = new Square<3>(Vector<3>({0.00f, 0.0f, 0.0f}), 0.20f, 0.12f);
    bridge2->baseColor = vec3(0.72f, 0.52f, 0.30f);
    bridge2->pastelColor = toPastel(bridge2->baseColor);
    objects.push_back(bridge2);

    // Bridge 3 (thin rectangular crossing)
    golfObject *bridgeSkew = new golfObject();
    bridgeSkew->id = "bridge_skew";
    bridgeSkew->type = obj_DECOR;
    bridgeSkew->shape = new Square<3>(Vector<3>({0.62f, 0.0f, 0.0f}), 0.20f, 0.08f);
    bridgeSkew->baseColor = vec3(0.70f, 0.50f, 0.28f);
    bridgeSkew->pastelColor = toPastel(bridgeSkew->baseColor);
    objects.push_back(bridgeSkew);

    // Ball (white)
    golfObject *ball = new golfObject();
    ball->id = "ball";
    ball->type = obj_BALL;
    ball->shape = new Circle<3>(Vector<3>({-0.72f, -0.72f, 0.0f}), 0.025f, 60);
    ball->baseColor = vec3(1.0f, 1.0f, 1.0f);
    ball->pastelColor = toPastel(ball->baseColor);
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
    obsA1->shape = new Square<3>(Vector<3>({-0.10f, -0.45f, 0.0f}), 0.12f, 0.12f);
    obsA1->baseColor = vec3(0.62f, 0.40f, 0.22f);
    obsA1->pastelColor = toPastel(obsA1->baseColor);
    objects.push_back(obsA1);

    // Selectable obstacle type B (triangles)
    golfObject *obsB1 = new golfObject();
    obsB1->id = "obstacle_b_1";
    obsB1->type = obj_OBSTACLE_B;
    obsB1->shape = new Triangle<3>(
        Vector<3>({0.08f, 0.50f, 0.0f}),
        Vector<3>({0.22f, 0.30f, 0.0f}),
        Vector<3>({-0.04f, 0.30f, 0.0f}));
    obsB1->baseColor = vec3(0.92f, 0.46f, 0.14f);
    obsB1->pastelColor = toPastel(obsB1->baseColor);
    objects.push_back(obsB1);

    // Third unique obstacle combo (low-poly circle)
    golfObject *obsC1 = new golfObject();
    obsC1->id = "obstacle_c_1";
    obsC1->type = obj_DECOR;
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
    window = glfwCreateWindow(1280, 800, "Experiment", NULL, NULL);
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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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

        // sync selected type to object
        for (size_t i = 0; i < objects.size(); i++)
        {
            golfObject *obj = objects[i];
            if (obj != NULL)
            {
                obj->selected = (select != -1 && obj->type == select);
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
