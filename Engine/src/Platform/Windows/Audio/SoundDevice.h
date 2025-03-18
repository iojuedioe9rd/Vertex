#pragma once
#include "Vertex/Core/Base.h"


struct ALCdevice;
struct ALCcontext;

typedef char ALCchar;

namespace Vertex
{
	class VERTEX_API WindowsSoundDevice
	{
	public:
		static WindowsSoundDevice* get();
		static void destroy();

	private:
		const ALCchar* m_Name = nullptr;

		WindowsSoundDevice();
		~WindowsSoundDevice();
		ALCdevice* m_Device;
		ALCcontext* m_Context;
		static WindowsSoundDevice* m_Instance;
	};
}