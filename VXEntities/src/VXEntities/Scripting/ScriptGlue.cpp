#include "ScriptGlue.h"

#include "mono/metadata/object.h"
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <Vertex/Core/Logger.h>


namespace Vertex
{
#define VX_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Vertex.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}
	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		glm::vec3 NewParameter = *parameter;
		VX_CORE_WARN("Value: {0}", NewParameter);
		*outResult = glm::normalize(NewParameter);
	}
	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		glm::vec3 NewParameter = *parameter;
		VX_CORE_WARN("Value: {0}", NewParameter);
		return glm::dot(NewParameter, NewParameter);
	}
	void ScriptGlue::RegisterFunctions()
	{
		VX_ADD_INTERNAL_CALL(NativeLog);
		VX_ADD_INTERNAL_CALL(NativeLog_Vector);
		VX_ADD_INTERNAL_CALL(NativeLog_VectorDot);
	}
}