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

	IntUUID Object::GetIntID()
	{
		return m_intUUID;
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
		m_intUUID = IntUUID();
	}
	void Object::SetID(UUID id)
	{
		VX_PROFILE_FUNCTION();
		m_id = id;
	}

	void Object::SetID(IntUUID intUUID)
	{
		VX_PROFILE_FUNCTION();
		m_intUUID = intUUID;
	}
}