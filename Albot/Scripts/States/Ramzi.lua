--TEMPLATE FOR A SCRIPTED STATE


Ramzi = 
{
	name = "Ramzi" 
}

--OnCreate, called when creating this state
Ramzi.OnCreateState = function(self)
	local EventMgr = EventManager.Get();
	EventMgr:PushLoadedState(false);
end

--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Ramzi.OnExitState = function(self) 
	local EventMgr = EventManager.Get();
end

--AT THE END, NEED TO ADD THIS LINE
return Ramzi;