#pragma once

class Calculation
{
private:

	Calculation() {}
	~Calculation() {}


public:
	static struct NearestResult
	{
		VECTOR linePos_start;
		VECTOR linePos_end;
		VECTOR nearestPoint;
	};

	static void Initialize();

	static void UnInitialize();
	static float radToDeg(float radian);

	static float area(const VECTOR& a, const VECTOR& b, const VECTOR& c);

	static VECTOR NearestPoint(const VECTOR& position_1, const VECTOR& position_2,
		const VECTOR& point);
	static MATRIX Rotate(const VECTOR& wall_normal);
	static VECTOR ProjectionDirection(const VECTOR& point, const VECTOR& a, const VECTOR& b);
	static VECTOR Leap(const VECTOR& changePosition, const VECTOR& latestPosition, const float& speed);
	static VECTOR SphereMeshOutsideTriangle(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground);
	static NearestResult SphereMeshOutsideTriangle_line(const MV1_COLL_RESULT_POLY& poly, const VECTOR& HitPos_ground);
};


