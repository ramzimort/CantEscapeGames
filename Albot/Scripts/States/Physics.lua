--TEMPLATE FOR A SCRIPTED STATE


Physics =
{
	name = "Physics"
}

--OnCreate, called when creating this state
Physics.OnCreateState = function(self)
	local EventMgr = EventManager.Get();
	EventMgr:PushLoadedState(false);
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Physics.OnExitState = function(self) 
end


--AT THE END, NEED TO ADD THIS LINE
return Physics;
