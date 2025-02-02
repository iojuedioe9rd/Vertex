#pragma once
#include "Vertex/Core/Base.h"
#include <Vertex/Core/Timestep.h>
#include "Vertex/Renderer/Texture.h"


namespace Vertex
{

	struct VERTEX_API ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColourBegin, ColourEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;

		Ref<Texture2D> Texture = 0;
	};

	class VERTEX_API ParticleSystem
	{
	public:
		ParticleSystem();
		~ParticleSystem() = default;
		void OnUpdate(Timestep& deltaTime);

		void Emit(const ParticleProps& particleProps);
	private:
		struct VERTEX_API Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColourBegin, ColourEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			Ref<Texture2D> Texture;

			bool Active = false;
		};
		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;

		friend class Renderer2D;

		
	};
}