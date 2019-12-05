MovingPlatformUpward = 
{
	name = "MovingPlatformUpward";
	
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;

	min = 0.0;
	max = 0.0;
	moveSpeed = 3.0;
}

MovingPlatformUpward.Init = function(self)
end

MovingPlatformUpward.Begin = function(self, owner, goMgr)
	self.rgdbdyComp = owner:GetRigidbodyComp();
	self.transformComp = owner:GetTransformComp();
	
	local position = self.transformComp:GetPosition();
	self.min = position.z - 1.5;
	self.max = position.z + 1.5;
	local velocity = Vector3.new(0.0, 0.0, self.moveSpeed);
	self.rgdbdyComp:SetVelocity(velocity);
end

MovingPlatformUpward.Update = function(self, dt, owner) 
	local velocity = self.rgdbdyComp:GetVelocity();
	local position = self.transformComp:GetPosition();
	if(position.z > self.max) then
		velocity.z = -1*self.moveSpeed;
	elseif(position.z < self.min) then
		velocity.z = self.moveSpeed;
	end
	self.rgdbdyComp:SetVelocity(velocity);
end

MovingPlatformUpward.OnDestruction = function(self)
end

return MovingPlatformUpward;