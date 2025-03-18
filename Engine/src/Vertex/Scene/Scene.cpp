#include "vxpch.h"


#include "Scene.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"
#include "Entities/Entities.h"
#include "Entities/point_camera_2d/point_camera_2d.h"
//#include "../Scripting/ScriptEngine.h"
#include <glm/ext/matrix_transform.hpp>

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include "Entities/base_box_collider_2d/base_box_collider_2d.h"
//#include "../MainLib.h"
#include "Entities/prop_static_sprite/prop_static_sprite.h"
//#include "../../VXEntities.h"

#include "mono/jit/jit.h"
#include <mono/metadata/assembly.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/debug-helpers.h>
#include "mono/metadata/object.h"

#include "Vertex/Scripting/ScriptEngine.h"

#define SPACE_CHAR ' '

static bool has_space(const std::string str)
{
	for (char c : str)
	{
		if (c == SPACE_CHAR)
		{
			return true;
		}
	}

	return false;
}

namespace Vertex {
	static b2BodyType Rigidbody2DTypeToBox2DBody(ENTBaseRigidbody2D::BodyType bodyType)
	{
		switch (bodyType)
		{
		case ENTBaseRigidbody2D::BodyType::Static:    return b2_staticBody;
		case ENTBaseRigidbody2D::BodyType::Dynamic:   return b2_dynamicBody;
		case ENTBaseRigidbody2D::BodyType::Kinematic: return b2_kinematicBody;
		}
		VX_CORE_ASSERT(false, "Unknown body type");
		return b2_staticBody;
	}
	static void DoMath(const glm::mat4& transform)
	{

	}

	Scene::Scene(std::string name)
	{
		

		m_name = name;
		
#if ENTT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();


		if (m_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);


		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif

		
	}

	Scene::~Scene()
	{
		for (Entity* ent : m_Entitys)
		{
			delete ent;
		}
	}

	


	Entity* Scene::CreateEntity(const std::string& ent_name, const std::string& name)
	{
		std::lock_guard<std::mutex> lock(m_EntityMutex);
		Entity* ent = EntityFactory::CreateEntity(ent_name, name, (Scene*)this);
		m_Entitys.push_back(ent);
		return ent;
	}

	Entity* Scene::FindEntityByName(std::string_view name)
	{
		std::lock_guard<std::mutex> lock(m_EntityMutex);
		for (Entity* ent : *this)
		{
			if (ent->name() == name)
			{
				return ent;
			}
		}
		return nullptr;
	}

	bool Scene::RemoveEntity(Entity& entity)
	{
		std::lock_guard<std::mutex> lock(m_EntityMutex);
		m_Entitys.erase(std::remove(m_Entitys.begin(), m_Entitys.end(), &entity), m_Entitys.end());
		delete& entity;
		return true;
	}

	b2Body* SetupRB2D(ENTBaseRigidbody2D* rb2d, b2World* m_PhysicsWorld)
	{
		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d->Type);
		bodyDef.position.Set(rb2d->pos.x, rb2d->pos.y);

		bodyDef.angle = glm::radians(rb2d->rotation.z);

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d->FixedRotation);
		rb2d->SetRuntimeBody(body);

		return body;
	}



	

	void Scene::OnRuntimeStart()
	{
		m_IsRunning = true;

		m_IsEditor = false;
		
		MonoClass* RB2DClass = nullptr;
		MonoClass* BC2DClass = nullptr;
		if (true)
		{
			ScriptEngine::OnRuntimeStart(this);
			RB2DClass = ScriptEngine::GetMonoClassFromName(ScriptEngine::GetCoreAssemblyImage(), "Vertex", "ENTBaseRigidbody2D");
			BC2DClass = ScriptEngine::GetMonoClassFromName(ScriptEngine::GetCoreAssemblyImage(), "Vertex", "ENTBaseBoxCollier2D");
			for (Entity* ent : m_Entitys)
			{
				ScriptEngine::OnCreateEntity(ent, [&](ENTEnvScript* sc) {
					

					for (auto& obj : sc->obj.GetAll() )
					{
						if (has_space(obj.name))
						{
							Ref<ScriptInstance> Instance = ScriptEngine::GetEntityInstance(ent->GetID());
							if (ScriptEngine::IsSubclassOf(Instance->m_ScriptClass->m_MonoClass, RB2DClass, false))
							{
								if (obj.name == "RB2D density")
								{
									MonoClassField* densityField = mono_class_get_field_from_name(Instance->m_ScriptClass->m_MonoClass, "Density");
									mono_field_set_value(Instance->m_Instance, densityField, &obj.value);
								}

								if (obj.name == "RB2D friction")
								{
									MonoClassField* frictionField = mono_class_get_field_from_name(Instance->m_ScriptClass->m_MonoClass, "Friction");
									mono_field_set_value(Instance->m_Instance, frictionField, &obj.value);
								}

								if (obj.name == "RB2D restitution")
								{
									MonoClassField* restitutionField = mono_class_get_field_from_name(Instance->m_ScriptClass->m_MonoClass, "Restitution");
									mono_field_set_value(Instance->m_Instance, restitutionField, &obj.value);
								}

								if (obj.name == "RB2D Restitution Threshold")
								{
									MonoClassField* restitutionThresholdField = mono_class_get_field_from_name(Instance->m_ScriptClass->m_MonoClass, "RestitutionThreshold");
									mono_field_set_value(Instance->m_Instance, restitutionThresholdField, &obj.value);
								}

							}

							
						}
					}
					return true;
				});

				ent->OnCreateTime();
			}

			
		}

		

		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		for (Entity* ent: m_Entitys)
		{
			if (auto boxCollider = dynamic_cast<ENTBaseBoxCollier2D*>(ent))
			{
				b2Body* body = SetupRB2D(boxCollider, m_PhysicsWorld);

				b2PolygonShape boxShape;
				boxShape.SetAsBox(boxCollider->Size.x * boxCollider->size.x, boxCollider->Size.y * boxCollider->size.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = boxCollider->Density;
				fixtureDef.friction = boxCollider->Friction;
				fixtureDef.restitution = boxCollider->Restitution;
				fixtureDef.restitutionThreshold = boxCollider->RestitutionThreshold;

				body->CreateFixture(&fixtureDef);
			}
			
			if (ent->GetEntName() == "env_script")
			{
				if (BC2DClass != nullptr)
				{
					MonoClass* bodyTypeEnumClass = mono_class_from_name(ScriptEngine::GetCoreAssemblyImage(), "Vertex", "ENTBaseRigidbody2D+BodyType");
					MonoMethod* getBodyTypeMethod = mono_class_get_method_from_name(RB2DClass, "get_Type", 0);

					Ref<ScriptInstance> Instance = ScriptEngine::GetEntityInstance(ent->GetID());

					if (Instance->m_Instance == nullptr)
					{
						VX_WARN("Instance is null");
						return;
					}

					if (ScriptEngine::IsSubclassOf(Instance->m_ScriptClass->m_MonoClass, BC2DClass, false))
					{
						MonoClassField* bodyTypeField = mono_class_get_field_from_name(RB2DClass, "Type");
						int32_t bodyTypeEnumValue;
						mono_field_get_value(Instance->m_Instance, bodyTypeField, &bodyTypeEnumValue); // Pass the address of the variable
						
						MonoClassField* fixedRotationField = mono_class_get_field_from_name(RB2DClass, "FixedRotation");
						bool FixedRotation;
						mono_field_get_value(Instance->m_Instance, fixedRotationField, &FixedRotation); // Pass the address of the variable


						VX_INFO("{0} {1}", bodyTypeEnumValue, FixedRotation);

						b2BodyDef bodyDef;
						bodyDef.type = Rigidbody2DTypeToBox2DBody((ENTBaseRigidbody2D::BodyType)bodyTypeEnumValue);
						bodyDef.position.Set(ent->pos.x, ent->pos.y);

						bodyDef.angle = ent->rotation.z;

						b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
						body->SetFixedRotation(FixedRotation);

						
						
						MonoClassField* bodyPtrField = mono_class_get_field_from_name(RB2DClass, "bodyPtr");

						

						mono_field_set_value(Instance->m_Instance, bodyPtrField, (void*)(&body));



						// Make Box 2d



						b2PolygonShape boxShape;

						boxShape.SetAsBox(0.5f * ent->size.x, 0.5f * ent->size.x);

						b2FixtureDef fixtureDef;
						fixtureDef.shape = &boxShape;

						MonoClassField* densityField = mono_class_get_field_from_name(Instance->m_ScriptClass->m_MonoClass, "Density");
						mono_field_get_value(Instance->m_Instance, densityField, &fixtureDef.density); // Pass the address of the variable
						
						// Friction
						MonoClassField* frictionField = mono_class_get_field_from_name(Instance->m_ScriptClass->m_MonoClass, "Friction");
						mono_field_get_value(Instance->m_Instance, frictionField, &fixtureDef.friction); // Pass the address of the variable

						// Restitution
						MonoClassField* restitutionField = mono_class_get_field_from_name(Instance->m_ScriptClass->m_MonoClass, "Restitution");
						mono_field_get_value(Instance->m_Instance, restitutionField, &fixtureDef.restitution); // Pass the address of the variable

						// Restitution
						MonoClassField* restitutionThresholdField = mono_class_get_field_from_name(Instance->m_ScriptClass->m_MonoClass, "RestitutionThreshold");
						mono_field_get_value(Instance->m_Instance, restitutionThresholdField, &fixtureDef.restitutionThreshold); // Pass the address of the variable

						body->CreateFixture(&fixtureDef);

					}

					
				}
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		m_IsRunning = false;

		m_IsEditor = true;
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
		
		for (Entity* ent : m_Entitys)
		{
			ent->OnDestroyTime();
		}

		if (true)
		{
			ScriptEngine::OnRuntimeStop();
		}
	}

	bool Scene::GetACameraInScene(Ref<Camera>* mainCamera, bool is2D, glm::mat4* cameraTransform, ENTPointCamera2D** cam, bool usePrimaryCam)
	{
		for (Entity* ent : m_Entitys)
		{
			if (is2D && ent->GetEntName() == "point_camera_2d")
			{
				ENTPointCamera2D* camera2D = dynamic_cast<ENTPointCamera2D*>(ent);
				
				if (0 || camera2D->isPrimary) // Optionally check if it's the primary camera
				{
					*mainCamera = camera2D->camera; // Set the main camera

					if (cam)
					{
						*cam = camera2D;
					}
					if (cameraTransform) // If the transform pointer is provided, update it
					{
						glm::mat4 transform = glm::translate(glm::mat4(1.0f), camera2D->pos)
							* glm::rotate(glm::mat4(1.0f), glm::radians(camera2D->rotation.z), { 0.0f, 0.0f, 1.0f })
							* glm::scale(glm::mat4(1.0f), { camera2D->size.x, camera2D->size.y, 1.0f });
						*cameraTransform = transform;
					}

					
					return true; // Successfully found a 2D camera
				}

			}
		}
		return false;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		VX_WARN("Use OnUpdateRuntime");
		OnUpdateRuntime(ts);

	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		for (Entity* ent : m_Entitys)
		{
			ent->UpdateTime(ts);
		}

		if (m_PhysicsWorld != nullptr)
		{



			const int32_t velocityIterations = 6 * 2;
			const int32_t positionIterations = 2 * 2;
			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);
			for (Entity* ent : m_Entitys)
			{
				ent->PhysUpdate(ts);
			}
		}
		
		for (Entity* ent : m_Entitys)
		{
			ent->DrawTime(ts);
		}

		{
			gc_thread();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		for (Entity* ent : m_Entitys)
		{
			ent->DrawTime(ts);
		}

		{
			gc_thread();
		}
	}

	void Scene::OnPostDeserialize()
	{
		for (Entity* ent : m_Entitys)
		{
			ent->PostDeserialize();
		}

		
	}

	void Scene::OnEvent(Event& e)
	{
		for (Entity* ent : m_Entitys)
		{
			ent->EventH(e);
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		for (Entity* ent : m_Entitys)
		{
			if (ent->GetEntName() == "point_camera_2d")
			{
				ENTPointCamera2D* camera2D = dynamic_cast<ENTPointCamera2D*>(ent);

				if (!camera2D->FixedAspectRatio)
				{
					camera2D->camera->SetViewportSize(width, height);
				}
				
			}
		}
	}

	template<typename EntityType>
	static void CopyEntity(EntityType* ent, Scene* scene, bool haveSameUUID)
	{
		if (!dynamic_cast<Entity*>(ent))
		{
			return;
		}

		EntityType* newEntity = &scene->CreateEntity<EntityType>(*ent);
		if (auto newEnt = dynamic_cast<Entity*>(newEntity))
		{
			newEnt->onAdded(scene);
			if (haveSameUUID)
			{
				newEnt->SetID(ent->GetID());
			}
		}
	}

	template<typename... ENT>
	static void CopyEntity(Scene* other, Scene* newScene)
	{
		(void(std::for_each(other->begin(), other->end(), [&](Entity* ent) {
			CopyEntity(dynamic_cast<ENT*>(ent), newScene, true);
			})), ...);
	}

	template<typename... ENT>
	static void CopyEntityWithGroup(Scene* other, Scene* newScene, EntityGroup<ENT...>)
	{
		VX_CORE_INFO("Registering Entities");
		CopyEntity<ENT...>(other, newScene);
		VX_CORE_INFO("~Registering Entities");
	}

	struct EntityPtrHash {
		std::size_t operator()(const Entity* entity) const noexcept {
			return reinterpret_cast<std::uintptr_t>(entity);
		}
	};

	struct BehaviourPtrHash {
		std::size_t operator()(const Behaviour* entity) const noexcept {
			return reinterpret_cast<std::uintptr_t>(entity);
		}
	};


	Scene* Scene::Copy(Scene* other, std::string& name)
	{
		Scene* newScene = new Scene(name);

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		for (Entity* entity : other->m_Entitys)
		{
			if (entity == nullptr || entity == (Entity*)0xdddddddddddddddd) continue;

			std::string entityType = entity->GetEntName();
			std::string entityName = entity->name();

			Entity* newEntity = newScene->CreateEntity(entityType, entityName);
			newEntity->SetID(entity->GetID());
			newEntity->SetID(entity->GetIntID());
			newEntity->DeSerialize(entity->Serialize());
		}

		return newScene;
        
	}
	void Scene::gc_thread()
	{
		/*auto start = std::chrono::steady_clock::now();

		std::unordered_set<Entity*, EntityPtrHash> uniqueEntities;
		std::unordered_set<Behaviour*, BehaviourPtrHash> uniqueBehaviours;

		std::lock_guard<std::mutex> lock(m_EntityMutex);

		// Entity cleanup
		m_Entitys.erase(std::remove_if(m_Entitys.begin(), m_Entitys.end(),
			[&uniqueEntities](Entity* entity)
			{
				if (uniqueEntities.find(entity) != uniqueEntities.end())
				{
					return true; // Mark for removal
				}
				uniqueEntities.insert(entity);
				return false;
			}), m_Entitys.end());

		// Behaviour cleanup
		for (Entity* entity : m_Entitys)
		{
			auto& behaviours = entity->GetBehaviours();
			behaviours.erase(std::remove_if(behaviours.begin(), behaviours.end(),
				[&uniqueBehaviours, entity](Behaviour* behaviour)
				{
					if (uniqueBehaviours.find(behaviour) != uniqueBehaviours.end())
					{
						//entity->RemoveBehaviour(behaviour);
						return false; // Mark for removal
					}
					uniqueBehaviours.insert(behaviour);
					return false;
				}), behaviours.end());
		}*/
	}


			


		//}
	//}
};