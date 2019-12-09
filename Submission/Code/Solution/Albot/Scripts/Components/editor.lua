
-- First approximation of a component script

editor = 
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
	RotationSpeed = -10.0;

	Transform = nil;
	Camera = nil;
}

--Init called when comp is created
editor.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});
	OnMouseScroll():Bind({self, self.OnMouseScroll});
end

--Begin called when obj has all comps
editor.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	self.Transform = owner:GetTransformComp();
	self.Transform:SetLocalPosition(0.0, 10.0, 0.0);
	self.Transform:Rotate(-30, 0, 0);
	self.Camera = owner:GetCameraComp():GetCamera();
end

--Update called every tick
editor.Update = function(self, dt, owner) 
	local movement_amount = Vector2.new(0.0);
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

	--TRACE("a" .. position.x .. "\n");
	
	-- Left Click Case
	if (self.LEFTCLICK) then
		local rotationX = self.DeltaPositionX * dt * self.RotationSpeed;
		local rotationY = self.DeltaPositionY * dt * self.RotationSpeed;
		self.Transform:Rotate(rotationX, 0.0, rotationY);

		--local rotation = Vector2.new(rotationX, rotationY);
		--local right = self.Camera:GetRight();
		--local matrix = GetRotationMatrix(right, rotation);
		--self.Camera:ApplyRotation(matrix);
	end
	--OutputPrint("Pos:" .. self.MousePositionX .. " " .. self.MousePositionY .. "\n");
	--OutputPrint("DeltaPos:" .. self.DeltaPositionX .. " " .. self.DeltaPositionY .. "\n");
	
	self.DeltaPositionX = 0.0;
	self.DeltaPositionY = 0.0;

	self.Transform:SetLocalPosition(position.x, position.y, position.z);
end

--Method
editor.OnKey = function(self, key, state)
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

editor.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
	self.DeltaPositionX = deltaposition.x;
	self.DeltaPositionY = deltaposition.y;
end

editor.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end

editor.OnMouseScroll = function(self, xScroll, yScroll)
	LOG("Scroll: " .. yScroll .. "\n");
end

editor.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnKey});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
	OnMouseScroll():Unbind({self, self.OnMouseScroll});
end

return editor;