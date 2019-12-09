--TEMPLATE FOR A SCRIPTED STATE


Loading =
{
	name = "Loading";
	NextLevel = "Assets\\Levels\\Menu.json";
}

--OnCreate, called when creating this state
Loading.OnCreateState = function(self)
	local World = EventManager.Get();
	World:LoadState(false, self.NextLevel);
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Loading.OnExitState = function(self) 

end


--AT THE END, NEED TO ADD THIS LINE
return Loading;
