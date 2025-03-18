#include "vxpch.h"
#include "WindowsAudio.h"
#include <Windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <filesystem>  // Because modern C++ is now playing in C#'s high-level playground.
#include <miniaudio.h>
#include <AL/al.h>
#include "SoundDevice.h"
#include "SoundBuffer.h"
#include <Vertex/AssetManager/AssetManager.h>
#include <Vertex/Audio/AudioManager.h>

#pragma once(lib, Winmm.lib)

bool isInit = 0;

uint16_t Vertex::WindowsAudio::s_RefCount = 0;



// TODO: Use OpenAL

namespace Vertex {

	// Constructor: Welcome to modern C++—trying to act like C#.
	WindowsAudio::WindowsAudio(std::filesystem::path filepath, AudioSpecification& specification)
		: m_filepath(filepath), m_specification(specification)
	{
	
		{
			s_RefCount++;
			WindowsSoundDevice::get();
			WindowsSoundBuffer::get();
		}

		

		alGenSources(1, &m_Source);
		alSourcef(m_Source, AL_PITCH, specification.Pitch);
		alSourcef(m_Source, AL_GAIN, specification.Gain);
		alSource3f(m_Source, AL_POSITION, specification.Position[0], specification.Position[1], specification.Position[2]);
		alSource3f(m_Source, AL_VELOCITY, specification.Velocity[0], specification.Velocity[1], specification.Velocity[2]);
		alSourcei(m_Source, AL_LOOPING, specification.Looped);
		m_Buffer = WindowsSoundBuffer::get()->addSoundEffect(m_filepath);
		alSourcei(m_Source, AL_BUFFER, (ALint)m_Buffer);
		
	}

	// Destructor: Cleaning up like a responsible modern C++ developer who still does manual cleanup.
	WindowsAudio::~WindowsAudio()
	{
		s_RefCount--;
		alDeleteSources(1, &m_Source);

		if (s_RefCount <= 0)
		{
			WindowsSoundDevice::destroy();
			WindowsSoundBuffer::destroy();

		}
	}

	
	void WindowsAudio::Play()
	{
		alSourcePlay(m_Source);
	}

	
	void WindowsAudio::Stop()
	{
		alSourceStop(m_Source);
	}

	AudioSpecification WindowsAudio::Get()
	{
		return m_specification;
	}

	bool WindowsAudio::IsLooped()
	{
		return m_specification.Looped;
	}

	float WindowsAudio::GetPitch()
	{
		return m_specification.Pitch;
	}

	float WindowsAudio::GetGain()
	{
		return m_specification.Gain;
	}

	glm::vec3 WindowsAudio::GetPosition()
	{
		return m_specification.Position;
	}

	glm::vec3 WindowsAudio::GetVelocity()
	{
		return m_specification.Velocity;
	}

	void WindowsAudio::SetPitch(float pitch)
	{
		m_specification.Pitch = pitch;
		alSourcef(m_Source, AL_PITCH, m_specification.Pitch);
	}

	void WindowsAudio::SetGain(float gain)
	{
		m_specification.Gain = gain;
		alSourcef(m_Source, AL_GAIN, m_specification.Gain);
	}

	void WindowsAudio::SetPosition(glm::vec3 Position)
	{
		m_specification.Position = Position;
		alSource3f(m_Source, AL_POSITION, m_specification.Position[0], m_specification.Position[1], m_specification.Position[2]);
		
	}

	void WindowsAudio::SetVelocity(glm::vec3 Velocity)
	{
		m_specification.Velocity = Velocity;
		alSource3f(m_Source, AL_VELOCITY, m_specification.Velocity[0], m_specification.Velocity[1], m_specification.Velocity[2]);
	}

	void WindowsAudio::SetLooped(bool Looped)
	{
		m_specification.Looped = Looped;
		alSourcei(m_Source, AL_LOOPING, m_specification.Looped);
	}

	

}
