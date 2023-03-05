#include "stdafx.h"
#include "TileMap.h"

Map::TileMap::~TileMap()
{
	for (auto elem : Tiles)
		delete elem;

	Tiles.clear();
}
void Map::TileMap::Start() {}
void Map::TileMap::Update() {}
void Map::TileMap::End() {}
void Map::TileMap::LinkTiles()
{
	for (auto tiles : Tiles)
	{
		for (auto adjacent_tiles : Tiles)
		{
			if (L1Distance(tiles->GetPosition() - adjacent_tiles->GetPosition()) == MOVEMENT_UNIT)
				tiles->Link(adjacent_tiles);
		}
	}
}

Object::ATile* Map::TileMap::GetTileAt(vector<2> position) const
{
	for (auto elem : Tiles)
		if (!L1Distance(elem->GetPosition() - position)) return elem;

	return nullptr;
}

vector<2> Map::TileMap::FindDestination(Object::APlayer* pPlayer)
{
	if (GetTileAt(pPlayer->GetPosition())->Decision(pPlayer))
		return dynamic_cast<Object::ASquareTile*>(GetTileAt(pPlayer->GetPosition()))->GetDestination();
	else
		return pPlayer->GetPosition();
}