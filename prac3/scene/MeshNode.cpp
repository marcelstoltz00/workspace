#include "MeshNode.h"
#include <GL/glew.h>

MeshNode::MeshNode(const ShapeData& shapeData, const Vector<3>& color) : hasOutline(false), primitiveType(GL_TRIANGLES) {
    baseColor = color;
    setupBuffers(shapeData);
}

MeshNode::~MeshNode() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void MeshNode::setupBuffers(const ShapeData& data) {
    vertexCount = data.vertices.size() / 3;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(float), data.vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void MeshNode::draw(unsigned int shaderProgram, bool isWireframe) {
    int matrixLocation = glGetUniformLocation(shaderProgram, "model");
    int colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
    
    float flatMatrix[16];
    int k = 0;
    for (int j = 0; j < 4; j++) {  
        for (int i = 0; i < 4; i++) { 
            flatMatrix[k++] = worldTransform[i][j]; 
        }
    }
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, flatMatrix);
    
    glBindVertexArray(VAO);
    
    if (isWireframe) {
        glUniform3f(colorLocation, baseColor[0], baseColor[1], baseColor[2]);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(primitiveType, 0, vertexCount);
    } else {
        glUniform3f(colorLocation, baseColor[0], baseColor[1], baseColor[2]);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(primitiveType, 0, vertexCount);
        
        if (hasOutline) {
            glUniform3f(colorLocation, outlineColor[0], outlineColor[1], outlineColor[2]);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(2.0f);
            glDrawArrays(primitiveType, 0, vertexCount);
            glLineWidth(1.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    
    glBindVertexArray(0);
    
    SceneNode::draw(shaderProgram, isWireframe);
}
