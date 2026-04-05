#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "SceneNode.h"
#include "MeshNode.h"
#include "../math/Matrix.h"
#include <GLFW/glfw3.h>
#include "../math/Vector.h"

class SceneManager {
private:
	SceneNode* rootNode;
	SceneNode* windmillBase;
	SceneNode* rotorNode;
	float rotorAngle;
	Vector<3> cameraPos;
	Vector<3> scenePos;
	Vector<3> sceneRot; // x, y, z rotations
	Matrix<4, 4> projectionMatrix;
	Matrix<4, 4> viewMatrix;
	bool isWireframeMode;
	bool enterPressed;

public:
	SceneManager();
	~SceneManager();

	void buildScene();
	void processInput(GLFWwindow* window, double deltaTime);
	void update();
	void draw(unsigned int shaderProgram);
};

#endif // SCENE_MANAGER_H
