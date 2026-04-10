#include "SceneManager.h"
#include <iostream>

SceneManager::SceneManager() : rootNode(new SceneNode()), isWireframeMode(false), enterPressed(false), rotorAngle(0.0f), rotorSpeed(0.0f) {
    cameraPos = Vector<3>({0.0f, -10.0f, -20.0f}); 
    scenePos = Vector<3>({0.0f, 0.0f, 0.0f});
    sceneRot = Vector<3>({35.0f, 0.0f, 0.0f});
}

SceneManager::~SceneManager() {
    delete rootNode;
}

void SceneManager::buildScene() {
    ShapeData cuboid = ShapeFactory::createCuboid(1.0f, 1.0f, 1.0f);
    
    float S = 6.0f; 

    Vector<3> colConcrete({0.70f, 0.70f, 0.70f});
    Vector<3> colBorder({0.28f, 0.17f, 0.09f});
    Vector<3> colGrass({0.20f, 0.62f, 0.26f});
    Vector<3> colWall({0.28f, 0.17f, 0.09f});
    Vector<3> colMetal({0.75f, 0.75f, 0.75f});
    Vector<3> colLeaf({0.0f, 0.40f, 0.0f});  

    ShapeData coneS = ShapeFactory::createCone(0.12f, 0.45f, 32); 
    ShapeData potS = ShapeFactory::createCylinder(0.10f, 0.15f, 32);
    for (int i = 0; i < 2; i++) {
        SceneNode* plantNode = new SceneNode();
        plantNode->setLocalTransform(Matrix<4,4>::translate(-1.0f - i, 0.0f, -4.0f));
        rootNode->addChild(plantNode);
        
        MeshNode* plantPot = new MeshNode(potS, colBorder);
        plantPot->setLocalTransform(Matrix<4,4>::translate(0, 0.075f, 0) * Matrix<4,4>::rotateX(90.0f));
        plantNode->addChild(plantPot);
        
        MeshNode* plantLeaf = new MeshNode(coneS, colLeaf);
        plantLeaf->setLocalTransform(Matrix<4,4>::translate(0, 0.35f, 0) * Matrix<4,4>::rotateX(90.0f));
        plantNode->addChild(plantLeaf);
    }

    ShapeData rampS = ShapeFactory::createTriangularPrism(0.8f, 0.4f, 0.6f);
    MeshNode* tRamp = new MeshNode(rampS, colConcrete);
    tRamp->setLocalTransform(Matrix<4,4>::translate(2.5f, 0.20f, -3.0f));
    rootNode->addChild(tRamp);

    MeshNode* concrete = new MeshNode(cuboid, colConcrete);
    concrete->setLocalTransform(Matrix<4,4>::translate(0.0f, -0.15f, 0.0f) * Matrix<4,4>::scale(2.90f * S, 0.1f, 1.95f * S));
    rootNode->addChild(concrete);

    float gx = 2.45f * S; // Grass Patch Width
    float gz = 1.65f * S; // Grass Patch Depth
    float wt = 0.08f * S; // Thinner walls
    float wh = 0.45f;      // Shorter walls (low profile)

    MeshNode* wallBack = new MeshNode(cuboid, colBorder);
    wallBack->setLocalTransform(Matrix<4,4>::translate(0.0f, wh/2, (gz + wt)/2) * Matrix<4,4>::scale(gx + wt*2, wh, wt));
    rootNode->addChild(wallBack);

    MeshNode* wallFront = new MeshNode(cuboid, colBorder);
    wallFront->setLocalTransform(Matrix<4,4>::translate(0.0f, wh/2, -(gz + wt)/2) * Matrix<4,4>::scale(gx + wt*2, wh, wt));
    rootNode->addChild(wallFront);

    MeshNode* wallRightBound = new MeshNode(cuboid, colBorder);
    wallRightBound->setLocalTransform(Matrix<4,4>::translate((gx + wt)/2, wh/2, 0.0f) * Matrix<4,4>::scale(wt, wh, gz));
    rootNode->addChild(wallRightBound);

    MeshNode* wallLeftBound = new MeshNode(cuboid, colBorder);
    wallLeftBound->setLocalTransform(Matrix<4,4>::translate(-(gx + wt)/2, wh/2, 0.0f) * Matrix<4,4>::scale(wt, wh, gz));
    rootNode->addChild(wallLeftBound);

    float grassHalfDepth = (gz - (0.20f * S)) / 2.0f;
    float grassOffset = (gz / 2.0f) - (grassHalfDepth / 2.0f);


    MeshNode* grassBottom = new MeshNode(cuboid, colGrass);
    grassBottom->setLocalTransform(Matrix<4,4>::translate(0.0f, -0.05f, -grassOffset) * Matrix<4,4>::scale(gx, 0.1f, grassHalfDepth));
    rootNode->addChild(grassBottom);

    float hX = 0.72f * S;
    float hZ = 0.72f * S;
    float hSize = 0.15f * S; // Size of the cutout
    float tZ = grassOffset;  // Positive Z center

    float gLeftWidth = (gx/2.0f) + (hX - hSize/2.0f);
    MeshNode* grassT_Left = new MeshNode(cuboid, colGrass);
    grassT_Left->setLocalTransform(Matrix<4,4>::translate(-(gx/2.0f - gLeftWidth/2.0f), -0.05f, tZ) * Matrix<4,4>::scale(gLeftWidth, 0.1f, grassHalfDepth));
    rootNode->addChild(grassT_Left);

    float gRightWidth = (gx/2.0f) - (hX + hSize/2.0f);
    MeshNode* grassT_Right = new MeshNode(cuboid, colGrass);
    grassT_Right->setLocalTransform(Matrix<4,4>::translate(gx/2.0f - gRightWidth/2.0f, -0.05f, tZ) * Matrix<4,4>::scale(gRightWidth, 0.1f, grassHalfDepth));
    rootNode->addChild(grassT_Right);

    float frontSlotStart = tZ - grassHalfDepth/2.0f;
    float frontSlotEnd = hZ - hSize/2.0f;
    float frontSlotDepth = frontSlotEnd - frontSlotStart;
    MeshNode* grassT_Front = new MeshNode(cuboid, colGrass);
    grassT_Front->setLocalTransform(Matrix<4,4>::translate(hX, -0.05f, frontSlotStart + frontSlotDepth/2.0f) * Matrix<4,4>::scale(hSize, 0.1f, frontSlotDepth));
    rootNode->addChild(grassT_Front);

    float backSlotStart = hZ + hSize/2.0f;
    float backSlotEnd = tZ + grassHalfDepth/2.0f;
    float backSlotDepth = backSlotEnd - backSlotStart;
    MeshNode* grassT_Back = new MeshNode(cuboid, colGrass);
    grassT_Back->setLocalTransform(Matrix<4,4>::translate(hX, -0.05f, backSlotStart + backSlotDepth/2.0f) * Matrix<4,4>::scale(hSize, 0.1f, backSlotDepth));
    rootNode->addChild(grassT_Back);

    Vector<3> colRiver({0.15f, 0.42f, 0.86f});
    MeshNode* river = new MeshNode(cuboid, colRiver);
    river->setLocalTransform(Matrix<4,4>::translate(0.0f, -0.10f, 0.0f) * Matrix<4,4>::scale(gx, 0.1f, 0.20f * S));
    rootNode->addChild(river);

    Vector<3> colBridge({0.72f, 0.52f, 0.30f});
    MeshNode* bridge1 = new MeshNode(cuboid, colBridge);
    bridge1->setLocalTransform(Matrix<4,4>::translate(-0.90f * S, 0.06f, 0.0f) * Matrix<4,4>::scale(0.10f * S, 0.12f, 0.30f * S));
    rootNode->addChild(bridge1);

    MeshNode* bridge2 = new MeshNode(cuboid, colBridge);
    bridge2->setLocalTransform(Matrix<4,4>::translate(0.00f * S, 0.06f, 0.0f) * Matrix<4,4>::scale(0.21f * S, 0.12f, 0.30f * S));
    rootNode->addChild(bridge2);

    MeshNode* bridge3 = new MeshNode(cuboid, colBridge);
    bridge3->setLocalTransform(Matrix<4,4>::translate(0.80f * S, 0.06f, 0.0f) * Matrix<4,4>::scale(0.05f * S, 0.12f, 0.30f * S));
    rootNode->addChild(bridge3);

    Vector<3> colStart({0.50f, 0.12f, 0.18f});
    MeshNode* startMat = new MeshNode(cuboid, colStart);
    startMat->setLocalTransform(Matrix<4,4>::translate(-0.72f * S, -0.045f, -0.72f * S) * Matrix<4,4>::scale(0.22f * S, 0.11f, 0.16f * S));
    rootNode->addChild(startMat);

    ShapeData sphere = ShapeFactory::createSphere(1.0f, 20, 20);
    ShapeData openCyl = ShapeFactory::createOpenCylinder(1.0f, 1.0f, 64);
    
    float ballRadius = 0.12f;
    MeshNode* ball = new MeshNode(sphere, Vector<3>({1.0f, 1.0f, 1.0f}));
    ball->setLocalTransform(Matrix<4,4>::translate(-0.72f * S, ballRadius, -0.72f * S) * Matrix<4,4>::scale(ballRadius, ballRadius, ballRadius));
    rootNode->addChild(ball);

    MeshNode* hole = new MeshNode(openCyl, Vector<3>({0.05f, 0.05f, 0.05f}));
    float hRadius = 0.035f * S;
    hole->setLocalTransform(Matrix<4,4>::translate(hX, -0.15f, hZ) * Matrix<4,4>::rotateX(90.0f) * Matrix<4,4>::scale(hRadius, hRadius, 0.3f));
    rootNode->addChild(hole);

    ShapeData cone = ShapeFactory::createCone(1.0f, 2.0f, 32);
    ShapeData trunkCyl = ShapeFactory::createCylinder(1.0f, 1.0f, 16);
    Vector<3> colTrunk({0.4f, 0.2f, 0.1f});

    float treeX1 = -1.35f * S; 
    MeshNode* trunk1 = new MeshNode(trunkCyl, colTrunk);
    trunk1->setLocalTransform(Matrix<4,4>::translate(treeX1, 0.5f, -0.50f * S) * Matrix<4,4>::rotateX(90.0f) * Matrix<4,4>::scale(0.2f, 0.2f, 1.0f));
    rootNode->addChild(trunk1);
    MeshNode* leaf1 = new MeshNode(cone, colLeaf);
    leaf1->setLocalTransform(Matrix<4,4>::translate(treeX1, 1.5f, -0.50f * S) * Matrix<4,4>::rotateX(90.0f) * Matrix<4,4>::scale(0.8f, 0.8f, 1.0f));
    rootNode->addChild(leaf1);

    float treeX2 = 1.35f * S;
    MeshNode* trunk2 = new MeshNode(trunkCyl, colTrunk);
    trunk2->setLocalTransform(Matrix<4,4>::translate(treeX2, 0.5f, 0.50f * S) * Matrix<4,4>::rotateX(90.0f) * Matrix<4,4>::scale(0.2f, 0.2f, 1.0f));
    rootNode->addChild(trunk2);
    MeshNode* leaf2 = new MeshNode(cone, colLeaf);
    leaf2->setLocalTransform(Matrix<4,4>::translate(treeX2, 1.5f, 0.50f * S) * Matrix<4,4>::rotateX(90.0f) * Matrix<4,4>::scale(0.8f, 0.8f, 1.0f));
    rootNode->addChild(leaf2);

    ShapeData cylinder = ShapeFactory::createCylinder(1.0f, 1.0f, 64);
    ShapeData prism = ShapeFactory::createTriangularPrism(1.0f, 1.0f, 1.0f);
    Vector<3> colObsA({0.62f, 0.40f, 0.22f});
    Vector<3> colObsC({0.56f, 0.24f, 0.48f});

    MeshNode* obsA = new MeshNode(cuboid, colObsA);
    obsA->setLocalTransform(Matrix<4,4>::translate(0.80f * S, 0.20f, -0.45f * S) * Matrix<4,4>::scale(0.12f * S, 0.40f, 0.12f * S));
    rootNode->addChild(obsA);

    MeshNode* obsC = new MeshNode(cylinder, colObsC);
    obsC->setLocalTransform(Matrix<4,4>::translate(-0.62f * S, 0.20f, 0.58f * S) * Matrix<4,4>::scale(0.12f * S, 0.40f, 0.12f * S));
    rootNode->addChild(obsC);

    MeshNode* wallL = new MeshNode(cuboid, colWall);
    wallL->setLocalTransform(Matrix<4,4>::translate(-0.42f * S, 0.20f, -0.21f * S) * Matrix<4,4>::scale(0.07f * S, 0.40f, 1.24f * S));
    rootNode->addChild(wallL);

    MeshNode* wallR = new MeshNode(cuboid, colWall);
    wallR->setLocalTransform(Matrix<4,4>::translate(0.42f * S, 0.20f, 0.21f * S) * Matrix<4,4>::scale(0.07f * S, 0.40f, 1.24f * S));
    rootNode->addChild(wallR);

    Vector<3> colWindmillRed({0.8f, 0.1f, 0.1f});
    Vector<3> colWhite({1.0f, 1.0f, 1.0f});
    float windmillHRadius = 0.25f;
    float windmillMRadius = 0.20f;
    float baseLayerH = 0.35f; // Thicker base layers
    float midLayerH = 0.35f;  // Thicker mid layers
    
    ShapeData hexBase = ShapeFactory::createCylinder(windmillHRadius, 0.12f, 6); // Hexagonal Base
    ShapeData hexBaseRim = ShapeFactory::createHexagonRim(windmillHRadius);
    ShapeData hexMid = ShapeFactory::createCylinder(windmillMRadius, 0.12f, 6); // Hexagonal Mid
    ShapeData hexMidRim = ShapeFactory::createHexagonRim(windmillMRadius);

    SceneNode* windmillNode = new SceneNode();
    windmillNode->setLocalTransform(Matrix<4,4>::translate(0.0f, 0.0f, 0.0f)); 
    rootNode->addChild(windmillNode);

    ShapeData pillarS = ShapeFactory::createBox(0.40f, 1.05f, 1.50f); 
    ShapeData hexBaseS = ShapeFactory::createCylinder(windmillHRadius, 0.35f, 6);
    ShapeData hexBaseRimS = ShapeFactory::createHexagonRim(windmillHRadius);

    MeshNode* pL = new MeshNode(pillarS, colWindmillRed);
    pL->setLocalTransform(Matrix<4,4>::translate(-0.55f, 0.525f, -0.35f * S));
    windmillNode->addChild(pL);
    
    MeshNode* pR = new MeshNode(pillarS, colWindmillRed);
    pR->setLocalTransform(Matrix<4,4>::translate(0.55f, 0.525f, -0.35f * S));
    windmillNode->addChild(pR);

    for (int i = 0; i < 2; i++) {
        float hY = 1.05f + (i * 0.35f) + 0.175f;
        MeshNode* hLayer = new MeshNode(hexBaseS, colWindmillRed);
        hLayer->setLocalTransform(Matrix<4,4>::translate(0.0f, hY, -0.35f * S) * 
                                  Matrix<4,4>::rotateX(-90.0f) * Matrix<4,4>::scale(1.0f * S, 1.0f * S, 1.0f));
        windmillNode->addChild(hLayer);

        MeshNode* r = new MeshNode(hexBaseRimS, colWhite);
        r->setPrimitiveType(GL_LINE_LOOP);
        r->setLocalTransform(Matrix<4,4>::translate(0, 0, 0.175f + 0.001f));
        hLayer->addChild(r);
    }

    float midStartH = 1.05f + (2 * 0.35f); 
    for (int i = 0; i < 10; i++) {
        float taper = 1.0f - (i * 0.05f);
        if (taper < 0.2f) taper = 0.2f;

        MeshNode* layer = new MeshNode(hexMid, colWindmillRed);
        layer->setLocalTransform(Matrix<4,4>::translate(0.0f, midStartH + midLayerH/2.0f + i * midLayerH, -0.35f * S) * 
                                 Matrix<4,4>::rotateX(90.0f) * Matrix<4,4>::scale(taper * S, taper * S, midLayerH/0.12f));
        windmillNode->addChild(layer);

        MeshNode* rT = new MeshNode(hexMidRim, colWhite);
        rT->setPrimitiveType(GL_LINE_LOOP);
        rT->setLocalTransform(Matrix<4,4>::translate(0, 0, (0.06f * (midLayerH/0.12f)) + 0.001f));
        layer->addChild(rT);
        MeshNode* rB = new MeshNode(hexMidRim, colWhite);
        rB->setPrimitiveType(GL_LINE_LOOP);
        rB->setLocalTransform(Matrix<4,4>::translate(0, 0, -(0.06f * (midLayerH/0.12f)) - 0.001f));
        layer->addChild(rB);
    }

    float roofElev = 1.05f + (2 * 0.35f) + (10 * 0.35f); // Pillars + 2 Hex + 10 Mid
    float roofH = 1.0f;
    float roofR = 0.15f;
    ShapeData roof = ShapeFactory::createCone(roofR, roofH, 6); // Hexagonal Roof
    ShapeData roofRim = ShapeFactory::createHexagonRim(roofR);

    MeshNode* wRoof = new MeshNode(roof, colWindmillRed);
    wRoof->setLocalTransform(Matrix<4,4>::translate(0.0f, roofElev + roofH/2.0f, -0.35f * S) * 
                             Matrix<4,4>::rotateX(-90.0f) * Matrix<4,4>::scale(1.0f * S, 1.0f * S, 1.0f));
    windmillNode->addChild(wRoof);

    MeshNode* rRim = new MeshNode(roofRim, colWhite);
    rRim->setPrimitiveType(GL_LINE_LOOP);
    rRim->setLocalTransform(Matrix<4,4>::translate(0, 0, -roofH/2.0f - 0.001f));
    wRoof->addChild(rRim);

    rotorAngle = 0.0f;
    rotorNode = new SceneNode();
    float rx = 0.00f; 
    float ry = 5.20f; // High position
    float rz = -0.50f; // Bridging the gap
    rotorNode->setLocalTransform(Matrix<4,4>::translate(rx, ry, rz));
    windmillNode->addChild(rotorNode);

    ShapeData axleS = ShapeFactory::createCylinder(0.06f, 2.0f, 32); 
    MeshNode* axle = new MeshNode(axleS, colMetal);
    axle->setLocalTransform(Matrix<4,4>::translate(0.0f, 5.20f, -1.50f));
    windmillNode->addChild(axle);

    ShapeData hS = ShapeFactory::createCylinder(0.12f, 0.10f, 16);
    MeshNode* hN = new MeshNode(hS, colWindmillRed);
    rotorNode->addChild(hN);

    ShapeData armB = ShapeFactory::createBox(0.12f, 0.60f, 0.06f); // Lengthened arm
    ShapeData bladeB = ShapeFactory::createBox(0.40f, 4.60f, 0.08f); // Resized blade

    for (int i = 0; i < 4; i++) {
        float angle = i * 90.0f;
        SceneNode* p = new SceneNode();
        p->setLocalTransform(Matrix<4,4>::rotateZ(angle));
        rotorNode->addChild(p);

        MeshNode* flatA = new MeshNode(armB, colWindmillRed);
        flatA->setLocalTransform(Matrix<4,4>::translate(0.0f, 0.42f, 0.01f)); // Shifted to hub edge
        p->addChild(flatA);

        MeshNode* b = new MeshNode(bladeB, colWhite);
        b->setLocalTransform(Matrix<4,4>::translate(0.0f, 2.80f, 0.02f) * Matrix<4,4>::rotateY(15.0f));
        p->addChild(b);
    }

    cameraPos = Vector<3>({0.0f, -10.0f, -20.0f}); 
    scenePos = Vector<3>({0.0f, 0.0f, 0.0f});
    sceneRot = Vector<3>({35.0f, 0.0f, 0.0f}); 

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

    // 4.5.2 Translation (I/K/L/J/O/U)
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) scenePos[1] += speed * deltaTime; 
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) scenePos[1] -= speed * deltaTime; 
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) scenePos[0] += speed * deltaTime; 
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) scenePos[0] -= speed * deltaTime; 
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) scenePos[2] += speed * deltaTime; 
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scenePos[2] -= speed * deltaTime; 

    // Wireframe toggle (ENTER)
    bool isEnterDown = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
    if (isEnterDown && !enterPressed) {
        isWireframeMode = !isWireframeMode;
    }
    enterPressed = isEnterDown;

    // Rotor Speed (+ and -)
    // Handle both = (for plus) and numpad plus
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        rotorSpeed += 1.5f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        rotorSpeed -= 1.5f * deltaTime;
        if (rotorSpeed < 0.0f) rotorSpeed = 0.0f; // Don't rotate backwards
    }
}

void SceneManager::update() {
    rotorAngle += rotorSpeed; 
    if (rotorNode) {
        float rX = 0.00f;
        float rY = 5.20f; // Synchronized with High Hub
        float rZ = -0.50f;
        rotorNode->setLocalTransform(Matrix<4,4>::translate(rX, rY, rZ) * Matrix<4,4>::rotateZ(rotorAngle));
    }

    viewMatrix = Matrix<4,4>::translate(cameraPos[0], cameraPos[1], cameraPos[2]);

    Matrix<4,4> sceneTransform = 
                 Matrix<4,4>::translate(scenePos[0], scenePos[1], scenePos[2]) *
                 Matrix<4,4>::rotateX(sceneRot[0]) *
                 Matrix<4,4>::rotateY(sceneRot[1]) *
                 Matrix<4,4>::rotateZ(sceneRot[2]);

    rootNode->update(viewMatrix * sceneTransform);
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

    rootNode->draw(shaderProgram, isWireframeMode);
}
