MovingPlatormVertical = 
{
	name = "MovingPlatormVertical";
	
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;

	min = 0.0;
	max = 0.0;
	moveSpeed = 3.0;
}

MovingPlatormVertical.Init = function(self)
end

MovingPlatormVertical.Begin = function(self, owner, goMgr)
	self.rgdbdyComp = owner:GetRigidbodyComp();
	self.transformComp = owner:GetTransformComp();

	local position = self.transformComp:GetPosition();
	min = position.y - 0;
	max = position.y + 20.0;
	local velocity = Vector3.new(0.0, self.moveSpeed, 0.0);
	self.rgdbdyComp:SetVelocity(velocity);
end

MovingPlatormVertical.Update = function(self, dt, owner) 
	local velocity = self.rgdbdyComp:GetVelocity();
	local position = self.transformComp:GetPosition();
	if(position.y > max) then
		velocity.y = -1*self.moveSpeed;
	elseif(position.y < min) then
		velocity.y = self.moveSpeed;
	end
	self.rgdbdyComp:SetVelocity(velocity);
end

MovingPlatormVertical.OnDestruction = function(self)
end

return MovingPlatormVertical;