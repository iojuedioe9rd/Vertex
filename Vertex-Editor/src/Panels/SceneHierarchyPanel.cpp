#include "SceneHierarchyPanel.h"
#include "Vertex/ImGui/ImGuiLink.h"
#include <glm/gtc/type_ptr.hpp>
#include "VXEntities.h"

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
			ImGuiLink::DrawVec3Control("Position", entity->pos);
			
			ImGuiLink::DrawVec3Control("Rotation", entity->rotation);
			
			ImGuiLink::DrawVec3Control("Size", entity->size, 1.0f);
			
			ImGuiLink::TreePop();
		}
	}

	void DrawCameraProp(ENTPointCamera2D* entity)
	{
		if (ImGuiLink::TreeNodeEx((void*)typeid(ENTPointCamera2D).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Point Camera 2D"))
		{
			auto& camera = entity->camera;
			ImGuiLink::Checkbox("Primary", &entity->isPrimary);

			 char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			 char* currentProjectionTypeString = projectionTypeStrings[(int)camera->GetProjectionType()];

			 if (ImGuiLink::BeginCombo("Projection", currentProjectionTypeString))
			 {
				 for (int i = 0; i < 2; i++)
				 {
					 bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					 if (ImGuiLink::Selectable(projectionTypeStrings[i], &isSelected))
					 {
						 currentProjectionTypeString = projectionTypeStrings[i];
						 camera->SetProjectionType((SceneCamera::ProjectionType)i);
					 }
				 }
				 ImGuiLink::EndCombo();
			 }
			 if (camera->GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			 {
				 float verticalFov = glm::degrees(camera->GetPerspectiveVerticalFOV());
				 if (ImGuiLink::DragFloat("Vertical FOV", &verticalFov))
					 camera->SetPerspectiveVerticalFOV(glm::radians(verticalFov));

				 float orthoNear = camera->GetPerspectiveNearClip();
				 if (ImGuiLink::DragFloat("Near", &orthoNear))
					 camera->SetPerspectiveNearClip(orthoNear);

				 float orthoFar = camera->GetPerspectiveFarClip();
				 if (ImGuiLink::DragFloat("Far", &orthoFar))
					 camera->SetPerspectiveFarClip(orthoFar);
			 }
			 if (camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			 {
				 float orthoSize = camera->GetOrthographicSize();
				 if (ImGuiLink::DragFloat("Size", &orthoSize))
					 camera->SetOrthographicSize(orthoSize);

				 float orthoNear = camera->GetOrthographicNearClip();
				 if (ImGuiLink::DragFloat("Near", &orthoNear))
					 camera->SetOrthographicNearClip(orthoNear);

				 float orthoFar = camera->GetOrthographicFarClip();
				 if (ImGuiLink::DragFloat("Far", &orthoFar))
					 camera->SetOrthographicFarClip(orthoFar);
			 }

			 ImGuiLink::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawEntity(Entity* entity)
	{
		if (entity == nullptr) return;
		DrawBaseEntity(entity);
		if (entity->GetEntName() == "point_camera_2d") { DrawCameraProp((ENTPointCamera2D*)entity); }
	}
#pragma endregion


	


}