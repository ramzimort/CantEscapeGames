/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

class CastResult
{
public:
	CastResult();
	explicit CastResult(void* clientData);
	CastResult(void* clientData, float time);

	// Comparison function used for unit testing
	bool operator<(const CastResult& rhs) const;

	// What time the object was hit in a raycast. In a frustum cast this left alone.
	float m_Time;
	// The object that was hit from the cast (so the user can check the original object).
	void* m_ClientData;
};

class CastResults
{
public:
	// Adds the result and sorts it based upon t so that all
	// results are in proper lower t first order.
	void AddResult(const CastResult& result);

	typedef std::vector<CastResult> Results;
	Results m_Results;
};