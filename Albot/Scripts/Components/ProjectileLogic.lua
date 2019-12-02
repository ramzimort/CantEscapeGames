-- First approximation of a component script

ProjectileLogic = 
{
	name = "ProjectileLogic";
	projectileOwnerName = "";
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;
	triggerComp = nil;
	lifeTime = 0.0;
	maxLifeTime = 25.0;
	ownerGameObject = nil;
}


--Method
--Init called when comp is created
ProjectileLogic.Init = function(self)
end


--Begin called when obj has all comps
ProjectileLogic.Begin = function(self, owner, goMgr)
	self.rgdbdyComp = owner:GetRigidbodyComp();
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});
	self.ownerGameObject = owner;

end


--Update called every tick
ProjectileLogic.Update = function(self, dt, owner) 
	self.lifeTime = self.lifeTime + dt;
	if (self.lifeTime > self.maxLifeTime) then
		owner:Destroy();
	end
end

ProjectileLogic.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter});
	self.triggerComp.OnExit:Unbind({self, self.OnExit});
end


ProjectileLogic.OnEnter = function(self, gameObj1, gameObj2)
	
	local transform1 = gameObj1:GetTransformComp();
	local transform2 = gameObj2:GetTransformComp();
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if(transform1 == nil or transform2 == nil or triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	local collisionMask1 = triggerComp1:GetCollisionMask();
	local collisionMask2 = triggerComp2:GetCollisionMask();

	if( (self.projectileOwnerName == "Enemy" and (collisionMask2 == CollisionMask.PLAYER or collisionMask2 == CollisionMask.PLAYER_PROJ) ) or 
		(self.projectileOwnerName == "Player" and (collisionMask2 == CollisionMask.ENEMY or collisionMask2 == CollisionMask.ENEMY_PROJ) )
	) then
		local farAway1 = Vector3.new(1000000, -1000000, 1000000);
		transform1:SetLocalPosition(farAway1);
		self.ownerGameObject:Destroy();
		local rigidBodyComp1 = gameObj1:GetRigidbodyComp();
		if(rigidBodyComp1 ~= nil) then
			rigidBodyComp1:SetVelocity(Vector3.new(1000.0, 100.0, 100.0));
		end
	end
	
	--[[if (collisionMask2 == CollisionMask.ENEMY or collisionMask2 == CollisionMask.PLAYER) then
		local farAway1 = Vector3.new(1000000, -1000000, 1000000);
		transform1:SetLocalPosition(farAway1);
	end]]

	--PLAY SOUND
	--SPAWN PARTICLES


end

ProjectileLogic.OnExit = function(self, gameObj1, gameObj2)
end

return ProjectileLogic;
