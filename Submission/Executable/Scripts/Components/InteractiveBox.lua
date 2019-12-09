InteractiveBox = 
{
	name = "InteractiveBox";
	
	--Custom stuff
	transformComp = nil;
	triggerComp = nil;

	playerNear = false;
	interactionComplete = false;
	moveSpeed = 0.5;
	yOffset = 0.0;
}

InteractiveBox.Init = function(self)
	OnJoystickButton():Bind({self, self.OnJoystickButton});
end

InteractiveBox.OnJoystickButton = function(self, ID, key, state)
	if(CONTROLLER.X == key and state and self.playerNear) then
		self.interactionComplete = true;
	end
end

InteractiveBox.Begin = function(self, owner, goMgr)
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});

end

InteractiveBox.Update = function(self, dt, owner) 
	if(self.interactionComplete and self.yOffset < 2) then
		local pos = self.transformComp:GetPosition();
		pos.y = pos.y - self.moveSpeed * dt;
		self.yOffset = self.yOffset + (self.moveSpeed*dt);
		self.transformComp:SetLocalPosition(pos);
	end
end

InteractiveBox.OnEnter = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	-- Add Code here to show the UI For interaction

	if (triggerComp1 == nil or triggerComp2 == nil or self.interactionComplete) then
		return;
	end

	if (gameObj2:GetTag() == "PlayerCube") then
			self.playerNear = true;
	end
end

InteractiveBox.OnExit = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	-- Add Code here to remove the UI For interaction

	if (triggerComp1 == nil or triggerComp2 == nil or self.interactionComplete) then
		return;
	end

	if (gameObj2:GetTag() == "PlayerCube") then
			self.playerNear = false;
	end
end

InteractiveBox.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
end

return InteractiveBox;