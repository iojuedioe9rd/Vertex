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

		static Audio* Create(const std::string& filepath, bool loop = 0);
	private:

	};

	

}