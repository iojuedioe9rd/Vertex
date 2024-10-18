#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include "Entity.h"
#include "../../VXEntities.h"

namespace YAML {
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

		if (entity->GetEntName() == "prop_static_sprite")
		{
			ENTPropStaticSprite* sprite = static_cast<ENTPropStaticSprite*>(entity);
			VX_CORE_ASSERT(sprite != nullptr, "sprite is null!");
			out << YAML::Key << "PropStaticSprite";
			out << YAML::BeginMap;
			out << YAML::Key << "Colour" << YAML::Value << sprite->colour;
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
				// Log error: Failed to create entity from ID
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

		// Deserialization for specific entity types
		if (entity->GetEntName() == "prop_static_sprite") {
			ENTPropStaticSprite* sprite = static_cast<ENTPropStaticSprite*>(entity);
			if (node["PropStaticSprite"]["Colour"]) {
				sprite->colour = node["PropStaticSprite"]["Colour"].as<glm::vec4>(); // Assuming colour is a glm::vec4
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
