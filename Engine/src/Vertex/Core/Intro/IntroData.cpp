#include <vxpch.h>
#include "Vertex/Core/Intro/IntroData.h"
#include "cat_image_data.h"
#include "circle_image_data.h"

namespace Vertex
{
	
	struct IntroData
	{
		Ref<Texture2D> catTex;
		Ref<Texture2D> circleTex;
	};

	static IntroData* s_Data;

	void VERTEX_API SetupIntroData()
	{
		s_Data = new IntroData();
		{
			TextureSpecification catTextureSpecification = TextureSpecification();
			catTextureSpecification.Format = ImageFormat::RGBA8;
			catTextureSpecification.Width = 1024;
			catTextureSpecification.Height = 1024;
		
			Buffer* buffer = new Buffer((void*)cat_image_data, sizeof(cat_image_data));
			s_Data->catTex = Texture2D::Create(catTextureSpecification, buffer);
		}
		
		{
			TextureSpecification circleTextureSpecification = TextureSpecification();
			circleTextureSpecification.Format = ImageFormat::RGBA8;
			circleTextureSpecification.Width = 1024;
			circleTextureSpecification.Height = 1024;

			Buffer* buffer = new Buffer((void*)circle_image_data, sizeof(circle_image_data));
			s_Data->circleTex = Texture2D::Create(circleTextureSpecification, buffer);
		}
	}

	Ref<Texture2D> GetCatTexture()
	{
		return s_Data->catTex;
	}
	Ref<Texture2D> GetCircleTexture()
	{
		return s_Data->circleTex;
	}
}