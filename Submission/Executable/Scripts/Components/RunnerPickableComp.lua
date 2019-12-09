-- First approximation of a component script

RunnerPickableComp = 
{
	name = "RunnerPickableComp";
	playerGO = nil;
	triggerComp = nil;
	frame = 0;
	origScale = Vector3.new(0.0);
}


--Init called when comp is created
RunnerPickableComp.Init = function(self)
end


--Begin called when obj has all comps
RunnerPickableComp.Begin = function(self, owner, goMgr)
	self.triggerComp = owner:GetTriggerComp();
	
	local transform = owner:GetTransformComp();
	self.origScale = transform:GetScale();
end


--Update called every tick
RunnerPickableComp.Update = function(self, dt, owner)

	if (self.playerGO == nil) then 
		self.frame = self.frame + 1;
		return;
	end
	
	if (self.frame == 5) then 
		local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
		self.triggerComp.OnEnter:Bind({playerComp, playerComp.OnGasTank});------------------------------------
	end

	--Affine animation so player notices tank
	local transform = owner:GetTransformComp();
	--local offset = Sin(dt * 100.0);
	--transform:Scale(self.origScale.x + offset, self.origScale.y + offset, self.origScale.z + offset);
	transform:Rotate(0, dt * 20, 0);

	self.frame = self.frame + 1;
end


--Called when the component is gonna be destroyed
RunnerPickableComp.OnDestruction = function(self)
end



return RunnerPickableComp;