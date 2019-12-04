-- First approximation of a component script

RunnerObstacle1_Comp = 
{
	name = "RunnerObstacle1_Comp";

	angularSpeed = 0.0;

	playerGO = nil;
	transformComp = nil;
	rigidbodyComp = nil;

	frame = 0;
}


--Init called when comp is created
RunnerObstacle1_Comp.Init = function(self)
end


--Begin called when obj has all comps
RunnerObstacle1_Comp.Begin = function(self, owner, goMgr)
	
	self.transformComp = owner:GetTransformComp();
	self.rigidbodyComp = owner:GetRigidbodyComp();

	--Start at random angle
	local angle = Range(0, 360);
	self.transformComp:Rotate(0, 0, angle);
	self.angularSpeed = Range(10, 25);

end


--Update called every tick
RunnerObstacle1_Comp.Update = function(self, dt, owner)

	if (self.frame == 2) then 
		local player = owner:Manager():FindGameObject("Player");
		if (player ~= nil) then
			self.playerGO = player;
			local playerComp = player:GetCustomComp("TestPlayerAnimComp");
			self.rigidbodyComp.OnCollision:Bind({playerComp, playerComp.ApplyKnockback_2});
		end
	end

	--Rotaion code
	local mgt = self.angularSpeed * dt;
	self.transformComp:Rotate(0, 0, mgt);

	self.frame = self.frame + 1;
end


--Called when the component is gonna be destroyed
RunnerObstacle1_Comp.OnDestruction = function(self)
end



return RunnerObstacle1_Comp;