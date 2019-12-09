-- First approximation of a component script

RunnerGround = 
{
	name = "RunnerGround";
	playerGO = nil;
	triggerComp = nil;
	frame = 0;
}


--Init called when comp is created
RunnerGround.Init = function(self)
end


--Begin called when obj has all comps
RunnerGround.Begin = function(self, owner, goMgr)
	self.triggerComp = owner:GetTriggerComp();
end


--Update called every tick
RunnerGround.Update = function(self, dt, owner)

	if (self.playerGO == nil) then 
		OutputPrint("PLAYER GO IS NIL!!. Frame: " .. self.frame .. "\n");
		self.frame = self.frame + 1;
		return;
	end
	
	if (self.frame == 5) then 
		local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
		self.triggerComp.OnEnter:Bind({playerComp, playerComp.ApplyKnockback});
	end

	self.frame = self.frame + 1;
end


--Called when the component is gonna be destroyed
RunnerGround.OnDestruction = function(self)
end



return RunnerGround;