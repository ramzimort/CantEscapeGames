--TEMPLATE FOR A SCRIPTED STATE


Setting = 
{
	name = "Setting"
}

--OnCreate, called when creating this state
Setting.OnCreateState = function(self)
	
	OutputPrint("Created " .. self.name .. " state");

end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Setting.OnExitState = function(self) 

	OutputPrint("Exiting (destroying) " .. self.name .. " state");

end


--AT THE END, NEED TO ADD THIS LINE
return Setting;
