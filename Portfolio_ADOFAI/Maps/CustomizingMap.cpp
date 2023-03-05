#include "stdafx.h"
#include "CustomizingMap.h"

Map::CustomizingMap::CustomizingMap()
{
	// 4 x 9 Ground tiles with center = (0, 0)
	for (int i = 0; i < 9; ++i)
		for (int j = 0; j < 4; ++j) { Tiles.push_back(new Object::ASquareTile(CONTENT_SQUARE_TILE, vector<2>{ -288 + 72 * i, -144 + 72 * j })); }

	// Comet tail / rotation customizing tiles
	Tiles.push_back(new Object::ASquareTile(CONTENT_SQUARE_TILE, vector<2>{ -144, -216 }));
	Tiles.push_back(new Object::ASquareTile(CONTENT_SQUARE_TILE, vector<2>{  144, -216 }));
	for (int i = 0; i < 3; ++i)
		Tiles.push_back(new Object::ASquareTile(CONTENT_SQUARE_TILE, vector<2>{ -144 + 144 * i, 144 }));

	// Entrance
	Tiles.push_back(new Object::AMuseDashTile("tile_crystal_musedash", vector<2>{ 0, -216 }));

	LinkTiles();
}

Map::CustomizingMap::~CustomizingMap()
{
	for (auto elem : Tiles)
		delete elem;

	Tiles.clear();
}

void Map::CustomizingMap::Start()
{
	for (auto elem : Tiles)
		elem->SetIsEffectOn(false);
}

void Map::CustomizingMap::Update()
{
	TileGlowTime += DELTA_TIME;
	AdjustFloat(TileGlowTime, 3.0f);

	Synchronize();

	for (auto elem : Tiles)
		elem->Update();

	for (auto elem : Tiles)
		if (elem->IsEffectOn()) elem->PresentEffect();
}

void Map::CustomizingMap::End() {}