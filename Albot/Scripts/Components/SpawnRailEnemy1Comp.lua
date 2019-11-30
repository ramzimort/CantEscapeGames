dofile("Scripts\\Helper\\ShooterRailPrototype_Common.lua")

SpawnRailEnemy1Comp = 
{
	spawnGameobjPrefabDir = "Assets\\Prefabs\\ShooterEnemies\\flying_bug.json";
	gameobjManager = nil;
	spawnTimeInterval = 1.0;
	numberOfSpawnGameObj = 5;
	numberOfAliveGameObj = 0;
	curTime = 0.0;
	motionSpeed = 5.0;
	curveGameObjTag = "enemy_rail01";
	beginSpawningFlag = true;
};

--Init called when obj has all comps
SpawnRailEnemy1Comp.Init  = function(self)

end

--Begin called when on that frame all objects has been instiated
SpawnRailEnemy1Comp.Begin = function(self, owner, gameobjManager)
	self.gameobjManager = gameobjManager;
end

--Update called every tick
SpawnRailEnemy1Comp.Update = function(self, dt, owner)
	if(self.beginSpawningFlag ~= true or self.numberOfSpawnGameObj <= 0) then
		return;
	end
	
	self.curTime = self.curTime + dt;

	while self.curTime >= self.spawnTimeInterval do
		self.curTime = self.curTime - self.spawnTimeInterval;
		self.numberOfSpawnGameObj = self.numberOfSpawnGameObj - 1;
		local newSpawnedGameObj = GameObject.Instantiate(self.gameobjManager, self.spawnGameobjPrefabDir);
		local followCurvesPathComp = newSpawnedGameObj:GetFollowCurvesPathComp();
		followCurvesPathComp:SetEnableMotionAlongPath(true);
		followCurvesPathComp:SetMotionSpeed(self.motionSpeed);
		followCurvesPathComp:SetBeforeInitCurveGameObjectToFollow(self.curveGameObjTag);
	end
end

SpawnRailEnemy1Comp.OnDestruction = function(self)
	
end

return SpawnRailEnemy1Comp;