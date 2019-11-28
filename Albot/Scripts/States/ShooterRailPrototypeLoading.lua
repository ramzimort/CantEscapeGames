--TEMPLATE FOR A SCRIPTED STATE


ShooterRailPrototypeLoading =
{
	name = "ShooterRailPrototypeLoading",
	NextLevel = "Assets\\Levels\\ShooterRailPrototype.json";
}

--OnCreate, called when creating this state
ShooterRailPrototypeLoading.OnCreateState = function(self)
	local World = EventManager.Get();
	World:LoadState(false, self.NextLevel);
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
ShooterRailPrototypeLoading.OnExitState = function(self) 

end


--AT THE END, NEED TO ADD THIS LINE
return ShooterRailPrototypeLoading;
