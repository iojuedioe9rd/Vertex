#pragma once
#include "../../Entity.h"
#include <Vertex/Renderer/Font.h>
#include <Vertex/Renderer/Renderer2D.h>


namespace Vertex
{
	class VERTEX_API ENTPropText : public Entity
	{
	public:
		ENTPropText()
		{
			
		}
		ENTPropText(std::string name, Scene* scene) : Entity(name, scene)
		{

		}

		virtual void Update(Timestep& ts) override;
		virtual void Draw(Timestep& ts)   override;

		~ENTPropText();


		virtual SerializationObject Serialize() override
		{
			SerializationObject obj = Entity::Serialize();
			
			obj.Set("Text", SerializationType::String, text);



			return obj;
		}

		virtual bool DeSerialize(SerializationObject obj) override
		{
			Entity::DeSerialize(obj);
			return true;
		}
	public:
		std::string text = "";
		Ref<Font> font = Font::GetDefault();
		Renderer2D::TextParams& textParams = Renderer2D::TextParams{};
	};

	class VERTEX_API ENTPropText2D : public ENTPropText
	{
	public:
		ENTPropText2D()
		{

		}
		ENTPropText2D(std::string name, Scene* scene) : ENTPropText(name, scene)
		{
			imgui_text_data = (char*)malloc(255);
			for (size_t i = 0; i < 255; i++)
			{
				imgui_text_data[i] = '\0';
			}
		}
		~ENTPropText2D();

		virtual void Update(Timestep& ts) override
		{

		}
		virtual void Draw(Timestep& ts)   override;

		virtual void EventH(Event& e) override
		{

		}

		char* imgui_text_data;

		VX_ENT_GET_ENT_NAME_DEF(prop_text_2d);

	
	};

	class VERTEX_API ENTPropText3D : ENTPropText
	{
	public:
		ENTPropText3D()
		{

		}
		ENTPropText3D(std::string name, Scene* scene) : ENTPropText(name, scene)
		{

		}
		~ENTPropText3D();

		VX_ENT_GET_ENT_NAME_DEF(prop_text_3d);
		
	protected:


	};
}