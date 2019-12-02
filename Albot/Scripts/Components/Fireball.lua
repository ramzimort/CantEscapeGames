Fireball = 
{
	name = "Fireball";
	
	--Custom stuff
	startingPosition = nil;
	transformComp = nil;
	triggerComp = nil;
	Player = nil;

	moveSpeed = Vector3.new(0.0, 0.0, 4.0);
	enabled = false;
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
	if(self.enabled) then
		local position = self.transformComp:GetPosition();
		local newPosition = position + self.moveSpeed*dt;
		self.transformComp:SetLocalPosition(newPosition);
		if(position.z > -35) then 
			self.enabled = false;
		end
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