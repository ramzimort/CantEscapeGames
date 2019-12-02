MovingPlatformHorizontal = 
{
	name = "MovingPlatformHorizontal";
	
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;

	xMin = 0.0;
	xMax = 0.0;
	moveSpeed = 3.0;
}

MovingPlatformHorizontal.Init = function(self)
end

MovingPlatformHorizontal.Begin = function(self, owner, goMgr)
	--self.rgdbdyComp = owner:GetRigidbodyComp();
	--self.transformComp = owner:GetTransformComp();
	--
	--local position = self.transformComp:GetPosition();
	--xMin = position.x - 1.5;
	--xMax = position.x + 1.5;
	--local velocity = Vector3.new(self.moveSpeed, 0.0, 0.0);
	--self.rgdbdyComp:SetVelocity(velocity);
end

MovingPlatformHorizontal.Update = function(self, dt, owner) 
	--local velocity = self.rgdbdyComp:GetVelocity();
	--local position = self.transformComp:GetPosition();
	--if(position.x > xMax) then
	--	velocity.x = -1*self.moveSpeed;
	--elseif(position.x < xMin) then
	--	velocity.x = self.moveSpeed;
	--end
	--self.rgdbdyComp:SetVelocity(velocity);
end

MovingPlatformHorizontal.OnDestruction = function(self)
end

return MovingPlatformHorizontal;