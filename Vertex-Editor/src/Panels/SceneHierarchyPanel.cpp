#include "SceneHierarchyPanel.h"
#include "Vertex/ImGui/ImGuiLink.h"
#include <glm/gtc/type_ptr.hpp>
#include "Vertex/Scene/Entities/Entities.h"
#include <Vertex/Scripting/ScriptEngine.h>
#include <Vertex/AssetManager/AssetManager.h>
#include <Vertex/Scene/Entities/prop_text/prop_text.h>
#include <Vertex/AssetManager/EditorAssetManager.h>



#define ImGuiTreeNodeFlags_Selected 1
#define ImGuiTreeNodeFlags_OpenOnArrow 128
#define ImGuiTreeNodeFlags_DefaultOpen 32

#define NUM_TEXTURE_EXTENSIONS 5
const char* texturesFileExtensions[NUM_TEXTURE_EXTENSIONS] = {
	".png",
	".jpg",
	".jpeg",
	".bmp",
	".tga"
};

namespace Vertex {

	extern const std::filesystem::path g_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene)
	{
		SetContext(scene);
		ScriptEngine::OnRuntimeStart(scene);
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

				// EZ way to create entities
				m_SelectionContext = m_Context->CreateEntity(type, tag);
				m_SelectionContext->AddBehaviour("lua_script");

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

		if (m_EntityToRemove)
		{
			m_Context->RemoveEntity(*m_EntityToRemove);
			if (m_SelectionContext == m_EntityToRemove)
				m_SelectionContext = nullptr;
			m_EntityToRemove = nullptr;
		}
	}

	Entity* popupContextEntity = nullptr;
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
	Entity* EntityToRemove = nullptr;
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
		ImGui::SameLine();
		if (ImGuiLink::Button("Delete"))
		{
			EntityToRemove = entity;
		}

		if (ImGuiLink::TreeNodeEx(entity->GetID() + "Transform", ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
		{
			ImGuiLink::DrawVec3Control("Position", entity->pos);
			
			ImGuiLink::DrawVec3Control("Rotation", entity->rotation);
			
			ImGuiLink::DrawVec3Control("Size", entity->size, 1.0f);
			
			ImGuiLink::TreePop();
		}

		if (entity->GetBehaviour() && ImGuiLink::TreeNodeEx(entity->GetID() + "Behaviours", ImGuiTreeNodeFlags_DefaultOpen, "Behaviours"))
		{
			entity->GetBehaviour()->ImGuiDrawProperties();
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

	void DrawStaticDynamicRendererProp(ENTPropDynamicSprite* entity)
	{
		if (ImGuiLink::TreeNodeEx((void*)typeid(ENTPropDynamicSprite).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
		{

			


			ImGui::ColorEdit4("Color", glm::value_ptr(entity->colour));

			std::string label = "None";
			bool isTextureValid = false;

			EditorAssetManager* AssetManager = ((EditorAssetManager*)(void*)AssetManager::GetAssetManager().get());

			if (entity->texture!= 0)
			{
				if (AssetManager->IsAssetHandleValid(entity->texture->Handle)
					&& AssetManager->GetAssetType(entity->texture->Handle) == AssetType::Texture2D)
				{
					const AssetMetadata& metadata = AssetManager->GetMetadata(entity->texture->Handle);
					label = metadata.FilePath.filename().string();
					isTextureValid = true;
				}
				else
				{
					label = "Invalid";
				}
			}

			ImVec2 buttonLabelSize = ImGui::CalcTextSize(label.c_str());
			buttonLabelSize.x += 20.0f;
			float buttonLabelWidth = glm::max<float>(100.0f, buttonLabelSize.x);

			ImGui::Button(label.c_str(), ImVec2(buttonLabelWidth, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					AssetHandle handle = *(AssetHandle*)payload->Data;
					if (AssetManager->GetAssetType(handle) == AssetType::Texture2D)
					{
						entity->texture = std::dynamic_pointer_cast<Texture2D>(AssetManager->GetAsset(handle));
					}
					else
					{
						VX_CORE_WARN("Wrong asset type!");
					}

				}
				ImGui::EndDragDropTarget();
			}

			if (isTextureValid)
			{
				ImGui::SameLine();
				ImVec2 xLabelSize = ImGui::CalcTextSize("X");
				float buttonSize = xLabelSize.y + ImGui::GetStyle().FramePadding.y * 2.0f;
				if (ImGui::Button("X", ImVec2(buttonSize, buttonSize)))
				{
					entity->texture.reset();
				}
			}

			ImGui::SameLine();
			ImGui::Text("Texture");

			ImGui::DragFloat("Tiling Factor", &entity->tilingFactor, 0.1f, 0.0f, 100.0f);
			ImGuiLink::TreePop();
		}
	}

	void DrawStaticSpriteRendererProp(ENTPropStaticSprite* entity)
	{
		if (ImGuiLink::TreeNodeEx((void*)typeid(ENTPropStaticSprite).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
		{
			
			ImGui::ColorEdit4("Color", glm::value_ptr(entity->colour));

			std::string label = "None";
			bool isTextureValid = false;

			EditorAssetManager* AssetManager = ((EditorAssetManager*)(void*)AssetManager::GetAssetManager().get());

			if (entity->texture != 0)
			{
				if (AssetManager->IsAssetHandleValid(entity->texture->Handle)
					&& AssetManager->GetAssetType(entity->texture->Handle) == AssetType::Texture2D)
				{
					const AssetMetadata& metadata = AssetManager->GetMetadata(entity->texture->Handle);
					label = metadata.FilePath.filename().string();
					isTextureValid = true;
				}
				else
				{
					label = "Invalid";
				}
			}

			ImVec2 buttonLabelSize = ImGui::CalcTextSize(label.c_str());
			buttonLabelSize.x += 20.0f;
			float buttonLabelWidth = glm::max<float>(100.0f, buttonLabelSize.x);

			ImGui::Button(label.c_str(), ImVec2(buttonLabelWidth, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					AssetHandle handle = *(AssetHandle*)payload->Data;
					if (AssetManager->GetAssetType(handle) == AssetType::Texture2D)
					{
						entity->texture = std::dynamic_pointer_cast<Texture2D>(AssetManager->GetAsset(handle));
					}
					else
					{
						VX_CORE_WARN("Wrong asset type!");
					}

				}
				ImGui::EndDragDropTarget();
			}

			if (isTextureValid)
			{
				ImGui::SameLine();
				ImVec2 xLabelSize = ImGui::CalcTextSize("X");
				float buttonSize = xLabelSize.y + ImGui::GetStyle().FramePadding.y * 2.0f;
				if (ImGui::Button("X", ImVec2(buttonSize, buttonSize)))
				{
					entity->texture.reset();
				}
			}

			ImGui::SameLine();
			ImGui::Text("Texture");

			ImGui::DragFloat("Tiling Factor", &entity->tilingFactor, 0.1f, 0.0f, 100.0f);

			ImGuiLink::TreePop();
		}
	}

	void DrawProp2DCircleImGui(ENTProp2DCircle* ent)
	{
		if (ImGuiLink::TreeNodeEx((void*)typeid(ENTProp2DCircle).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Prop 2D Circle"))
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(ent->colour));
			ImGui::DragFloat("Thickness", &ent->Thickness, 0.025f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &ent->Fade, 0.00025f, 0.0f, 1.0f);
			ImGuiLink::TreePop();
		}
	}

	void DrawPropText2DImGui(ENTPropText2D* ent)
	{
		if (ImGuiLink::TreeNodeEx((void*)typeid(ENTPropText2D).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Prop Text 2D"))
		{
			if (ImGui::InputTextMultiline("Text String", ent->imgui_text_data, 255))
			{
				ent->text = std::string(ent->imgui_text_data);
			}
			ImGui::ColorEdit4("Color", glm::value_ptr(ent->textParams.Color));
			ImGui::DragFloat("Kerning", &ent->textParams.Kerning, 0.025f);
			ImGui::DragFloat("Line Spacing", &ent->textParams.LineSpacing, 0.025f);
			ImGuiLink::TreePop();
		}
	}

	void DrawBoxCollider2DImGui(ENTBaseBoxCollier2D* ent)
	{
		if (ImGuiLink::TreeNodeEx((void*)typeid(ENTBaseBoxCollier2D).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Box Collider 2D"))
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(ent->Offset));
			ImGui::DragFloat("Density", &ent->Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &ent->Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &ent->Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &ent->RestitutionThreshold, 0.01f, 0.0f);
			ImGuiLink::TreePop();
		}
	}

	void DrawRB2DImGui(ENTBaseRigidbody2D* ent)
	{
		if (ImGuiLink::TreeNodeEx((void*)typeid(ENTBaseRigidbody2D).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Rigidbody 2D"))
		{
			
			if (!(ent->GetEntName() == "prop_static_sprite" || ent->GetEntName() == "prop_dynamic_sprite"))
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)ent->Type];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							ent->Type = (ENTBaseRigidbody2D::BodyType)i;
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

			ImGui::Checkbox("Fixed Rotation", &ent->FixedRotation);
			ImGuiLink::TreePop();
		}
	}

	void DrawEnvScript(ENTEnvScript* script, Scene* scene)
	{
		int ENTBaseRigidbody2DID = typeid(ENTBaseRigidbody2D).hash_code() + (int)script;
		if (script->useRB2D && ImGuiLink::TreeNodeEx((void*)ENTBaseRigidbody2DID, ImGuiTreeNodeFlags_DefaultOpen, "Rigidbody 2D"))
		{
			if (!script->obj.Contains("RB2D density"))
			{
				script->obj.Set("RB2D density", SerializationType::Float, 1.0f);
			}

			float density = script->obj.Get<float>("RB2D density", SerializationType::Float);
			if (ImGui::DragFloat("Density", &density, 0.01f, 0.0f, 1.0f)) 
			{
				script->obj.Set("RB2D density", SerializationType::Float, density);
			}

			if (!script->obj.Contains("RB2D friction"))
			{
				script->obj.Set("RB2D friction", SerializationType::Float, 0.5f);
			}

			float friction = script->obj.Get<float>("RB2D friction", SerializationType::Float);
			if (ImGui::DragFloat("Friction", &friction, 0.01f, 0.0f, 1.0f))
			{
				script->obj.Set("RB2D friction", SerializationType::Float, friction);
			}

			if (!script->obj.Contains("RB2D restitution"))
			{
				script->obj.Set("RB2D restitution", SerializationType::Float, 0.0f);
			}

			float restitution = script->obj.Get<float>("RB2D restitution", SerializationType::Float);
			if (ImGui::DragFloat("Restitution", &restitution, 0.01f, 0.0f, 1.0f))
			{
				script->obj.Set("RB2D restitution", SerializationType::Float, restitution);
			}

			if (!script->obj.Contains("RB2D Restitution Threshold"))
			{
				script->obj.Set("RB2D Restitution Threshold", SerializationType::Float, 0.5f);
			}

			float restitutionThreshold = script->obj.Get<float>("RB2D Restitution Threshold", SerializationType::Float);
			if (ImGui::DragFloat("Restitution Threshold", &restitutionThreshold, 0.01f, 0.0f, 1.0f))
			{
				script->obj.Set("RB2D Restitution Threshold", SerializationType::Float, restitutionThreshold);
			}

			ImGuiLink::TreePop();
		}

		if (ImGuiLink::TreeNodeEx((void*)typeid(ENTEnvScript).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Script"))
		{
			bool scriptClassExists = ScriptEngine::EntityClassExists(script->classname);

			ImGui::Checkbox("Use Rigidbody 2D", &script->useRB2D);

			if (!scriptClassExists)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

			ImGui::InputText("Class Name", script->classname.data(), 128);


			if (!scriptClassExists)
				ImGui::PopStyleColor();

			scriptClassExists = ScriptEngine::EntityClassExists(script->classname);

			

			bool sceneRunning = scene->IsRunning();
			if (sceneRunning)
			{
				Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(script->GetID());
				if (scriptInstance)
				{
					const auto& fields = scriptInstance->GetScriptClass()->GetFields();
					for (const auto& [name, field] : fields)
					{
						if (field.Type == ScriptFieldType::Float)
						{
							float data = scriptInstance->GetFieldValue<float>(name);
							if (ImGui::DragFloat(name.c_str(), &data))
							{
								scriptInstance->SetFieldValue(name, data);
							}
						}
					}
				}
			}
			else
			{
				if (scriptClassExists)
				{
					Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(script->classname);
					const auto& fields = entityClass->GetFields();

					auto& entityFields = ScriptEngine::GetScriptFieldMap(script);
					for (const auto& [name, field] : fields)
					{
						// Field has been set in editor
						if (entityFields.find(name) != entityFields.end())
						{
							ScriptFieldInstance& scriptField = entityFields.at(name);

							// Display control to set it maybe
							if (field.Type == ScriptFieldType::Float)
							{
								float data = scriptField.GetValue<float>();
								if (ImGui::DragFloat(name.c_str(), &data))
									scriptField.SetValue(data);
							}
						}
						else
						{
							// Display control to set it maybe
							if (field.Type == ScriptFieldType::Float)
							{
								float data = 0.0f;
								if (ImGui::DragFloat(name.c_str(), &data))
								{
									ScriptFieldInstance& fieldInstance = entityFields[name];
									fieldInstance.Field = field;
									fieldInstance.SetValue(data);
								}
							}
						}
					}
				}
			}

			
			ImGuiLink::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawEntity(Entity* entity)
	{
		if (entity == nullptr) return;
		DrawBaseEntity(entity);
		if (EntityToRemove != nullptr)
		{
			m_EntityToRemove = EntityToRemove;
			EntityToRemove = nullptr;
			return;
		}
		if (entity->GetEntName() == "point_camera_2d") { DrawCameraProp((ENTPointCamera2D*)entity); }
		if (entity->GetEntName() == "prop_static_sprite") { DrawStaticSpriteRendererProp((ENTPropStaticSprite*)entity); }
		if (entity->GetEntName() == "prop_dynamic_sprite") { DrawStaticDynamicRendererProp((ENTPropDynamicSprite*)entity); }
		if (auto boxCollider2D = dynamic_cast<ENTBaseBoxCollier2D*>(entity))
		{
			DrawBoxCollider2DImGui(boxCollider2D);
		}
		if (auto rb2D = dynamic_cast<ENTBaseRigidbody2D*>(entity))
		{
			DrawRB2DImGui(rb2D);
		}

		if (entity->GetEntName() == "env_script") { DrawEnvScript((ENTEnvScript*)entity, m_Context); }

		if (entity->GetEntName() == "prop_2d_circle") { DrawProp2DCircleImGui((ENTProp2DCircle*)entity); }

		if (entity->GetEntName() == "prop_text_2d") { DrawPropText2DImGui((ENTPropText2D*)entity); }

	}
#pragma endregion


	


}