/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

class Sat
{
	struct MinkowskiPoint
	{
		Vector3 m_pointA;		  // point in object A space
		Vector3 m_pointB;		  // point in object B space
		Vector3 m_minkowskiPoint; // point in minkowski space 
	};
};