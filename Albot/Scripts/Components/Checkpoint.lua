Checkpoint = 
{
	name = "Checkpoint";
	
	--Custom stuff
	position = nil;
	triggerComp = nil;
	PlayerScript = nil;
}

Checkpoint.Init = function(self)
end

Checkpoint.Begin = function(self, owner, goMgr)
	self.position = owner:GetTransformComp():GetPosition();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.Player = goMgr:FindGameObject("Player"):GetCustomComp("RamziPlayer");
end

Checkpoint.Update = function(self, dt, owner) 
end

Checkpoint.OnEnter = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if (triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	if (gameObj2:GetTag() == "PlayerCube") then
			self.Player.originalPosition = self.position;
	end
end

Checkpoint.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
end

return Checkpoint;