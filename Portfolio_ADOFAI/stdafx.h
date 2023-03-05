#pragma once

// vvvvvvv Pre-defined macros vvvvvvv
#define _USE_MATH_DEFINES
#define DELTA_TIME							Time::Get::Delta()

// Basic constants
#define SIDE_LENGTH_PLANET					65.0f
#define MOVEMENT_UNIT						72.0f
#define MOVEMENT_UNIT_LEVEL					108.0f
#define CONTENT_SQUARE_TILE					"tile_square_neonlight"

// Vector-type constants
#define VEC_UNIT_SQUARE						vector<2>{ 1, 1 }
#define VEC_POSITION_ORIGIN					vector<2>{ 0, 0 }
#define VEC_POSITION_NULL					vector<2>{ 0, 0 }
#define VEC_POSITION_CURSOR					vector<2>{ Input::Get::Cursor::X(), Input::Get::Cursor::Y() }
#define VEC_LENGTH_NULL						vector<2>{ 0, 0 }
#define VEC_LENGTH_SCREEN					vector<2>{ 1280.0f, 720.0f }
#define VEC_LENGTH_PLANET					vector<2>{ SIDE_LENGTH_PLANET, SIDE_LENGTH_PLANET }
#define VEC_LENGTH_INTROTILE				vector<2>{ 88, 88 }
#define VEC_LENGTH_LEVELTILE				vector<2>{ 145, 145 }
#define VEC_ANGLE_NULL						vector<3>{ 0, 0, 0 }

// Text Colors
#define COLOR_WHITE							{ 255, 255, 255 }
#define COLOR_PURPLE						{ 141, 35, 144 }
#define COLOR_BLACK							{ 0, 0, 0 }

// Decision
#define DECISION_RADIUS_LENIENT_SQUARE		55.106f
#define DECISION_RADIUS_LENIENT_LEVEL		80.0f
#define DECISION_RADIUS_NORMAL_LEVEL		DECISION_RADIUS_LENIENT_LEVEL * 0.8f
#define DECISION_RADIUS_STRICT_LEVEL		65.0f

// Managers
#define PLAYER								(INSTANCE(ObjectManager)->GetActor<Object::APlayer>())
#define CAMERA								(INSTANCE(CameraManager))
#define CURRENT_SCENE						(INSTANCE(SceneManager)->GetCurrentScene())
#define SOUND(_sound)						(INSTANCE(SoundManager)->GetSound(_sound))
#define BEATS_PER_MINUTE					(CURRENT_SCENE->GetBPM())
#define BEATS_PER_SECOND					(BEATS_PER_MINUTE / 60.0f)
#define SECONDS_PER_BEAT					(60.0f / BEATS_PER_MINUTE)

// Functions
#define BoundedFloat(_float, _min, _max)	_float = _float < _min ? _min : (_float > _max ? _max : _float)
#define AdjustFloat(_float, _upperBound)	_float = _float < 0 ? _float += _upperBound : _float >= _upperBound ? _float -= _upperBound : _float
#define AdjustAngle(_degree)				_degree = _degree <= -180 ? _degree += 360.0f : _degree > 180.0f ? _degree -= 360.0f : _degree
#define ConvertToRadian(_degree)			(_degree * static_cast<float>(M_PI) / 180.0f)
#define ConvertToDegree(_radian)			(_radian * 180.0f / static_cast<float>(M_PI))
#define L1Distance(_vector)					(abs((_vector)[0]) + abs((_vector)[1]))
#define NormL1Distance(_vector)				static_cast<unsigned>(L1Distance(_vector) / MOVEMENT_UNIT)
// ^^^^^^^ Pre-defined macros ^^^^^^^


// Include standard headers(.h)
#include <Windows.h>
#include <string>
#include <vector>
#include <list>
#include <map>

#ifdef _DEBUG
#include <iostream>
#define PrintBool(_bool)			printf("%s\n", _bool ? "true" : "false")
#define PrintInt(_int)				printf("%4i\n", _int)
#define PrintFloat(_float)			printf("%12.7f\n", _float)
#define PrintString(_string)		printf("%s\n", _string)
#define PrintVector(_vector)		printf("%12.7f\t%12.7f\n", _vector[0], _vector[1])
#define PrintVector3(_vector)		printf("%12.7f\t%12.7f\t%12.7f\n", _vector[0], _vector[1], _vector[2])
#define PrintCursorPos()			if (Input::Get::Key::Down(0x11)) printf("%4d\t%4d\n", Input::Get::Cursor::X(), Input::Get::Cursor::Y())
#endif

// Include user-defined headers(.h)
#include "Engine/Collision.h"
#include "Engine/Rendering.h"
#include "Engine/Resource.h"
#include "Engine/Input.h"
#include "Engine/Time.h"
#include "Engine/Sound.h"
#include "Engine/vector.h"

#include "Managers/DataManager.h"
#include "Managers/CameraManager.h"
#include "Managers/SceneManager.h"
#include "Managers/SoundManager.h"
#include "Managers/ObjectManager.h"