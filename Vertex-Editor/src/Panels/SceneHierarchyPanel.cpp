#include "SceneHierarchyPanel.h"
#include "Vertex/ImGui/ImGuiLink.h"
#include <glm/gtc/type_ptr.hpp>

#define ImGuiTreeNodeFlags_Selected 1
#define ImGuiTreeNodeFlags_OpenOnArrow 128
#define ImGuiTreeNodeFlags_DefaultOpen 32


namespace Vertex {

	SceneHierarchyPanel::SceneHierarchyPanel(const Scene* scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Scene* scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (ImGuiLink::IsMouseDown(0) && ImGuiLink::IsWindowHovered())
			m_SelectionContext = nullptr;

		ImGuiLink::Begin("Scene Hierarchy");
		for (Entity* ent : *m_Context)
		{
			DrawEntityNode(ent);
		}
		ImGuiLink::End();

		ImGuiLink::Begin("Properties");
		if (m_SelectionContext)
			DrawEntity(m_SelectionContext);
		ImGuiLink::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity* entity)
	{
		std::string& tag = entity->name();

		int flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened = ImGuiLink::TreeNodeEx(entity->GetID(), flags, tag);
		if (ImGuiLink::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			int flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGuiLink::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGuiLink::TreePop();
			ImGuiLink::TreePop();
		}
	}

#pragma region DrawEntity Code

	void DrawBaseEntity(Entity* entity)
	{
		
		auto& tag = entity->name();

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tag.c_str());
		if (ImGuiLink::InputText("Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}

		if (ImGuiLink::TreeNodeEx(entity->GetID() + "Transform", ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
		{
			ImGuiLink::DragFloat3("Position", glm::value_ptr(entity->pos), 0.1f);
			ImGuiLink::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawEntity(Entity* entity)
	{
		if (entity == nullptr) return;
		DrawBaseEntity(entity);
	}
#pragma endregion


	


}