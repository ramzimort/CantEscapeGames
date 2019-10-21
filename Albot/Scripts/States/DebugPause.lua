--TEMPLATE FOR A SCRIPTED STATE


DebugPause = 
{
	name = "DebugPause"
}

--OnCreate, called when creating this state
DebugPause.OnCreateState = function(self)
	
	OutputPrint("Created " .. self.name .. " state");

end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
DebugPause.OnExitState = function(self) 

	OutputPrint("Exiting (destroying) " .. self.name .. " state");

end


--AT THE END, NEED TO ADD THIS LINE
return DebugPause;
