#include "stdafx.h"
#include "CustomLevelMap.h"

Map::CustomLevelMap::CustomLevelMap(int num_custom_levels)
{
	// Entrance to custom level (first tile in Tiles)
	Tiles.push_back(new Object::AEntranceTile(CONTENT_SQUARE_TILE, vector<2>{ -72, 0 }));
	dynamic_cast<Object::AEntranceTile*>(*Tiles.begin())->SetCustomLevelEntrance();

	// n x 1 Ground tiles with center = (0, 0)
	for (int i = 0; i < num_custom_levels; ++i) { Tiles.push_back(new Object::ASquareTile(CONTENT_SQUARE_TILE, vector<2>{ 0, -72 * i })); }

	// Exit to intro level
	Tiles.push_back(new Object::AMuseDashTile("tile_crystal_green", vector<2>{ 0, 72 }));
	Tiles.push_back(new Object::AMuseDashTile("tile_crystal_green", vector<2>{ 0, -72 * (num_custom_levels) }));

	LinkTiles();
}

Map::CustomLevelMap::~CustomLevelMap()
{
	for (auto elem : Tiles)
		delete elem;

	Tiles.clear();
}

void Map::CustomLevelMap::Start()
{
	for (auto elem : Tiles)
		elem->SetIsEffectOn(false);
}

void Map::CustomLevelMap::Update()
{
	LinkTiles();

	(*Tiles.begin())->SetPosition({ -72, CAMERA->GetPosition()[1] });

	for (auto elem : Tiles)
		elem->Update();

	for (auto elem : Tiles)
		if (elem->IsEffectOn()) elem->PresentEffect();
}

void Map::CustomLevelMap::End() {}