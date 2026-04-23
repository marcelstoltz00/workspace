#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H

#include <vector>

struct ShapeData {
    std::vector<float> vertices; 
    std::vector<float> texCoords;
    std::vector<float> shadeFactors;
    std::vector<float> alphaFactors;
};

class ShapeFactory {
public:
    static ShapeData createCuboid(float width, float height, float depth);
    static ShapeData createGridPlane(float width, float depth, int xSegments, int zSegments);
    static ShapeData createCylinder(float radius, float height, int slices);
    static ShapeData createCone(float radius, float height, int slices);
    static ShapeData createTriangularPrism(float base, float height, float depth);
    static ShapeData createSphere(float radius, int lats, int longs);
    static ShapeData createOpenCylinder(float radius, float height, int slices);
    static ShapeData createHexagonRim(float radius);
    static ShapeData createBox(float w, float h, float d);
};

#endif
