--TEMPLATE FOR A SCRIPTED STATE


Physics =
{
	name = "Physics";
	NextLevel = "Assets\\Levels\\Physics.json";
}

--OnCreate, called when creating this state
Physics.OnCreateState = function(self)
	local World = EventManager.Get();
	World:LoadState(false, self.NextLevel);
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Physics.OnExitState = function(self) 

end


--AT THE END, NEED TO ADD THIS LINE
return Physics;
