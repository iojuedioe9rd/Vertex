#pragma once
#include "Vertex/Core/Base.h"
#include "Vertex/AssetManager/Asset.h"
#include <string>


namespace Vertex
{

	struct VERTEX_API AudioSpecification
	{
		bool Looped = false;
		float Pitch = 1.0;
		float Gain = 1.0;
		glm::vec3 Position = glm::vec3(0, 0, 0);
		glm::vec3 Velocity = glm::vec3(0, 0, 0);
	};

	class VERTEX_API Audio : public Asset
	{
	public:
		virtual ~Audio() {};
		

		virtual void Play() = 0;
		virtual void Stop() = 0;

		static Audio* Create(std::filesystem::path filepath, AudioSpecification& specification = AudioSpecification{});

		virtual AudioSpecification Get() = 0;

		virtual bool IsLooped()			= 0;
		virtual float GetPitch()		= 0;
		virtual float GetGain()			= 0;
		virtual glm::vec3 GetPosition() = 0;
		virtual glm::vec3 GetVelocity() = 0;

		virtual void SetPitch(float pitch)			 = 0;
		virtual void SetGain(float gain)			 = 0;
		virtual void SetPosition(glm::vec3 Position) = 0;
		virtual void SetVelocity(glm::vec3 Velocity) = 0;
		virtual void SetLooped(bool Looped)			 = 0;

		virtual AssetType GetType() const override { return AssetType::Audio; }
	};

	

}