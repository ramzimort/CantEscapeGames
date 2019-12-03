/**
 * @file CastResults.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Inteface for the ray cast results 
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once

/**
 * @brief Stores information of a single collision of a ray in a ray cast - its time of collision and related user data
 * 
 */
class CastResult
{
public:
	CastResult();
	explicit CastResult(void* clientData);
	CastResult(void* clientData, float time);

	/**
	 * @brief Compares 2 results, first by time of collision, then resolve conflicts by client data
	 * 
	 * @param rhs 
	 * @return true 
	 * @return false 
	 */
	bool operator<(const CastResult& rhs) const;

	// What time the object was hit in a raycast. In a frustum cast this left alone.
	float m_Time;
	// The object that was hit from the cast (so the user can check the original object).
	void* m_ClientData;
};

/**
 * @brief Stores ray cast results, all the collisions with the ray
 * 
 */
class CastResults
{
public:
	/**
	 * @brief Adds the result and sorts it based upon t so that all results are in proper lower t first order
	 * 
	 * @param result 
	 */
	void AddResult(const CastResult& result);

	typedef std::vector<CastResult> Results;
	Results m_Results;
};