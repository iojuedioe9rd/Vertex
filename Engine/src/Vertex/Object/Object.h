#pragma once
#include "Vertex/Core/UUID.h"
#include "Vertex/Core/Base.h"

namespace Vertex {

	class VERTEX_API Object
	{
	private:
		UUID m_id;

	public:
		bool operator==(const Object& other) const {
			return other.m_id == this->m_id;
		}

		Object();
		UUID GetID();
		void Destroy();
		void RegenID();
	};

}