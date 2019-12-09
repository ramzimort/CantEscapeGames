/**
 * @file QueryResults.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief A helper classes that are stores information about which objects are colliding 
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once

/**
 * @brief Stores which objects are interacting for only one interaction
 * 
 */
class QueryResult
{
public:
	QueryResult();
	QueryResult(void* clientData0, void* clientData1);
	/**
	 * @brief Checks the raw pointers to see if the other result stores pair of exactly same client data
	 * 
	 * @param rhs 
	 * @return true 
	 * @return false 
	 */
	bool operator==(const QueryResult& rhs) const;

	void* m_clientData0;
	void* m_clientData1;
};

/**
 * @brief  A collection of query results. These are not sorted in any specific order.
 * 
 */
class QueryResults
{
public:
	/**
	 * @brief Adds a new entry into the vector of querry results
	 * 
	 * @param result 
	 */
	void AddResult(const QueryResult& result);
	/**
	 * @brief Deletes any duplicated querries
	 * 
	 */
	void DeleteDuplicates();

	typedef std::vector<QueryResult> Results;
	Results m_results;
};
