#ifndef MESH_NODE_H
#define MESH_NODE_H

#include "SceneNode.h"
#include "../geo/ShapeFactory.h"
#include "../math/Vector.h"

class MeshNode : public SceneNode {
private:
    unsigned int VAO, VBO;
    int vertexCount;
    Vector<3> baseColor;

    void setupBuffers(const ShapeData& data);

public:
    MeshNode(const ShapeData& shapeData, const Vector<3>& color);
    ~MeshNode();

    void draw(unsigned int shaderProgram, bool isWireframe) override;
    //NB
// / Inside MeshNode::draw()
// float flatMatrix[16];
// int k = 0;
// // Note: OpenGL expects Column-Major. If your matrix multiplication 
// // is Row-Major, swap the loops (j then i) to transpose it for OpenGL.
// for (int j = 0; j < 4; j++) {     // Columns
//     for (int i = 0; i < 4; i++) { // Rows
//         flatMatrix[k++] = worldTransform[i][j]; 
//     }
// }
// glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, flatMatrix);
    
};

#endif