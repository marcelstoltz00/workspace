#include "ShapeFactory.h"
#include "Square.h" 
#include "Circle.h"
#include "Triangle.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include <cmath>


static void addTri(ShapeData& data, const float v1[3], const float v2[3], const float v3[3]) {
    data.vertices.push_back(v1[0]); data.vertices.push_back(v1[1]); data.vertices.push_back(v1[2]);
    data.vertices.push_back(v2[0]); data.vertices.push_back(v2[1]); data.vertices.push_back(v2[2]);
    data.vertices.push_back(v3[0]); data.vertices.push_back(v3[1]); data.vertices.push_back(v3[2]);
}

ShapeData ShapeFactory::createCuboid(float width, float height, float depth) {
    ShapeData data;
    
    // Front face (Z = depth/2)
    float cFront[] = {0.0f, 0.0f, depth/2.0f};
    Square<3> front(Vector<3>(cFront), height, width);
    float* fPts = front.getPoints();
    
    // Back face (Z = -depth/2)
    float cBack[] = {0.0f, 0.0f, -depth/2.0f};
    Square<3> back(Vector<3>(cBack), height, width);
    float* bPts = back.getPoints();
    
    
    addTri(data, fPts, fPts+9, fPts+6);
    addTri(data, fPts, fPts+6, fPts+3);
    
    addTri(data, bPts+3, bPts+6, bPts+9);
    addTri(data, bPts+3, bPts+9, bPts);
    
    addTri(data, fPts, bPts, bPts+9);
    addTri(data, fPts, bPts+9, fPts+9);
    
    addTri(data, bPts+3, fPts+3, fPts+6);
    addTri(data, bPts+3, fPts+6, bPts+6);
    
    addTri(data, bPts, bPts+3, fPts+3);
    addTri(data, bPts, fPts+3, fPts);
    
    addTri(data, fPts+9, fPts+6, bPts+6);
    addTri(data, fPts+9, bPts+6, bPts+9);
    
    delete[] fPts;
    delete[] bPts;
    return data;
}

ShapeData ShapeFactory::createGridPlane(float width, float depth, int xSegments, int zSegments) {
    ShapeData data;
    if (xSegments < 1) xSegments = 1;
    if (zSegments < 1) zSegments = 1;

    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;
    float cellWidth = width / static_cast<float>(xSegments);
    float cellDepth = depth / static_cast<float>(zSegments);

    for (int x = 0; x < xSegments; ++x) {
        for (int z = 0; z < zSegments; ++z) {
            float x0 = -halfWidth + x * cellWidth;
            float x1 = x0 + cellWidth;
            float z0 = -halfDepth + z * cellDepth;
            float z1 = z0 + cellDepth;

            float v0[] = {x0, 0.0f, z0};
            float v1[] = {x1, 0.0f, z0};
            float v2[] = {x1, 0.0f, z1};
            float v3[] = {x0, 0.0f, z1};

            addTri(data, v0, v1, v2);
            addTri(data, v0, v2, v3);
        }
    }

    return data;
}

ShapeData ShapeFactory::createCylinder(float radius, float height, int slices) {
    ShapeData data;
    if (slices < 3) slices = 3;
    
    float cTop[] = {0.0f, 0.0f, height/2.0f};
    float cBot[] = {0.0f, 0.0f, -height/2.0f};
    Circle<3> top(Vector<3>(cTop), radius, slices);
    Circle<3> bot(Vector<3>(cBot), radius, slices);
    
    float* tPts = top.getPoints();
    float* bPts = bot.getPoints();
    
    for (int i = 0; i < slices; i++) {
        int next = (i + 1) % slices;
        
        float* t1 = tPts + i * 3;
        float* t2 = tPts + next * 3;
        float* b1 = bPts + i * 3;
        float* b2 = bPts + next * 3;
        
        addTri(data, cTop, t1, t2);
        
        addTri(data, cBot, b2, b1);
        
        addTri(data, t1, b1, b2);
        addTri(data, t1, b2, t2);
    }
    
    delete[] tPts;
    delete[] bPts;
    return data;
}

ShapeData ShapeFactory::createCone(float radius, float height, int slices) {
    ShapeData data;
    if (slices < 3) slices = 3;
    
    float cTop[] = {0.0f, 0.0f, height/2.0f};
    float cBot[] = {0.0f, 0.0f, -height/2.0f};
    Circle<3> bot(Vector<3>(cBot), radius, slices);
    float* bPts = bot.getPoints();
    
    for (int i = 0; i < slices; i++) {
        int next = (i + 1) % slices;
        float* b1 = bPts + i * 3;
        float* b2 = bPts + next * 3;
        
        addTri(data, cBot, b2, b1);
        
        addTri(data, b1, b2, cTop);
    }
    
    delete[] bPts;
    return data;
}

ShapeData ShapeFactory::createTriangularPrism(float base, float height, float depth) {
    ShapeData data;
    
    float fZ = depth / 2.0f;
    float bZ = -depth / 2.0f;
    
    float p1[] = {-base/2.0f, -height/2.0f, fZ};
    float p2[] = {base/2.0f, -height/2.0f, fZ};
    float p3[] = {0.0f, height/2.0f, fZ};
    
    float bp1[] = {-base/2.0f, -height/2.0f, bZ};
    float bp2[] = {base/2.0f, -height/2.0f, bZ};
    float bp3[] = {0.0f, height/2.0f, bZ};
    
    Triangle<3> front{Vector<3>(p1), Vector<3>(p2), Vector<3>(p3)};
    Triangle<3> back{Vector<3>(bp1), Vector<3>(bp2), Vector<3>(bp3)};
    
    float* fPts = front.getPoints();
    float* bPts = back.getPoints();
    
    // Front face
    addTri(data, fPts, fPts+3, fPts+6);
    
    // Back face (reversed)
    addTri(data, bPts, bPts+6, bPts+3);
    
    // Bottom face
    addTri(data, fPts, bPts, bPts+3);
    addTri(data, fPts, bPts+3, fPts+3);
    
    // Right face
    addTri(data, fPts+3, bPts+3, bPts+6);
    addTri(data, fPts+3, bPts+6, fPts+6);
    
    // Left face
    addTri(data, fPts+6, bPts+6, bPts);
    addTri(data, fPts+6, bPts, fPts);
    
    delete[] fPts;
    delete[] bPts;
    return data;
}

ShapeData ShapeFactory::createSphere(float radius, int lats, int longs) {
    ShapeData data;
    if (lats < 3) lats = 3;
    if (longs < 3) longs = 3;
    for (int i = 0; i < lats; i++) {
        float phi1 = M_PI * float(i) / float(lats);
        float phi2 = M_PI * float(i + 1) / float(lats);
        for (int j = 0; j < longs; j++) {
            float theta1 = 2.0f * M_PI * float(j) / float(longs);
            float theta2 = 2.0f * M_PI * float(j + 1) / float(longs);

            float x1 = radius * std::sin(phi1) * std::cos(theta1);
            float y1 = radius * std::cos(phi1);
            float z1 = radius * std::sin(phi1) * std::sin(theta1);

            float x2 = radius * std::sin(phi2) * std::cos(theta1);
            float y2 = radius * std::cos(phi2);
            float z2 = radius * std::sin(phi2) * std::sin(theta1);

            float x3 = radius * std::sin(phi1) * std::cos(theta2);
            float y3 = radius * std::cos(phi1);
            float z3 = radius * std::sin(phi1) * std::sin(theta2);

            float x4 = radius * std::sin(phi2) * std::cos(theta2);
            float y4 = radius * std::cos(phi2);
            float z4 = radius * std::sin(phi2) * std::sin(theta2);

            float v1[] = {x1, y1, z1}, v2[] = {x2, y2, z2}, v3[] = {x3, y3, z3}, v4[] = {x4, y4, z4};
            addTri(data, v1, v2, v4);
            addTri(data, v1, v4, v3);
        }
    }
    return data;
}

ShapeData ShapeFactory::createOpenCylinder(float radius, float height, int slices) {
    ShapeData data;
    if (slices < 8) slices = 8;
    float* bPts = new float[slices * 3];
    float* fPts = new float[slices * 3];
    for (int i = 0; i < slices; i++) {
        float angle = 2.0f * M_PI * i / slices;
        bPts[i*3] = radius * std::cos(angle);
        bPts[i*3+1] = radius * std::sin(angle);
        bPts[i*3+2] = -height / 2.0f;
        fPts[i*3] = radius * std::cos(angle);
        fPts[i*3+1] = radius * std::sin(angle);
        fPts[i*3+2] = height / 2.0f;
    }
    float cBot[] = {0.0f, 0.0f, -height/2.0f};
    for (int i = 0; i < slices; i++) {
        int next = (i + 1) % slices;
        // Keep bottom face
        addTri(data, cBot, bPts + next*3, bPts + i*3);
        // Sides
        float p1[] = {bPts[i*3], bPts[i*3+1], bPts[i*3+2]};
        float p2[] = {bPts[next*3], bPts[next*3+1], bPts[next*3+2]};
        float p3[] = {fPts[i*3], fPts[i*3+1], fPts[i*3+2]};
        float p4[] = {fPts[next*3], fPts[next*3+1], fPts[next*3+2]};
        addTri(data, p1, p2, p4);
        addTri(data, p1, p4, p3);
    }
    delete[] fPts;
    delete[] bPts;
    return data;
}

ShapeData ShapeFactory::createHexagonRim(float radius) {
    ShapeData data;
    for (int i = 0; i < 6; i++) {
        float angle = 2.0f * M_PI * i / 6.0f;
        data.vertices.push_back(radius * std::cos(angle));
        data.vertices.push_back(radius * std::sin(angle));
        data.vertices.push_back(0.0f);
    }
    return data;
}

ShapeData ShapeFactory::createBox(float w, float h, float d) {
    ShapeData data;
    float hw = w / 2.0f;
    float hh = h / 2.0f;
    float hd = d / 2.0f;

    float p[8][3] = {
        {-hw, -hh, -hd}, {hw, -hh, -hd}, {hw, hh, -hd}, {-hw, hh, -hd},
        {-hw, -hh,  hd}, {hw, -hh,  hd}, {hw, hh,  hd}, {-hw, hh,  hd}
    };

    auto addTri = [&](int a, int b, int c) {
        for(int i : {a, b, c}) {
            data.vertices.push_back(p[i][0]);
            data.vertices.push_back(p[i][1]);
            data.vertices.push_back(p[i][2]);
        }
    };

    addTri(0, 1, 2); addTri(0, 2, 3); // Front
    addTri(5, 4, 7); addTri(5, 7, 6); // Back
    addTri(4, 0, 3); addTri(4, 3, 7); // Left
    addTri(1, 5, 6); addTri(1, 6, 2); // Right
    addTri(3, 2, 6); addTri(3, 6, 7); // Top
    addTri(4, 5, 1); addTri(4, 1, 0); // Bottom

    return data;
}
