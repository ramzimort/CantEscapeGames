InteractiveBox2 = 
{
	name = "InteractiveBox2";
	
	--Custom stuff
	transformComp = nil;
	triggerComp = nil;

	playerNear = false;
	interactionComplete = false;
	moveSpeed = 0.5;
	yOffset = 0.0;

	owner = nil;
	fire = nil;
	Elevator = nil;
	rightLight = nil;
	newLightColor = Vector3.new(0.0, 1.0, 0.0);

}

InteractiveBox2.OnKeyPressed = function(self, key, state)
	if(SCANCODE.X == key and state and self.playerNear) then
		self.interactionComplete = true;
		self.rightLight:GetLightComp():SetColor(self.newLightColor);
		self.rightLight:GetHaloEffectComp():SetColor(self.newLightColor);
		local elevatorScript = self.Elevator:GetCustomComp("MovingPlatformVertical"); 
		elevatorScript.enabled1 = true;
	end
end

InteractiveBox2.Init = function(self)
	OnJoystickButton():Bind({self, self.OnJoystickButton});
	OnKeyEvent():Bind({self, self.OnKeyPressed});
end

InteractiveBox2.OnJoystickButton = function(self, ID, key, state)
	if(CONTROLLER.X == key and state and self.playerNear) then
		self.interactionComplete = true;
		self.rightLight:GetLightComp():SetColor(self.newLightColor);
		self.rightLight:GetHaloEffectComp():SetColor(self.newLightColor);
		local elevatorScript = self.Elevator:GetCustomComp("MovingPlatformVertical"); 
		elevatorScript.enabled1 = true;
	end
end

InteractiveBox2.Begin = function(self, owner, goMgr)
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});

	self.owner = owner;
	self.fire = goMgr:FindGameObject("Firewall_Left");
	self.rightLight = goMgr:FindGameObject("Lantern_Right");
	self.Elevator = goMgr:FindGameObject("Elevator");
end

InteractiveBox2.Update = function(self, dt, owner) 
	if(self.interactionComplete and self.yOffset < 2) then
		local pos = self.transformComp:GetPosition();
		pos.y = pos.y - self.moveSpeed * dt;
		self.yOffset = self.yOffset + (self.moveSpeed*dt);
		self.transformComp:SetLocalPosition(pos);
	end
	if(self.yOffset >= 2) then
		self.owner:Destroy();
		self.fire:Destroy();
	end
end

InteractiveBox2.OnEnter = function(self, gameObj1, gameObj2)
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

InteractiveBox2.OnExit = function(self, gameObj1, gameObj2)
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

InteractiveBox2.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
	OnKeyEvent():Unbind({self, self.OnKeyPressed});
end

return InteractiveBox2;