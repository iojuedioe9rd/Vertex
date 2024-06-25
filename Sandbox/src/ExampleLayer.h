#pragma once
#include "Vertex.h"

class ExampleLayer : public Vertex::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate() override;
	virtual void OnImGuiRender() override;
	void OnEvent(Vertex::Event& e) override;

private:

};

