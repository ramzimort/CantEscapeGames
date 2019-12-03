InteractiveBox3 = 
{
	name = "InteractiveBox3";
	
	--Custom stuff
	transformComp = nil;
	triggerComp = nil;

	playerNear = false;
	interactionComplete = false;
	moveSpeed = 0.5;
	yOffset = 0.0;

	owner = nil;
	Elevator = nil;
	Fireball1 = nil;
	Fireball2 = nil;
	leftLight = nil;
	newLightColor = Vector3.new(0.0, 1.0, 0.0);
}

InteractiveBox3.Init = function(self)
	OnJoystickButton():Bind({self, self.OnJoystickButton});
end

InteractiveBox3.OnJoystickButton = function(self, ID, key, state)
	if(CONTROLLER.X == key and state and self.playerNear) then
		self.interactionComplete = true;
		self.Fireball1:GetTransformComp():SetLocalPosition(-34.0,-3.0,-70.0);
		self.Fireball2:GetTransformComp():SetLocalPosition(-31.0,-3.0,-70.0);
		self.leftLight:GetLightComp():SetColor(self.newLightColor);
		self.leftLight:GetHaloEffectComp():SetColor(self.newLightColor);
		local elevatorScript = self.Elevator:GetCustomComp("MovingPlatformVertical"); 
		elevatorScript.enabled2 = true;
	end
end

InteractiveBox3.Begin = function(self, owner, goMgr)
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});

	self.owner = owner;
	self.Fireball1 = goMgr:FindGameObject("Fireball1");
	self.Fireball2 = goMgr:FindGameObject("Fireball2");
	self.leftLight = goMgr:FindGameObject("Lantern_Left");
	self.Elevator = goMgr:FindGameObject("Elevator");
end

InteractiveBox3.Update = function(self, dt, owner) 
	if(self.interactionComplete and self.yOffset < 2) then
		local delta = self.moveSpeed * dt;
		local pos = self.transformComp:GetPosition();
		pos.y = pos.y - delta;
		self.yOffset = self.yOffset + delta;
		self.transformComp:SetLocalPosition(pos);
		self.Fireball1:GetTransformComp():Translate(0.0, delta, 0.0);
		self.Fireball2:GetTransformComp():Translate(0.0, delta, 0.0);
	end
	if(self.yOffset >= 2) then
		local fireballScript1 = self.Fireball1:GetCustomComp("Fireball"); 
		fireballScript1.enabled = true;
		local fireballScript2 = self.Fireball2:GetCustomComp("Fireball"); 
		fireballScript2.enabled = true;
		self.owner:Destroy();
	end
end

InteractiveBox3.OnEnter = function(self, gameObj1, gameObj2)
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

InteractiveBox3.OnExit = function(self, gameObj1, gameObj2)
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

InteractiveBox3.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
end

return InteractiveBox3;