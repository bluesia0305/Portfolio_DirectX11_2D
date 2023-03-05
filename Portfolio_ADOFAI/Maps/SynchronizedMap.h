#pragma once
#include "TileMap.h"

namespace Map
{
	class SynchronizedMap : public TileMap
	{
	protected:
		float TileGlowTime;

	protected:
		virtual void Synchronize() final;
	};
}