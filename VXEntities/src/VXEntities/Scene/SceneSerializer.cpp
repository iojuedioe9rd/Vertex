#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include "Entity.h"
#include "../../VXEntities.h"

extern "C" uint64_t CustomChecksumAsm(const char* data, uint64_t length, uint64_t initialSeed, uint64_t rotateAmount, uint64_t additiveFactor, uint64_t rotateCount);

namespace YAML {
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Vertex {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(Scene** scene)
	{
		m_Scene = *scene;
	}

	static std::string RigidBody2DBodyTypeToString(ENTBaseRigidbody2D::BodyType bodyType)
	{
		switch (bodyType)
		{
			case ENTBaseRigidbody2D::BodyType::Static:    return "Static";
			case ENTBaseRigidbody2D::BodyType::Dynamic:   return "Dynamic";
			case ENTBaseRigidbody2D::BodyType::Kinematic: return "Kinematic";
		}

		VX_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static ENTBaseRigidbody2D::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return ENTBaseRigidbody2D::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return ENTBaseRigidbody2D::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return ENTBaseRigidbody2D::BodyType::Kinematic;

		VX_CORE_ASSERT(false, "Unknown body type");
		return ENTBaseRigidbody2D::BodyType::Static;
	}

	std::string SceneSerializer::sanitizeID(const std::string& id) {
		std::string sanitized;
		for (char c : id) {
			if (c != '\x00') { // Filter out null bytes
				sanitized += c;
			}
		}
		return sanitized;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity* entity)
	{
		
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << sanitizeID(entity->GetID());

		out << YAML::Key << "Tag" << YAML::Value << entity->name();
		out << YAML::Key << "EntityType" << YAML::Value << entity->GetEntName();

		out << YAML::Key << "Transform";
		out << YAML::BeginMap;
		out << YAML::Key << "Translation" << YAML::Value << entity->pos;
		out << YAML::Key << "Size" << YAML::Value << entity->size;
		out << YAML::Key << "Rotation" << YAML::Value << entity->rotation;
		out << YAML::EndMap; // TransformComponent

		if (auto rb2d = dynamic_cast<ENTBaseRigidbody2D*>(entity))
		{
			out << YAML::Key << "BaseRigidbody2D";
			out << YAML::BeginMap; // Base Rigidbody 2D

			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rb2d->Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2d->FixedRotation;

			out << YAML::EndMap; // Base Rigidbody 2D
		}

		if (auto bc2d = dynamic_cast<ENTBaseBoxCollier2D*>(entity))
		{
			out << YAML::Key << "BaseBoxCollier2D";
			out << YAML::BeginMap; // Base Box Collier2D

			out << YAML::Key << "Offset" << YAML::Value << bc2d->Offset;
			out << YAML::Key << "Size" << YAML::Value << bc2d->Size;
			out << YAML::Key << "Density" << YAML::Value << bc2d->Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2d->Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2d->Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2d->RestitutionThreshold;

			out << YAML::EndMap; // Base Box Collier2D
		}

		if (entity->GetEntName() == "prop_static_sprite")
		{
			ENTPropStaticSprite* sprite = static_cast<ENTPropStaticSprite*>(entity);
			VX_CORE_ASSERT(sprite != nullptr, "sprite is null!");
			out << YAML::Key << "PropStaticSprite";
			out << YAML::BeginMap;
			out << YAML::Key << "Colour" << YAML::Value << sprite->colour;
			if (sprite->texture && !sprite->texture->GetPath().empty())
				out << YAML::Key << "TexturePath" << YAML::Value << sprite->texture->GetPath();
			out << YAML::Key << "TilingFactor" << YAML::Value << sprite->tilingFactor;
			out << YAML::EndMap;
		}

		if (entity->GetEntName() == "prop_dynamic_sprite")
		{
			ENTPropStaticSprite* sprite = static_cast<ENTPropStaticSprite*>(entity);
			VX_CORE_ASSERT(sprite != nullptr, "sprite is null!");
			out << YAML::Key << "PropDynamicSprite";
			out << YAML::BeginMap;
			out << YAML::Key << "Colour" << YAML::Value << sprite->colour;
			if (sprite->texture && !sprite->texture->GetPath().empty())
				out << YAML::Key << "TexturePath" << YAML::Value << sprite->texture->GetPath();
			out << YAML::Key << "TilingFactor" << YAML::Value << sprite->tilingFactor;
			out << YAML::EndMap;
		}

		if (entity->GetEntName() == "point_camera_2d")
		{
			ENTPointCamera2D* cam = static_cast<ENTPointCamera2D*>(entity);
			VX_CORE_ASSERT(cam != nullptr, "cam is null!");
			auto& camera = cam->camera;

			out << YAML::Key << "PointCamera2D";
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera->GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera->GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera->GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera->GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera->GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera->GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera->GetOrthographicFarClip();
			out << YAML::Key << "Primary" << YAML::Value << cam->isPrimary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cam->FixedAspectRatio;
			out << YAML::EndMap; // CameraComponent
			
		}

		if (entity->GetEntName() == "env_static_tilemap")
		{
			ENTEnvStaticTilemap* tilemap = static_cast<ENTEnvStaticTilemap*>(entity);
			out << YAML::Key << "EnvStaticTilemap";
			out << YAML::BeginMap;

			out << YAML::Key << "Tiles" << YAML::Value << YAML::BeginSeq;

			for (ENTEnvStaticTilemap::Tile tile : tilemap->GetTiles())
			{
				out << YAML::BeginMap;
				
				out << YAML::Key << "Pos" << YAML::Value << glm::vec3(tile.pos.x, tile.pos.y, 0);
				out << YAML::Key << "Colour" << YAML::Value << tile.colour;

				out << YAML::EndMap;
			}

			out << YAML::EndSeq;

			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Entity

		
	}

	uint64_t CalculateChecksumFromNode(const YAML::Node& node, uint64_t initialSeed = 0, uint64_t rotateAmount = 13, uint64_t additiveFactor = 7, uint64_t rotateCount = 5) {
		// Convert the YAML node to a string
		std::string yamlString = YAML::Dump(node);

		// Calculate checksum using the assembly function
		return CustomChecksumAsm(yamlString.c_str(), yamlString.size(), initialSeed, rotateAmount, additiveFactor, rotateCount);
	}


	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (Entity* entity : *m_Scene)
		{
			SerializeEntity(out, entity);
		}

		out << YAML::EndSeq;

		out << YAML::EndMap;

		// Convert emitted YAML to string for checksum calculation
		std::string yamlString = out.c_str();

		uint64_t checksum = CalculateChecksumFromNode(YAML::Load(yamlString), 0, 13, 7, 5);
		// Output the checksum to the YAML as a new key-value pair (optional)
		out << YAML::Key << "Checksum" << YAML::Value << checksum;
		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	
	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
	}

	bool SceneSerializer::Deserialize(const std::string& filepath) {
		std::ifstream stream(filepath);
		if (!stream.is_open()) {
			// Log error or throw exception: Unable to open file
			return false;
		}

		std::stringstream strStream;
		strStream << stream.rdbuf();
		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"]) {
			// Log error: Scene not found in the file
			return false;
		}

		auto entitiesNode = data["Entities"];
		if (!entitiesNode) {
			// Log error: Entities node not found
			return false;
		}

		for (const auto& entityNode : entitiesNode) {
			std::string entityID = entityNode["Tag"].as<std::string>();
			std::string entityType = entityNode["EntityType"].as<std::string>();
			//EntityType

			Entity* entity = nullptr; // Assume CreateEntity creates the entity
			if (entityType == "prop_static_sprite")
			{
				entity = &m_Scene->CreateEntity<ENTPropStaticSprite>(entityID);
			}
			if (entityType == "prop_dynamic_sprite")
			{
				entity = &m_Scene->CreateEntity<ENTPropDynamicSprite>(entityID);
			}
			if (entityType == "point_camera_2d")
			{
				entity = &m_Scene->CreateEntity<ENTPointCamera2D>(entityID);
				
			}
			if (entityType == "env_static_tilemap")
			{
				entity = &m_Scene->CreateEntity<ENTEnvStaticTilemap>(entityID);
			}
			if (entity) {
				DeserializeEntity(entityNode, entity);
			}
			else {
				VX_CORE_ASSERT(false, "Failed to create entity from ID");
			}
		}

		return true;
	}

	void SceneSerializer::DeserializeEntity(const YAML::Node& node, Entity* entity) {
		
		std::string tag = node["Tag"].as<std::string>();
		

		if (node["Transform"]) {
			auto transformNode = node["Transform"];
			if (transformNode["Translation"]) {
				glm::vec3 translation = transformNode["Translation"].as<glm::vec3>();
				entity->pos = translation; // Assuming you have a pos attribute
			}
			if (transformNode["Size"]) {
				glm::vec3 size = transformNode["Size"].as<glm::vec3>();
				entity->size = size; // Assuming you have a size attribute
			}
			if (transformNode["Rotation"]) {
				glm::vec3 rotation = transformNode["Rotation"].as<glm::vec3>();
				entity->rotation = rotation; // Assuming you have a rotation attribute
			}
		}

		if (auto rb2d = dynamic_cast<ENTBaseRigidbody2D*>(entity))
		{
			auto rigidbody2DNode = node["BaseRigidbody2D"];
			if (rigidbody2DNode)
			{
				rb2d->Type = RigidBody2DBodyTypeFromString(rigidbody2DNode["BodyType"].as<std::string>());
				rb2d->FixedRotation = rigidbody2DNode["FixedRotation"].as<bool>();
			}
		}

		if (auto bc2d = dynamic_cast<ENTBaseBoxCollier2D*>(entity))
		{
			auto boxCollier2DNode = node["BaseBoxCollier2D"];

			if (boxCollier2DNode)
			{
				bc2d->Offset = boxCollier2DNode["Offset"].as<glm::vec2>();
				bc2d->Size = boxCollier2DNode["Size"].as<glm::vec2>();
				bc2d->Density = boxCollier2DNode["Density"].as<float>();
				bc2d->Friction = boxCollier2DNode["Friction"].as<float>();
				bc2d->Restitution = boxCollier2DNode["Restitution"].as<float>();
				bc2d->RestitutionThreshold = boxCollier2DNode["RestitutionThreshold"].as<float>();
			}
			
		}

		// Deserialization for specific entity types
		if (entity->GetEntName() == "prop_static_sprite") {
			auto propStaticSpriteNode = node["PropStaticSprite"];
			ENTPropStaticSprite* sprite = static_cast<ENTPropStaticSprite*>(entity);
			if (propStaticSpriteNode && propStaticSpriteNode["Colour"]) {
				sprite->colour = propStaticSpriteNode["Colour"].as<glm::vec4>(); // Assuming colour is a glm::vec4
			}
			if (propStaticSpriteNode && propStaticSpriteNode["TexturePath"])
				sprite->texture = Texture2D::Create(propStaticSpriteNode["TexturePath"].as<std::string>());
			if (propStaticSpriteNode && propStaticSpriteNode["TilingFactor"])
				sprite->tilingFactor = propStaticSpriteNode["TilingFactor"].as<float>();
		}


		if (entity->GetEntName() == "prop_dynamic_sprite") {
			auto propDynamicSpriteNode = node["PropDynamicSprite"];
			ENTPropDynamicSprite* sprite = static_cast<ENTPropDynamicSprite*>(entity);
			if (propDynamicSpriteNode && propDynamicSpriteNode["Colour"]) {
				sprite->colour = propDynamicSpriteNode["Colour"].as<glm::vec4>(); // Assuming colour is a glm::vec4
				if (propDynamicSpriteNode["TexturePath"])
					sprite->texture = Texture2D::Create(propDynamicSpriteNode["TexturePath"].as<std::string>());
				if (propDynamicSpriteNode["TilingFactor"])
					sprite->tilingFactor = propDynamicSpriteNode["TilingFactor"].as<float>();
			}
		}

		if (entity->GetEntName() == "point_camera_2d") {
			ENTPointCamera2D* cam = static_cast<ENTPointCamera2D*>(entity);
			if (node["PointCamera2D"]) {
				auto camNode = node["PointCamera2D"];
				// Set camera properties here
				cam->camera->SetProjectionType(static_cast<SceneCamera::ProjectionType>(camNode["ProjectionType"].as<int>()));
				cam->camera->SetPerspectiveVerticalFOV(camNode["PerspectiveFOV"].as<float>());
				cam->camera->SetPerspectiveNearClip(camNode["PerspectiveNear"].as<float>());
				cam->camera->SetPerspectiveFarClip(camNode["PerspectiveFar"].as<float>());
				cam->camera->SetOrthographicSize(camNode["OrthographicSize"].as<float>());
				cam->camera->SetOrthographicNearClip(camNode["OrthographicNear"].as<float>());
				cam->camera->SetOrthographicFarClip(camNode["OrthographicFar"].as<float>());
			}
		}

		if (entity->GetEntName() == "env_static_tilemap")
		{
			ENTEnvStaticTilemap* tilemap = static_cast<ENTEnvStaticTilemap*>(entity);
			auto tilesNode = node["EnvStaticTilemap"]["Tiles"];
			for (const auto& tileNode : tilesNode)
			{
				glm::i32vec2 pos = glm::i32vec2(tileNode["Pos"].as<glm::vec3>().x, tileNode["Pos"].as<glm::vec3>().y);
				glm::vec4 colour = tileNode["Colour"].as<glm::vec4>();

				tilemap->AddTile(pos, NULL, colour);
			}
		}
	}


	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		return false;
	}
}
