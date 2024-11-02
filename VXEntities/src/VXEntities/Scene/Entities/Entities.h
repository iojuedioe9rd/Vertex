#pragma once
#include "prop_static_sprite/prop_static_sprite.h"
#include "prop_dynamic_sprite/prop_dynamic_sprite.h"
#include "env_static_tilemap/env_static_tilemap.h"
#include "point_camera_2d/point_camera_2d.h"
#include "prop_dynamic_sprite/prop_dynamic_sprite.h"
#include "prop_2d_circle/prop_2d_circle.h"
#include "env_script/env_script.h"

namespace Vertex
{
	template<typename... Entity>
	struct EntityGroup
	{
	};

	using AllEntities =
		EntityGroup<ENTPropDynamicSprite,
		ENTPropStaticSprite, ENTEnvStaticTilemap, ENTEnvScript,
		ENTPointCamera2D>;
}