#pragma once

#include "Vertex/Renderer/Camera.h"
#include "Vertex/Object/SerializationObject.h"

namespace Vertex
{

	class VERTEX_API SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }
		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		SerializationObject Serialize() const
		{
			SerializationObject obj = SerializationObject();
			obj.Set("ProjectionType", SerializationType::Int, (int)m_ProjectionType);
			obj.Set("PerspectiveFOV", SerializationType::Float, m_PerspectiveFOV);
			obj.Set("PerspectiveNear", SerializationType::Float, m_PerspectiveNear);
			obj.Set("PerspectiveFar", SerializationType::Float, m_PerspectiveFar);
			obj.Set("OrthographicSize", SerializationType::Float, m_OrthographicSize);
			obj.Set("OrthographicNear", SerializationType::Float, m_OrthographicNear);
			obj.Set("OrthographicFar", SerializationType::Float, m_OrthographicFar);
			obj.Set("AspectRatio", SerializationType::Float, m_AspectRatio);
			return obj;
		}

		bool DeSerialize(SerializationObject obj)
		{
			m_ProjectionType = (ProjectionType)obj.Get<int>("ProjectionType", SerializationType::Int);
			m_PerspectiveFOV = obj.Get<float>("PerspectiveFOV", SerializationType::Float);
			m_PerspectiveNear = obj.Get<float>("PerspectiveNear", SerializationType::Float);
			m_PerspectiveFar = obj.Get<float>("PerspectiveFar", SerializationType::Float);
			m_OrthographicSize = obj.Get<float>("OrthographicSize", SerializationType::Float);
			m_OrthographicNear = obj.Get<float>("OrthographicNear", SerializationType::Float);
			m_OrthographicFar = obj.Get<float>("OrthographicFar", SerializationType::Float);
			m_AspectRatio = obj.Get<float>("AspectRatio", SerializationType::Float);
			RecalculateProjection();
			return true;
		}
	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
		float m_AspectRatio = 0.0f;
	};
}