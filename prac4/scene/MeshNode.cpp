#include "MeshNode.h"
#include <GL/glew.h>
#include <vector>

MeshNode::MeshNode(const ShapeData& shapeData, const Vector<3>& color) : hasOutline(false), receivesLight(false), opacity(1.0f), primitiveType(GL_TRIANGLES) {
    baseColor = color;
    setupBuffers(shapeData);
    setupWireBuffers(shapeData);
}

MeshNode::~MeshNode() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &wireVAO);
    glDeleteBuffers(1, &wireVBO);
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

void MeshNode::setupWireBuffers(const ShapeData& data) {
    std::vector<float> wireVertices;

    // Most shapes are uploaded as triangle lists, so we derive GL_LINES edges per triangle.
    if (primitiveType == GL_TRIANGLES) {
        for (size_t i = 0; i + 8 < data.vertices.size(); i += 9) {
            const float* v0 = &data.vertices[i];
            const float* v1 = &data.vertices[i + 3];
            const float* v2 = &data.vertices[i + 6];

            // Edge v0-v1
            wireVertices.insert(wireVertices.end(), {v0[0], v0[1], v0[2], v1[0], v1[1], v1[2]});
            // Edge v1-v2
            wireVertices.insert(wireVertices.end(), {v1[0], v1[1], v1[2], v2[0], v2[1], v2[2]});
            // Edge v2-v0
            wireVertices.insert(wireVertices.end(), {v2[0], v2[1], v2[2], v0[0], v0[1], v0[2]});
        }
    } else {
        wireVertices = data.vertices;
    }

    wireVertexCount = wireVertices.size() / 3;

    glGenVertexArrays(1, &wireVAO);
    glGenBuffers(1, &wireVBO);

    glBindVertexArray(wireVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wireVBO);
    glBufferData(GL_ARRAY_BUFFER, wireVertices.size() * sizeof(float), wireVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void MeshNode::draw(unsigned int shaderProgram, bool isWireframe) {
    int matrixLocation = glGetUniformLocation(shaderProgram, "model");
    int colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
    int receiveLightLocation = glGetUniformLocation(shaderProgram, "receivesLight");
    int alphaLocation = glGetUniformLocation(shaderProgram, "objectAlpha");
    
    float flatMatrix[16];
    int k = 0;
    for (int j = 0; j < 4; j++) {  
        for (int i = 0; i < 4; i++) { 
            flatMatrix[k++] = worldTransform[i][j]; 
        }
    }
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, flatMatrix);
    glUniform1i(receiveLightLocation, receivesLight ? 1 : 0);
    glUniform1f(alphaLocation, opacity);

    bool translucentFill = (!isWireframe && opacity < 0.999f);
    if (translucentFill) {
        glDepthMask(GL_FALSE);
    }
    
    if (isWireframe) {
        glUniform3f(colorLocation, baseColor[0], baseColor[1], baseColor[2]);
        glBindVertexArray(wireVAO);
        glDrawArrays(GL_LINES, 0, wireVertexCount);
    } else {
        glUniform3f(colorLocation, baseColor[0], baseColor[1], baseColor[2]);
        glBindVertexArray(VAO);
        glDrawArrays(primitiveType, 0, vertexCount);
        
        if (hasOutline) {
            glUniform3f(colorLocation, outlineColor[0], outlineColor[1], outlineColor[2]);
            glLineWidth(2.0f);
            glBindVertexArray(wireVAO);
            glDrawArrays(GL_LINES, 0, wireVertexCount);
            glLineWidth(1.0f);
        }
    }
    
    glBindVertexArray(0);

    if (translucentFill) {
        glDepthMask(GL_TRUE);
    }
    
    SceneNode::draw(shaderProgram, isWireframe);
}
