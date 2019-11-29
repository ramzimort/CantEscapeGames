Fireball = 
{
	name = "Fireball";
	
	--Custom stuff
	startingPosition = nil;
	transformComp = nil;
	triggerComp = nil;
	PlayerScript = nil;

	moveSpeed = Vector3.new(0.0, 0.0, 8.0);
}

Fireball.Init = function(self)
end

Fireball.Begin = function(self, owner, goMgr)
	self.transformComp = owner:GetTransformComp();
	self.startingPosition = Vector3.new(self.transformComp:GetPosition());
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.Player = goMgr:FindGameObject("Player"):GetCustomComp("RamziPlayer");
end

Fireball.Update = function(self, dt, owner) 
	local position = self.transformComp:GetPosition();
	local newPosition = position + self.moveSpeed*dt;

	if(position.z > -30) then 
		self.transformComp:SetLocalPosition(self.startingPosition);
	else
		self.transformComp:SetLocalPosition(newPosition);
	end
end

Fireball.OnEnter = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if (triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	if (gameObj2:GetTag() == "PlayerCube") then
			self.Player.dead = true;
	end
end

Fireball.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
end

return Fireball;