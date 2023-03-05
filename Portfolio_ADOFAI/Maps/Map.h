#pragma once
#include "Objects/Tiles.h"

namespace Object { class APlayer; }

namespace Map
{
	class IMap abstract
	{
	protected:
		virtual void LinkTiles() abstract;
	
	public:
		virtual Object::ATile* GetTileAt(vector<2> position) const abstract;
		virtual vector<2> FindDestination(Object::APlayer* pPlayer) abstract;
	};
}