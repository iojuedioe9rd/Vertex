#pragma once

#include "Scene.h"

namespace YAML
{
	class Emitter;
	class Node;
}

namespace Vertex
{
	class VERTEX_API SceneSerializer
	{
	public:
		SceneSerializer(Scene** scene);
		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);
		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		
		Scene* m_Scene;

		std::string sanitizeID(const std::string& id);
		void SerializeEntity(YAML::Emitter& out, Entity* entity);
		void DeserializeEntity(const YAML::Node& node, Entity* entity);
	};
}