
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
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

using Vec2 = Vector<2>;
using Vec3 = Vector<3>;

enum type_of_object
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
    string id;      
    int type;       
    Shape<3> *shape; 
    Vec3 baseColor;  
    Vec3 pastelColor;
    bool selected;    


    Vec2 position;   
    Vec2 scale;      
    float rotationDeg; 

    vector<float> fillVertices;
    vector<float> wireVertices;

    golfObject() 
        : type(obj_DECOR), shape(NULL), baseColor({1.0f, 1.0f, 1.0f}),
          pastelColor({1.0f, 1.0f, 1.0f}), selected(false),
          position({0.0f, 0.0f}), scale({1.0f, 1.0f}), rotationDeg(0.0f)
    {
    }
};
vector<golfObject *> objects;
golfObject *selectedObject = NULL;
vector<golfObject *> selectedObjects;
bool WF = false;
double lastTime = 0.0;

const float SCENE_MIN_X = -0.9f;   
const float SCENE_MAX_X = 0.9f;    // +width/2 of border
const float SCENE_MIN_Y = -1.325f; // -height/2 of border
const float SCENE_MAX_Y = 1.325f;  // +height/2 of border
const char *LAYOUT_FILE = "layout.txt";

void logLoopDiagnostics(GLFWwindow *window, int frameCount)
{
    static double lastLogTime = 0.0;
    static int previousEscState = GLFW_RELEASE;
    static int previousShouldClose = 0;

    int escState = glfwGetKey(window, GLFW_KEY_ESCAPE);
    int shouldClose = glfwWindowShouldClose(window);
    double now = glfwGetTime();

    if (escState != previousEscState)
    {
        cout << "[loop] ESC state changed to "
             << (escState == GLFW_PRESS ? "PRESS" : "RELEASE")
             << " at frame " << frameCount << endl;
        previousEscState = escState;
    }

    if (shouldClose != previousShouldClose)
    {
        cout << "[loop] Window close flag changed to " << shouldClose
             << " at frame " << frameCount << endl;
        previousShouldClose = shouldClose;
    }

    if (now - lastLogTime >= 1.0)
    {
        cout << "[loop] alive frame=" << frameCount
             << " esc=" << (escState == GLFW_PRESS ? "PRESS" : "RELEASE")
             << " close=" << shouldClose << endl;
        lastLogTime = now;
    }
}

Vec3 toPastel(const Vec3 &c)
{
    return c * 0.5f + Vec3({1.0f, 1.0f, 1.0f}) * 0.5f;
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

bool isObjectSelected(golfObject *obj)
{
    return std::find(selectedObjects.begin(), selectedObjects.end(), obj) != selectedObjects.end();
}

void clearSelection()
{
    selectedObjects.clear();
    selectedObject = NULL;
}

void addSelection(golfObject *obj)
{
    if (obj == NULL)
        return;

    if (!isObjectSelected(obj))
        selectedObjects.push_back(obj);
    selectedObject = obj;
}


void handleDynamicObjectKeys(GLFWwindow *window)
{
    static bool cmdSDown = false;
    static bool cmdTDown = false;
    static bool cmdCDown = false;
    static bool optDDown = false;

    bool cmd = (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS);
    bool opt = (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS);

    bool sPressed = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool tPressed = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;
    bool cPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
    bool dPressed = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

    if (cmd && sPressed && !cmdSDown) {
        golfObject *sq = new golfObject();
        sq->id = "user_square_" + to_string(rand());
        sq->type =obj_OBSTACLE_A;
        sq->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 0.2f, 0.2f);
        sq->baseColor = Vec3({0.5f, 0.5f, 1.0f});
        sq->pastelColor = toPastel(sq->baseColor);
        objects.push_back(sq);
     
    }
    if (cmd && tPressed && !cmdTDown) {
        golfObject *tri = new golfObject();
        tri->id = "user_triangle_" + to_string(rand());
        tri->type = obj_OBSTACLE_B;
        tri->shape = new Triangle<3>(
            Vector<3>({-0.1f, -0.1f, 0.0f}),
            Vector<3>({0.1f, -0.1f, 0.0f}),
            Vector<3>({0.0f, 0.1f, 0.0f})
        );
        tri->baseColor = Vec3({1.0f, 0.5f, 0.5f});
        tri->pastelColor = toPastel(tri->baseColor);
        objects.push_back(tri);
      
    }
    if (cmd && cPressed && !cmdCDown) {
        golfObject *circ = new golfObject();
        circ->id = "user_circle_" + to_string(rand());
        circ->type = obj_OBSTACLE_C;
        circ->shape = new Circle<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 0.12f, 32);
        circ->baseColor = Vec3({0.5f, 1.0f, 0.5f});
        circ->pastelColor = toPastel(circ->baseColor);
        objects.push_back(circ);
      
    }
    if (opt && dPressed && !optDDown) {
        if (!selectedObjects.empty()) {
            for (golfObject *obj : selectedObjects) {
                auto it = std::find(objects.begin(), objects.end(), obj);
                if (it != objects.end()) {
                    delete obj->shape;
                    delete obj;
                    objects.erase(it);
                }
            }
           
            clearSelection();
        }
    }

    cmdSDown = cmd && sPressed;
    cmdTDown = cmd && tPressed;
    cmdCDown = cmd && cPressed;
    optDDown = opt && dPressed;
}

void toggleSelection(golfObject *obj)
{
    if (obj == NULL)
        return;

    vector<golfObject *>::iterator it = std::find(selectedObjects.begin(), selectedObjects.end(), obj);
    if (it != selectedObjects.end())
    {
        selectedObjects.erase(it);
    }
    else
    {
        selectedObjects.push_back(obj);
    }

    selectedObject = selectedObjects.empty() ? NULL : selectedObjects.back();
}

bool canMoveObjectBy(golfObject *obj, float dx, float dy)
{
    if (obj == NULL || obj->shape == NULL)
        return false;

    float *rawVerts = obj->shape->getPoints();
    int numPoints = obj->shape->getNumPoints();
    if (numPoints <= 0)
    {
        delete[] rawVerts;
        return false;
    }

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

    float minX = 1e9f;
    float maxX = -1e9f;
    float minY = 1e9f;
    float maxY = -1e9f;

    for (int j = 0; j < numPoints; j++)
    {
        float x = rawVerts[j * 3 + 0];
        float y = rawVerts[j * 3 + 1];

        float localX = (x - centerX) * obj->scale[0];
        float localY = (y - centerY) * obj->scale[1];

        float rotatedX = localX * cosine - localY * sine;
        float rotatedY = localX * sine + localY * cosine;

        float worldX = centerX + rotatedX + obj->position[0] + dx;
        float worldY = centerY + rotatedY + obj->position[1] + dy;

        minX = std::min(minX, worldX);
        maxX = std::max(maxX, worldX);
        minY = std::min(minY, worldY);
        maxY = std::max(maxY, worldY);
    }

    delete[] rawVerts;
    return true;
}

bool saveLayout(const string &path)
{
    ofstream out(path.c_str());
    if (!out.is_open())
        return false;

    size_t count = 0;
    for (size_t i = 0; i < objects.size(); i++)
    {
        if (objects[i] != NULL && objects[i]->shape != NULL)
            count++;
    }

    out << count << "\n";
    for (size_t i = 0; i < objects.size(); i++)
    {
        golfObject *obj = objects[i];
        if (obj == NULL || obj->shape == NULL)
            continue;

        out << obj->id << " " << obj->type << " "
            << obj->position[0] << " " << obj->position[1] << " "
            << obj->scale[0] << " " << obj->scale[1] << " "
            << obj->rotationDeg << " "
            << obj->baseColor[0] << " " << obj->baseColor[1] << " " << obj->baseColor[2] << " "
            << obj->pastelColor[0] << " " << obj->pastelColor[1] << " " << obj->pastelColor[2] << "\n";
    }

    return true;
}

golfObject *findObjectByIdAndType(const string &id, int type)
{
    for (size_t i = 0; i < objects.size(); i++)
    {
        if (objects[i] != NULL && objects[i]->id == id && objects[i]->type == type)
            return objects[i];
    }
    return NULL;
}

bool loadLayout(const string &path)
{
    ifstream in(path.c_str());
    if (!in.is_open())
        return false;

    size_t count = 0;
    if (!(in >> count))
        return false;

    for (size_t i = 0; i < count; i++)
    {
        string id;
        int type = 0;
        float px = 0.0f, py = 0.0f;
        float sx = 1.0f, sy = 1.0f;
        float rot = 0.0f;
        float br = 1.0f, bg = 1.0f, bb = 1.0f;
        float pr = 1.0f, pg = 1.0f, pb = 1.0f;

        if (!(in >> id >> type >> px >> py >> sx >> sy >> rot >> br >> bg >> bb >> pr >> pg >> pb))
            return false;

        golfObject *target = findObjectByIdAndType(id, type);
        if (target != NULL)
        {
            target->position = Vec2({px, py});
            target->scale = Vec2({sx, sy});
            target->rotationDeg = rot;
            target->baseColor = Vec3({br, bg, bb});
            target->pastelColor = Vec3({pr, pg, pb});

            if (target->type == obj_BALL)
            {
                const float diff = fabsf(target->pastelColor[0] - target->baseColor[0]) +
                                   fabsf(target->pastelColor[1] - target->baseColor[1]) +
                                   fabsf(target->pastelColor[2] - target->baseColor[2]);
                if (diff < 0.02f)
                    target->pastelColor = Vec3({1.0f, 0.85f, 0.35f});
            }
        }
    }

    clearSelection();
    return true;
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

    bool additiveSelect = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                          glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

    auto applySelection = [&](golfObject *candidate)
    {
        if (candidate == NULL)
            return;

        if (additiveSelect)
            toggleSelection(candidate);
        else
        {
            clearSelection();
            addSelection(candidate);
        }
    };

    if (key1Pressed && !key1Down)
        applySelection(findNextOfType(obj_BALL));
    if (key2Pressed && !key2Down)
        applySelection(findNextOfType(obj_OBSTACLE_A));
    if (key3Pressed && !key3Down)
        applySelection(findNextOfType(obj_OBSTACLE_B));
    if (key5Pressed && !key5down)
        applySelection(findNextOfType(obj_OBSTACLE_C));
    if (key4Pressed && !key4Down)
        applySelection(findNextOfType(obj_HOLE));
    if (key0Pressed && !key0Down)
        clearSelection();

    key1Down = key1Pressed;
    key2Down = key2Pressed;
    key3Down = key3Pressed;
    key4Down = key4Pressed;
    key0Down = key0Pressed;
    key5down = key5Pressed;
}

void handleTransformKeys(GLFWwindow *window)
{
    if (selectedObjects.empty())
        return;

    const float moveStep = 0.01f;
    const float scaleStep = 0.01f;
    const float rotateStep = 2.0f;

    float dx = 0.0f;
    float dy = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        dy += moveStep;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        dy -= moveStep;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        dx -= moveStep;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        dx += moveStep;

    if (dx != 0.0f || dy != 0.0f)
    {
        for (size_t i = 0; i < selectedObjects.size(); i++)
        {
            golfObject *obj = selectedObjects[i];
            if (canMoveObjectBy(obj, dx, dy))
            {
                obj->position[0] += dx;
                obj->position[1] += dy;
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
    {
        for (size_t i = 0; i < selectedObjects.size(); i++)
        {
            selectedObjects[i]->scale[0] += scaleStep;
            selectedObjects[i]->scale[1] += scaleStep;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
    {
        for (size_t i = 0; i < selectedObjects.size(); i++)
        {
            selectedObjects[i]->scale[0] = std::max(0.05f, selectedObjects[i]->scale[0] - scaleStep);
            selectedObjects[i]->scale[1] = std::max(0.05f, selectedObjects[i]->scale[1] - scaleStep);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        for (size_t i = 0; i < selectedObjects.size(); i++)
            selectedObjects[i]->rotationDeg += rotateStep;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        for (size_t i = 0; i < selectedObjects.size(); i++)
            selectedObjects[i]->rotationDeg -= rotateStep;
    }
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

void handleLayoutIOKeys(GLFWwindow *window)
{
    static bool saveDown = false;
    static bool loadDown = false;

    bool savePressed = glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS;
    bool loadPressed = glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS;

    if (savePressed && !saveDown)
    {
        if (saveLayout(LAYOUT_FILE))
            cout << "Layout saved to " << LAYOUT_FILE << endl;
        else
            cout << "Failed to save layout to " << LAYOUT_FILE << endl;
    }

    if (loadPressed && !loadDown)
    {
        if (loadLayout(LAYOUT_FILE))
            cout << "Layout loaded from " << LAYOUT_FILE << endl;
        else
            cout << "Failed to load layout from " << LAYOUT_FILE << endl;
    }

    saveDown = savePressed;
    loadDown = loadPressed;
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

    
        vector<float> transVerts;
        transVerts.reserve(numPoints * 3);

        for (int j = 0; j < numPoints; j++)
        {
            float x = rawVerts[j * 3 + 0];
            float y = rawVerts[j * 3 + 1];
            float z = rawVerts[j * 3 + 2];

            float localX = (x - centerX) * obj->scale[0];
            float localY = (y - centerY) * obj->scale[1];

            float rotatedX = localX * cosine - localY * sine;
            float rotatedY = localX * sine + localY * cosine;

            transVerts.push_back(centerX + rotatedX + obj->position[0]);
            transVerts.push_back(centerY + rotatedY + obj->position[1]);
            transVerts.push_back(z);
        }

        obj->fillVertices.clear();
        obj->wireVertices.clear();

        if (numPoints >= 3)
        {
            bool isCircle = (dynamic_cast<Circle<3> *>(obj->shape) != NULL);

            if (isCircle)
            {
                const float cx = centerX + obj->position[0];
                const float cy = centerY + obj->position[1];
                const float cz = transVerts[2];

                for (int j = 0; j < numPoints; j++)
                {
                    int next = (j + 1) % numPoints;

                    const float x1 = transVerts[j * 3 + 0];
                    const float y1 = transVerts[j * 3 + 1];
                    const float z1 = transVerts[j * 3 + 2];
                    const float x2 = transVerts[next * 3 + 0];
                    const float y2 = transVerts[next * 3 + 1];
                    const float z2 = transVerts[next * 3 + 2];

                    obj->fillVertices.insert(obj->fillVertices.end(), {cx, cy, cz,
                                                                       x1, y1, z1,
                                                                       x2, y2, z2});

                    obj->wireVertices.insert(obj->wireVertices.end(), {cx, cy, cz,
                                                                       x1, y1, z1,
                                                                       x1, y1, z1,
                                                                       x2, y2, z2,
                                                                       x2, y2, z2,
                                                                       cx, cy, cz});
                }
            }
            else
            {
                const float v0x = transVerts[0];
                const float v0y = transVerts[1];
                const float v0z = transVerts[2];

                for (int j = 1; j < numPoints - 1; j++)
                {
                    const float x1 = transVerts[j * 3 + 0];
                    const float y1 = transVerts[j * 3 + 1];
                    const float z1 = transVerts[j * 3 + 2];
                    const float x2 = transVerts[(j + 1) * 3 + 0];
                    const float y2 = transVerts[(j + 1) * 3 + 1];
                    const float z2 = transVerts[(j + 1) * 3 + 2];

                    obj->fillVertices.insert(obj->fillVertices.end(), {v0x, v0y, v0z,
                                                                       x1, y1, z1,
                                                                       x2, y2, z2});

                    obj->wireVertices.insert(obj->wireVertices.end(), {v0x, v0y, v0z,
                                                                       x1, y1, z1,
                                                                       x1, y1, z1,
                                                                       x2, y2, z2,
                                                                       x2, y2, z2,
                                                                       v0x, v0y, v0z});
                }
            }
        }

        vector<float> &activeVertices = WF ? obj->wireVertices : obj->fillVertices;

        glBufferData(GL_ARRAY_BUFFER, activeVertices.size() * sizeof(float),
                     activeVertices.data(), GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        Vec3 c = obj->selected ? obj->pastelColor : obj->baseColor;
        glUniform3f(clrLOC, c[0], c[1], c[2]);

        if (WF)
        {
            glLineWidth(2.0f);
            glDrawArrays(GL_LINES, 0, (GLsizei)(obj->wireVertices.size() / 3));
            glLineWidth(1.0f);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(obj->fillVertices.size() / 3));
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
{   // VLOER
    golfObject *concrete = new golfObject();
    concrete->id = "concrete_floor";
    concrete->type = obj_DECOR;
    concrete->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 1.95f, 2.90f);
    concrete->baseColor = Vec3({0.70f, 0.70f, 0.70f});
    concrete->pastelColor = toPastel(concrete->baseColor);
    objects.push_back(concrete);
    // BUITE GROND
    golfObject *border = new golfObject();
    border->id = "course_border";
    border->type = obj_DECOR;
    border->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 1.80f, 2.65f);
    border->baseColor = Vec3({0.28f, 0.17f, 0.09f});
    border->pastelColor = toPastel(border->baseColor);
    objects.push_back(border);
    // GRAS
    golfObject *grass = new golfObject();
    grass->id = "grass_patch";
    grass->type = obj_DECOR;
    grass->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 1.65f, 2.45f);
    grass->baseColor = Vec3({0.20f, 0.62f, 0.26f});
    grass->pastelColor = toPastel(grass->baseColor);
    objects.push_back(grass);

    // BEGIN AREA
    golfObject *start = new golfObject();
    start->id = "start_area";
    start->type = obj_DECOR;
    start->shape = new Square<3>(Vector<3>({-0.72f, -0.72f, 0.0f}), 0.16f, 0.22f);
    start->baseColor = Vec3({0.50f, 0.12f, 0.18f});
    start->pastelColor = toPastel(start->baseColor);
    objects.push_back(start);

    // RIVIER
    golfObject *river = new golfObject();
    river->id = "river";
    river->type = obj_DECOR;
    river->shape = new Square<3>(Vector<3>({0.0f, 0.0f, 0.0f}), 0.20f, 2.45f);
    river->baseColor = Vec3({0.15f, 0.42f, 0.86f});
    river->pastelColor = toPastel(river->baseColor);
    objects.push_back(river);

    // MUUR
    golfObject *wallLeft = new golfObject();
    wallLeft->id = "wall_left";
    wallLeft->type = obj_DECOR;
    wallLeft->shape = new Square<3>(Vector<3>({-0.42f, -0.21f, 0.0f}), 1.24f, 0.07f);
    wallLeft->baseColor = Vec3({0.28f, 0.17f, 0.09f});
    wallLeft->pastelColor = toPastel(wallLeft->baseColor);
    objects.push_back(wallLeft);

    // MUUR
    golfObject *wallRight = new golfObject();
    wallRight->id = "wall_right";
    wallRight->type = obj_DECOR;
    wallRight->shape = new Square<3>(Vector<3>({0.42f, 0.21f, 0.0f}), 1.24f, 0.07f);
    wallRight->baseColor = Vec3({0.28f, 0.17f, 0.09f});
    wallRight->pastelColor = toPastel(wallRight->baseColor);
    objects.push_back(wallRight);

    // BRUG
    golfObject *bridge1 = new golfObject();
    bridge1->id = "bridge_1";
    bridge1->type = obj_DECOR;
    bridge1->shape = new Square<3>(Vector<3>({-0.9f, 0.0f, 0.0f}), 0.20f, 0.1f);
    bridge1->baseColor = Vec3({0.72f, 0.52f, 0.30f});
    bridge1->pastelColor = toPastel(bridge1->baseColor);
    objects.push_back(bridge1);

    // BRUG
    golfObject *bridge2 = new golfObject();
    bridge2->id = "bridge_2";
    bridge2->type = obj_DECOR;
    bridge2->shape = new Square<3>(Vector<3>({0.00f, 0.0f, 0.0f}), 0.20f, 0.21f);
    bridge2->baseColor = Vec3({0.72f, 0.52f, 0.30f});
    bridge2->pastelColor = toPastel(bridge2->baseColor);
    objects.push_back(bridge2);

    // BRUG
    golfObject *bridgeSkew = new golfObject();
    bridgeSkew->id = "bridge_skew";
    bridgeSkew->type = obj_DECOR;
    bridgeSkew->shape = new Square<3>(Vector<3>({0.8f, 0.0f, 0.0f}), 0.20f, 0.05f);
    bridgeSkew->baseColor = Vec3({0.70f, 0.50f, 0.28f});
    bridgeSkew->pastelColor = toPastel(bridgeSkew->baseColor);
    objects.push_back(bridgeSkew);

    // G BALL
    golfObject *ball = new golfObject();
    ball->id = "ball";
    ball->type = obj_BALL;
    ball->shape = new Circle<3>(Vector<3>({-0.72f, -0.72f, 0.0f}), 0.025f, 60);
    ball->baseColor = Vec3({1.0f, 1.0f, 1.0f});
    ball->pastelColor = Vec3({1.0f, 0.85f, 0.35f});
    objects.push_back(ball);

    // GOLF HOLE
    golfObject *hole = new golfObject();
    hole->id = "hole";
    hole->type = obj_HOLE;
    hole->shape = new Circle<3>(Vector<3>({0.72f, 0.72f, 0.0f}), 0.035f, 60);
    hole->baseColor = Vec3({0.10f, 0.10f, 0.10f});
    hole->pastelColor = toPastel(hole->baseColor);
    objects.push_back(hole);

    golfObject *obsA1 = new golfObject();
    obsA1->id = "obstacle_a_1";
    obsA1->type = obj_OBSTACLE_A;
    obsA1->shape = new Square<3>(Vector<3>({0.8f, -0.45f, 0.0f}), 0.12f, 0.12f);
    obsA1->baseColor = Vec3({0.62f, 0.40f, 0.22f});
    obsA1->pastelColor = toPastel(obsA1->baseColor);
    objects.push_back(obsA1);

    golfObject *obsA3 = new golfObject();
    obsA3->id = "obstacle_a_3";
    obsA3->type = obj_OBSTACLE_A;
    obsA3->shape = new Square<3>(Vector<3>({0.6f, -0.45f, 0.0f}), 0.12f, 0.12f);
    obsA3->baseColor = Vec3({0.62f, 0.40f, 0.22f});
    obsA3->pastelColor = toPastel(obsA3->baseColor);
    objects.push_back(obsA3);

    golfObject *obsA2 = new golfObject();
    obsA2->id = "obstacle_a_2";
    obsA2->type = obj_OBSTACLE_A;
    obsA2->shape = new Square<3>(Vector<3>({1.0f, -0.45f, 0.0f}), 0.12f, 0.12f);
    obsA2->baseColor = Vec3({0.62f, 0.40f, 0.22f});
    obsA2->pastelColor = toPastel(obsA2->baseColor);
    objects.push_back(obsA2);

    golfObject *obsB1 = new golfObject();
    obsB1->id = "obstacle_b_1";
    obsB1->type = obj_OBSTACLE_B;
    obsB1->shape = new Triangle<3>(
        Vector<3>({-0.24f, -0.19f, 0.0f}),
        Vector<3>({-0.20f, -0.25f, 0.0f}),
        Vector<3>({-0.28f, -0.25f, 0.0f}));
    obsB1->baseColor = Vec3({0.92f, 0.46f, 0.14f});
    obsB1->pastelColor = toPastel(obsB1->baseColor);
    objects.push_back(obsB1);

    golfObject *obsB2 = new golfObject();
    obsB2->id = "obstacle_b_2";
    obsB2->type = obj_OBSTACLE_B;
    obsB2->shape = new Triangle<3>(
        Vector<3>({-0.10f, -0.17f, 0.0f}),
        Vector<3>({-0.06f, -0.22f, 0.0f}),
        Vector<3>({-0.14f, -0.22f, 0.0f}));
    obsB2->baseColor = Vec3({0.92f, 0.46f, 0.14f});
    obsB2->pastelColor = toPastel(obsB2->baseColor);
    objects.push_back(obsB2);

    golfObject *obsB3 = new golfObject();
    obsB3->id = "obstacle_b_3";
    obsB3->type = obj_OBSTACLE_B;
    obsB3->shape = new Triangle<3>(
        Vector<3>({0.03f, -0.20f, 0.0f}),
        Vector<3>({0.07f, -0.27f, 0.0f}),
        Vector<3>({-0.01f, -0.27f, 0.0f}));
    obsB3->baseColor = Vec3({0.92f, 0.46f, 0.14f});
    obsB3->pastelColor = toPastel(obsB3->baseColor);
    objects.push_back(obsB3);

    golfObject *obsB4 = new golfObject();
    obsB4->id = "obstacle_b_4";
    obsB4->type = obj_OBSTACLE_B;
    obsB4->shape = new Triangle<3>(
        Vector<3>({0.18f, -0.18f, 0.0f}),
        Vector<3>({0.22f, -0.24f, 0.0f}),
        Vector<3>({0.14f, -0.24f, 0.0f}));
    obsB4->baseColor = Vec3({0.92f, 0.46f, 0.14f});
    obsB4->pastelColor = toPastel(obsB4->baseColor);
    objects.push_back(obsB4);

    golfObject *obsB5 = new golfObject();
    obsB5->id = "obstacle_b_5";
    obsB5->type = obj_OBSTACLE_B;
    obsB5->shape = new Triangle<3>(
        Vector<3>({-0.18f, -0.33f, 0.0f}),
        Vector<3>({-0.13f, -0.40f, 0.0f}),
        Vector<3>({-0.23f, -0.40f, 0.0f}));
    obsB5->baseColor = Vec3({0.92f, 0.46f, 0.14f});
    obsB5->pastelColor = toPastel(obsB5->baseColor);
    objects.push_back(obsB5);

    golfObject *obsB6 = new golfObject();
    obsB6->id = "obstacle_b_6";
    obsB6->type = obj_OBSTACLE_B;
    obsB6->shape = new Triangle<3>(
        Vector<3>({-0.02f, -0.36f, 0.0f}),
        Vector<3>({0.02f, -0.42f, 0.0f}),
        Vector<3>({-0.06f, -0.42f, 0.0f}));
    obsB6->baseColor = Vec3({0.92f, 0.46f, 0.14f});
    obsB6->pastelColor = toPastel(obsB6->baseColor);
    objects.push_back(obsB6);

    golfObject *obsB7 = new golfObject();
    obsB7->id = "obstacle_b_7";
    obsB7->type = obj_OBSTACLE_B;
    obsB7->shape = new Triangle<3>(
        Vector<3>({0.14f, -0.34f, 0.0f}),
        Vector<3>({0.18f, -0.39f, 0.0f}),
        Vector<3>({0.10f, -0.39f, 0.0f}));
    obsB7->baseColor = Vec3({0.92f, 0.46f, 0.14f});
    obsB7->pastelColor = toPastel(obsB7->baseColor);
    objects.push_back(obsB7);

    golfObject *obsB8 = new golfObject();
    obsB8->id = "obstacle_b_8";
    obsB8->type = obj_OBSTACLE_B;
    obsB8->shape = new Triangle<3>(
        Vector<3>({-0.11f, -0.50f, 0.0f}),
        Vector<3>({-0.07f, -0.55f, 0.0f}),
        Vector<3>({-0.15f, -0.55f, 0.0f}));
    obsB8->baseColor = Vec3({0.92f, 0.46f, 0.14f});
    obsB8->pastelColor = toPastel(obsB8->baseColor);
    objects.push_back(obsB8);

    golfObject *obsB9 = new golfObject();
    obsB9->id = "obstacle_b_9";
    obsB9->type = obj_OBSTACLE_B;
    obsB9->shape = new Triangle<3>(
        Vector<3>({0.09f, -0.49f, 0.0f}),
        Vector<3>({0.13f, -0.56f, 0.0f}),
        Vector<3>({0.05f, -0.56f, 0.0f}));
    obsB9->baseColor = Vec3({0.92f, 0.46f, 0.14f});
    obsB9->pastelColor = toPastel(obsB9->baseColor);
    objects.push_back(obsB9);

    golfObject *obsC1 = new golfObject();
    obsC1->id = "obstacle_c_1";
    obsC1->type = obj_OBSTACLE_C;
    obsC1->shape = new Circle<3>(Vector<3>({-0.62f, 0.58f, 0.0f}), 0.06f, 8);
    obsC1->baseColor = Vec3({0.56f, 0.24f, 0.48f});
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
    cout << "Controls: Shift+1..5 multi-select, 0 clear selection, F5 save layout, F9 load layout." << endl;

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
    bool shouldExit = false;
    do
    {
        int fbW = 1, fbH = 1;
        glfwGetFramebufferSize(window, &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);
        float aspect = (fbH != 0) ? (float)fbW / (float)fbH : 1.0f;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen

        // Dynamic add/remove
        handleDynamicObjectKeys(window);

        // iNPUTS LOGGING
        handleSelectionKeys(window);
        handleTransformKeys(window);
        handleWireframeToggle(window);
        handleLayoutIOKeys(window);

        // Sync exact selected object to object flag
        for (size_t i = 0; i < objects.size(); i++)
        {
            golfObject *obj = objects[i];
            if (obj != NULL)
            {
                obj->selected = isObjectSelected(obj);
            }
        }

        // Draw golf course objects
        drawObjects(progID, clrLOC, aspectLOC, aspect, VAO, VBO);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        frameCount++;
        logLoopDiagnostics(window, frameCount);

        bool escPressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        bool closeRequested = glfwWindowShouldClose(window) != 0;
        shouldExit = escPressed || closeRequested;

        if (frameCount == 1)
        {
            cout << "Rendering started..." << endl;
        }

    } while (!shouldExit);

    cout << "Closing window. Rendered " << frameCount << " frames." << endl;

    // make clean ayy
    cleanupScene();
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(progID);
    glfwTerminate();

    return 0;
}
