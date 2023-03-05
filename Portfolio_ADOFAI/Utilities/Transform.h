#pragma once
#include "Engine/vector.h"

namespace Transform
{
	inline matrix<2, 2> Scaling(vector<2> scale)
	{
		return matrix<2, 2>
		{
			scale[0],	 0,
			0,			 scale[1]
		};
	}

	inline matrix<2, 2> Rotation(float angle)
	{
		float const rad = static_cast<float>(M_PI) / 180.0f * angle;

		return matrix<2, 2>
		{
			cos(rad),	-sin(rad),	
			sin(rad),	 cos(rad)
		};
	}
}