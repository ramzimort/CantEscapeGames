
-- First approximation of a component script

shooter_fpscontroller = 
{
	-- Rotation (Mouse/Joystick)
	MousePositionX = 0;
	MousePositionY = 0;
	DeltaPositionX = 0;
	DeltaPositionY = 0;
	Rotation = Vector3.new(0.0);
	RotationMultiplier = 10.0;
	RotationSpeed = 20.0;
	LEFTCLICK = false;
	RIGHTCLICK = false;
	RotationEnabled = true;
	
	triggerComp = nil;

	movement_amount = Vector2.new(0.0);
	MoveSpeed = 10.0;
	Transform = nil;
	Camera = nil;
	
	life = 10;
}

--Init called when comp is created
shooter_fpscontroller.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});

end

--Begin called when obj has all comps
shooter_fpscontroller.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end

	self.Transform = owner:GetTransformComp();
	self.Camera = owner:GetCameraComp():GetCamera();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});
end

--Update called every tick
shooter_fpscontroller.Update = function(self, dt, owner) 
	if (self.life < 1.0) then
		OutputPrint("GAME OVER!!!");
	end

	local position = self.Transform:GetPosition();
	local movement = self.movement_amount * self.MoveSpeed * dt;
	local strafe = self.Camera:GetRight() * movement.x;
	local forward = self.Camera:GetForward() * movement.y;
	position = position + strafe + forward;

	local rotation = self.Rotation;
	if (self.LEFTCLICK) then
		rotation.x = -1.0*self.DeltaPositionY;
		rotation.y = -1.0*self.DeltaPositionX;
		self.DeltaPositionX = 0.0;
		self.DeltaPositionY = 0.0;
		rotation = rotation * dt * self.RotationSpeed;
		self.Transform:Rotate(rotation.x, rotation.y, 0.0);
		self.Rotation.x = 0.0;
		self.Rotation.y = 0.0;
		self.Rotation.z = 0.0;
	end
	self.Transform:SetLocalPosition(position.x, position.y, position.z);
end

shooter_fpscontroller.OnEnter = function(self, gameObj1, gameObj2)
	
	local transform1 = gameObj1:GetTransformComp();
	local transform2 = gameObj2:GetTransformComp();
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if(transform1 == nil or transform2 == nil or triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	local collisionMask1 = triggerComp1:GetCollisionMask();
	local collisionMask2 = triggerComp2:GetCollisionMask();
	if (collisionMask2 == CollisionMask.ENEMY_PROJ) then
		self.life = self.life - 1;
	end

	--PLAY SOUND
	--SPAWN PARTICLES
end

shooter_fpscontroller.OnExit = function(self, gameObj1, gameObj2)
end

--Method
shooter_fpscontroller.OnKey = function(self, key, state)
	local delta = 0.0;
	if(state) then
		delta = 1.0;
	end
	
	if(SCANCODE.W == key) then
		self.movement_amount.y = delta;
	elseif(SCANCODE.S == key) then
		self.movement_amount.y = -1.0*delta;
	elseif(SCANCODE.A == key) then
		self.movement_amount.x = -1.0*delta;
	elseif(SCANCODE.D == key) then
		self.movement_amount.x = delta;
	end
end

shooter_fpscontroller.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
	self.DeltaPositionX = deltaposition.x;
	self.DeltaPositionY = deltaposition.y;
end

shooter_fpscontroller.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end


shooter_fpscontroller.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnKey});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
	
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter});
	self.triggerComp.OnExit:Unbind({self, self.OnExit});
end

return shooter_fpscontroller;