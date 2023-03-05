#pragma once
#include "Map.h"

namespace Map
{
	class TileMap abstract : public IMap
	{
	protected:
		std::list<Object::ATile*>	Tiles;

	public:
		virtual ~TileMap();
		virtual void Start() abstract;
		virtual void Update() abstract;
		virtual void End() abstract;

	protected:
		virtual void LinkTiles() override;

	public:
		virtual Object::ATile* GetTileAt(vector<2> position) const override final;
		virtual vector<2> FindDestination(Object::APlayer* pPlayer) override;
	};
}