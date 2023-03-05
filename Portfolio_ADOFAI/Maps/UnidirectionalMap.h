#pragma once
#include "TileMap.h"

namespace Map
{
	class UnidirectionalMap : public TileMap
	{
	protected:
		std::list<Object::ATile*>::iterator	CurrentTile;
		std::list<Object::ATile*>::iterator LeadingTile;
		int									LeadingNumber;

	public:
		UnidirectionalMap();
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;
		
		virtual void LinkTiles() override final;
		virtual vector<2> FindDestination(Object::APlayer* pPlayer) override;

		virtual void Import();

		virtual Object::ATile* GetStartTile() const final;
		virtual Object::ATile* GetEndTile() const final;
		virtual Object::ATile* GetCurrentTile() const final;
		virtual Object::ATile* GetPrevTile(Object::ATile* pTile) const;
		virtual Object::ATile* GetNextTile() const final;
		virtual unsigned GetNumTiles() const final;
		virtual void SetDecisionRadius(float radius) final;

	protected:
		virtual void ShowFrontTiles(int leading_number) final;
	};
}