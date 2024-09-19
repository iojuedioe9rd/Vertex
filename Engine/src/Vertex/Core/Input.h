#pragma once

#include "Vertex/Core/Base.h"
#include "Vertex/Core/KeyCodes.h"
#include "Vertex/Core/MouseCodes.h"
#include <glm/glm.hpp>

namespace Vertex {

	class VERTEX_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static glm::vec2 GetMousePositionVec2();
		static float GetMouseX();
		static float GetMouseY();
	};

}