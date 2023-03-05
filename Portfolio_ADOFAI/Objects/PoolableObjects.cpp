#include "stdafx.h"
#include "PoolableObjects.h"

Object::ACometTail::ACometTail()
{
	Effect		 = { nullptr, VEC_POSITION_NULL, { 30, 60 }, VEC_ANGLE_NULL, 1.0f, 0.0f, 0.4f};
	isRecyclable = false;
}
void Object::ACometTail::Start() {}
void Object::ACometTail::Update()
{
	if (isRecyclable)
		return;

	if (!Effect.End)
		Effect.Draw();
	else
		isRecyclable = true;
}
void Object::ACometTail::End() {}
bool Object::ACometTail::IsRecyclable()	{ return isRecyclable; }
void Object::ACometTail::ForceRecycle() { isRecyclable = true; }
void Object::ACometTail::OffRecycle()	{ isRecyclable = false; }
void Object::ACometTail::Display(APlayer* pPlayer, const char* content)
{
	std::mt19937						gen(RandomDevice());
	std::uniform_int_distribution<int>	dis(0, 100);
	
	Effect.Content     = content;
	Effect.Position	   = pPlayer->GetPosition();
	Effect.RevRadius   = pPlayer->GetRevRadius() * 1.02f;
	Effect.RevAngle[2] = pPlayer->GetRevAngle() - 5.0f * static_cast<short>(pPlayer->GetAngleDirection());
	Effect.Playback	   = 0.0f;
	Effect.End		   = false;

	if (std::string(content).substr(11, 6) == std::string("simple"))
	{
		Effect.Duration = 0.2f;
		Effect.Angle[2]   = 90.0f;
	}
	else
	{
		Effect.Duration	  = 0.3f;
		Effect.Angle	  = -Effect.RevAngle;
		Effect.RevRadius += 10.0f * (50.0f - dis(gen)) / 50.0f;
	}
}
void Object::ACometTail::Display(vector<2> position, vector<2> length, float revRadius, float revAngle, const char* content)
{
	std::mt19937						gen(RandomDevice());
	std::uniform_int_distribution<int>	dis(0, 100);

	Effect.Content	   = content;
	Effect.Position	   = position;
	Effect.Length	   = length;
	Effect.RevRadius   = revRadius;
	Effect.RevAngle[2] = revAngle;
	Effect.Playback	   = 0.0f;
	Effect.End		   = false;

	if (std::string(content).substr(11, 6) == std::string("simple"))
	{
		Effect.Duration = 0.2f;
		Effect.Angle[2] = 90.0f;
	}
	else
	{
		Effect.Duration = 0.4f;
		Effect.Angle = -Effect.RevAngle;
		Effect.RevRadius += 5.0f * (50.0f - dis(gen)) / 50.0f;
	}
}

Object::ADecision::ADecision()
{
	DecisionTexts[0] = { "effect_decision_too_fast", VEC_POSITION_NULL, { 113.75f, 36.024f }, VEC_ANGLE_NULL, 1.0f };
	DecisionTexts[1] = { "effect_decision_fast",	 VEC_POSITION_NULL, { 54.176f, 36.024f }, VEC_ANGLE_NULL, 1.0f };
	DecisionTexts[2] = { "effect_decision_perfect",  VEC_POSITION_NULL, { 54.176f, 36.024f }, VEC_ANGLE_NULL, 1.0f };
	DecisionTexts[3] = { "effect_decision_slow",	 VEC_POSITION_NULL, { 54.176f, 36.024f }, VEC_ANGLE_NULL, 1.0f };
	DecisionTexts[4] = { "effect_decision_too_slow", VEC_POSITION_NULL, { 113.75f, 36.024f }, VEC_ANGLE_NULL, 1.0f };
	Indicator		 = { "effect_indicator_miss",	 VEC_POSITION_NULL, { 32.5f, 32.5f },	  VEC_ANGLE_NULL, 1.0f };

	DecisionIndex = 0;
	isRecyclable  = false;
}
void Object::ADecision::Start() {}
void Object::ADecision::Update()
{
	if (isRecyclable)
		return;

	DecisionTexts[DecisionIndex].AlphaLevel -= DELTA_TIME / SECONDS_PER_BEAT / 2.0f;

	if (!(DecisionIndex % 4))
		Indicator.AlphaLevel -= DELTA_TIME / SECONDS_PER_BEAT / 2.0f;

	if (DecisionIndex == 0)
		DecisionTexts[DecisionIndex].Angle[2] += 30.0f * DELTA_TIME / SECONDS_PER_BEAT / 2.0f;
	else if (DecisionIndex == 4)
		DecisionTexts[DecisionIndex].Angle[2] -= 30.0f * DELTA_TIME / SECONDS_PER_BEAT / 2.0f;

	if (DecisionTexts[DecisionIndex].AlphaLevel > 0)
	{
		DecisionTexts[DecisionIndex].Draw();
		if (!(DecisionIndex % 4)) Indicator.Draw();
	}
	else
	{
		DecisionTexts[DecisionIndex].AlphaLevel = 1.0f;
		Indicator.AlphaLevel = 1.0f;
		isRecyclable = true;
	}
}
void Object::ADecision::End() {}
bool Object::ADecision::IsRecyclable()	{ return isRecyclable; }
void Object::ADecision::ForceRecycle()	{ isRecyclable = true; }
void Object::ADecision::OffRecycle()	{ isRecyclable = false; }
void Object::ADecision::Display(vector<2> tile_pos, vector<2> satellite_pos, unsigned char index)
{
	Indicator.Position = satellite_pos;
	DecisionIndex = index;

	// 너무 빠름, 너무 느림
	DecisionTexts[0].Position = DecisionTexts[4].Position = satellite_pos + vector<2>{ 0, MOVEMENT_UNIT };

	// 정확, 빠름, 느림
	DecisionTexts[1].Position = DecisionTexts[2].Position = DecisionTexts[3].Position = tile_pos + vector<2>{ 0, MOVEMENT_UNIT };
}