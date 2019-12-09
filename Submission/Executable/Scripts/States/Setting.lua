--TEMPLATE FOR A SCRIPTED STATE


Setting = 
{
	name = "Setting"
}

--OnCreate, called when creating this state
Setting.OnCreateState = function(self)
	local EventMgr = EventManager.Get();
	EventMgr:PushLoadedState(false);
end

--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Setting.OnExitState = function(self) 
end

--AT THE END, NEED TO ADD THIS LINE
return Setting;
