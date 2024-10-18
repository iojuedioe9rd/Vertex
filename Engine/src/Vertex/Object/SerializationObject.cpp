#include "vxpch.h"
#include "SerializationObject.h"

namespace Vertex
{
	void SerializationObject::Add(std::string name, int data)
	{
		fields[name] = data;
		DataTypes[name] = SerializationObjectType::Int;
	}

	void SerializationObject::Add(std::string name, glm::vec2 data)
	{
		fields[name] = data;
		DataTypes[name] = SerializationObjectType::Vec2;
	}

	void SerializationObject::Add(std::string name, glm::vec3 data)
	{
		fields[name] = data;
		DataTypes[name] = SerializationObjectType::Vec3;
	}

	void SerializationObject::Add(std::string name, glm::vec4 data)
	{
		fields[name] = data;
		DataTypes[name] = SerializationObjectType::Vec4;
	}

	void SerializationObject::Add(std::string name, float data)
	{
		fields[name] = data;
		DataTypes[name] = SerializationObjectType::Float;
	}

	void SerializationObject::Add(std::string name, std::string data)
	{
		fields[name] = data;
		DataTypes[name] = SerializationObjectType::String;
	}

	SerializationObject::SerializationObjectType SerializationObject::GetType(std::string name) const
	{
		auto it = DataTypes.find(name);
		if (it != DataTypes.end())
		{
			return it->second;
		}
		throw std::runtime_error("Field not found");
	}
}
