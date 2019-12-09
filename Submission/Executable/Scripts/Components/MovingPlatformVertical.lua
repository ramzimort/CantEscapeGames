MovingPlatormVertical = 
{
	name = "MovingPlatormVertical";
	
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;
	triggerComp = nil;

	min = 0.0;
	max = 0.0;
	moveSpeed = Vector3.new(0.0, 3.0, 0.0);
	enabled1 = false;
	enabled2 = false;
	playerNear = false;
}

MovingPlatormVertical.Init = function(self)
end

MovingPlatormVertical.Begin = function(self, owner, goMgr)
	self.rgdbdyComp = owner:GetRigidbodyComp();
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});
end

MovingPlatormVertical.Update = function(self, dt, owner) 
end

MovingPlatormVertical.OnEnter = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if (triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end
	if (gameObj2:GetTag() == "PlayerCube" and self.enabled1 and self.enabled2) then
			self.rgdbdyComp:SetVelocity(self.moveSpeed);
	end
end

MovingPlatormVertical.OnExit = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	-- Add Code here to remove the UI For interaction

	if (triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	if (gameObj2:GetTag() == "PlayerCube") then
		self.rgdbdyComp:SetVelocity(Vector3.new(0.0,0.0,0.0));
	end
end

MovingPlatormVertical.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter});
	self.triggerComp.OnExit:Unbind({self, self.OnExit});
end

return MovingPlatormVertical;