--TEMPLATE FOR A SCRIPTED STATE


ShooterRailPrototype =
{
	name = "ShooterRailPrototype"
}

--OnCreate, called when creating this state
ShooterRailPrototype.OnCreateState = function(self)
	local EventMgr = EventManager.Get();
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
ShooterRailPrototype.OnExitState = function(self) 

end


--AT THE END, NEED TO ADD THIS LINE
return ShooterRailPrototype;
