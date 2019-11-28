-- First approximation of a component script

TriggerTest = 
{
	name = "TriggerTest";
	
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;
	triggerComp = nil;
}


--Method
--Init called when comp is created
TriggerTest.Init = function(self)
end


--Begin called when obj has all comps
TriggerTest.Begin = function(self, owner, goMgr)
	self.rgdbdyComp = owner:GetRigidbodyComp();
	self.transformComp = owner:GetTransformComp();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});

end


--Update called every tick
TriggerTest.Update = function(self, dt, owner) 

end

TriggerTest.OnDestruction = function(self)
	--self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	--self.triggerComp.OnExit:Unbind({self, self.OnExit})
end


TriggerTest.OnEnter = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if (triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	local collisionMask1 = triggerComp1:GetCollisionMask();
	local collisionMask2 = triggerComp2:GetCollisionMask();

	if (collisionMask2 == CollisionMask.PLAYER) then
		local rigidbody1 = gameObj1:GetRigidbodyComp();
		if (rigidbody1 == nil) then
			return;
		end
		local vel = Vector3.new(0,10,0);
		rigidbody1:SetVelocity(vel);
	
	end

	OutputPrint("OnEnter\n");
	OutputPrint(">> " .. collisionMask1 .. "\n");
	OutputPrint(">> " .. collisionMask2 .. "\n");

end

TriggerTest.OnExit = function(self, gameObj1, gameObj2)
	OutputPrint("OnExit");

end

return TriggerTest;
