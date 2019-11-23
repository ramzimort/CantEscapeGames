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
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
	self.triggerComp.OnExit:Unbind({self, self.OnExit})
end


TriggerTest.OnEnter = function(self, gameObj1, gameObj2)
	OutputPrint("OnEnter");

end

TriggerTest.OnExit = function(self, gameObj1, gameObj2)
	OutputPrint("OnExit");

end

return TriggerTest;
