#include "stdafx.h"
#include "SynchronizedMap.h"

void Map::SynchronizedMap::Synchronize()
{
	unsigned FXMode = static_cast<unsigned>(TileGlowTime / 1.5f);

	// Synchronizes tile effects
	for (auto elem : Tiles)
	{
		Object::ASquareTile* elemSquare = dynamic_cast<Object::ASquareTile*>(elem);
		Object::AEntranceTile* elemEntrance = dynamic_cast<Object::AEntranceTile*>(elem);
		Object::AMuseDashTile* elemMuseDash = dynamic_cast<Object::AMuseDashTile*>(elem);
		if (elemSquare && !elemEntrance && !elemMuseDash)
		{
			if (NormL1Distance(elemSquare->GetPosition()) % 2 == FXMode)
				elemSquare->SetEffectContent("effect_topglow_yellow");
			else
				elemSquare->SetEffectContent("effect_topglow_white");
		}
	}
}