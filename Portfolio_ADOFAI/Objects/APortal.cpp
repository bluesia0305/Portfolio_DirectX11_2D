#include "stdafx.h"
#include "APortal.h"

// Vector-type constants
#define VEC_LENGTH_PREVIEW			(vector<2>{ 180.0f, 217.4f })
#define VEC_LENGTH_BORDER			(vector<2>{ 46.8f,  46.8f  })
#define VEC_LENGTH_CLOUD1_FX		(vector<2>{ 74.8f,  125.2f })
#define VEC_LENGTH_CLOUD2_FX		(vector<2>{ 54.0f,  105.2f })
#define VEC_LENGTH_CLOUD3_FX		(vector<2>{ 54.0f,  110.8f })
#define VEC_LENGTH_CLOUD4_FX		(vector<2>{ 60.0f,  130.8f })
#define VEC_LENGTH_CLOUD5_FX		(vector<2>{ 54.0f,  115.2f })
#define VEC_LENGTH_CLOUD6_FX		(vector<2>{ 54.0f,  115.2f })

// Pre-computed values
#define PENDULUM_START_ANGLE		270.0f
#define PENDULUM_AMPLITUDE			15.0f
#define PENDULUM_PERIOD				2.0f
#define PENDULUM_OFFSET_R			0.0f
#define PENDULUM_OFFSET_B			2.07f
#define PENDULUM_OFFSET_Y			3.66f
#define VIBRATION_START_YPOS		133.2f
#define VIBRATION_AMPLITUDE			12.0f
#define VIBRATION_PERIOD			8.0f
#define SHRINK_RATIO				0.85f
#define SHRINK_PER_SECOND			4.0f
#define ECCENTRICITY				0.560665f	// eccentricity = sqrt(1 - pow(minorAxis, 2)/pow(majorAxis, 2))
														
#define Pendulum(_amplitude, _time, _period, _offset)	(_amplitude * cos(ConvertToRadian(360.0f / _period * _time) + _offset))		// derived from d^2s/dt^2 = -mgsin(angle)
#define Vibration(_amplitude, _time, _period)			Pendulum(_amplitude, _time, _period, 0.0f)
#define EllipseRadius(_minorAxis, _angle)				(_minorAxis / 2 / sqrt(1 - pow(ECCENTRICITY * sin(ConvertToRadian(_angle)), 2.0f)))

Object::APortal::APortal(vector<2> position, const char* content_levelName, const char* content_preview, const char* content_text)
{
	DescPos = position + vector<2>{ 0.0f, -72.0f };
	Text    = Text::Component{ content_text, { 640, 670 }, { "godoMaum", 70 }, COLOR_WHITE };

	{
		//								  Content						 Position								   Length						  Angle					 AlphaLevel
		NameTag		  = Image::Component{ "object_portal_nametag",		 position + vector<2>{  0.0f,   432.0f  }, vector<2>{ 432.0f,  216.0f },  VEC_ANGLE_NULL,		 1.0f };
		LevelName	  = Image::Component{ content_levelName,			 position + vector<2>{  0.0f,   398.88f }, vector<2>{ 131.33f, 33.05f },  VEC_ANGLE_NULL,		 1.0f };
		Preview		  = Image::Component{ content_preview,				 position + vector<2>{  0.0f,   0.0f    }, VEC_LENGTH_PREVIEW,			  VEC_ANGLE_NULL,		 1.0f };
		Lanterns[0]	  = Image::Component{ "object_lantern_red",			 position + vector<2>{  108.0f, 383.04f }, vector<2>{ 86.4f,   86.4f  },  vector<3>{ 0, 0, 90 }, 1.0f };
		Lanterns[1]	  = Image::Component{ "object_lantern_blue",		 position + vector<2>{ -108.0f, 383.04f }, vector<2>{ 86.4f,   86.4f  },  vector<3>{ 0, 0, 90 }, 1.0f };
		Lanterns[2]	  = Image::Component{ "object_lantern_yellow",		 position + vector<2>{ -3.6f,   361.44f }, vector<2>{ 86.4f,   86.4f  },  vector<3>{ 0, 0, 90 }, 1.0f };
		Lights[0]	  = Image::Component{ "effect_lantern_light_red",	 position + vector<2>{  108.0f, 383.04f }, vector<2>{ 120.0f,  120.0f  }, VEC_ANGLE_NULL,		 0.4f };
		Lights[1]	  = Image::Component{ "effect_lantern_light_blue",	 position + vector<2>{ -108.0f, 383.04f }, vector<2>{ 120.0f,  120.0f  }, VEC_ANGLE_NULL,		 0.4f };
		Lights[2]	  = Image::Component{ "effect_lantern_light_yellow", position + vector<2>{ -3.6f,   361.44f }, vector<2>{ 120.0f,  120.0f  }, VEC_ANGLE_NULL,		 0.4f };
	}

	for (int i = 0; i < 6; ++i)
	{
		PreviewFX1[i] = Image::Component{ nullptr,					position + vector<2>{  0.0f,    0.0f    },	VEC_LENGTH_BORDER,				VEC_ANGLE_NULL,			0.8f };
		PreviewFX2[i] = Image::Component{ nullptr,					position + vector<2>{  0.0f,    0.0f    },	VEC_LENGTH_NULL,				VEC_ANGLE_NULL,			1.0f };

		PreviewFX1[i].Angle[2]	  = 90.0f;
		PreviewFX1[i].RevAngle[2] = 10.0f * i;
	}
	
	PreviewFX1[0].Content = "object_portal_clouds1";
	PreviewFX1[1].Content = "object_portal_clouds2";
	PreviewFX1[2].Content = "object_portal_clouds3";
	PreviewFX1[3].Content = "object_portal_clouds4";
	PreviewFX1[4].Content = "object_portal_clouds5";
	PreviewFX1[5].Content = "object_portal_clouds6";

	PreviewFX2[0].Content = "object_portal_clouds1_outer";
	PreviewFX2[1].Content = "object_portal_clouds2_outer";
	PreviewFX2[2].Content = "object_portal_clouds3_outer";
	PreviewFX2[3].Content = "object_portal_clouds4_outer";
	PreviewFX2[4].Content = "object_portal_clouds5_outer";
	PreviewFX2[5].Content = "object_portal_clouds6_outer";

	PreviewFX2[0].Length  = VEC_LENGTH_CLOUD1_FX;
	PreviewFX2[1].Length  = VEC_LENGTH_CLOUD2_FX;
	PreviewFX2[2].Length  = VEC_LENGTH_CLOUD3_FX;
	PreviewFX2[3].Length  = VEC_LENGTH_CLOUD4_FX;
	PreviewFX2[4].Length  = VEC_LENGTH_CLOUD5_FX;
	PreviewFX2[5].Length  = VEC_LENGTH_CLOUD6_FX;

	PendulumAmplitude  = PENDULUM_AMPLITUDE;
	VibrationAmplitude = VIBRATION_AMPLITUDE;
}
void Object::APortal::Start()
{
	Lanterns[0].RevRadius = Lanterns[1].RevRadius = Lanterns[2].RevRadius = 31.536f;
	Lights[0].RevRadius	  = Lights[1].RevRadius	  = Lights[2].RevRadius   = 38.88f;

	for (int i = 0; i < 3; ++i)
	{
		PreviewFX2[2 * i].RevAngle[2]	  = 15.0f;
		PreviewFX2[2 * i + 1].RevAngle[2] = 60.0f;
	}
}
void Object::APortal::Update()
{
	// Shrinkage & Expansion
	if (!L1Distance(INSTANCE(ObjectManager)->GetActor<APlayer>()->GetPosition() - DescPos))
	{
		Shrink();
		Text.Draw();
	}
	else
		Expand();

	// Pendulum movements (Lanterns, Lights)
	PendulumMovements();
	
	// Randomized Lantern enligtenings (Lights)
	float lengthLevel = RhythmicalFluctuation(0.0f, 1.0f);
	for (auto& elem : Lights)
	{ elem.Length[0] = elem.Length[1] = 60.0f + 60.0f * lengthLevel; }

	// Simple vibration movements (Preview)
	SimpleVibrations();

	// Draw
	NameTag.Draw();
	LevelName.Draw();
	Preview.Draw();

	// Elliptic movements (PreviewFX1, PreviewFX2) - !! Draw functions embedded !!
	EllipticMovements();

	for (int i = 0; i < 3; ++i)
	{
		Lanterns[i].Draw();
		Lights[i].Draw();
	}
}
void Object::APortal::End() {}
void Object::APortal::PendulumMovements()
{
	PendulumTimer += DELTA_TIME;
	if (PendulumTimer > PENDULUM_PERIOD)
		AdjustFloat(PendulumTimer, PENDULUM_PERIOD);

	Lanterns[0].RevAngle[2] = PENDULUM_START_ANGLE + Pendulum(PendulumAmplitude, PendulumTimer, PENDULUM_PERIOD, PENDULUM_OFFSET_R);
	Lanterns[1].RevAngle[2] = PENDULUM_START_ANGLE + Pendulum(PendulumAmplitude, PendulumTimer, PENDULUM_PERIOD, PENDULUM_OFFSET_B);
	Lanterns[2].RevAngle[2] = PENDULUM_START_ANGLE + Pendulum(PendulumAmplitude, PendulumTimer, PENDULUM_PERIOD, PENDULUM_OFFSET_Y);
	
	for (int i = 0; i < 3; ++i)
		Lights[i].RevAngle[2] = Lanterns[i].RevAngle[2];
}
void Object::APortal::SimpleVibrations()
{
	VibrationTimer += DELTA_TIME;
	if (VibrationTimer > VIBRATION_PERIOD)
		AdjustFloat(VibrationTimer, VIBRATION_PERIOD);

	Preview.Position[1] = VIBRATION_START_YPOS + Vibration(VibrationAmplitude, VibrationTimer, VIBRATION_PERIOD);

	for (int i = 0; i < 6; ++i)
	{
		PreviewFX1[i].Position[1] = Preview.Position[1];
		PreviewFX2[i].Position[1] = Preview.Position[1];
	}
}
void Object::APortal::EllipticMovements()
{
	for (int i = 0; i < 6; ++i)
	{
		// PreviewFX1
		PreviewFX1[i].RevAngle[2] += 15.0f * DELTA_TIME;
		for (int j = 0; j < 36; ++j)
		{
			PreviewFX1[i].RevAngle[2] += 10.0f * j;
			if (PreviewFX1[i].RevAngle[2] > 180.0f)
				AdjustAngle(PreviewFX1[i].RevAngle[2]);

			PreviewFX1[i].RevRadius = EllipseRadius(Preview.Length[0], PreviewFX1[i].RevAngle[2]) * 1.1f;
			PreviewFX1[i].Draw();
		}

		// PreviewFX2
		PreviewFX2[i].RevAngle[2] += 10.0f * DELTA_TIME;
		float quadrantAngle = AdjustFloat(PreviewFX2[i].RevAngle[2], 90.0f);

		if (quadrantAngle > 0.0f + 12.0f * i && quadrantAngle <= 30.0f + 12.0f * i)
			PreviewFX2[i].AlphaLevel += DELTA_TIME;
		else
			PreviewFX2[i].AlphaLevel -= DELTA_TIME;
		BoundedFloat(PreviewFX2[i].AlphaLevel, 0.0f, 1.0f);

		for (int j = 0; j < 4; ++j)
		{
			PreviewFX2[i].RevAngle[2] += 90.0f * j;
			PreviewFX2[i].RevRadius = EllipseRadius(Preview.Length[0], PreviewFX2[i].RevAngle[2]) * 1.15f;
			PreviewFX2[i].Draw();
		}
	}
}
void Object::APortal::Shrink()
{
	VibrationTimer += 0.15f * DELTA_TIME;
	if (Preview.Length[0] - VEC_LENGTH_PREVIEW[0] * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME > VEC_LENGTH_PREVIEW[0] * SHRINK_RATIO)
	{
		VibrationAmplitude -= VIBRATION_AMPLITUDE * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		Preview.Length	   -= VEC_LENGTH_PREVIEW  * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		Preview.AlphaLevel -= 0.5f * SHRINK_PER_SECOND * DELTA_TIME;

		for (auto& elem : PreviewFX1)
		{
			elem.Length		-= VEC_LENGTH_BORDER * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
			elem.AlphaLevel  = Preview.AlphaLevel;
		}

		PreviewFX2[0].Length -= VEC_LENGTH_CLOUD1_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		PreviewFX2[1].Length -= VEC_LENGTH_CLOUD2_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		PreviewFX2[2].Length -= VEC_LENGTH_CLOUD3_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		PreviewFX2[3].Length -= VEC_LENGTH_CLOUD4_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		PreviewFX2[4].Length -= VEC_LENGTH_CLOUD5_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		PreviewFX2[5].Length -= VEC_LENGTH_CLOUD6_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
	}
	else
	{
		//VibrationAmplitude = VIBRATION_AMPLITUDE * SHRINK_RATIO;
		Preview.Length	   = VEC_LENGTH_PREVIEW  * SHRINK_RATIO;
		Preview.AlphaLevel = 0.5f;

		for (auto& elem : PreviewFX1)
		{
			elem.Length		= VEC_LENGTH_BORDER * SHRINK_RATIO;
			elem.AlphaLevel = Preview.AlphaLevel;
		}

		PreviewFX2[0].Length = VEC_LENGTH_CLOUD1_FX * SHRINK_RATIO;
		PreviewFX2[1].Length = VEC_LENGTH_CLOUD2_FX * SHRINK_RATIO;
		PreviewFX2[2].Length = VEC_LENGTH_CLOUD3_FX * SHRINK_RATIO;
		PreviewFX2[3].Length = VEC_LENGTH_CLOUD4_FX * SHRINK_RATIO;
		PreviewFX2[4].Length = VEC_LENGTH_CLOUD5_FX * SHRINK_RATIO;
		PreviewFX2[5].Length = VEC_LENGTH_CLOUD6_FX * SHRINK_RATIO;
	}
}
void Object::APortal::Expand()
{
	if (Preview.Length[0] + VEC_LENGTH_PREVIEW[0] * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME < VEC_LENGTH_PREVIEW[0])
	{
		VibrationAmplitude += VIBRATION_AMPLITUDE * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		Preview.Length	   += VEC_LENGTH_PREVIEW  * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		Preview.AlphaLevel += 0.5f * SHRINK_PER_SECOND * DELTA_TIME;

		for (auto& elem : PreviewFX1)
		{
			elem.Length		+= VEC_LENGTH_BORDER * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
			elem.AlphaLevel  = Preview.AlphaLevel;
		}

		PreviewFX2[0].Length += VEC_LENGTH_CLOUD1_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		PreviewFX2[1].Length += VEC_LENGTH_CLOUD2_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		PreviewFX2[2].Length += VEC_LENGTH_CLOUD3_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		PreviewFX2[3].Length += VEC_LENGTH_CLOUD4_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		PreviewFX2[4].Length += VEC_LENGTH_CLOUD5_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
		PreviewFX2[5].Length += VEC_LENGTH_CLOUD6_FX * (1 - SHRINK_RATIO) * SHRINK_PER_SECOND * DELTA_TIME;
	}
	else if (Preview.Length[0] != VEC_LENGTH_PREVIEW[0])
	{
		VibrationAmplitude = VIBRATION_AMPLITUDE;
		Preview.Length	   = VEC_LENGTH_PREVIEW;
		Preview.AlphaLevel = 1.0f;

		for (auto& elem : PreviewFX1)
		{
			elem.Length		= VEC_LENGTH_BORDER;
			elem.AlphaLevel = Preview.AlphaLevel;
		}

		PreviewFX2[0].Length = VEC_LENGTH_CLOUD1_FX;
		PreviewFX2[1].Length = VEC_LENGTH_CLOUD2_FX;
		PreviewFX2[2].Length = VEC_LENGTH_CLOUD3_FX;
		PreviewFX2[3].Length = VEC_LENGTH_CLOUD4_FX;
		PreviewFX2[4].Length = VEC_LENGTH_CLOUD5_FX;
		PreviewFX2[5].Length = VEC_LENGTH_CLOUD6_FX;
	}
}