#include "vxpch.h"
#include "Object.h"

namespace Vertex {

	Object::Object()
	{
		VX_PROFILE_FUNCTION();
		RegenID();
	}

	UUID Object::GetID()
	{
		VX_PROFILE_FUNCTION();
		return m_id;
	}

	void Object::Destroy()
	{
		VX_PROFILE_FUNCTION();
		m_id = nullptr;
	}

	void Object::RegenID()
	{
		VX_PROFILE_FUNCTION();
		m_id = get_uuid();
	}
}