#include <vxpch.h>
#include <AL/alc.h>
#include "SoundDevice.h"


namespace Vertex
{
	WindowsSoundDevice* WindowsSoundDevice::m_Instance = nullptr;

	WindowsSoundDevice* WindowsSoundDevice::get()
	{
		if (m_Instance == nullptr) { m_Instance = new WindowsSoundDevice(); }
		return m_Instance;
	}
	void WindowsSoundDevice::destroy()
	{
		if (m_Instance == nullptr) { return; }
		delete m_Instance;
		m_Instance = nullptr;
	}
	WindowsSoundDevice::WindowsSoundDevice()
	{
		m_Device = alcOpenDevice(NULL);
		if (!m_Device)
		{
			VX_CORE_ERROR("Failed to open OpenAL device");
		}
		m_Context = alcCreateContext(m_Device, NULL);
		if (!m_Context)
		{
			VX_CORE_ERROR("Failed to create OpenAL context");
		}
		if (!alcMakeContextCurrent(m_Context))
		{
			VX_CORE_ERROR("Failed to make OpenAL context current");
		}

		
		if (alcIsExtensionPresent(m_Device, "ALC_ENUMERATE_ALL_EXT"))
		{
			m_Name = alcGetString(m_Device, ALC_ALL_DEVICES_SPECIFIER);
		}
		if (!m_Name || alcGetError(m_Device) != ALC_NO_ERROR)
		{
			m_Name = alcGetString(m_Device, ALC_DEVICE_SPECIFIER);
		}
		VX_CORE_INFO("Opened OpenAL device: {0}", m_Name);
	}
	WindowsSoundDevice::~WindowsSoundDevice()
	{
		printf("Opened OpenAL device: %s", m_Name);
		//VX_CORE_INFO("Closeing OpenAL device: {0}", m_Name);

		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_Context);
		alcCloseDevice(m_Device);
	}
}