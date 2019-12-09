-- First approximation of a component script

rgdbdyPlayer = 
{
	name = "rgdbdyPlayer";
	
	--Custom stuff
	rgdbdyComp = nil;
	transformComp = nil;
	MovementEnabled = false;

	Acceleration = Vector3.new(0.0); --m/s
	AccelerationMultiplier = 10;
	Jump = false;
	isGrounded = false;
}


--Method
rgdbdyPlayer.OnKeyPressed = function(self, key, state)
	local delta = 0.0;
	if(state) then
		delta = 1.0;
	end
	if(SCANCODE.UP == key) then
		self.Acceleration.z = -1.0*delta;
	elseif(SCANCODE.DOWN == key) then
		self.Acceleration.z = delta;
	elseif(SCANCODE.LEFT == key) then
		self.Acceleration.x = -1.0*delta;
	elseif(SCANCODE.RIGHT == key) then
		self.Acceleration.x = delta;
	end

end

rgdbdyPlayer.OnJoystickButton = function(self, joystickId, button, state)
	if(button == CONTROLLER.LB and state) then
		self.MovementEnabled = not self.MovementEnabled;
	end

	if(button == CONTROLLER.A) then
		self.Jump = state;
	end
end

rgdbdyPlayer.OnJoystickMotion = function(self, joystickId, axis, value)
	if(value < 0.2 and value > -0.2) then
		value = 0.0;
	end
	if(self.MovementEnabled) then
	if(axis == 3) then
		self.Acceleration.x = 1*value;
	end
	if(axis == 4) then
		self.Acceleration.z = 1*value;
	end
	end
end

--Init called when comp is created
rgdbdyPlayer.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKeyPressed});
	OnJoystickButton():Bind({self, self.OnJoystickButton});
	OnJoystickMotion():Bind({self, self.OnJoystickMotion});
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
	if(self.Jump) then
		velocity.y = 10.0;
	end

	velocity = velocity + (self.Acceleration * self.AccelerationMultiplier * dt);

	self.rgdbdyComp:SetVelocity(velocity);
end

rgdbdyPlayer.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnKeyPressed});
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
	OnJoystickMotion():Unbind({self, self.OnJoystickMotion});
end


return rgdbdyPlayer;
