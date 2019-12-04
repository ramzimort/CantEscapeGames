-- First approximation of a component script

ShootingPlayerLogic = 
{
	name = "ShootingPlayerLogic";
	spawnGameobjPrefabDir = "Assets\\Prefabs\\Projectiles\\EnemyProjectile.json";
	maxCooldown = 5.0;
	bulletSpeed = 1.0;
	curHealth = 1.0;


	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;
	triggerComp = nil;
	gameobjManager = nil;
	animComp = nil;
	rendererComp = nil;
	ownerGameObj = nil;
	--gun variables
	onCooldown = true;
	cooldown = 0.0;

	offsetProjectileY = 0.0;

	xAxis = nil;
	yAxis = nil;
	zAxis = nil;
	

	finalOffsetProjectile = nil;

	blinkingTime = 1.0;
	curBlinkingTime = 0.0;
	blinkingTimeInterval = 0.1;
	curElapBlinkingTime = 0.0;
}


--Method
--Init called when comp is created
ShootingPlayerLogic.Init = function(self)
	self.xAxis = Vector3.new(1.0, 0.0, 0.0);
	self.yAxis = Vector3.new(0.0, 1.0, 0.0);
	self.zAxis = Vector3.new(0.0, 0.0, 1.0);
	self.finalOffsetProjectile = Vector3.new(0.0, self.offsetProjectileY, 0.0);
end


--Begin called when obj has all comps
ShootingPlayerLogic.Begin = function(self, owner, goMgr)
	self.rgdbdyComp = owner:GetRigidbodyComp();
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();
	self.rendererComp = owner:GetRendererComp();

	self.ownerGameObj = owner;
	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});
	self.animComp = owner:GetAnimationComp();
	self:AnimatorSetup();
	self.gameobjManager = goMgr;
	--self.playerTransformComp = self.playerGameObj:GetTransformComp();
end

--Update called every tick
ShootingPlayerLogic.Update = function(self, dt, owner)
	self.rendererComp:SetEnableRendering(true);

	if(self.curBlinkingTime > 0) then
		self.curBlinkingTime = self.curBlinkingTime - dt;
		self.curElapBlinkingTime = self.curElapBlinkingTime + dt;
		if(self.curElapBlinkingTime >= self.blinkingTimeInterval) then
			self.rendererComp:SetEnableRendering(true);
			self.curElapBlinkingTime = self.curElapBlinkingTime - self.blinkingTimeInterval;
		else
			self.rendererComp:SetEnableRendering(false);
		end
	end
	
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
	pos = pos + self.finalOffsetProjectile;

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
		EventManager.Get():PlaySFX(false, "Assets\\SFX\\AllienGunshot.ogg");
	end
end

ShootingPlayerLogic.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
	OutputPrint("Flying bug destruction \n");
end


ShootingPlayerLogic.OnEnter = function(self, gameObj1, gameObj2)
	
	
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
		EventManager.Get():PlaySFX(false, "Assets\\SFX\\AllienTakesDamage.wav");
		self.curHealth = self.curHealth - 1;
		self.curBlinkingTime = self.blinkingTime;
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

ShootingPlayerLogic.OnExit = function(self, gameObj1, gameObj2)
end

ShootingPlayerLogic.AnimatorSetup = function(self)
	
	--STATES-
	local idle_State =		self.animComp:CreateState("idle", "ShootAnim");

	--STARTING STATE FOR THE STAGTE MACHINE
	self.animComp:SetEntryState(idle_State);
end

return ShootingPlayerLogic;
