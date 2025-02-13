#pragma once
#include "Vertex/Core/UUID.h"
#include "Vertex/Core/Base.h"

namespace Vertex {

	class VERTEX_API Object
	{
	private:
		UUID m_id;
		IntUUID m_intUUID;

	public:
		bool operator==(const Object& other) const {
			return other.m_id == this->m_id && other.m_intUUID == this->m_intUUID;
		}

		Object();
		UUID GetID();
		IntUUID GetIntID();
		void Destroy();
		void RegenID();
		void SetID(UUID id);
		void SetID(IntUUID intUUID);
	private:
		
		friend class SceneSerializer;
		friend class Scene;
		friend class Entity;
	};

}