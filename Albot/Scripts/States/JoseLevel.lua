--TEMPLATE FOR A SCRIPTED STATE


JoseLevel = 
{
	name = "JoseLevel";
}

--OnCreate, called when creating this state
JoseLevel.OnCreateState = function(self)
	LOG("Created " .. self.name .. " state\n");
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
JoseLevel.OnExitState = function(self) 

	LOG("Exiting (destroying) " .. self.name .. " state\n");

end


--AT THE END, NEED TO ADD THIS LINE
return JoseLevel;
