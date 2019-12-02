dofile("Scripts\\Helper\\ShooterRailPrototype_Common.lua")

SpawnRailEnemy1Comp = 
{
	spawnGameobjPrefabDir = "Assets\\Prefabs\\ShooterEnemies\\flying_bug.json";
	gameobjManager = nil;
	ownerGameObj = nil;
	spawnTimeInterval = 0.5;
	numberOfSpawnGameObj = 5;
	numberOfAliveGameObj = 0;
	curTime = 0.0;
	minMotionSpeed = 4.0;
	maxMotionSpeed = 8.0;
	motionSpeed = 5.0;
	curveGameObjTag = "enemy_rail01";
	beginSpawningFlag = false;
	triggerComp = nil;
	transformComp = nil;
	playerGameObj = nil;
	enableMotionOrientation = false;
};

--Init called when obj has all comps
SpawnRailEnemy1Comp.Init  = function(self)

end

--Begin called when on that frame all objects has been instiated
SpawnRailEnemy1Comp.Begin = function(self, owner, gameobjManager)
	self.gameobjManager = gameobjManager;
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();
	self.ownerGameObj = owner;
	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});
	self.playerGameObj = gameobjManager:FindGameObject("player01");
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
		self.numberOfAliveGameObj = self.numberOfAliveGameObj + 1;
		local newSpawnedGameObj = GameObject.Instantiate(self.gameobjManager, self.spawnGameobjPrefabDir);
		
		local spawnedEnemyScriptComp = newSpawnedGameObj:GetCustomComp("SpawnedEnemyComp");
		spawnedEnemyScriptComp.OnDestroyedMulticast:suscribe({self, self.OnSpawnedEnemyDestroyed});
		
		local spawnedTransformComp = newSpawnedGameObj:GetTransformComp();
		spawnedTransformComp:Scale(0.001, 0.001, 0.001);

		local followCurvesPathComp = newSpawnedGameObj:GetFollowCurvesPathComp();
		local newMotionSpeed = RandF(self.minMotionSpeed, self.maxMotionSpeed);

		followCurvesPathComp:SetEnableMotionAlongPath(true);
		followCurvesPathComp:SetMotionSpeed(newMotionSpeed);
		followCurvesPathComp:SetBeforeInitCurveGameObjectToFollow(self.curveGameObjTag);
		followCurvesPathComp:SetEnableMotionOrientation(self.enableMotionOrientation);
	end
end

SpawnRailEnemy1Comp.OnSpawnedEnemyDestroyed = function(self)
	self.numberOfAliveGameObj = self.numberOfAliveGameObj - 1;
	if(self.numberOfSpawnGameObj <= 0 and self.numberOfAliveGameObj <= 0) then
		local playerCurvesPathComp = self.playerGameObj:GetFollowCurvesPathComp();
		playerCurvesPathComp:SetEnableMotionAlongPath(true);
	end
end

SpawnRailEnemy1Comp.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
end


SpawnRailEnemy1Comp.OnEnter = function(self, gameObj1, gameObj2)
	if(gameObj1:GetTag() ~= "player01" and gameObj2:GetTag() ~= "player01") then
		return;
	end
	self.beginSpawningFlag = true;
	local playerCurvesPathComp = self.playerGameObj:GetFollowCurvesPathComp();
	playerCurvesPathComp:SetEnableMotionAlongPath(false);
end

SpawnRailEnemy1Comp.OnExit = function(self, gameObj1, gameObj2)
end

return SpawnRailEnemy1Comp;