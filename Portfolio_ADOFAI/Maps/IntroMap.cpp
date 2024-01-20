#include "stdafx.h"
#include "IntroMap.h"

using namespace Rendering;

Map::IntroMap::IntroMap()
{
	// 5 x 5 Ground tiles with center = (0, 0)
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j) { Tiles.push_back(new Object::ASquareTile(CONTENT_SQUARE_TILE, vector<2>{ -144 + 72 * i, 144 - 72 * j })); }

	// 14 Hotizontally located ground tiles
	for (int i = 0; i < 18; ++i)	{ Tiles.push_back(new Object::ASquareTile(CONTENT_SQUARE_TILE, vector<2>{ 216 + 72 * i, -144 })); }

	// Entrance tiles
	{
		Tiles.push_back(new Object::AEntranceTile(CONTENT_SQUARE_TILE,	 vector<2>{ -216,  144 }));		// Level Editor
		Tiles.push_back(new Object::AEntranceTile(CONTENT_SQUARE_TILE,	 vector<2>{ -216, -144 }));		// Custom Level
		Tiles.push_back(new Object::AEntranceTile(CONTENT_SQUARE_TILE,	 vector<2>{  936, -72 }));		// Level1
		Tiles.push_back(new Object::AEntranceTile(CONTENT_SQUARE_TILE,	 vector<2>{ 1440, -72 }));		// Level2
		Tiles.push_back(new Object::AMuseDashTile("tile_crystal_musedash", vector<2>{  576, -72 }));		// Muse Dash
	}

	LinkTiles();
}

Map::IntroMap::~IntroMap()
{
	for (auto elem : Tiles)
		delete elem;

	Tiles.clear();
}

void Map::IntroMap::Start()
{
	for (auto elem : Tiles)
	{
		elem->Start();

		if (length(elem->GetPosition()) <= sqrt(2) * MOVEMENT_UNIT + 0.1f)
			elem->SetAlphaLevel(1.0f);
		else
			elem->SetAlphaLevel(0.0f);
	}

	isShowAll = false;
}

void Map::IntroMap::Update()
{
	TileGlowTime += DELTA_TIME;
	AdjustFloat(TileGlowTime, 3.0f);

	Synchronize();

	for (auto elem : Tiles)
	{
		if (isShowAll)
		{
			{
				float alphaLevel = elem->GetAlphaLevel() + 4.0f * DELTA_TIME;
				elem->SetAlphaLevel(BoundedFloat(alphaLevel, 0.0f, 1.0f));
			}
		}
		else
		{
			if (length(elem->GetPosition()) > 1.5f * MOVEMENT_UNIT)
			{
				float alphaLevel = elem->GetAlphaLevel() - 4.0f * DELTA_TIME;
				elem->SetAlphaLevel(BoundedFloat(alphaLevel, 0.0f, 1.0f));
			}
		}
		elem->Update();
	}

	for (auto elem : Tiles)
		if (elem->IsEffectOn()) elem->PresentEffect();
}

void Map::IntroMap::End() {}

void Map::IntroMap::SetShowAll(bool isShowAll) { this->isShowAll = isShowAll; }