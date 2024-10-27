#include "vxpch.h"


#include "Scene.h"
#include "Vertex/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"
#include "Entities/point_camera_2d/point_camera_2d.h"
#include <glm/ext/matrix_transform.hpp>

// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include "Entities/base_box_collider_2d/base_box_collider_2d.h"
#include "../MainLib.h"
#include "Entities/prop_static_sprite/prop_static_sprite.h"
#include "../../VXEntities.h"


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

	


	bool Scene::RemoveEntity(Entity& entity)
	{
		m_Entitys.erase(std::remove(m_Entitys.begin(), m_Entitys.end(), &entity), m_Entitys.end());
		delete& entity;
		return true;
	}

	b2Body* SetupRB2D(ENTBaseRigidbody2D* rb2d, b2World* m_PhysicsWorld)
	{
		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d->Type);
		bodyDef.position.Set(rb2d->pos.x, rb2d->pos.y);

		bodyDef.angle = rb2d->rotation.z;

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d->FixedRotation);
		rb2d->SetRuntimeBody(body);

		return body;
	}



	void Scene::OnRuntimeStart()
	{
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
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
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
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		for (Entity* ent : m_Entitys)
		{
			ent->DrawTime(ts);
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
		if (!dynamic_cast<Entity*>(ent) || !dynamic_cast<EntityType*>(ent))
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

	Scene* Scene::Copy(Scene* other, std::string& name)
	{
		Scene* newScene = VXEntities_MakeOrGetScene(name);

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		for (Entity* ent: *other)
		{
			CopyEntity<ENTPropStaticSprite>(dynamic_cast<ENTPropStaticSprite*>(ent), newScene, true);
			CopyEntity<ENTPropDynamicSprite>(dynamic_cast<ENTPropDynamicSprite*>(ent), newScene, true);
			CopyEntity<ENTPointCamera2D>(dynamic_cast<ENTPointCamera2D*>(ent), newScene, true);
			CopyEntity<ENTEnvStaticTilemap>(dynamic_cast<ENTEnvStaticTilemap*>(ent), newScene, true);
		}

		return newScene;
	}

}