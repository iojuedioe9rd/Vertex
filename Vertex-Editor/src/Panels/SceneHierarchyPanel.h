#pragma once

#include "Vertex/Core/Base.h"
#include "Vertex/Core/Logger.h"
#include "Vertex/Scene/Entities/Entities.h"

namespace Vertex {
	
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Scene* scene);
		void SetContext(Scene* scene);
		void OnImGuiRender();

		Entity* GetSelectedEntity() { return m_SelectionContext; }

	private:
		void DrawEntityNode(Entity* entity);
		void DrawEntity(Entity* entity);

		
	private:
		Scene* m_Context;
		bool m_CreateEntityPopup;
		bool m_CreateBehaviourPopup;
		Entity* m_SelectionContext = nullptr;
		Entity* m_EntityToRemove = nullptr;
	};

	
}