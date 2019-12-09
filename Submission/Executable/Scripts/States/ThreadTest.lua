--TEMPLATE FOR A SCRIPTED STATE


Testing = 
{
	name = "level1";
}

--OnCreate, called when creating this state
Testing.OnCreateState = function(self)
	
	OutputPrint("Created " .. self.name .. " state");

end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Testing.OnExitState = function(self) 

	OutputPrint("Exiting (destroying) " .. self.name .. " state");

end


--AT THE END, NEED TO ADD THIS LINE
return Testing;
