CrouchObstacle = 
{
	name = "CrouchObstacle";
	
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;
	triggerComp = nil;
}

CrouchObstacle.Init = function(self)
end

CrouchObstacle.Begin = function(self, owner, goMgr)
	self.rgdbdyComp = owner:GetRigidbodyComp();
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});

end

CrouchObstacle.Update = function(self, dt, owner) 
end

CrouchObstacle.OnEnter = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if (triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	if (gameObj2:GetTag() == "Player") then
			gameObj2:GetCustomComp("RamziPlayer").nearCrouch = true;
	end
end

CrouchObstacle.OnExit = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if (triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	if (gameObj2:GetTag() == "Player") then
			gameObj2:GetCustomComp("RamziPlayer").nearCrouch = false;
	end
end

CrouchObstacle.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
end

return CrouchObstacle;