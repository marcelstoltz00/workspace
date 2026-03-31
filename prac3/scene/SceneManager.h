#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "SceneNode.h"
#include "MeshNode.h"
#include "../math/Matrix.h"
#include <GLFW/glfw3.h>

class SceneManager {
private:
	SceneNode* rootNode;
	Matrix<4, 4> projectionMatrix;
	Matrix<4, 4> viewMatrix;

public:
	SceneManager();
	~SceneManager();

	void buildScene();
	void processInput(GLFWwindow* window, double deltaTime);
	void update();
	void draw(unsigned int shaderProgram);
};

#endif // SCENE_MANAGER_H
