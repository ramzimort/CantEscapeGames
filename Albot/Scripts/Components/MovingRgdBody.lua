
-- First approximation of a component script

MovingRgdBody = 
{
	-- MOUSE
	MousePositionX = 0;
	MousePositionY = 0;
	DeltaPositionX = 0;
	DeltaPositionY = 0;
	LEFTCLICK = false;
	RIGHTCLICK = false;
	Test = false;
	-- KEYBOARD
	Forward = false;
	Backward = false;
	Left = false;
	Right = false;

	MoveSpeed = 10.0;
	RotationSpeed = 10.0;

	Transform = nil;
	Camera = nil;
}

--Init called when comp is created
MovingRgdBody.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});
end

--Begin called when obj has all comps
MovingRgdBody.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end

	self.Transform = owner:GetTransformComp();
	self.Camera = owner:GetCameraComp():GetCamera();
end

--Update called every tick
MovingRgdBody.Update = function(self, dt, owner) 

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

	local position = self.Transform:GetPosition();
	local movement = movement_amount * self.MoveSpeed * dt;
	local strafe = self.Camera:GetRight() * movement.x;
	local forward = self.Camera:GetForward() * movement.y;
	position = position + strafe + forward;

	if (self.LEFTCLICK) then

		local rotationX = self.DeltaPositionX * dt * self.RotationSpeed;
		local rotationY = self.DeltaPositionY * dt * self.RotationSpeed;
		
		self.Transform:Rotate(-rotationY, -rotationX, 0.0);
	end

	self.Transform:SetLocalPosition(position.x, position.y, position.z);
	--TRACE("a" .. position.x .. " " .. position.y .. " " .. position.z .. "\n");
	
	self.DeltaPositionX = 0.0;
	self.DeltaPositionY = 0.0;
end

--Method
MovingRgdBody.OnKey = function(self, key, state)
	if(SCANCODE.W == key) then
		self.Forward = state;
	elseif(SCANCODE.S == key) then
		self.Backward = state;
	elseif(SCANCODE.A == key) then
		self.Left = state;
	elseif(SCANCODE.D == key) then
		self.Right = state;
	elseif(SCANCODE.L == key) then
		self.Test = state;
		
		--LOG("World: " .. World .. "\n");
		--test:PlaySong(false, "Assets\\Songs\\Techno_1.mp3");
	end
end

MovingRgdBody.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
	self.DeltaPositionX = deltaposition.x;
	self.DeltaPositionY = deltaposition.y;
end

MovingRgdBody.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end

MovingRgdBody.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnKey});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
end

return MovingRgdBody;