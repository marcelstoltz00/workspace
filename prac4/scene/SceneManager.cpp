#include "SceneManager.h"
#include <png.h>
#include <algorithm>
#include <cstdio>
#include <vector>

namespace {
struct CpuImage {
    int width;
    int height;
    std::vector<unsigned char> pixels;
    bool valid;

    CpuImage() : width(0), height(0), valid(false) {}
};

CpuImage loadPngImage(const char* path) {
    CpuImage image;

    FILE* fp = fopen(path, "rb");
    if (!fp) return image;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(fp);
        return image;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        fclose(fp);
        return image;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(fp);
        return image;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    image.width = static_cast<int>(png_get_image_width(png, info));
    image.height = static_cast<int>(png_get_image_height(png, info));
    png_byte colorType = png_get_color_type(png, info);
    png_byte bitDepth = png_get_bit_depth(png, info);

    if (bitDepth == 16) png_set_strip_16(png);
    if (colorType == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }
    if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);
    image.pixels.resize(image.width * image.height * 4);
    std::vector<png_bytep> rows(image.height);
    for (int y = 0; y < image.height; ++y) {
        rows[y] = reinterpret_cast<png_bytep>(&image.pixels[y * image.width * 4]);
    }
    png_read_image(png, rows.data());

    png_destroy_read_struct(&png, &info, nullptr);
    fclose(fp);
    image.valid = true;
    return image;
}

float sampleImageLuminance(const CpuImage& image, float u, float v) {
    if (!image.valid || image.width <= 0 || image.height <= 0) return 1.0f;

    while (u < 0.0f) u += 1.0f;
    while (u > 1.0f) u -= 1.0f;
    v = std::max(0.0f, std::min(1.0f, v));

    int x = static_cast<int>(u * float(image.width - 1));
    int y = static_cast<int>((1.0f - v) * float(image.height - 1));
    int index = (y * image.width + x) * 4;

    float r = image.pixels[index] / 255.0f;
    float g = image.pixels[index + 1] / 255.0f;
    float b = image.pixels[index + 2] / 255.0f;
    float a = image.pixels[index + 3] / 255.0f;
    return ((r + g + b) / 3.0f) * a;
}

void applyCpuTextureMaps(ShapeData& sphere,
                         const CpuImage& colorMap,
                         const CpuImage& displacementMap,
                         const CpuImage& alphaMap,
                         bool useColorTexture,
                         bool useDisplacementTexture,
                         bool useAlphaTexture,
                         float sharedAlpha) {
    int vertexCount = static_cast<int>(sphere.vertices.size() / 3);
    sphere.shadeFactors.assign(vertexCount, 1.0f);
    sphere.alphaFactors.assign(vertexCount, 1.0f);

    for (int i = 0; i < vertexCount; ++i) {
        float u = sphere.texCoords.empty() ? 0.0f : sphere.texCoords[i * 2];
        float v = sphere.texCoords.empty() ? 0.0f : sphere.texCoords[i * 2 + 1];

        if (useColorTexture) {
            float colorLum = sampleImageLuminance(colorMap, u, v);
            sphere.shadeFactors[i] = 0.30f + 0.70f * colorLum;
        }

        if (useDisplacementTexture) {
            float displacementLum = sampleImageLuminance(displacementMap, u, v);
            float dimpleDepth = (1.0f - displacementLum) * 0.28f;
            float scale = 1.0f - dimpleDepth;
            sphere.vertices[i * 3] *= scale;
            sphere.vertices[i * 3 + 1] *= scale;
            sphere.vertices[i * 3 + 2] *= scale;
        }

        if (useAlphaTexture) {
            float alphaLum = sampleImageLuminance(alphaMap, u, v);
            float dimpleMask = 1.0f - alphaLum;
            sphere.alphaFactors[i] = 1.0f - dimpleMask * (1.0f - sharedAlpha);
        }
    }
}
}

SceneManager::SceneManager()
    : rootNode(new SceneNode()),
      floorMesh(nullptr),
      ballMesh(nullptr),
      lightMarker(nullptr),
      floorColorIndex(3),
      ballColorIndex(3),
      lightColorIndex(4),
      ballLatitudeSegments(36),
      ballLongitudeSegments(36),
      floorXSegments(8),
      floorZSegments(8),
      ballOpacity(0.62f),
      isWireframeMode(false),
      enterPressed(false),
      floorColorPrevPressed(false),
      floorColorNextPressed(false),
      ballColorPrevPressed(false),
      ballColorNextPressed(false),
      lightColorPrevPressed(false),
      lightColorNextPressed(false),
      ballResolutionUpPressed(false),
      ballResolutionDownPressed(false),
      floorResolutionUpPressed(false),
      floorResolutionDownPressed(false),
      colorTexturePressed(false),
      displacementTexturePressed(false),
      alphaTexturePressed(false),
      alphaUpPressed(false),
      alphaDownPressed(false),
      resetPressed(false),
      useColorTexture(false),
      useDisplacementTexture(false),
      useAlphaTexture(false) {
    cameraPos = Vector<3>({0.0f, -1.15f, -5.2f});
    scenePos = Vector<3>({0.0f, 0.0f, 0.0f});
    sceneRot = Vector<3>({28.0f, 0.0f, 0.0f});
    initialLightPos = Vector<3>({0.0f, 0.12f, 0.0f});
    lightPos = initialLightPos;
    secondLightPos = Vector<3>({0.28f, 0.34f, 0.22f});
    secondLightColor = Vector<3>({0.30f, 0.55f, 1.00f});
    initializeColorPresets();
}

SceneManager::~SceneManager() {
    delete rootNode;
}

void SceneManager::buildScene() {
    clearScene();

    CpuImage colorMap = loadPngImage("scene/colour.png");
    CpuImage displacementMap = loadPngImage("scene/displace.png");
    CpuImage alphaMap = loadPngImage("scene/alpha.png");

    ShapeData floorPlane = ShapeFactory::createGridPlane(6.5f, 6.5f, floorXSegments, floorZSegments);
    ShapeData sphere = ShapeFactory::createSphere(1.0f, ballLatitudeSegments, ballLongitudeSegments);
    ShapeData lightSphere = ShapeFactory::createSphere(1.0f, 12, 12);
    applyCpuTextureMaps(sphere, colorMap, displacementMap, alphaMap, useColorTexture, useDisplacementTexture, useAlphaTexture, ballOpacity);

    floorMesh = new MeshNode(floorPlane, surfaceColorPresets[floorColorIndex]);
    floorMesh->setReceivesLight(true);
    floorMesh->setObjectKind(1);
    rootNode->addChild(floorMesh);

    float ballRadius = 0.12f;
    ballMesh = new MeshNode(sphere, surfaceColorPresets[ballColorIndex]);
    ballMesh->setLocalTransform(
        Matrix<4,4>::translate(0.0f, ballRadius, 0.0f) *
        Matrix<4,4>::scale(ballRadius, ballRadius, ballRadius)
    );
    ballMesh->setOpacity(useAlphaTexture ? 1.0f : ballOpacity);
    ballMesh->setOutline(surfaceColorPresets[ballColorIndex]);
    ballMesh->setReceivesLight(true);
    ballMesh->setObjectKind(2);

    lightMarker = new MeshNode(lightSphere, lightColorPresets[lightColorIndex]);
    lightMarker->setLocalTransform(
        Matrix<4,4>::translate(lightPos[0], lightPos[1], lightPos[2]) *
        Matrix<4,4>::scale(0.03f, 0.03f, 0.03f)
    );
    lightMarker->setObjectKind(3);

    rootNode->addChild(lightMarker);
    rootNode->addChild(ballMesh);

    projectionMatrix = Matrix<4,4>::perspective(45.0f, 1.0f, 0.1f, 100.0f);
    applyCurrentColors();
}

void SceneManager::processInput(GLFWwindow* window, double deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float speed = 4.0f;
    float rotSpeed = 35.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) sceneRot[0] += rotSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) sceneRot[0] -= rotSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) sceneRot[1] += rotSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) sceneRot[1] -= rotSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) sceneRot[2] += rotSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) sceneRot[2] -= rotSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) lightPos[1] += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) lightPos[1] -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lightPos[0] -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lightPos[0] += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) lightPos[2] += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) lightPos[2] -= speed * deltaTime;

    bool isEnterDown = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
    if (isEnterDown && !enterPressed) {
        isWireframeMode = !isWireframeMode;
    }
    enterPressed = isEnterDown;

    bool isFloorColorPrevDown = glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;
    if (isFloorColorPrevDown && !floorColorPrevPressed) {
        cycleColorIndex(floorColorIndex, -1, static_cast<int>(surfaceColorPresets.size()));
        applyCurrentColors();
    }
    floorColorPrevPressed = isFloorColorPrevDown;

    bool isFloorColorNextDown = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
    if (isFloorColorNextDown && !floorColorNextPressed) {
        cycleColorIndex(floorColorIndex, 1, static_cast<int>(surfaceColorPresets.size()));
        applyCurrentColors();
    }
    floorColorNextPressed = isFloorColorNextDown;

    bool isBallColorPrevDown = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
    if (isBallColorPrevDown && !ballColorPrevPressed) {
        cycleColorIndex(ballColorIndex, -1, static_cast<int>(surfaceColorPresets.size()));
        applyCurrentColors();
    }
    ballColorPrevPressed = isBallColorPrevDown;

    bool isBallColorNextDown = glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS;
    if (isBallColorNextDown && !ballColorNextPressed) {
        cycleColorIndex(ballColorIndex, 1, static_cast<int>(surfaceColorPresets.size()));
        applyCurrentColors();
    }
    ballColorNextPressed = isBallColorNextDown;

    bool isLightColorPrevDown = glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS;
    if (isLightColorPrevDown && !lightColorPrevPressed) {
        cycleColorIndex(lightColorIndex, -1, static_cast<int>(lightColorPresets.size()));
        applyCurrentColors();
    }
    lightColorPrevPressed = isLightColorPrevDown;

    bool isLightColorNextDown = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;
    if (isLightColorNextDown && !lightColorNextPressed) {
        cycleColorIndex(lightColorIndex, 1, static_cast<int>(lightColorPresets.size()));
        applyCurrentColors();
    }
    lightColorNextPressed = isLightColorNextDown;

    bool isBallResolutionUpDown = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;
    if (isBallResolutionUpDown && !ballResolutionUpPressed) {
        ballLatitudeSegments += 2;
        ballLongitudeSegments += 2;
        rebuildScene();
    }
    ballResolutionUpPressed = isBallResolutionUpDown;

    bool isBallResolutionDownDown = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
    if (isBallResolutionDownDown && !ballResolutionDownPressed) {
        if (ballLatitudeSegments > 6 && ballLongitudeSegments > 6) {
            ballLatitudeSegments -= 2;
            ballLongitudeSegments -= 2;
            rebuildScene();
        }
    }
    ballResolutionDownPressed = isBallResolutionDownDown;

    bool isFloorResolutionUpDown = glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS;
    if (isFloorResolutionUpDown && !floorResolutionUpPressed) {
        floorXSegments += 2;
        floorZSegments += 2;
        rebuildScene();
    }
    floorResolutionUpPressed = isFloorResolutionUpDown;

    bool isFloorResolutionDownDown = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS;
    if (isFloorResolutionDownDown && !floorResolutionDownPressed) {
        if (floorXSegments > 2 && floorZSegments > 2) {
            floorXSegments -= 2;
            floorZSegments -= 2;
            rebuildScene();
        }
    }
    floorResolutionDownPressed = isFloorResolutionDownDown;

    bool isColorTextureDown = glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS;
    if (isColorTextureDown && !colorTexturePressed) {
        useColorTexture = !useColorTexture;
        rebuildScene();
    }
    colorTexturePressed = isColorTextureDown;

    bool isDisplacementTextureDown = glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS;
    if (isDisplacementTextureDown && !displacementTexturePressed) {
        useDisplacementTexture = !useDisplacementTexture;
        rebuildScene();
    }
    displacementTexturePressed = isDisplacementTextureDown;

    bool isAlphaTextureDown = glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS;
    if (isAlphaTextureDown && !alphaTexturePressed) {
        useAlphaTexture = !useAlphaTexture;
        rebuildScene();
    }
    alphaTexturePressed = isAlphaTextureDown;

    bool isAlphaUpDown = glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS;
    if (isAlphaUpDown && !alphaUpPressed) {
        ballOpacity += 0.05f;
        if (ballOpacity > 1.0f) ballOpacity = 1.0f;
        if (useAlphaTexture) {
            rebuildScene();
        } else if (ballMesh) {
            ballMesh->setOpacity(ballOpacity);
        }
    }
    alphaUpPressed = isAlphaUpDown;

    bool isAlphaDownDown = glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS;
    if (isAlphaDownDown && !alphaDownPressed) {
        ballOpacity -= 0.05f;
        if (ballOpacity < 0.05f) ballOpacity = 0.05f;
        if (useAlphaTexture) {
            rebuildScene();
        } else if (ballMesh) {
            ballMesh->setOpacity(ballOpacity);
        }
    }
    alphaDownPressed = isAlphaDownDown;

    bool isResetDown = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    if (isResetDown && !resetPressed) {
        resetSceneState();
    }
    resetPressed = isResetDown;
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
    float slx = secondLightPos[0];
    float sly = secondLightPos[1];
    float slz = secondLightPos[2];
    float secondLightViewX = sceneViewMatrix[0][0] * slx + sceneViewMatrix[0][1] * sly + sceneViewMatrix[0][2] * slz + sceneViewMatrix[0][3];
    float secondLightViewY = sceneViewMatrix[1][0] * slx + sceneViewMatrix[1][1] * sly + sceneViewMatrix[1][2] * slz + sceneViewMatrix[1][3];
    float secondLightViewZ = sceneViewMatrix[2][0] * slx + sceneViewMatrix[2][1] * sly + sceneViewMatrix[2][2] * slz + sceneViewMatrix[2][3];

    int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPosition");
    int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    int secondLightPosLoc = glGetUniformLocation(shaderProgram, "secondLightPosition");
    int secondLightColorLoc = glGetUniformLocation(shaderProgram, "secondLightColor");
    int ballTintColorLoc = glGetUniformLocation(shaderProgram, "ballTintColor");
    int ambientLoc = glGetUniformLocation(shaderProgram, "ambientStrength");
    int ballAlphaLoc = glGetUniformLocation(shaderProgram, "ballAlpha");
    Vector<3> lightColor = lightColorPresets[lightColorIndex];
    Vector<3> ballColor = surfaceColorPresets[ballColorIndex];

    glUniform3f(lightPosLoc, lightViewX, lightViewY, lightViewZ);
    glUniform3f(lightColorLoc, lightColor[0], lightColor[1], lightColor[2]);
    glUniform3f(secondLightPosLoc, secondLightViewX, secondLightViewY, secondLightViewZ);
    glUniform3f(secondLightColorLoc, secondLightColor[0], secondLightColor[1], secondLightColor[2]);
    glUniform3f(ballTintColorLoc, ballColor[0], ballColor[1], ballColor[2]);
    glUniform1f(ambientLoc, 0.35f);
    glUniform1f(ballAlphaLoc, ballOpacity);

    rootNode->draw(shaderProgram, isWireframeMode);
}

void SceneManager::initializeColorPresets() {
    surfaceColorPresets = {
        Vector<3>({1.00f, 0.00f, 0.00f}), // Red
        Vector<3>({0.00f, 1.00f, 0.00f}), // Green
        Vector<3>({0.00f, 0.00f, 1.00f}), // Blue
        Vector<3>({1.00f, 1.00f, 1.00f}), // White
        Vector<3>({0.00f, 0.00f, 0.00f}), // Black
        Vector<3>({1.00f, 1.00f, 0.00f}), // Yellow
        Vector<3>({0.00f, 1.00f, 1.00f}), // Cyan
        Vector<3>({1.00f, 0.00f, 1.00f}), // Magenta
        Vector<3>({1.00f, 0.50f, 0.00f}), // Orange
        Vector<3>({0.60f, 0.20f, 0.80f})  // Purple
    };

    lightColorPresets = {
        Vector<3>({1.00f, 0.00f, 0.00f}), // Red
        Vector<3>({0.00f, 1.00f, 0.00f}), // Green
        Vector<3>({0.00f, 0.00f, 1.00f}), // Blue
        Vector<3>({1.00f, 1.00f, 1.00f}), // White
        Vector<3>({1.00f, 1.00f, 0.00f}), // Yellow
        Vector<3>({0.00f, 1.00f, 1.00f}), // Cyan
        Vector<3>({1.00f, 0.00f, 1.00f}), // Magenta
        Vector<3>({1.00f, 0.50f, 0.00f}), // Orange
        Vector<3>({0.60f, 0.20f, 0.80f})  // Purple
    };
}

void SceneManager::applyCurrentColors() {
    if (floorMesh) {
        floorMesh->setBaseColor(surfaceColorPresets[floorColorIndex]);
    }
    if (ballMesh) {
        ballMesh->setBaseColor(surfaceColorPresets[ballColorIndex]);
        ballMesh->setOutline(surfaceColorPresets[ballColorIndex]);
    }
    if (lightMarker) {
        lightMarker->setBaseColor(lightColorPresets[lightColorIndex]);
    }
}

void SceneManager::resetSceneState() {
    scenePos = Vector<3>({0.0f, 0.0f, 0.0f});
    sceneRot = Vector<3>({28.0f, 0.0f, 0.0f});
    lightPos = initialLightPos;
    floorColorIndex = 3;
    ballColorIndex = 3;
    lightColorIndex = 4;
    ballLatitudeSegments = 36;
    ballLongitudeSegments = 36;
    floorXSegments = 8;
    floorZSegments = 8;
    ballOpacity = 0.62f;
    useColorTexture = false;
    useDisplacementTexture = false;
    useAlphaTexture = false;
    rebuildScene();
}

void SceneManager::rebuildScene() {
    buildScene();
}

void SceneManager::clearScene() {
    delete rootNode;
    rootNode = new SceneNode();
    floorMesh = nullptr;
    ballMesh = nullptr;
    lightMarker = nullptr;
}

void SceneManager::cycleColorIndex(int& colorIndex, int direction, int colorCount) {
    colorIndex = (colorIndex + direction + colorCount) % colorCount;
}
