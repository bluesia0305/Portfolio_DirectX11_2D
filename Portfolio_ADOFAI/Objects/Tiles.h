#pragma once
#include "AActor.h"
#include <cstdarg>

using namespace Rendering;

namespace Object
{
	// Forward declaration;
	class APlayer;
	
	class ATile abstract : public AActor
	{
	protected:
		std::string			ContentTile;
		Image::Component	TileComponent;
		Image::Component	Effect;
		float				DecisionRadius;
		float				Magnification;		// Magnification of RPM
		bool				isReversalTile;		// Revolution direction

		bool				isEffectOn;

	public:
		ATile(vector<2> main_pos, float main_angle = 0.0f);
		ATile(vector<2> main_pos, vector<3> total_angle);
		virtual void Link(ATile* pTile) abstract;
		virtual bool Decision(APlayer* pPlayer) abstract;
		
		virtual std::string GetContentTile() const final;
		virtual vector<2> GetPosition() const final;
		virtual vector<3> GetAngle() const final;
		virtual float GetMainAngle() const final;
		virtual float GetMagnification() const final;
		virtual float GetAlphaLevel() const final;
		virtual bool IsEffectOn() const final;
		virtual bool IsReversalTile() const final;

		virtual void SetPosition(vector<2> main_pos);
		virtual void SetMainAngle(float main_angle);
		virtual void SetAlphaLevel(float alphaLevel);
		virtual void SetIsEffectOn(bool isEffectOn) final;
		virtual void PresentEffect() final;
	};


	class AUnidirectionalTile final : public ATile
	{
	private:
		std::string			TileTheme;
		std::string			ContentIcon[2];
		Image::Component	IconComponent[2];	// 이동효과 부여 아이콘
		float				BentAngle;			// relative angle
		bool				isPassingTile;
		std::string			HitSound;

		ATile*				LinkedTile;

	public:
		AUnidirectionalTile(std::string tile_theme, vector<2> main_pos, float main_angle, float bent_angle);
		AUnidirectionalTile(std::string tile_theme, std::string content_tile, std::string content_icon1, std::string content_icon2, std::string hit_sound,
							vector<2> main_position, vector<3> total_angle, float bent_angle, float magnification, bool isPassingTile, bool isReversalTile);
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;
		virtual void Link(ATile* pTile) override;
		virtual bool Decision(APlayer* pPlayer) override;

		virtual Object::ATile* GetLinkedTile() const final;
		virtual std::string GetTileTheme() const final;
		virtual std::string GetContentIcon(unsigned char index) const;
		virtual float GetBentAngle() const;
		virtual std::string GetHitSound() const;
		virtual bool IsPassingTile() const;

	public:
		void FindContentTile(float bent_angle);

	public:
		virtual void SetAlphaLevel(float alphaLevel) override;
		virtual void SetTileTheme(std::string tile_theme);
		virtual void SetContentTile(std::string content);
		virtual void SetContentIcon(unsigned char index, std::string content, unsigned int reversalCount);
		virtual void SetDecisionRadius(float radius);
		virtual void SetHitSound(std::string hit_sound);
		virtual void SetPosition(vector<2> main_pos) override;
		virtual void SetBentAngle(float bent_angle);
		virtual void SetAsPassingTile();
		virtual void SetIsReversalTile(bool isReversalTile);
		virtual void SetMagnification(float magnification);

	};

	
	class ASquareTile : public ATile
	{
	protected:
		std::vector<ATile*>	LinkedTiles;
		vector<2>			Destination;

	public:
		ASquareTile(std::string content_tile, vector<2> main_pos);
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;
		virtual void Link(ATile* pTile) override final;
		virtual bool Decision(APlayer* pPlayer) override final;

		virtual vector<2> GetDestination() const final;
		virtual void SetEffectContent(const char* content) final;
	};


	class AEntranceTile final : public ASquareTile
	{
	protected:
		Image::Component	Portal;

	public:
		AEntranceTile(std::string content_tile, vector<2> main_pos);
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;

		virtual void SetPosition(vector<2> main_pos) override;
		virtual void SetMainAngle(float main_angle) override;
		virtual void SetAlphaLevel(float alphaLevel) override;
		virtual void SetCustomLevelEntrance();
	};


	class AMuseDashTile final : public ASquareTile
	{
	public:
		AMuseDashTile(std::string content_tile, vector<2> main_pos);
		virtual void Start() override;
		virtual void Update() override;
		virtual void End() override;
	};
}