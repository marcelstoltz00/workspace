#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H

#include <vector>

struct ShapeData {
    std::vector<float> vertices; 
};

class ShapeFactory {
public:
    static ShapeData createCuboid(float width, float height, float depth);
    static ShapeData createCylinder(float radius, float height, int slices);
    static ShapeData createCone(float radius, float height, int slices);
    static ShapeData createTriangularPrism(float base, float height, float depth);
    static ShapeData createSphere(float radius, int lats, int longs);
    static ShapeData createOpenCylinder(float radius, float height, int slices);
    static ShapeData createHexagonRim(float radius);
    static ShapeData createBox(float w, float h, float d);
};

#endif