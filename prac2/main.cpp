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

enum type_of_object//only selected objects of a type can be transformed
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
    string id;// what is this object->human readable name
    int type;//what is this object in terms of attributes
    Shape<3> *shape;//point to the geometric source 
    vec3 baseColor;//normal render colour
    vec3 pastelColor;//colour shown when selected
    bool selected;//is this object selected?

    // Transform state (cumulative).
    vec2 position;//w//a/s/d position changes 
    vec2 scale;//+ and - behaviour changes
    float rotationDeg;//q and e behaviour changes

    // cache for easy acess to transformed vertices for rendering
    vector<float> fillVertices;
    vector<float> wireVertices;

    golfObject()//safe defaults 
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

void drawObjects(GLuint progID, GLuint clrLOC, GLuint VAO, GLuint VBO)
{
    glUseProgram(progID);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // TODO: For each object:
    // 1) Build transformed vertex list from shape + transform state.
    // 2) Use pastel color when object is selected.
    // 3) Upload vertices with glBufferData.
    // 4) Set attribute pointer location 0 as vec2.
    // 5) Draw GL_TRIANGLES in normal mode, GL_LINES in wireframe mode.
    // Note: Use object->fillVertices and object->wireVertices as needed.

    for (size_t i = 0; i < objects.size(); i++)
    {
        golfObject *obj = objects[i];
        if (obj == NULL)
            continue;

        vec3 c = (obj->selected ? obj->pastelColor : obj->baseColor);
        glUniform3f(clrLOC, c.x, c.y, c.z);

        // TODO: Replace with actual per-object draw call once vertices are prepared.
        // glDrawArrays(WF ? GL_LINES : GL_TRIANGLES, 0, vertexCount);
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
    // TODO: Create all required practical objects and push to `objects`.
    // Required at minimum:
    // - Floor (grey rectangle)
    // - Start area (maroon rectangle)
    // - River (blue rectangle)
    // - Hole (circle)
    // - Ball (circle)
    // - Barrier rectangles
    // - At least 3 obstacle types with distinct shape+color combos
    //
    // Example allocation pattern:
    // golfObject *o = new golfObject();
    // o->id = "ball";
    // o->type = OBJ_BALL;
    // o->shape = new Circle<3>(Vector<3>({0.0f, 0.0f, 1.0f}), 0.04f, 50);
    // o->baseColor = vec3(1.0f, 1.0f, 1.0f);
    // o->pastelColor = toPastel(o->baseColor);
    // objects.push_back(o);
}

inline GLFWwindow *setUp()
{
    startUpGLFW();
    glfwWindowHint(GLFW_SAMPLES, 4);            
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
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

    GLuint progID = LoadShaders("simple.vert", "simple.frag");//reads both shader files, compiles them and links them to create a shader program -> returns the id of the shader program->this program gets used to render the objects 
    GLuint clrLOC = glGetUniformLocation(progID, "inColor");//essentially fast lookup for the memory location of the inColor uniform variable in the shader program -> allows us to set the color for rendering objects by passing values to this location before drawing calls

    GLuint VAO = 0;//Vertex Array Object -> describes vertex data layout for rendering -> the instructuons to read the bucket (layout description of vertex data)
    GLuint VBO = 0;//Vertex Buffer Object -> stores raw vertex data in gpu memory for fast access -> takes in the shapes vertices (bucket if vertex numbers)(vertex data)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);

    buildscene();


    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

 
    int frameCount = 0;
    do
    {
      
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear screen

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
        drawObjects(progID, clrLOC, VAO, VBO);

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
