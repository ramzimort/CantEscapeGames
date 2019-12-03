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
	-- Image Tag Names
	objectTag = "Pic1";
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
end

CrouchObstacle.Update = function(self, dt, owner) 
end

CrouchObstacle.OnEnter = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if (triggerComp1 == nil or triggerComp2 == nil) then
		local uiGO = self.GOManager:FindGameObject(self.objectTag);
		if (uiGO == nil) then
			OutputPrint(">>> GO with tag UiObject not found\n");
			return;
		end
		local uiCompGO = uiGO:GetUiComp();
		uiCompGO:SetRenderEnable(false);
		return;
	end

	if (gameObj2:GetTag() == "PlayerCube") then
		self.Player.nearCrouch = true;
		local uiGO = self.GOManager:FindGameObject(self.objectTag);
		if (uiGO == nil) then
			OutputPrint(">>> GO with tag UiObject not found\n");
			return;
		end
		local uiCompGO = uiGO:GetUiComp();
		uiCompGO:SetRenderEnable(true);
		
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
	end
end

CrouchObstacle.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
end

return CrouchObstacle;