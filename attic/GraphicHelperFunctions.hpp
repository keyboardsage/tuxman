#ifndef GRAPHIC_HELPER_FUNCTIONS_HPP
#define GRAPHIC_HELPER_FUNCTIONS_HPP

#include "Point.hpp"
#include "Object2D.hpp"

namespace Engine
{
	/**
	 * Gets the center point of a quadrilateral.
	 * \param width to use when generating points
	 * \param height to use when generating points
	 * \param objectParam 2D object that has four points
	 * \return a Point object with all the center point data
	 */
	Point calculateCenterPoint(float width, float height, Object2D& objectParam);

	/**
	 * Calculates the four quad points using a central point.
	 * \note the top points are along the y axis and left points along the x axis
	 * \param centerPointParam the center point used to generate the other four points
	 * \param desiredWidth to use when generating points
	 * \param desiredHeight to use when generating points
	 * \param objectParam reference to 2D object that will be given the four generated points
	 * \param useZasX determines orientation of the square/quad. Built upon x and y axis is false, otherwise true.
	 */
	void calcuateFromCenter(const Point& centerPointParam, float desiredWidth, float desiredHeight, Object2D& objectParam, bool useZasX = false);

	/**
	 * Calculates the result of the slope-intercept formula.
	 * TODO: Make this work without the needing the y intercept point as first point.
	 * \param beginPointParam The beginning point, which is also the y-intercept.
	 * \param endPointParam The destination point.
	 * \patam xPointParam An x point along the line.
	 * \param yInterceptParam The y intercept of the line
	 * \return A Point that contains the y for the corresponding xPointParam.
	 */
	Point calculateSlope(Point& beginPointParam, Point& endPointParam, int xPointParam, float yInterceptParam);

	/**
	 * Calculates the hypotenuse.
	 * \param a The first side.
	 * \param b The second side.
	 * \return The hypotenuse as a float.
	 */
	float calculateHypotenuse(float a, float b);
}

#endif