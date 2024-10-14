#pragma once
#include "Vertex/Core/Base.h"
#include <string>


namespace Vertex
{

	class VERTEX_API Audio
	{
	public:
		
		

		virtual void Play() = 0;
		virtual void Stop() = 0;
		virtual void SetVolume(float volume) = 0;

		static Audio* Create(const char* filepath, bool loop = 0);
		static Audio* Create(int resourceId, bool loop = 0);
	private:

	};

	

}