
-- First approximation of a component script

fpscontroller = 
{
	-- MOUSE
	MousePositionX = 0;
	MousePositionY = 0;
	DeltaPositionX = 0;
	DeltaPositionY = 0;
	LEFTCLICK = false;
	RIGHTCLICK = false;
	
	-- KEYBOARD
	Forward = false;
	Backward = false;
	Left = false;
	Right = false;

	MoveSpeed = 10.0;
	RotationSpeed = -10.0 * 3.14159 / 180.0;
}

--Init called when comp is created
fpscontroller.Init = function(self)

end

--Begin called when obj has all comps
fpscontroller.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	
	OnKeyEvent():Bind({self, self.OnKey});

	OnMouseMotion():Bind({self, self.OnMouseMotion});
	
	OnMouseClick():Bind({self, self.OnMouseClick});
end

--Update called every tick
fpscontroller.Update = function(self, dt, owner) 

	--local a = EventManager.Get();
	--a:EnqueueWindowSizeEvent(4, 4);
	
	
	local transform_comp = owner:GetTransformComp();
	local camera_comp = owner:GetCameraComp();
	local camera = camera_comp:GetCamera();

	local movement_amount = Vector2.new(0);
	if (self.Forward) then
		movement_amount.y = 1.0;
	end
	if (self.Backward) then
		movement_amount.y = -1.0;
	end
	if (self.Left) then
		movement_amount.x = -1.0;
	end
	if (self.Right) then
		movement_amount.x = 1.0;
	end

	local position = transform_comp:GetPosition();
	local movement = movement_amount * self.MoveSpeed * dt;
	local strafe = camera:GetRight() * movement.x;
	local forward = camera:GetForward() * movement.y;
	position = position + strafe + forward;

	--TRACE("a" .. position.x .. "\n");
	
	-- Left Click Case
	if (self.LEFTCLICK) then
		local rotationX = self.DeltaPositionX * dt * self.RotationSpeed;
		local rotationY = self.DeltaPositionY * dt * self.RotationSpeed;
		local rotation = Vector2.new(rotationX, rotationY);
		local right = camera:GetRight();
		local matrix = GetRotationMatrix(right, rotation);
		camera:ApplyRotation(matrix);
	end

	
	self.DeltaPositionX = 0.0;
	self.DeltaPositionY = 0.0;

	transform_comp:SetLocalPosition(position.x, position.y, position.z);
	camera:SetCameraPosition(position);
end

--Method
fpscontroller.OnKey = function(self, key, state)
	if(SCANCODE.W == key) then
		self.Forward = state;
	elseif(SCANCODE.S == key) then
		self.Backward = state;
	elseif(SCANCODE.A == key) then
		self.Left = state;
	elseif(SCANCODE.D == key) then
		self.Right = state;
	end
end

fpscontroller.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
	self.DeltaPositionX = deltaposition.x;
	self.DeltaPositionY = deltaposition.y;

--	OutputPrint("x" .. self.DeltaPositionX .. "y" .. self.DeltaPositionY .. "\n");

end

fpscontroller.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end

return fpscontroller;