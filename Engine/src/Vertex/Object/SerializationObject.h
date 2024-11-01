#pragma once
#include "Vertex/Core/Base.h"
#include <map>
#include <variant>
#include <glm/glm.hpp>

namespace Vertex
{
	class VERTEX_API SerializationObject
	{
	public:
		using VariantType = std::variant<int, glm::vec2, glm::vec3, glm::vec4, float, std::string, bool, double>;
		enum class VERTEX_API SerializationObjectType : int32_t
		{
			Int = BIT(0),
			Vec2 = BIT(1),
			Vec3 = BIT(2),
			Vec4 = BIT(3),
			Float = BIT(4),
			String = BIT(5),
			Bool = BIT(6),
			Double = BIT(7),
		};

		SerializationObject() = default;
		~SerializationObject() = default;

		void Add(std::string name, int data);
		void Add(std::string name, glm::vec2 data);
		void Add(std::string name, glm::vec3 data);
		void Add(std::string name, glm::vec4 data);
		void Add(std::string name, float data);
		void Add(std::string name, std::string data);

		void Add(std::string name, bool data) {
			m_Fields[name] = data;
			m_DataTypes[name] = SerializationObjectType::Bool;
		}

		void Add(std::string name, double data) {
			m_Fields[name] = data;
			m_DataTypes[name] = SerializationObjectType::Double;
		}

		template <typename T>
		T Get(std::string name) const
		{
			auto it = m_Fields.find(name);
			if (it != m_Fields.end())
			{
				return std::get<T>(it->second);
			}
			throw std::runtime_error("Field not found or wrong type");
		}

		bool HasField(std::string name) const
		{
			auto it = m_Fields.find(name);
			if (it != m_Fields.end())
			{
				return true;
			}
			return false;
		}

		std::map<std::string, VariantType> GetFields()
		{
			return m_Fields;
		}

		std::map<std::string, SerializationObjectType> GetDataTypes()
		{
			return m_DataTypes;
		}

		SerializationObjectType GetType(std::string name) const;

		

	private:
		
		std::map<std::string, VariantType> m_Fields;  // Store data
		std::map<std::string, SerializationObjectType> m_DataTypes;  // Store types
	};
}
