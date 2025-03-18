#include "vxpch.h"
#include "Vertex/Core/ErrorBox.h"

// Convert narrow string to wide string
static std::wstring StringToWideString(const std::string& str) {
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wideStr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wideStr[0], size_needed);
	return wideStr;
}

VERTEX_API void ErrorBox(std::string message)
{
#if UNICODE
	MessageBox(NULL, StringToWideString(message).c_str(), L"Error", MB_SYSTEMMODAL | MB_ICONERROR);
#else
	MessageBox(NULL, message.c_str(), "Error", MB_SYSTEMMODAL | MB_ICONERROR);
#endif
}