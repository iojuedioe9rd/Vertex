#pragma once
#include "Vertex/Audio/Audio.h"

namespace Vertex
{
	class WindowsAudio : public Audio
	{
	public:
		WindowsAudio(std::filesystem::path filepath, AudioSpecification& specification);

		~WindowsAudio();

		virtual void Play() override;
		virtual void Stop() override;

		virtual AudioSpecification Get() override;

		virtual bool IsLooped() override;
		virtual float GetPitch() override;
		virtual float GetGain() override;
		virtual glm::vec3 GetPosition() override;
		virtual glm::vec3 GetVelocity() override;

		virtual void SetPitch(float pitch)			 override;
		virtual void SetGain(float gain)			 override;
		virtual void SetPosition(glm::vec3 Position) override;
		virtual void SetVelocity(glm::vec3 Velocity) override;
		virtual void SetLooped(bool Looped)			 override;

	private:
		AudioSpecification m_specification;
		ALuint m_Source;
		ALuint m_Buffer = 0;
		std::filesystem::path m_filepath;

		static uint16_t s_RefCount;
	};

}