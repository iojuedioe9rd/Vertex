#pragma once
#include "../../Entity.h"
#include "../base_rb2d/base_rb2d.h"

namespace Vertex
{
	class ENTBaseBoxCollier2D : public ENTBaseRigidbody2D
	{
	public:
		
		ENTBaseBoxCollier2D();
		ENTBaseBoxCollier2D(std::string name, Scene* scene) : ENTBaseRigidbody2D(name, scene)
		{

		}

		void SetRuntimeFixture(void* NewRuntimeFixture)
		{
			RuntimeFixture = NewRuntimeFixture;
		}

		~ENTBaseBoxCollier2D();

		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		// TODO: move into physics material in the future maybe
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;


	protected:
		void* RuntimeFixture = nullptr;
	private:

	};


}