#pragma once
#include "Vertex/Core/Base.h"
#include "Vertex/Object/Object.h"
#include <map>
#include <variant>
#include <glm/glm.hpp>
#include <any>
#include <vector>

namespace Vertex
{
	enum class VERTEX_API SerializationType : uint8_t
	{
		None = 0,
		Bool = 1,
		Int8 = 2,
		Int16 = 3,
		Int32 = 4,
		Int64 = 5,
		Int = Int32,
		Uint8 = 6,
		Uint16 = 7,
		Uint32 = 8,
		Uint64 = 9,
		Uint = Uint32,
		Float = 10,
		Double = 11,
		String = 12,
		Vector2 = 13,
		Vector3 = 14,
		Vector4 = 15,
		Vector2Int = 16,
		Vector3Int = 17,
		Vector4Int = 18,
		Colour = 19,
		SerializationObject = 20,
		SerializationObjectArray = 21
	};

	

	class VERTEX_API SerializationObject : public Object
	{
	public:
		
		

		struct VERTEX_API SerializationData
		{
			std::string name;
			SerializationType type;
			std::any value;
		};
		
		SerializationObject() = default;
		SerializationObject(const SerializationObject& other) = default;
		
		SerializationData Get(std::string name)
		{
			for (auto& data : m_data)
			{
				if (data.name == name)
				{
					return data;
				}
			}
			return SerializationData{"", SerializationType::None, 0};
		}

		std::vector< SerializationData> GetAll() const
		{
			return m_data;
		}

		template<typename T>
		void Set(std::string name, SerializationType type, T value)
		{
			for (auto& data : m_data)
			{
				if (data.name == name)
				{
					data.value = value;
					return;
				}
			}
			m_data.push_back({ name, type, value });
		}

		bool Remove(std::string name)
		{
			for (auto it = m_data.begin(); it != m_data.end(); it++)
			{
				if (it->name == name)
				{
					m_data.erase(it);
					return true;
				}
			}
			return false;
		}

		bool Contains(std::string name)
		{
			for (auto& data : m_data)
			{
				if (data.name == name)
				{
					return true;
				}
			}
			return false;
		}

		template<typename T>
		T Get(std::string name, SerializationType type)
		{
			for (auto& data : m_data)
			{
				if (data.name == name && data.type == type)
				{
					return std::any_cast<T>(data.value);
				}
			}
			return T();
		}
	private:
		std::vector< SerializationData> m_data;
		
	public:

		
	};

	class VERTEX_API SerializationObjectArray : public Object
	{
	public:
		SerializationObjectArray() = default;
		SerializationObjectArray(const SerializationObjectArray& other) = default;
		SerializationObjectArray(std::vector<SerializationObject> data)
			: m_data(data)
		{
		}
		std::vector<SerializationObject> GetAll() const
		{
			return m_data;
		}
		void Add(SerializationObject data)
		{
			m_data.push_back(data);
		}
		void Remove(SerializationObject data)
		{
			for (auto it = m_data.begin(); it != m_data.end(); it++)
			{
				if (*it == data)
				{
					m_data.erase(it);
					return;
				}
			}
		}
		bool Contains(SerializationObject data) const
		{
			for (auto& d : m_data)
			{
				if (d == data)
					return true;
			}
			return false;
		}

		int Size() const
		{
			return m_data.size();
		}

	private:
		std::vector<SerializationObject> m_data;
	};
}
