// RenderObject.h
// Responsibility:
// - Declares the runtime render object abstraction for one scene entity.
// - Stores render metadata (color, selection state, draw mode) and
//   transform state (position, scale, rotation, pivot).
// - Defines the interface used by main.cpp to update and draw objects.

#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

enum class RenderCategory {
	Ball,
	Hole,
	ObstacleType1,
	ObstacleType2,
	ObstacleType3,
	Decor
};

class RenderObject {
public:
	std::string id;
	RenderCategory category;

	std::vector<float> baseVertices;
	std::vector<float> transformedVertices;
	std::vector<unsigned int> lineIndices;

	glm::vec3 color;
	glm::vec3 pastelColor;

	glm::vec2 position;
	glm::vec2 scale;
	float rotationRadians;
	glm::vec2 pivot;

	bool selected;

	RenderObject();
	RenderObject(const std::string& id, RenderCategory category);

	void setBaseVertices(const std::vector<float>& vertices);
	void setLineIndices(const std::vector<unsigned int>& indices);

	void updateTransformedVertices();
	const glm::vec3& currentColor() const;
};

#endif /*RENDER_OBJECT_H*/

