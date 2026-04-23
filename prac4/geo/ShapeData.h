
#ifndef SHAPE_DATA_H
#define SHAPE_DATA_H

#include <vector>

struct ShapeData {
	std::vector<float> vertices; // Interleaved vertex attributes (e.g., position, normal, color)
	std::vector<unsigned int> indices; // Optional: for indexed drawing
};

#endif // SHAPE_DATA_H
