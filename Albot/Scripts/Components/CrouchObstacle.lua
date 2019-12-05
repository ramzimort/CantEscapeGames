CrouchObstacle = 
{
	name = "CrouchObstacle";
	
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;
	triggerComp = nil;
	PlayerScript = nil;
	-- Game Object Manager
	GOManager = nil;
	
	-- UIObject LUA Component
	UIObjectLUA = nil;
	
}

CrouchObstacle.Init = function(self)
end

CrouchObstacle.Begin = function(self, owner, goMgr)
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});
	self.Player = goMgr:FindGameObject("Player"):GetCustomComp("RamziPlayer");
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

CrouchObstacle.Update = function(self, dt, owner) 
end

CrouchObstacle.OnEnter = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if (triggerComp1 == nil or triggerComp2 == nil) then
		--self.UIObjectLUA:DisableImage(1);
		return;
	end

	if (gameObj2:GetTag() == "PlayerCube") then
		self.Player.nearCrouch = true;
		self.UIObjectLUA:EnableImage(1);
	end
end

CrouchObstacle.OnExit = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if (triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	if (gameObj2:GetTag() == "PlayerCube") then
		self.Player.nearCrouch = false;
		self.UIObjectLUA:DisableImage(1);
	end
end

CrouchObstacle.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
end

return CrouchObstacle;