#include "Collision.h"
#define RotatedVector(hVector, angle) hVector * transpose(Rotation(angle))

namespace Collision
{
	inline matrix<2, 2> Rotation(float const& angle)
	{
		float const radian = angle * static_cast<float>(M_PI) / 180.0f;
		return matrix<2, 2>
		{
			 cos(radian),  sin(radian),
			-sin(radian),  cos(radian)
		};
	}
	bool Collide(Point const& LHS, Point const& RHS)		{ return length(LHS.Position - RHS.Position) == 0; }
	bool Collide(Point const& LHS, Circle const& RHS)		{ return length(LHS.Position - RHS.Position) <= RHS.Radius;	}
	bool Collide(Point const& LHS, Rectangle const& RHS)
	{
		if (!RHS.Angle)
		{
			vector<2> minPos	= RHS.Position - RHS.Length / 2;
			vector<2> maxPos	= RHS.Position + RHS.Length / 2;

			return LHS.Position[0] >= minPos[0] &&
				   LHS.Position[0] <= maxPos[0] &&
				   LHS.Position[1] >= minPos[1] &&
				   LHS.Position[1] <= maxPos[1];
		}
		else return Collide(Point	 { RotatedVector(LHS.Position, -RHS.Angle) },
							Rectangle{ RotatedVector(RHS.Position, -RHS.Angle), RHS.Length });
	}
	bool Collide(Circle const& LHS, Point const& RHS)		{ return Collide(RHS, LHS); }
	bool Collide(Circle const& LHS, Circle const& RHS)		{ return length(LHS.Position - RHS.Position) <= (LHS.Radius + RHS.Radius); }
	bool Collide(Circle const& LHS, Rectangle const& RHS)
	{
		if (!RHS.Angle)
		{
			Rectangle extendedV	{ RHS.Position, vector<2>{ RHS.Length[0], RHS.Length[1] + 2 * LHS.Radius } };
			Rectangle extendedH	{ RHS.Position, vector<2>{ RHS.Length[0] + 2 * LHS.Radius, RHS.Length[1] } };
			vector<2> minPos	= RHS.Position - RHS.Length / 2;
			vector<2> maxPos	= RHS.Position + RHS.Length / 2;

			return Collide(Point{ LHS.Position }, extendedV) ||
				   Collide(Point{ LHS.Position }, extendedH) ||
				   Collide(LHS, Point{ vector<2>{ minPos[0], maxPos[1] } }) ||
				   Collide(LHS, Point{ vector<2>{ maxPos[0], maxPos[1] } }) ||
				   Collide(LHS, Point{ vector<2>{ minPos[0], minPos[1] } }) ||
				   Collide(LHS, Point{ vector<2>{ maxPos[0], minPos[1] } });
		}
		else return Collide(Circle	 { RotatedVector(LHS.Position, -RHS.Angle), LHS.Radius },
							Rectangle{ RotatedVector(RHS.Position, -RHS.Angle), RHS.Length });
	}
	bool Collide(Rectangle const& LHS, Point const& RHS)	{ return Collide(RHS, LHS);	}
	bool Collide(Rectangle const& LHS, Circle const& RHS)	{ return Collide(RHS, LHS); }
	bool Collide(Rectangle const& LHS, Rectangle const& RHS)
	{
		// AABB(Axis-aligned bounding box)	: 두 사각형이 이루는 각도가 같을 때의 사각형 간의 충돌처리.
		if (LHS.Angle == RHS.Angle)
		{
			if (!LHS.Angle)
			{
				vector<2> minPos_LHS = LHS.Position - LHS.Length / 2;
				vector<2> maxPos_LHS = LHS.Position + LHS.Length / 2;
				vector<2> minPos_RHS = RHS.Position - RHS.Length / 2;
				vector<2> maxPos_RHS = RHS.Position + RHS.Length / 2;

				return minPos_LHS[0] <= maxPos_RHS[0] &&
					   maxPos_LHS[0] >= minPos_RHS[0] &&
					   minPos_LHS[1] <= maxPos_RHS[1] &&
					   maxPos_LHS[1] >= minPos_RHS[1];
			}
			else return Collide(Rectangle{ RotatedVector(LHS.Position, -LHS.Angle), LHS.Length },
								Rectangle{ RotatedVector(RHS.Position, -LHS.Angle), RHS.Length });
		}

		// OBB(Oriented bounding box)		: 두 사각형이 이루는 각도가 다를 때의 사각형 간의 충돌처리.
		else
		{
			vector<2> const displacement = RHS.Position - LHS.Position;
			if ((length(LHS.Length) + length(RHS.Length) / 2 < length(displacement))) return false;

			vector<2> const axes[4]
			{
				(vector<2>(1, 0) * Rotation(LHS.Angle), LHS.Angle),
				(vector<2>(0, 1) * Rotation(LHS.Angle), LHS.Angle),
				(vector<2>(1, 0) * Rotation(RHS.Angle), RHS.Angle),
				(vector<2>(0, 1) * Rotation(RHS.Angle), RHS.Angle)
			};

			vector<2> const vectors[4]
			{
				axes[0] * LHS.Length[0],
				axes[1] * LHS.Length[1],
				axes[2] * RHS.Length[0],
				axes[3] * RHS.Length[1]
			};

			for (unsigned i = 0; i < 4; ++i)
			{
				float projectionRect = 0;
				for (unsigned j = 0; j < 4; ++j) projectionRect += abs(dot(axes[i], vectors[j]));
				if (projectionRect / 2 < abs(dot(axes[i], displacement))) return false;
			}

			return true;
		}
	}
}