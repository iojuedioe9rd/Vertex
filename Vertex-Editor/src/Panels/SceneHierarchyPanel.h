#pragma once

#include "Vertex/Core/Base.h"
#include "Vertex/Core/Logger.h"
#include "VXEntities.h"

namespace Vertex {
	
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Scene* scene);
		void SetContext(const Scene* scene);
		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity* entity);
		void DrawEntity(Entity* entity);
	private:
		const Scene* m_Context;
		Entity* m_SelectionContext = nullptr;

	};

	
}