#pragma once
#define _USE_MATH_DEFINES

#include "vector.h"

namespace Collision
{
	struct Point final
	{
		vector<2>	Position;
	};

	struct Circle final
	{
		vector<2>	Position;
		float		Radius;
	};

	struct Rectangle final
	{
		vector<2>	Position;
		vector<2>	Length;
		float		Angle;
	};

	bool Collide(Point const&, Point const&);
	bool Collide(Point const&, Circle const&);
	bool Collide(Point const&, Rectangle const&);
	bool Collide(Circle const&, Point const&);
	bool Collide(Circle const&, Circle const&);
	bool Collide(Circle const&, Rectangle const&);
	bool Collide(Rectangle const&, Point const&);
	bool Collide(Rectangle const&, Circle const&);
	bool Collide(Rectangle const&, Rectangle const&);
};