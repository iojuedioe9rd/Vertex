#pragma once
#include "Vertex/Core/Base.h"
#include <glm/glm.hpp>


namespace Vertex::Math {

	bool VERTEX_API DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

	glm::mat4 VERTEX_API ComposeTransform(const glm::vec3 pos, const glm::vec3 scale, const glm::vec3 rotation);

}