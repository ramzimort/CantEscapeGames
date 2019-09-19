/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

class Aabb;
class Sphere;

class Shape
{
public:
	virtual void DebugDraw(const DirectX::SimpleMath::Vector4& color) const = 0;

	virtual bool IsIntersect(const Aabb& aabb) const = 0;
	virtual bool IsIntersect(const Sphere& sphere) const = 0;
	
	virtual ~Shape() {}
};


