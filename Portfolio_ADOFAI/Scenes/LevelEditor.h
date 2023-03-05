#pragma once
#include "Level.h"
#include "Maps/EditableMap.h"

enum class IMPORT_RESOURCE_TYPE
{
	BACKGROUND,
	IMAGE,
	SOUND
};

class LevelEditor final : public Level
{
private:
	vector<2>				CameraMoveBasis;
	vector<2>				CameraPosBasis;
	bool					isCameraFollow;

	Image::Component		CreateDirection;
	Image::Component		CreateDirectionKeys[24];
	Image::Component		CreateDirectionOpt[6];

public:
	LevelEditor(unsigned short bpm, float movement_unit);
	virtual ~LevelEditor();
	virtual void Start() override;
	virtual bool Update() override;
	virtual void End() override;

	virtual std::string GetTileTheme() const;

private:
	void Operation();

	void EditLevelInfo();
	void EditTileInfo();
	void Export() const;
	void Import() override;

	void ImportResource(IMPORT_RESOURCE_TYPE resource_type);

	std::string InputSpacedString() const;
	std::string InputSelectNumber(unsigned start, unsigned end, std::string turnback = "") const;
	void PrintLevelInfo() const;
	void PrintTileInfo(Object::AUnidirectionalTile* pSelectedTile) const;
	void PrintTermination() const;
	void PrintInappropriateInput() const;

	void ResetCameraZoom();
	void ResetMap();
	void ResetLevel();
};