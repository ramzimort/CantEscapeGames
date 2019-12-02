-- First approximation of a component script

FlyingBugLogic = 
{
	name = "FlyingBugLogic";
	spawnGameobjPrefabDir = "Assets\\Prefabs\\Projectiles\\EnemyProjectile.json";
	
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;
	triggerComp = nil;
	gameobjManager = nil;
	animComp = nil;
	ownerGameObj = nil;
	--gun variables
	onCooldown = false;
	cooldown = 0.0;
	maxCooldown = 1.0;
	bulletSpeed = 10.0;
	curHealth = 1;
}


--Method
--Init called when comp is created
FlyingBugLogic.Init = function(self)
end


--Begin called when obj has all comps
FlyingBugLogic.Begin = function(self, owner, goMgr)
	self.rgdbdyComp = owner:GetRigidbodyComp();
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();

	self.ownerGameObj = owner;
	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});
	self.animComp = owner:GetAnimationComp();
	self:AnimatorSetup();
	self.gameobjManager = goMgr;
	--self.playerTransformComp = self.playerGameObj:GetTransformComp();
end

--Update called every tick
FlyingBugLogic.Update = function(self, dt, owner) 
	local followCurvesPathComp = owner:GetFollowCurvesPathComp();
	if (followCurvesPathComp == nil) then 
		return;
	end
	local isInMotion = followCurvesPathComp:IsMotionAlongPathEnabled();
	if (not isInMotion) then
		owner:Destroy();
		return;
	end

	local playerGameObj = self.gameobjManager:FindGameObject("player01");
	if(playerGameObj == nil) then
		return;
	end
	local playerTransformComp = playerGameObj:GetTransformComp()
	local playerPosition = playerTransformComp:GetPosition();
	local pos = self.transformComp:GetPosition();

	local dir = playerPosition - pos;
	dir:normalize();

	if (self.onCooldown) then
		self.cooldown = self.cooldown + dt;			
		if (self.cooldown > self.maxCooldown) then
			self.cooldown = 0.0
			self.onCooldown = false
		end
	end

	if (not self.onCooldown) then
		local newSpawnedProjectile = GameObject.Instantiate(self.gameobjManager, self.spawnGameobjPrefabDir);
		local projectileTransform = newSpawnedProjectile:GetTransformComp();
		projectileTransform:SetLocalPosition(pos + dir);
		local projectileRigidbody = newSpawnedProjectile:GetRigidbodyComp();
		local pojectileVelocity = dir * self.bulletSpeed;
		projectileRigidbody:SetVelocity(pojectileVelocity);
		self.onCooldown = true;
	end
end

FlyingBugLogic.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
	OutputPrint("Flying bug destruction \n");
end


FlyingBugLogic.OnEnter = function(self, gameObj1, gameObj2)
	
	local transform1 = gameObj1:GetTransformComp();
	local transform2 = gameObj2:GetTransformComp();
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if(transform1 == nil or transform2 == nil or triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	local collisionMask1 = triggerComp1:GetCollisionMask();
	local collisionMask2 = triggerComp2:GetCollisionMask();
	
	if (collisionMask2 == CollisionMask.PLAYER_PROJ) then
		--local farAway1 = Vector3.new(1100000, -1100000, 1100000);
		
		--OutputPrint("Flying bug set to be destroyed \n");
		--transform1:SetLocalPosition(farAway1);
		self.curHealth = self.curHealth - 1;
		if(self.curHealth <= 0) then
			self.ownerGameObj:Destroy();
			local followCurvesPathComp = gameObj1:GetFollowCurvesPathComp();
			if (followCurvesPathComp) then
				followCurvesPathComp:SetEnableMotionAlongPath(false);
			end
		end
	end

	--PLAY SOUND
	--SPAWN PARTICLES


end

FlyingBugLogic.OnExit = function(self, gameObj1, gameObj2)
end

FlyingBugLogic.AnimatorSetup = function(self)
	
	--STATES-
	local idle_State =		self.animComp:CreateState("idle", "ShootAnim");

	--STARTING STATE FOR THE STAGTE MACHINE
	self.animComp:SetEntryState(idle_State);
end

return FlyingBugLogic;
