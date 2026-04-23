#include "SceneManager.h"

SceneManager::SceneManager() : rootNode(new SceneNode()), floorMesh(nullptr), ballMesh(nullptr), lightMarker(nullptr), isWireframeMode(false), enterPressed(false) {
    cameraPos = Vector<3>({0.0f, -4.0f, -20.0f}); 
    scenePos = Vector<3>({0.0f, 0.0f, 0.0f});
    sceneRot = Vector<3>({45.0f, 0.0f, 0.0f});
    lightPos = Vector<3>({0.0f, 0.12f, 0.0f});
}

SceneManager::~SceneManager() {
    delete rootNode;
}

void SceneManager::buildScene() {
    ShapeData cuboid = ShapeFactory::createCuboid(1.0f, 1.0f, 1.0f);
    ShapeData sphere = ShapeFactory::createSphere(1.0f, 20, 20);

    Vector<3> floorColor({0.60f, 0.60f, 0.60f});
    Vector<3> ballColor({0.35f, 0.75f, 0.85f});

    floorMesh = new MeshNode(cuboid, floorColor);
    floorMesh->setLocalTransform(
        Matrix<4,4>::translate(0.0f, -0.15f, 0.0f) *
        Matrix<4,4>::scale(6.5f, 0.1f, 6.5f)
    );
    floorMesh->setReceivesLight(true);
    rootNode->addChild(floorMesh);

    float ballRadius = 0.12f;
    ballMesh = new MeshNode(sphere, ballColor);
    ballMesh->setLocalTransform(
        Matrix<4,4>::translate(0.0f, ballRadius, 0.0f) *
        Matrix<4,4>::scale(ballRadius, ballRadius, ballRadius)
    );
    ballMesh->setOpacity(0.35f);
    rootNode->addChild(ballMesh);

    ShapeData lightSphere = ShapeFactory::createSphere(1.0f, 12, 12);
    lightMarker = new MeshNode(lightSphere, Vector<3>({1.0f, 0.95f, 0.4f}));
    lightMarker->setLocalTransform(
        Matrix<4,4>::translate(lightPos[0], lightPos[1], lightPos[2]) *
        Matrix<4,4>::scale(0.03f, 0.03f, 0.03f)
    );
    rootNode->addChild(lightMarker);

    projectionMatrix = Matrix<4,4>::perspective(45.0f, 1.0f, 0.1f, 100.0f);
}

void SceneManager::processInput(GLFWwindow* window, double deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
    float speed = 4.0f;
    float rotSpeed = 35.0f;

    // 4.5.1 Rotations (W/S/A/D/Q/E)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) sceneRot[0] += rotSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) sceneRot[0] -= rotSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) sceneRot[1] += rotSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) sceneRot[1] -= rotSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) sceneRot[2] += rotSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) sceneRot[2] -= rotSpeed * deltaTime;

    // 4.5.2 Translation (light local movement)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) lightPos[1] += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) lightPos[1] -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lightPos[0] -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lightPos[0] += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) lightPos[2] += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) lightPos[2] -= speed * deltaTime;

    // Wireframe toggle (ENTER)
    bool isEnterDown = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
    if (isEnterDown && !enterPressed) {
        isWireframeMode = !isWireframeMode;
    }
    enterPressed = isEnterDown;

    // Reset all scene controls to baseline state.
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        scenePos = Vector<3>({0.0f, 0.0f, 0.0f});
        sceneRot = Vector<3>({45.0f, 0.0f, 0.0f});
        lightPos = Vector<3>({0.0f, 0.12f, 0.0f});
    }
}

void SceneManager::update() {
    if (lightMarker) {
        lightMarker->setLocalTransform(
            Matrix<4,4>::translate(lightPos[0], lightPos[1], lightPos[2]) *
            Matrix<4,4>::scale(0.03f, 0.03f, 0.03f)
        );
    }

    viewMatrix = Matrix<4,4>::translate(cameraPos[0], cameraPos[1], cameraPos[2]);

    Matrix<4,4> sceneTransform = 
                 Matrix<4,4>::translate(scenePos[0], scenePos[1], scenePos[2]) *
                 Matrix<4,4>::rotateX(sceneRot[0]) *
                 Matrix<4,4>::rotateY(sceneRot[1]) *
                 Matrix<4,4>::rotateZ(sceneRot[2]);

    sceneViewMatrix = viewMatrix * sceneTransform;
    rootNode->update(sceneViewMatrix);
}

void SceneManager::draw(unsigned int shaderProgram) {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    
    int projLoc = glGetUniformLocation(shaderProgram, "projection");
    float flatProj[16];
    int k = 0;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            flatProj[k++] = projectionMatrix[i][j];
        }
    }
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, flatProj);

    float lx = lightPos[0];
    float ly = lightPos[1];
    float lz = lightPos[2];

    float lightViewX = sceneViewMatrix[0][0] * lx + sceneViewMatrix[0][1] * ly + sceneViewMatrix[0][2] * lz + sceneViewMatrix[0][3];
    float lightViewY = sceneViewMatrix[1][0] * lx + sceneViewMatrix[1][1] * ly + sceneViewMatrix[1][2] * lz + sceneViewMatrix[1][3];
    float lightViewZ = sceneViewMatrix[2][0] * lx + sceneViewMatrix[2][1] * ly + sceneViewMatrix[2][2] * lz + sceneViewMatrix[2][3];

    int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPosition");
    int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    int ambientLoc = glGetUniformLocation(shaderProgram, "ambientStrength");
    glUniform3f(lightPosLoc, lightViewX, lightViewY, lightViewZ);
    glUniform3f(lightColorLoc, 1.0f, 0.95f, 0.8f);
    glUniform1f(ambientLoc, 0.35f);

    rootNode->draw(shaderProgram, isWireframeMode);
}
