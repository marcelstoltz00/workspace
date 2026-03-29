#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <vector>
#include "../math/Matrix.h" 

class SceneNode {
protected:
    Matrix<4, 4> localTransform;
    Matrix<4, 4> worldTransform;
    std::vector<SceneNode*> children;

public:
    SceneNode() {
        localTransform = Matrix<4, 4>::identity();
        worldTransform = Matrix<4, 4>::identity();
    }
    
    virtual ~SceneNode() {
        for(auto child : children) delete child;
    }

    void addChild(SceneNode* child) {
        children.push_back(child);
    }
    
    void setLocalTransform(const Matrix<4, 4>& transform) {
        localTransform = transform;
    }

    // This is where the magic happens. Your operator* is used here!
    virtual void update(const Matrix<4, 4>& parentWorldTransform) {
        worldTransform = parentWorldTransform * localTransform; 
        
        for (auto child : children) {
            child->update(worldTransform);
        }
    }
    
    virtual void draw(unsigned int shaderProgram, bool isWireframe) {
        for (auto child : children) {
            child->draw(shaderProgram, isWireframe);
        }
    }
};

#endif