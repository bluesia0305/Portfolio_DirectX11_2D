#pragma once
#include "UnidirectionalMap.h"

enum class OPERATION_MODE;
enum class SELECT_TILE
{
	BEGIN_TILE = 0,
	PREV_TILE = -1,
	NEXT_TILE = 1,
	END_TILE = 2,
};

namespace Map
{
	class EditableMap final : public UnidirectionalMap
	{
	protected:
		OPERATION_MODE						OperationMode;
		std::list<Object::ATile*>::iterator	SelectedTile;
		Image::Component					SelectionEffect;

	public:
		EditableMap();
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;

		virtual void Export() const;
		virtual void Import() override;
		
		virtual Object::ATile* GetSelectedTile() const;
		virtual void SelectTile(SELECT_TILE type);
		virtual void PrintTileInfo() const;

		virtual void AddTile(float creation_angle, bool isPassingTile = false);
		virtual void RemoveTile();
		virtual void AdjustedRemoveTile();
		virtual void ReadjustTile();

		virtual void SetTileTheme(std::string tile_theme);
		virtual void SetHitSound(std::string tile_theme);
		virtual void SetMagnification(float magnification);
		virtual void Synchronize();
	};
}