dofile("Scripts\\Helper\\ShooterRailPrototype_Common.lua")

SpawnedEnemyComp = 
{
	spawnerGameObjTag = "";
	gameobjManager = nil;
	OnDestroyedMulticast = multicast();
};

--Init called when obj has all comps
SpawnedEnemyComp.Init  = function(self)
end

--Begin called when on that frame all objects has been instiated
SpawnedEnemyComp.Begin = function(self, owner, gameobjManager)
	self.gameobjManager = gameobjManager;
end

--Update called every tick
SpawnedEnemyComp.Update = function(self, dt, owner)
	
end

SpawnedEnemyComp.OnDestruction = function(self)
	self.OnDestroyedMulticast();
	--[[if(self.spawnerGameObjTag == "") then
		return;
	end
	self.gameobjManager:FindGameObject(self.spawnerGameObjTag);]]
	OutputPrint("Destroyed gameobject\n");
end


SpawnedEnemyComp.OnEnter = function(self, gameObj1, gameObj2)
	
end

SpawnedEnemyComp.OnExit = function(self, gameObj1, gameObj2)
	
end

return SpawnedEnemyComp;