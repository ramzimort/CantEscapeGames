--TEMPLATE FOR A SCRIPTED STATE


Loading = 
{
	name = "Loading";
	NextLevel = "Assets\\Levels\\level1.json";
}

--OnCreate, called when creating this state
Loading.OnCreateState = function(self)

	local World = EventManager.Get();
	World:LoadState(false, "Assets\\Levels\\level1.json");
	LOG("Created " .. self.name .. " state");
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Loading.OnExitState = function(self) 

	LOG("Exiting (destroying) " .. self.name .. " state");

end


--AT THE END, NEED TO ADD THIS LINE
return Loading;
