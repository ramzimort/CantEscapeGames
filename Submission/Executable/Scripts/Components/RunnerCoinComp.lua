-- First approximation of a component script

RunnerCoinComp = 
{
	name = "RunnerCoinComp";
	playerGO = nil;
	triggerComp = nil;
	frame = 0;
	origScale = Vector3.new(0.0);
}


--Init called when comp is created
RunnerCoinComp.Init = function(self)
end


--Begin called when obj has all comps
RunnerCoinComp.Begin = function(self, owner, goMgr)
	self.triggerComp = owner:GetTriggerComp();
	
	local transform = owner:GetTransformComp();
	self.origScale = transform:GetScale();
end


--Update called every tick
RunnerCoinComp.Update = function(self, dt, owner)

	if (self.playerGO == nil) then 
		self.frame = self.frame + 1;
		return;
	end
	
	if (self.frame == 5) then 
		local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
		self.triggerComp.OnEnter:Bind({playerComp, playerComp.OnPickupCoin});------------------------------------
	end

	--Affine animation so player notices tank
	local transform = owner:GetTransformComp();
	transform:Rotate(0, dt * 30, 0);

	self.frame = self.frame + 1;
end


--Called when the component is gonna be destroyed
RunnerCoinComp.OnDestruction = function(self)
end



return RunnerCoinComp;