#ifndef MESH_NODE_H
#define MESH_NODE_H

#include "SceneNode.h"
#include "../geo/ShapeFactory.h"
#include "../math/Vector.h"

class MeshNode : public SceneNode {
private:
    unsigned int VAO, VBO;
    unsigned int shadeVBO, alphaVBO;
    unsigned int wireVAO, wireVBO;
    int vertexCount;
    int wireVertexCount;
    Vector<3> baseColor;
    Vector<3> outlineColor;
    bool hasOutline;
    bool receivesLight;
    float opacity;
    int objectKind;

    unsigned int primitiveType;

    void setupBuffers(const ShapeData& data);
    void setupWireBuffers(const ShapeData& data);

public:
    MeshNode(const ShapeData& shapeData, const Vector<3>& color);
    ~MeshNode();

    void setPrimitiveType(unsigned int type) { primitiveType = type; }
    void setBaseColor(const Vector<3>& color) { baseColor = color; }
    void setOutline(const Vector<3>& color) { outlineColor = color; hasOutline = true; }
    void setReceivesLight(bool value) { receivesLight = value; }
    void setOpacity(float value) { opacity = value; }
    void setObjectKind(int value) { objectKind = value; }

    void draw(unsigned int shaderProgram, bool isWireframe) override;
    
};

#endif
