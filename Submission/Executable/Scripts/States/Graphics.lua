--TEMPLATE FOR A SCRIPTED STATE


Graphics =
{
	name = "Graphics"
}

--OnCreate, called when creating this state
Graphics.OnCreateState = function(self)
	local EventMgr = EventManager.Get();
	EventMgr:PushLoadedState(false);
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Graphics.OnExitState = function(self) 

end


--AT THE END, NEED TO ADD THIS LINE
return Graphics;
