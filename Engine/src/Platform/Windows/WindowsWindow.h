#pragma once

#include "Vertex/Core/Window.h"
#include "Vertex/Debug/Instrumentor.h"
#include <GLFW/glfw3.h>
#include <Vertex/Renderer/GraphicsContext.h>

namespace Vertex {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { VX_PROFILE_FUNCTION(); return m_Data.Width; }
		inline unsigned int GetHeight() const override { VX_PROFILE_FUNCTION(); return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return m_Window; }

		uint8_t* test_data;
		uint32_t tex_handle;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}