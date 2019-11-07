-- First approximation of a component script

PresentationParticleEmitOnce = 
{
	name = "PresentationParticleEmitOnce";
	
	--Custom stuff
	timeElapsed = 0.0;
	emitTimePeriod = 5.0;

	firstSet = false;
	firstSetAnglePitch = 360;
	secondSetAnglePitch = 45;
	firstSetAngleYaw = 270;
	secondSetAngleYaw = 45;
}


--Method
PresentationParticleEmitOnce.OnKey = function(self, key, state)
end


--Init called when comp is created
PresentationParticleEmitOnce.Init = function(self)
end


--Begin called when obj has all comps
PresentationParticleEmitOnce.Begin = function(self, owner)

	self.animComp = owner:GetAnimationComp();

end


--Update called every tick
PresentationParticleEmitOnce.Update = function(self, dt, owner) 

	self.timeElapsed = self.timeElapsed + dt;

	if (self.timeElapsed >= self.emitTimePeriod) then
		self.timeElapsed = self.timeElapsed - self.emitTimePeriod;
		local particleEmitterComp = owner:GetParticleEmitterComp();
		if(self.firstSet) then
			particleEmitterComp:SetEmitterSpreadAngleYaw(self.firstSetAngleYaw);
			particleEmitterComp:SetEmitterSpreadAngleYaw(self.firstSetAnglePitch);
			self.firstSet  = false;
		else
			particleEmitterComp:SetEmitterSpreadAngleYaw(self.secondSetAngleYaw);
			particleEmitterComp:SetEmitterSpreadAngleYaw(self.secondSetAnglePitch);
			self.firstSet  = true;
		end
		
	end
	--]]

end

PresentationParticleEmitOnce.OnDestruction = function(self)
end


return PresentationParticleEmitOnce;
