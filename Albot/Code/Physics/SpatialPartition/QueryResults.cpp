/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "QueryResults.h"

///////////////////////////// QueryResult ////////////////////////////////////
QueryResult::QueryResult() : mClientData0(nullptr), mClientData1(nullptr)
{
}

QueryResult::QueryResult(void* clientData0, void* clientData1) : mClientData0(clientData0), mClientData1(clientData1)
{
}

bool QueryResult::operator==(const QueryResult& rhs) const
{
	return ((mClientData0 == rhs.mClientData0 && mClientData1 == rhs.mClientData1) ||
		(mClientData0 == rhs.mClientData1 && mClientData1 == rhs.mClientData0));
}

///////////////////////////// QueryResults ////////////////////////////////////
void QueryResults::AddResult(const QueryResult& result)
{
	mResults.push_back(result);
}

void QueryResults::DeleteDuplicates()
{
	auto end = mResults.end();

	for (auto it = mResults.begin(); it != end; ++it)
	{
		end = std::remove(it + 1, end, *it);
	}

	mResults.erase(end, mResults.end());
}
