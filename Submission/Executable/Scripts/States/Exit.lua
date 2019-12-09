--TEMPLATE FOR A SCRIPTED STATE


Exit = 
{
	name = "Exit"
}

--OnCreate, called when creating this state
Exit.OnCreateState = function(self)
	
	OutputPrint("Created " .. self.name .. " state");

end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Exit.OnExitState = function(self) 

	OutputPrint("Exiting (destroying) " .. self.name .. " state");

end


--AT THE END, NEED TO ADD THIS LINE
return Exit;
