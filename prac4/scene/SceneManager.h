#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "SceneNode.h"
#include "MeshNode.h"
#include "../math/Matrix.h"
#include <GLFW/glfw3.h>
#include "../math/Vector.h"
#include <vector>

class SceneManager {
private:
	SceneNode* rootNode;
	MeshNode* floorMesh;
	MeshNode* ballMesh;
	MeshNode* lightMarker;
	Vector<3> cameraPos;
	Vector<3> scenePos;
	Vector<3> sceneRot; // x, y, z rotations
	Vector<3> lightPos;
	Vector<3> initialLightPos;
	Vector<3> secondLightPos;
	Vector<3> secondLightColor;
	std::vector<Vector<3>> surfaceColorPresets;
	std::vector<Vector<3>> lightColorPresets;
	int floorColorIndex;
	int ballColorIndex;
	int lightColorIndex;
	int ballLatitudeSegments;
	int ballLongitudeSegments;
	int floorXSegments;
	int floorZSegments;
	float ballOpacity;
	Matrix<4, 4> projectionMatrix;
	Matrix<4, 4> viewMatrix;
	Matrix<4, 4> sceneViewMatrix;
	bool isWireframeMode;
	bool enterPressed;
	bool floorColorPrevPressed;
	bool floorColorNextPressed;
	bool ballColorPrevPressed;
	bool ballColorNextPressed;
	bool lightColorPrevPressed;
	bool lightColorNextPressed;
	bool ballResolutionUpPressed;
	bool ballResolutionDownPressed;
	bool floorResolutionUpPressed;
	bool floorResolutionDownPressed;
	bool colorTexturePressed;
	bool displacementTexturePressed;
	bool alphaTexturePressed;
	bool alphaUpPressed;
	bool alphaDownPressed;
	bool resetPressed;
	bool useColorTexture;
	bool useDisplacementTexture;
	bool useAlphaTexture;

	void initializeColorPresets();
	void applyCurrentColors();
	void resetSceneState();
	void cycleColorIndex(int& colorIndex, int direction, int colorCount);
	void rebuildScene();
	void clearScene();

public:
	SceneManager();
	~SceneManager();

	void buildScene();
	void processInput(GLFWwindow* window, double deltaTime);
	void update();
	void draw(unsigned int shaderProgram);
};

#endif // SCENE_MANAGER_H
