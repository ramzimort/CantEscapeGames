--TEMPLATE FOR A SCRIPTED STATE


Physics =
{
	name = "Physics"
}

--OnCreate, called when creating this state
Physics.OnCreateState = function(self)
	
	OutputPrint("Created " .. self.name .. " state\n");

end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Physics.OnExitState = function(self) 

	OutputPrint("Exiting (destroying) " .. self.name .. " state\n");

end


--AT THE END, NEED TO ADD THIS LINE
return Physics;
