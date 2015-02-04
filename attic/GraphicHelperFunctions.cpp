#include <iostream>
#include <cmath>
using namespace std;
#include "GraphicHelperFunctions.hpp"

namespace Engine
{
	// TODO: improve calculateCenterPoint so it works even if the z's are different
	Point calculateCenterPoint(float width, float height, Object2D& objectParam)
	{
		// constants
		const float HALF_HEIGHT = height / 2.0f;
		const float HALF_WIDTH = width / 2.0f;

		// determine center point
		Point topLeft = objectParam.getTopLeftPoint();
		Point topRight = objectParam.getTopRightPoint();
		Point bottomRight = objectParam.getBottomRightPoint();
		Point bottomLeft = objectParam.getBottomLeftPoint();

		Point tempPoint;
		if (topLeft.getZ() == topRight.getZ() && bottomRight.getZ() == bottomLeft.getZ()) //aligned along x axis
		{
			tempPoint = Point(HALF_WIDTH, HALF_HEIGHT, topLeft.getZ());
		}
		else if (topLeft.getX() == topRight.getX() && bottomRight.getX() == bottomLeft.getX()) // aligned along z axis
		{
			tempPoint = Point(topLeft.getX(), HALF_HEIGHT, HALF_WIDTH);
		}

		return tempPoint;
	}

	void calcuateFromCenter(const Point& centerPointParam, float desiredWidth, float desiredHeight, Object2D& objectParam, bool useZasX)
	{
		// constants
		const float HALF_WIDTH = desiredWidth / 2.0f;
		const float HALF_HEIGHT = desiredHeight / 2.0f;

		// variables
		float left = 0.0f;
		float right = 0.0f;
		float top = 0.0f;
		float bottom = 0.0f;
		float z = 0.0f;

		// calculate where the points should be
		if (useZasX == false)
		{
			left = centerPointParam.getX() - HALF_WIDTH;
			right = centerPointParam.getX() + HALF_WIDTH;
			top = centerPointParam.getY() + HALF_HEIGHT;
			bottom = centerPointParam.getY() - HALF_HEIGHT;
			z = centerPointParam.getZ();

			objectParam.setTopLeftPoint(Point(left, top, z));
			objectParam.setTopRightPoint(Point(right, top, z));
			objectParam.setBottomRightPoint(Point(right, bottom, z));
			objectParam.setBottomLeftPoint(Point(left, bottom, z));
		}
		else
		{
			left = centerPointParam.getX() - HALF_WIDTH;
			right = centerPointParam.getX() + HALF_WIDTH;
			top = centerPointParam.getY() + HALF_HEIGHT;
			bottom = centerPointParam.getY() - HALF_HEIGHT;
			z = centerPointParam.getZ();
			
			objectParam.setTopLeftPoint(Point(z, top, left));
			objectParam.setTopRightPoint(Point(z, top, right));
			objectParam.setBottomRightPoint(Point(z, bottom, right));
			objectParam.setBottomLeftPoint(Point(z, bottom, left));
		}
	}

	Point calculateSlope(Point& beginPointParam, Point& endPointParam, int xPointParam, float yInterceptParam)
	{
		// determine the m, x and b
		float m = (endPointParam.getY() - beginPointParam.getY()) / endPointParam.getX() - beginPointParam.getX();
		int x = xPointParam;
		float b = yInterceptParam;

		// calculate the y
		float y = m * x + b;

		return Point(static_cast<float>(x), y, 0.0f);
	}

	float calculateHypotenuse(float a, float b)
	{
		return sqrt(pow(a, 2) + pow(b, 2));
	}
}