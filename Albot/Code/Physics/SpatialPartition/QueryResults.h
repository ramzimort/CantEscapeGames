/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

class QueryResult
{
public:
	QueryResult();
	QueryResult(void* clientData0, void* clientData1);
	bool operator==(const QueryResult& rhs) const;

	void* mClientData0;
	void* mClientData1;
};

// A collection of query results. These are not sorted in any specific order.
class QueryResults
{
public:
	void AddResult(const QueryResult& result);
	void DeleteDuplicates();

	typedef std::vector<QueryResult> Results;
	Results mResults;
};
