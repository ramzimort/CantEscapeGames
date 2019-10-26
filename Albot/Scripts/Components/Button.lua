
-- First approximation of a component script

Button = 
{
	-- States
	NextStatePath = "Assets\\Levels\\level1.json";
	
	-- Component
	uiComp;
	uiTransform;
	uiAffineAnimation;
	
	-- Data
	position;
	initialPosition;
	finalPosition;
	scale;
	loacation;
	maxPosX;
	minPosX;
	maxPosY;
	minPosY;
	deltaTime;
	velocity;
	initialAnimationEnable = false;
	finalAnimationEnable = false;
	enableButton = false;
	
	
	-- MOUSE
	MousePositionX = 0;
	MousePositionY = 0;
	LEFTCLICK = false;
	RIGHTCLICK = false;
	ENTER = false;
	-- KEYBOARD
	Up = false;
	Down = false;	
	
	--UI Camera
	cameraObj;
	cameraComp;
	camera;
	projectionMatrix;
	viewProjectionMatrix;
	viewMatrix;
	screenWidth;
	screenHeight;
	
}

--Init called when comp is created
Button.Init = function(self)

end

--Begin called when obj has all comps
Button.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	
	OnKeyEvent():Bind({self, self.OnKey});

	OnMouseMotion():Bind({self, self.OnMouseMotion});
	
	OnMouseClick():Bind({self, self.OnMouseClick});
	
	self.cameraObj = goMgr:FindGameObject("UIObject1");
	if (self.cameraObj == nil) then
		OutputPrint(">>> GO with tag UIObject1 not found\n");
		return;
	end
	
	self.cameraComp = self.cameraObj:GetCameraComp();
	if (self.cameraComp == nil) then
		OutputPrint(">>> Camera Component not found\n");
		return;
	end

	self.camera = self.cameraComp:GetCamera();
	if (self.camera == nil) then
		OutputPrint(">>> Camera  not found\n");
		return;
	end
	
	
	self.uiComp = owner:GetUiComp();
	if (self.uiComp == nil) then 
		OutputPrint("ERROR, UIComponent IS NIL\n");
	end
	
	self.uiTransform = owner:GetTransformComp();
	if (self.uiTransform == nil) then 
		OutputPrint("ERROR, UITransform IS NIL\n");
	end
	
	self.uiAffineAnimation = owner:GetAffineAnimationComp();
	if (self.uiAffineAnimation == nil) then 
		OutputPrint("ERROR, Ui Affine Animation IS NIL\n");
	end
	
	self.initialPosition = self.uiAffineAnimation:GetInitialPosition();
	self.position = self.initialPosition;
	self.finalPosition = self.uiAffineAnimation:GetFinalPosition();
	self.deltaTime = self.uiAffineAnimation:GetDeltaTime();
	self.velocity = self.uiAffineAnimation:GetVelocity();
	
	self.loacation = self.uiComp:GetLocation();

	self.initialAnimationEnable = true;
	
end

--Update called every tick
Button.Update = function(self, dt, owner) 

	if(self.ENTER == true) then
		self.finalAnimationEnable = true;
		self.enableButton = false;
		
	end
	
	
	if(self.initialAnimationEnable == true) then
		if(self.position == self.finalPosition) then
			self.initialAnimationEnable = false;
			self.enableButton = true;
			
			return;
		end
		self.position = self.position + self.velocity * self.deltaTime;
		self.uiTransform:SetLocalPosition(self.position.x, self.position.y, self.position.z);
	end
	
	if(self.finalAnimationEnable == true) then
	
		if(self.position == self.initialPosition) then
			OutputPrint(">>> Reached\n");
			World.Get():PushStateEvent(false, self.NextStatePath);
			return;
		end
		self.position = self.position - self.velocity * self.deltaTime;
		self.uiTransform:SetLocalPosition(self.position.x, self.position.y, self.position.z);
	end
end
--Method
Button.OnKey = function(self, key, state)
	if(SCANCODE.ENTER == key) then
		self.ENTER = state;
		
	end
end
Button.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
end
Button.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end
return Button;