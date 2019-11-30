/**
 * @file Shape.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Base class of the other shape classes 
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once

class Aabb;
class Sphere;
class AppRenderer;

/**
 * @brief Base class of the other shape classes
 * 
 */
class Shape
{
public:
#ifdef DEVELOPER
	virtual void DebugDraw(AppRenderer* pAppRenderer, const Vector4& color) const = 0;
#endif
	/**
	 * @brief Every derived shape have to define on its own how to detect intersection with an Aabb
	 * 
	 * @param aabb 
	 * @return true 
	 * @return false 
	 */
	virtual bool IsIntersect(const Aabb& aabb) const = 0;
	/**
	 * @brief Every derived shape have to define on its own how to detect intersection with a sphere
	 * 
	 * @param sphere 
	 * @return true 
	 * @return false 
	 */
	virtual bool IsIntersect(const Sphere& sphere) const = 0;
	
	virtual ~Shape() {}
};


