-- First approximation of a component script

rgdbdyPlayer = 
{
	name = "rgdbdyPlayer";
	
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;

	move_speed = Vector3.new(10,0,10); --m/s
	curr_velocity = 0; -- m/s
	max_velocity = 2; -- m/s
	
	rightPressed = false;
	leftPressed = false;
	upPressed = false;
	downPressed = false;
	space = false;

	isGrounded = false;
}


--Method
rgdbdyPlayer.OnKeyPressed = function(self, key, state)
	if(SCANCODE.UP == key) then
		self.upPressed = state;
	end
	if(SCANCODE.DOWN == key) then
		self.downPressed = state;
	end
	if(SCANCODE.LEFT == key) then
		self.leftPressed = state;
	end	
	if(SCANCODE.RIGHT == key) then
		self.rightPressed = state;
	end
	if (SCANCODE.SPACE == key) then 
		self.space = state;
	end

end

--Init called when comp is created
rgdbdyPlayer.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKeyPressed});
end


--Begin called when obj has all comps
rgdbdyPlayer.Begin = function(self, owner, goMgr)
	self.rgdbdyComp = owner:GetRigidbodyComp();
	self.transformComp = owner:GetTransformComp();
end


--Update called every tick
rgdbdyPlayer.Update = function(self, dt, owner) 

	if (self.rgdbdyComp == nil) then return end
	if (self.transformComp == nil) then return end

	local velocity = self.rgdbdyComp:GetVelocity();
	--self.curr_velocity = velocity:len2();
	
	--if(self.curr_velocity < self.max_velocity)
	if (self.upPressed) then 
		velocity.z = velocity.z - (self.move_speed.z * dt);
	end 
	if (self.downPressed) then 
		velocity.z = velocity.z + (self.move_speed.z * dt);
	end 
	if (self.leftPressed) then 
		velocity.x = velocity.x - (self.move_speed.x * dt);
	end 
	if (self.rightPressed) then 
		velocity.x = velocity.x + (self.move_speed.x * dt);
	end 
	--end

	--if (abs(velocity.y) < 0.001) then
	--	self.isGrounded = true;
	--end

	--if (self.space) then
	--	if (self.isGrounded) then
			--velocity.y = velocity.y + 4.0;
	--		isGrounded = false;
	--	end
	--end

	self.rgdbdyComp:SetVelocity(velocity);


end

rgdbdyPlayer.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnKeyPressed});
end


return rgdbdyPlayer;
