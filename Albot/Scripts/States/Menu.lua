--TEMPLATE FOR A SCRIPTED STATE


Menu = 
{
	name = "Menu"
}

--OnCreate, called when creating this state
Menu.OnCreateState = function(self)
	OutputPrint("Created " .. self.name .. " state");
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Menu.OnExitState = function(self) 
	OutputPrint("Exiting (destroying) " .. self.name .. " state");
end

--AT THE END, NEED TO ADD THIS LINE
return Menu;
