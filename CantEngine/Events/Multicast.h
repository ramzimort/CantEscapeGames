/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Jose Rosenbluth
Other Authors : <None>
-End Header--------------------------------------------------------*/

#pragma once
#include "Delegate.h"


template <typename T>
class Multicast;


template<typename RET, typename ... PARAMS>
class Multicast<RET(PARAMS ...)>
{
	//BHVR - Should have a list of delegates
	// Should be able to add and remove them dynamically
	// Should call them all when the multicast is called


public:
	using Delegates_List_Type = std::list< delegate<RET(PARAMS...)>* >;


public:
	Multicast() { }
	//Multicast(Multicast const& rhs) = delete;
	//Multicast& operator= (Multicast const& rhs) = delete;


	~Multicast()
	{
		//Clear the subscribers
		for (auto *suscriber : suscribers)
			delete suscriber;
		suscribers.clear();

		//Clear the lua subscribers
		lua_suscribers.clear();
	}


	void operator() (PARAMS ... params)
	{
		//OutputDebugString("CALLED MULTICAST FROM LUA");

		for (auto *suscriber : suscribers)
		{
 			(*suscriber)(params ...);
		}

		for (auto& suscriber : lua_suscribers)
		{
			try 
			{
				if (suscriber[1] != sol::nil && suscriber[2] != sol::nil)
				{
					sol::table self = suscriber[1];
					sol::function func = suscriber[2];
					func(self, params ...);
				}
				else if (suscriber[2] != sol::nil)
				{
					sol::function func = suscriber[2];
					func(params ...);
				}
				else
				{
					OutputDebugString("\nERROR RUNNING LUA CODE BOUND TO MULTICAST!\n");
				}
			}
			catch (const sol::error& e)
			{
				const char *errorName = e.what();
				DEBUG_LOG(errorName); //TODO - erase this
			}
		}
	}


	//Delegate created is passed here by ref
	Multicast& operator += (delegate<RET(PARAMS...)> const& rhs)
	{
		//Copy ctor (from ref, new heap obj)
		delegate<RET(PARAMS...)> *d = new delegate<RET(PARAMS...)>(rhs);

		suscribers.push_back(d);

		return *this;
	}

	//First attempt, a table which has a self and a function
	void BindLuaFunction(sol::table entry)
	{
		this->lua_suscribers.push_back(entry);
	}

	//Removing the entry (not sure if right fix but does the job)
	void UnbindLuaFunction(sol::table entry)
	{
		for (std::list<sol::table>::const_iterator it = lua_suscribers.begin(); it != lua_suscribers.end(); ++it ) 
		{
			const sol::function& func1 = (*it)[2];
			const sol::function& func2 = entry[2];
			if (func1 == func2)
			{
				lua_suscribers.erase(it);
				break;
			}
		}
	}


	Multicast& operator -=(delegate<RET(PARAMS...)> const& to_remove)
	{
		for (auto *dlgte : suscribers)
		{
			if (*dlgte == to_remove)
			{
				suscribers.erase(dlgte);
				delete dlgte;
				return *this;
			}
		}
		return *this;
	}


private:
	//Cpp suscribers
	Delegates_List_Type suscribers;
	std::list<sol::table> lua_suscribers;
};