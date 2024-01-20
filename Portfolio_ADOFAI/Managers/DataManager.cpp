#include "stdafx.h"
#include "Managers/DataManager.h"
#include <fstream>

DataManager::DataManager()
{
	PlayerInfo = new FPlayerInfo();
	LevelInfo  = new FLevelInfo();
	TilesInfo  = new FTilesInfo();
}

DataManager::~DataManager()
{
	delete PlayerInfo;
	delete LevelInfo;
	delete TilesInfo;
}

void DataManager::ClearTilesInfo()
{
	TilesInfo->NumTiles = 0;
	TilesInfo->TileTheme.clear();
	TilesInfo->ContentTile.clear();
	TilesInfo->ContentIcon1.clear();
	TilesInfo->ContentIcon2.clear();
	TilesInfo->MainPosition.clear();
	TilesInfo->Angle.clear();
	TilesInfo->BentAngle.clear();
	TilesInfo->isPassingTile.clear();
	TilesInfo->isReversalTile.clear();
	TilesInfo->Magnification.clear();
	TilesInfo->HitSound.clear();
}

void DataManager::ExportPlayer()
{
	std::ofstream fout;
	fout.open("Data/Player.PlayerInfo", std::ios::out);

	if (fout.is_open())
	{
		fout << PlayerInfo->ContentPlanet[0] << std::endl;
		fout << PlayerInfo->ContentPlanet[1] << std::endl;
		fout << PlayerInfo->ContentCometTail[0] << std::endl;
		fout << PlayerInfo->ContentCometTail[1] << std::endl;
		fout << PlayerInfo->isRotating << std::endl;
	}
	else
	{
		printf("Data/Player.PlayerInfo 파일이 정상적으로 생성되지 않았습니다.\n");
		return;
	}

	fout.close();
}

bool DataManager::ExportLevel(std::string file_basename)
{
	std::ofstream fout;
	std::string file = "Data/CustomLevels/" + file_basename + ".Level";
	fout.open(file.c_str(), std::ios::out);

	if (fout.is_open())
	{
		fout << LevelInfo->ContentBGM					 << std::endl;
		fout << LevelInfo->ContentBG					 << std::endl;
		fout << LevelInfo->ContentPortal				 << std::endl;
		fout << LevelInfo->TileTheme					 << std::endl;
		fout << LevelInfo->HitSound						 << std::endl;
		fout << LevelInfo->BGMArtist					 << std::endl;
		fout << LevelInfo->BGMName						 << std::endl;

		fout << static_cast<int>(LevelInfo->TextColor.R) << std::endl;
		fout << static_cast<int>(LevelInfo->TextColor.G) << std::endl;
		fout << static_cast<int>(LevelInfo->TextColor.B) << std::endl;
		fout << LevelInfo->BPM							 << std::endl;
		fout << LevelInfo->BGMOffset					 << std::endl;
		fout << LevelInfo->CountTicks					 << std::endl;
		fout << LevelInfo->isCleared					 << std::endl;

		for (unsigned i = 0; i < TilesInfo->NumTiles; ++i)
		{
			fout << TilesInfo->TileTheme[i]			<< std::endl;
			fout << TilesInfo->ContentTile[i]		<< std::endl;
			fout << TilesInfo->ContentIcon1[i]		<< std::endl;
			fout << TilesInfo->ContentIcon2[i]		<< std::endl;
			fout << TilesInfo->HitSound[i]			<< std::endl;
			fout << TilesInfo->MainPosition[i][0]	<< std::endl;
			fout << TilesInfo->MainPosition[i][1]	<< std::endl;
			fout << TilesInfo->Angle[i][0]			<< std::endl;
			fout << TilesInfo->Angle[i][1]			<< std::endl;
			fout << TilesInfo->Angle[i][2]			<< std::endl;
			fout << TilesInfo->BentAngle[i]			<< std::endl;
			fout << TilesInfo->Magnification[i]		<< std::endl;
			fout << TilesInfo->isPassingTile[i]		<< std::endl;
			fout << TilesInfo->isReversalTile[i]	<< std::endl;
		}
		ClearTilesInfo();
	}
	else
	{
		printf("%s 파일이 정상적으로 생성되지 않았습니다.\n", file.c_str());
		return false;
	}
	fout.close();
	return true;
}

void DataManager::UpdateLevel(std::string file_basename)
{
	//ImportLevel(file_basename, "CustomLevels");
	//LevelInfo->isCleared = true;
	//ExportLevel(file_basename);
}

bool DataManager::ImportPlayer()
{
	std::string   filename = "Data/Player.PlayerInfo";
	std::ifstream fin;
	fin.open(filename, std::ios::in);

	if (fin.is_open())
	{
		fin >> PlayerInfo->ContentPlanet[0];
		fin >> PlayerInfo->ContentPlanet[1];
		fin >> PlayerInfo->ContentCometTail[0];
		fin >> PlayerInfo->ContentCometTail[1];
		fin >> PlayerInfo->isRotating;
	}
	else
	{
		printf("PlayerInfo 파일이 존재하지 않습니다 (최초 실행 이후 커스텀 세팅 적용 전에 발생할 수 있는 메세지 입니다).\n");
		return false;
	}
	fin.close();
	return true;
}

bool DataManager::ImportLevel(std::string file_basename, std::string sub_directory_name)
{
	std::ifstream fin;
	std::string file = "Data/" + sub_directory_name + "/" + file_basename + ".Level";

	fin.open(file.c_str(), std::ios::in);

	if (fin.is_open())
	{
		int color_r;
		int color_g;
		int color_b;

		getline(fin, LevelInfo->ContentBGM);
		getline(fin, LevelInfo->ContentBG);
		getline(fin, LevelInfo->ContentPortal);
		getline(fin, LevelInfo->TileTheme);
		getline(fin, LevelInfo->HitSound);
		getline(fin, LevelInfo->BGMArtist);
		getline(fin, LevelInfo->BGMName);
		fin.clear();

		fin >> color_r;
		fin >> color_g;
		fin >> color_b;

		LevelInfo->TextColor =
		{
			static_cast<unsigned char>(color_r),
			static_cast<unsigned char>(color_g),
			static_cast<unsigned char>(color_b)
		};

		fin >> LevelInfo->BPM;
		fin >> LevelInfo->BGMOffset;
		fin >> LevelInfo->CountTicks;
		fin >> LevelInfo->isCleared;

		while (fin)
		{
			std::string		tile_theme;
			std::string		content_tile;
			std::string		content_icon1;
			std::string		content_icon2;
			std::string		hitSound;
			vector<2>		main_pos;
			vector<3>		angle;
			float			bent_angle;
			float			magnification;
			bool			isPassingTile;
			bool			isReversalTile;

			fin >> tile_theme;
			if (tile_theme == "") break;
			fin >> content_tile;
			fin >> content_icon1;
			fin >> content_icon2;
			fin >> hitSound;

			fin >> main_pos[0];
			fin >> main_pos[1];
			fin >> angle[0];
			fin >> angle[1];
			fin >> angle[2];
			fin >> bent_angle;
			fin >> magnification;

			fin >> isPassingTile;
			fin >> isReversalTile;

			TilesInfo->TileTheme.push_back(tile_theme);
			TilesInfo->ContentTile.push_back(content_tile);
			TilesInfo->ContentIcon1.push_back(content_icon1);
			TilesInfo->ContentIcon2.push_back(content_icon2);
			TilesInfo->HitSound.push_back(hitSound);
			TilesInfo->MainPosition.push_back(main_pos);
			TilesInfo->Angle.push_back(angle);
			TilesInfo->BentAngle.push_back(bent_angle);
			TilesInfo->Magnification.push_back(magnification);
			TilesInfo->isPassingTile.push_back(isPassingTile);
			TilesInfo->isReversalTile.push_back(isReversalTile);
			++TilesInfo->NumTiles;
		}
	}
	else
	{
		printf("%s 파일이 존재하지 않습니다.\n", file.c_str());
		return false;
	}
	fin.close();
	return true;
}