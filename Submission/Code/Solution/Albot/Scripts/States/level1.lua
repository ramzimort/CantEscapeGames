--TEMPLATE FOR A SCRIPTED STATE


level1 = 
{
	name = "level1";
}

--OnCreate, called when creating this state
level1.OnCreateState = function(self)
	local EventMgr = EventManager.Get();
	EventMgr:PopState(false);
	EventMgr:PushLoadedState(false);
	LOG("Created " .. self.name .. " state\n");
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
level1.OnExitState = function(self) 

	LOG("Exiting (destroying) " .. self.name .. " state\n");

end


--AT THE END, NEED TO ADD THIS LINE
return level1;
