#pragma once
#include "TileMap.h"

namespace Map
{
	class CustomLevelMap final : public TileMap
	{
	public:
		CustomLevelMap(int num_custom_levels);
		virtual ~CustomLevelMap();
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;
	};
}