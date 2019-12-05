InteractiveBox1 = 
{
	name = "InteractiveBox1";
	
	--Custom stuff
	transformComp = nil;
	triggerComp = nil;

	playerNear = false;
	interactionComplete = false;
	moveSpeed = 0.5;
	yOffset = 0.0;

	owner = nil;
	fire = nil;

}	-- UIObject LUA Component
	UIObjectLUA = nil;
	-- Game Object Manager
	GOManager = nil;

InteractiveBox1.Init = function(self)
	OnJoystickButton():Bind({self, self.OnJoystickButton});
end

InteractiveBox1.OnJoystickButton = function(self, ID, key, state)
	if(CONTROLLER.X == key and state and self.playerNear) then
		self.interactionComplete = true;
		self.UIObjectLUA:DisableImage(1);
	end
end

InteractiveBox1.Begin = function(self, owner, goMgr)
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});

	self.owner = owner;
	self.fire = goMgr:FindGameObject("Firewall_Right");
	self.GOManager = goMgr;

	-- Get UICamera LUA Script
	local Tag = "UIObject";
	local GO = goMgr:FindGameObject(Tag);
	if (GO == nil) then
		--OutputPrint(">>> GO  not found\n");
		return;
	end
	self.UIObjectLUA = GO:GetCustomComp("UICameraProtoType1");
end

InteractiveBox1.Update = function(self, dt, owner) 
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

InteractiveBox1.OnEnter = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	-- Add Code here to show the UI For interaction

	if (triggerComp1 == nil or triggerComp2 == nil or self.interactionComplete) then
		return;
	end

	if (gameObj2:GetTag() == "PlayerCube") then
			self.playerNear = true;
		self.UIObjectLUA:EnableImage(1);
	end
end

InteractiveBox1.OnExit = function(self, gameObj1, gameObj2)
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

InteractiveBox1.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
end

return InteractiveBox1;