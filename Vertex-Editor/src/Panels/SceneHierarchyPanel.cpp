#include "SceneHierarchyPanel.h"
#include "Vertex/ImGui/ImGuiLink.h"
#include <glm/gtc/type_ptr.hpp>
#include "VXEntities.h"

#define ImGuiTreeNodeFlags_Selected 1
#define ImGuiTreeNodeFlags_OpenOnArrow 128
#define ImGuiTreeNodeFlags_DefaultOpen 32


namespace Vertex {

	SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(Scene* scene)
	{
		m_Context = scene;
		m_SelectionContext = nullptr;
	}
	char ent_name_buffer[256];
	char ent_type_buffer[256];
	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (ImGuiLink::IsMouseDown(0) && ImGuiLink::IsWindowHovered())
			m_SelectionContext = nullptr;

		if (ImGuiLink::BeginMenuBar())
		{
			if (ImGuiLink::BeginMenu("Entitys"))
			{
				if (ImGuiLink::MenuItem("Create Entity"))
				{
					memset(ent_name_buffer, 0, sizeof(ent_name_buffer));
					strcpy_s(ent_name_buffer, sizeof(ent_name_buffer), "name");

					memset(ent_type_buffer, 0, sizeof(ent_type_buffer));
					strcpy_s(ent_type_buffer, sizeof(ent_type_buffer), "prop_static_sprite");

					m_CreateEntityPopup = true;
				}
				ImGuiLink::EndMenu();
			}
			
				
			ImGuiLink::EndMenuBar();
		}

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

		if (m_CreateEntityPopup)
		{
			ImGuiLink::Begin("Entity Maker", &m_CreateEntityPopup);
			
			ImGuiLink::InputText("Tag", ent_name_buffer, sizeof(ent_name_buffer));
			ImGuiLink::InputText("Entity type", ent_type_buffer, sizeof(ent_type_buffer));

			if (ImGuiLink::Button("Create Entity"))
			{
				m_CreateEntityPopup = false;
				std::string tag = std::string(ent_name_buffer);
				std::string type = std::string(ent_type_buffer);

				if (type == "prop_static_sprite")
				{
					m_Context->CreateEntity<ENTPropStaticSprite>(tag);
				}

				if (type == "point_camera_2d")
				{
					ENTPointCamera2D* cam = &m_Context->CreateEntity<ENTPointCamera2D>(tag);
					cam->isPrimary = false;
				}
			}

			ImGuiLink::End();

			if (m_EntityToRemove != nullptr)
			{
				m_Context->RemoveEntity(*m_EntityToRemove);
				if (m_SelectionContext == m_EntityToRemove)
					m_SelectionContext = nullptr;
				m_EntityToRemove = nullptr;
			}
		}
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

		bool entityDeleted = false;
		if (ImGuiLink::BeginPopupContextItem())
		{
			if (ImGuiLink::MenuItem("Delete Entity"))
				entityDeleted = true;
			ImGuiLink::EndPopup();
		}

		if (opened)
		{
			int flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGuiLink::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGuiLink::TreePop();
			ImGuiLink::TreePop();
		}

		if (entityDeleted)
		{
			m_EntityToRemove = entity;
			
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

	void DrawSpriteRendererProp(ENTPropStaticSprite* entity)
	{
		if (ImGuiLink::TreeNodeEx((void*)typeid(ENTPropStaticSprite).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
		{
			
			ImGuiLink::ColorEdit4("Color", glm::value_ptr(entity->colour));
			ImGuiLink::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawEntity(Entity* entity)
	{
		if (entity == nullptr) return;
		DrawBaseEntity(entity);
		if (entity->GetEntName() == "point_camera_2d") { DrawCameraProp((ENTPointCamera2D*)entity); }
		if (entity->GetEntName() == "prop_static_sprite") { DrawSpriteRendererProp((ENTPropStaticSprite*)entity); }
	}
#pragma endregion


	


}