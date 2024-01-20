#include "stdafx.h"
#include "CameraManager.h"

// Macro constants
#define SCALE_CAMERA_BOUNCE				1.02f
#define VEC_DEFAULT_ZOOM				vector<2>{ 1.0f, 1.0f }

//										Action Speed					=	(Revolutions per second)		* (Actions per revolution)
#define MOVE_PER_SECOND					0.0111f * BPM * Magnification	//	(BPM * 0.5 rev / beat / 60 sec) * (1 camera moves / 0.75 rev) = 0.0111 * BPM
#define ZOOM_PER_SECOND					0.0083f * BPM * Magnification	//	(BPM * 0.5 rev / beat / 60 sec) * (1 zoom recovers / 1 rev)	  = 0.0083 * BPM
#define CAMERA_MOVE_VELOCITY			(Displacement * MOVE_PER_SECOND)
#define CAMERA_ZOOM_VELOCITY			(VEC_UNIT_SQUARE * (1 - SCALE_CAMERA_BOUNCE)	 * ZOOM_PER_SECOND)
#define BACKGR_ZOOM_VELOCITY			(VEC_UNIT_SQUARE * (1 - 1 / SCALE_CAMERA_BOUNCE) * ZOOM_PER_SECOND)

#define CAMERA_CLOSING_EXPAND_SPEED		(VEC_LENGTH_SCREEN[0] * 5.0f * static_cast<float>(pow(2,  1 - CameraCloser.Length[0] / VEC_LENGTH_SCREEN[0])))
#define CAMERA_OPENING_SHRINK_SPEED		(VEC_LENGTH_SCREEN[0] * 5.0f * static_cast<float>(pow(2, -1 + CameraCloser.Length[0] / VEC_LENGTH_SCREEN[0])))
#define CAMERA_CLOSING_MOVE_SPEED		CAMERA_CLOSING_EXPAND_SPEED / 2.0f
#define CAMERA_OPENING_MOVE_SPEED		CAMERA_OPENING_SHRINK_SPEED / 2.0f

CameraManager::CameraManager()
{
	OperationMode	 = OPERATION_MODE::PLAY_MODE;
	Camera			 = Rendering::Camera();
	CameraCloser	 = Background::Component{ "bg_black_square", VEC_POSITION_NULL, { 0.0f, VEC_LENGTH_SCREEN[1] }, VEC_ANGLE_NULL, 1.0f };
	Backgr			 = Background::Component{ nullptr, VEC_POSITION_ORIGIN, VEC_LENGTH_SCREEN, VEC_ANGLE_NULL, 1.0f, VEC_POSITION_ORIGIN, VEC_DEFAULT_ZOOM };
	Announcements[0] = Background::Component{ "bg_editor_announcements1", VEC_POSITION_NULL, VEC_LENGTH_SCREEN, VEC_ANGLE_NULL, 1.0f };
	Announcements[1] = Background::Component{ "bg_editor_announcements2", VEC_POSITION_NULL, VEC_LENGTH_SCREEN, VEC_ANGLE_NULL, 1.0f };
	Otto			 = Background::Component{ "button_otto_off", { 540, 310 }, { 125, 125 }, VEC_ANGLE_NULL, 1.0f };
	BPM				 = 100;
	Magnification	 = 1.0f;
	isCloseable		 = true;
	isClosing		 = false;
	isOpenable		 = false;
	isOpening		 = false;
	isFadeable		 = true;
	isFadingIn		 = false;
	isNotifying		 = true;

	CameraCloser.ZoomPos   = VEC_POSITION_ORIGIN;
	CameraCloser.ZoomScale = VEC_DEFAULT_ZOOM;
	for (auto& elem : Announcements)
	{
		elem.ZoomPos = VEC_POSITION_ORIGIN;
		elem.ZoomScale = VEC_DEFAULT_ZOOM;
	}
}
void CameraManager::Start()
{
	OperationMode	 = CURRENT_SCENE->GetOperationMode();
	
	ContentBG		 = CURRENT_SCENE->GetContentBG() == "" ? "bg_black_square" : CURRENT_SCENE->GetContentBG();
	Backgr.Content	 = ContentBG.c_str();

	BPM				 = CURRENT_SCENE->GetBPM();
	Magnification	 = 1.0f;
	Camera.ZoomScale = VEC_UNIT_SQUARE;

	if (OperationMode != OPERATION_MODE::EDIT_MODE)
		Camera.Position = CURRENT_SCENE->GetStartPos();

	Destination		 = CURRENT_SCENE->GetStartPos();
	Displacement	 = Destination - Camera.Position;

	isFadingIn		 = false;
	isFadeable		 = true;
}
void CameraManager::Update()
{
	if (OperationMode == OPERATION_MODE::EDIT_MODE)
	{
		// Zoom should not recover (Edit mode)
		BoundedFloat(Camera.ZoomScale[0], 0.2f, 4.0f);
		BoundedFloat(Camera.ZoomScale[1], 0.2f, 4.0f);
	}
	else
	{
		// Zoom recovery
		if (Camera.ZoomScale[0] - VEC_DEFAULT_ZOOM[0])
		{
			vector<2> delta_zoomScale = CAMERA_ZOOM_VELOCITY * DELTA_TIME;
			if (-(VEC_DEFAULT_ZOOM[0] - Camera.ZoomScale[0]) > -delta_zoomScale[0])
				Camera.ZoomScale += delta_zoomScale;
			else
				Camera.ZoomScale = VEC_DEFAULT_ZOOM;
		}
	}

	// Movement
	vector<2> delta_distance;
	if (OperationMode == OPERATION_MODE::EDIT_MODE)
		delta_distance = Displacement * 5.0f * DELTA_TIME;
	else
		delta_distance = CAMERA_MOVE_VELOCITY * DELTA_TIME;

	if (L1Distance(Destination - Camera.Position) > L1Distance(delta_distance))
		Camera.Position += delta_distance;
	else
		Camera.Position = Destination;

	Otto.ZoomScale = CameraCloser.ZoomScale = Backgr.ZoomScale = VEC_UNIT_SQUARE / Camera.ZoomScale[0];
	Otto.ZoomPos   = CameraCloser.ZoomPos   = Backgr.ZoomPos   = Camera.ZoomPos = Backgr.Position = Camera.Position;
	Otto.Position  = Camera.Position + vector<2>{ 540, 310 };

	if (dynamic_cast<Level*>(CURRENT_SCENE))
		Otto.Content = dynamic_cast<Level*>(CURRENT_SCENE)->IsAutoPlay() ? "button_otto_on" : "button_otto_off";
	
	for (auto& elem : Announcements)
	{
		elem.Position  = elem.ZoomPos = Camera.Position;
		elem.ZoomScale = VEC_UNIT_SQUARE / Camera.ZoomScale[0];
	}

	CameraCloser.Position = Camera.Position - vector<2>{ 640.0f, 0.0f };

	// Draw
	Camera.Set();
	Backgr.Draw();
	if (isNotifying)
	{
		if (OperationMode == OPERATION_MODE::EDIT_MODE)
			Announcements[0].Draw();
		else if (OperationMode == OPERATION_MODE::TEST_MODE)
			Announcements[1].Draw();
	}
	if (OperationMode == OPERATION_MODE::TEST_MODE ||
		OperationMode == OPERATION_MODE::PLAY_MODE)
		Otto.Draw();
}
void CameraManager::End() {}

void CameraManager::SetMode(OPERATION_MODE mode)				{ OperationMode = mode; }
void CameraManager::SetBackground(std::string content_bg)		{ ContentBG = content_bg; Backgr.Content = ContentBG.c_str(); }

vector<2> CameraManager::GetPosition() const					{ return Camera.Position; }
bool CameraManager::IsMoving() const							{ return L1Distance(Destination - Camera.Position) ? true : false; }
bool CameraManager::IsNotifying() const							{ return isNotifying; }
bool CameraManager::IsCloseable() const							{ return isCloseable; }
bool CameraManager::IsOpenable() const							{ return isOpenable; }
bool CameraManager::IsClosing() const							{ return isClosing; }
bool CameraManager::IsOpening() const							{ return isOpening; }
bool CameraManager::IsFadeable() const							{ return isFadeable; }
bool CameraManager::IsFadingIn() const							{ return isFadingIn; }

void CameraManager::SetMagnification(float magnification)		{ Magnification = magnification; }
void CameraManager::SetPosition(vector<2> position)				{ Camera.Position = position; Destination = position; }
void CameraManager::SetBPM(unsigned short bpm)					{ BPM = bpm; }
void CameraManager::SetOttoContent(const char* content)			{ Otto.Content = content; }
void CameraManager::SetIsNotifying(bool isNotifying)			{ this->isNotifying = isNotifying; }
void CameraManager::SetDragBasis(vector<2> cursorPos)			{ DragBasis = cursorPos; PosBasis = Camera.Position; }
void CameraManager::Drag(vector<2> cursorPos)					{ SetPosition(PosBasis + DragBasis - cursorPos); }
void CameraManager::Zoom(long scale)							{ Camera.ZoomScale += VEC_UNIT_SQUARE * 0.2f * static_cast<float>(scale); }
void CameraManager::Bounce()									{ Camera.ZoomScale = VEC_DEFAULT_ZOOM * SCALE_CAMERA_BOUNCE; }
void CameraManager::Move(vector<2> destination)
{
	Destination  = destination;
	Displacement = Destination - Camera.Position;
}

void CameraManager::PresentCameraCloser()						{ if (CameraCloser.AlphaLevel) CameraCloser.Draw(); }
bool CameraManager::CameraClosing()
{
	if (isCloseable)
	{
		//CameraCloser.Position   = Destination + vector<2>{ VEC_LENGTH_SCREEN[0] / 2.0f, 0.0f };
		CameraCloser.Length		= { 0, VEC_LENGTH_SCREEN[1] };
		CameraCloser.AlphaLevel = 1.0f;
		isFadeable  = false;
		isCloseable = false;
		isClosing   = true;
		SOUND("CameraClosing")->Play();
	}

	if (isClosing)
	{
		if (CameraCloser.Length[0] < 3000.0f)
		{
			CameraCloser.Length[0]   += CAMERA_CLOSING_EXPAND_SPEED * DELTA_TIME;
		}
		else
		{
			isClosing  = false;
			isOpenable = true;
		}
	}

	return isClosing;
}
bool CameraManager::CameraOpening()
{
	if (isOpenable)
	{
		CameraCloser.Length[0]	= 2 * VEC_LENGTH_SCREEN[0];
		CameraCloser.AlphaLevel = 1.0f;
		isOpenable = false;
		isOpening  = true;
		SOUND("CameraOpening")->Play();
	}

	if (isOpening)
	{
		if (CameraCloser.Length[0] > 0.0f)
		{
			CameraCloser.Length[0]   -= CAMERA_OPENING_SHRINK_SPEED * DELTA_TIME;
		}
		else
		{
			CameraCloser.AlphaLevel = 0.0f;
			CameraCloser.Length[0]  = 0.0f;
			isOpening	= false;
			isCloseable = true;
			isFadeable  = true;
		}
	}
	
	return isOpening;
}
bool CameraManager::CameraFadeIn()
{
	if (isFadeable)
	{
		CameraCloser.Length[0]	= 2 * VEC_LENGTH_SCREEN[0];
		CameraCloser.Length[1]	= VEC_LENGTH_SCREEN[1];
		CameraCloser.AlphaLevel = 1.0f;
		isFadingIn  = true;
		isFadeable  = false;
		isCloseable = false;
	}

	if (isFadingIn)
	{
		if (CameraCloser.AlphaLevel > 0.0f)
			CameraCloser.AlphaLevel -= 2.0f * DELTA_TIME;
		else
		{
			CameraCloser.AlphaLevel = 0.0f;
			isFadingIn  = false;
			isFadeable  = true;
			isCloseable = true;
		}
	}

	return isFadingIn;
}