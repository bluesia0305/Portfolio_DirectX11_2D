#pragma once
#include "Utilities/Singleton.h"
#include "Objects/APlayer.h"
#include "Scenes/LevelEditor.h"

struct TEXT_COLOR;

struct FPlayerInfo
{
	std::string			ContentPlanet[2];
	std::string			ContentCometTail[2];
	bool				isRotating;
};

struct FLevelInfo
{
	std::string			ContentBGM;
	std::string			ContentBG;
	std::string			ContentPortal;
	std::string			TileTheme;
	std::string			HitSound;
	std::string			BGMArtist;
	std::string			BGMName;
	TEXT_COLOR			TextColor;
	unsigned short		BPM;
	float				BGMOffset;
	unsigned short		CountTicks;
	bool				isCleared;
};

struct FTilesInfo
{
	unsigned					NumTiles;
	std::vector<std::string>	TileTheme;
	std::vector<std::string>	ContentTile;
	std::vector<std::string>	ContentIcon1;
	std::vector<std::string>	ContentIcon2;
	std::vector<std::string>	HitSound;
	std::vector<vector<2>>		MainPosition;
	std::vector<vector<3>>		Angle;
	std::vector<float>			BentAngle;
	std::vector<float>			Magnification;
	std::vector<bool>			isPassingTile;
	std::vector<bool>			isReversalTile;
};

class DataManager final : public Singleton<DataManager>
{
public :
	FPlayerInfo*		PlayerInfo;
	FLevelInfo*			LevelInfo;
	FTilesInfo*			TilesInfo;

public :
	DataManager();
	~DataManager();

	void ClearTilesInfo();

	void ExportPlayer();
	bool ExportLevel(std::string file_basename);
	void UpdateLevel(std::string file_basename);

	bool ImportPlayer();
	bool ImportLevel(std::string file_basename, std::string sub_directory_name);
};