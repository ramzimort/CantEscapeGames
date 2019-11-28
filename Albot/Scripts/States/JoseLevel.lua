--TEMPLATE FOR A SCRIPTED STATE


JoseLevel = 
{
	name = "JoseLevel";
}

--OnCreate, called when creating this state
JoseLevel.OnCreateState = function(self)
	local EventMgr = EventManager.Get();
	EventMgr:PushLoadedState(false);
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
JoseLevel.OnExitState = function(self) 
end


--AT THE END, NEED TO ADD THIS LINE
return JoseLevel;
