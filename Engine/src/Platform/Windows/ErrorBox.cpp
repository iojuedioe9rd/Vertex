#include "vxpch.h"
#include "Vertex/ErrorBox.h"

VERTEX_API void ErrorBox(std::string message)
{
	MessageBoxA(NULL, message.c_str(), "Error", MB_SYSTEMMODAL | MB_ICONERROR);
}