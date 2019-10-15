--TEMPLATE FOR A SCRIPTED STATE


level1 = 
{
	name = "level1"
}

--OnCreate, called when creating this state
level1.OnCreateState = function(self)
	
	OutputPrint("Created " .. self.name .. " state");

end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
level1.OnExitState = function(self) 

	OutputPrint("Exiting (destroying) " .. self.name .. " state");

end


--AT THE END, NEED TO ADD THIS LINE
return level1;
