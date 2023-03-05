#pragma once
#include "Objects/APlayer.h"
#include "Utilities/Singleton.h"
#include "Utilities/Transform.h"

using namespace Rendering;

enum class OPERATION_MODE
{
	EDIT_MODE,
	TEST_MODE,
	PLAY_MODE,
	INTRO_MODE
};

class CameraManager final : public Singleton<CameraManager>
{
private:
	OPERATION_MODE			OperationMode;

private:
	Camera					Camera;
	Background::Component	Backgr;
	Background::Component	Otto;
	unsigned short			BPM;
	float					Magnification;
	std::string				ContentBG;
	vector<2>				Destination;
	vector<2>				Displacement;
	vector<2>				PosBasis;
	vector<2>				DragBasis;

private:
	Background::Component	CameraCloser;
	Background::Component	Announcements[2];

private:
	bool					isCloseable;
	bool					isClosing;
	bool					isOpenable;
	bool					isOpening;
	bool					isFadeable;
	bool					isFadingIn;
	bool					isNotifying;

public:
	CameraManager();
	void Start();
	void Update();
	void End();
	
	void SetMode(OPERATION_MODE mode);
	void SetBackground(std::string);

	vector<2> GetPosition() const;
	bool IsMoving() const;
	bool IsNotifying() const;
	bool IsCloseable() const;
	bool IsOpenable() const;
	bool IsClosing() const;
	bool IsOpening() const;
	bool IsFadeable() const;
	bool IsFadingIn() const;

	void SetMagnification(float magnification);
	void SetPosition(vector<2> position);
	void SetBPM(unsigned short bpm);
	void SetOttoContent(const char* content);
	void SetIsNotifying(bool isNotifying);
	void SetDragBasis(vector<2> cursorPos);
	void Drag(vector<2> cursorPos);
	void Zoom(long scale);
	void Bounce();
	void Move(vector<2> destination);

public:
	void PresentCameraCloser();
	bool CameraClosing();
	bool CameraOpening();
	bool CameraFadeIn();
};