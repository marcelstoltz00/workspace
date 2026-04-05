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
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void MeshNode::draw(unsigned int shaderProgram, bool isWireframe) {
    int matrixLocation = glGetUniformLocation(shaderProgram, "model");
    int colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
    
    float flatMatrix[16];
    int k = 0;
    // OpenGL expects Column-Major. Assuming Matrix is Row-Major based on normal C++ arrays [row][col]
    for (int j = 0; j < 4; j++) {     // Columns
        for (int i = 0; i < 4; i++) { // Rows
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
        // 1. Draw solid fill
        glUniform3f(colorLocation, baseColor[0], baseColor[1], baseColor[2]);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(primitiveType, 0, vertexCount);
        
        // 2. Draw outline if requested
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
    
    // Also draw children
    SceneNode::draw(shaderProgram, isWireframe);
}
