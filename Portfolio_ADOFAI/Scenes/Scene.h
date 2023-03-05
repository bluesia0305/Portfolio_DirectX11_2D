#pragma once

// Forward declaration
namespace Map	 { class TileMap; }
namespace Object { class APlayer; }
enum class OPERATION_MODE;

class Scene abstract
{
protected:
	// Should be saved
	Map::TileMap*		Map;
	Sound::Sound*		BGM;
	std::string			ContentBGM;
	std::string			ContentBG;
	unsigned short		BPM;
	float				BGMOffset;
	unsigned short		CountTicks;
	
	// Not to be saved
	OPERATION_MODE		OperationMode;
	Object::APlayer*	Player;
	vector<2>			StartPos;
	std::string			NextScene;

	float				BGMTimer;
	float				CountdownTimer;
	unsigned short		CountdownIndex;

	bool				isStarted;
	bool				isPlayable;
	bool				isEnded;
	bool				isClosing;
	bool				isFadingIn;

public:
	Scene(unsigned short bpm, float movement_unit);
	virtual ~Scene();
	virtual void Start() abstract;
	virtual bool Update() abstract;
	virtual void End() abstract;

	virtual OPERATION_MODE GetOperationMode() const final;
	virtual std::string GetContentBG() const final;
	virtual unsigned short GetBPM() const final;
	virtual vector<2> GetStartPos() const final;
	virtual bool IsStarted() const final;
	virtual bool IsPlayable() const final;

protected:
	bool InputProcess();	// Input component 추가 후 제거
};