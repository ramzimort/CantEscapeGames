
-- First approximation of a component script

Button = 
{
	-- States
	NextStatePath;
	
	-- Component
	uiComp;
	uiTransform;
	cameraComp;
	
	-- Data
	position;
	initialPosition;
	finalPosition;
	scale;
	touchedScale;
	location;
	maxPosX;
	maxPosY;
	deltaTime;
	positveVelocity;
	negativeVelocity;
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
	camera;
	projectionMatrix;
	viewProjectionMatrix;
	viewMatrix;
	screenWidth;
	screenHeight;
}

--Init called when comp is created
Button.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});
end

--Begin called when obj has all comps
Button.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	
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
	
	

	self.NextStatePath =  self.uiComp:GetStateAddress();
	self.position = self.uiComp:GetFinalPosition();
	
	self.scale = Vector3.new(self.uiTransform:GetScale());
	OutputPrint("\n" .. self.scale.x);
	self.touchedScale = Vector3.new( self.scale.x,self.scale.y,self.scale.z);
	self.touchedScale = self.touchedScale * 1.1;
	_G.PlayFinalAnimation = false;
	
	self.maxPosX = self.scale.x + self.position.x;
	self.maxPosY = self.scale.y + self.position.y;
	
	self.location = self.uiComp:GetLocation();
end

--Update called every tick
Button.Update = function(self, dt, owner) 

	
	if( _G.PlayFinalAnimation == true or self.uiComp:GetFinalAnimationState() == true) then
		self.uiComp:FinalAnimationEnabled();
		self.enableButton = false;	
	end

	
	if(self.location == _G.CurrentButtonTouched) then
		self.uiTransform:Scale(self.touchedScale.x,self.touchedScale.y,self.touchedScale.z);
		if(self.ENTER == true ) then
			_G.State = self.NextStatePath;
			self.uiComp:FinalAnimationEnabled();
			self.enableButton = false;		
			_G.PlayFinalAnimation = true;
		end
	else
		
		self.uiTransform:Scale(self.scale.x,self.scale.y,self.scale.z);
	end
	
	if(self.MousePositionX < self.maxPosX) then
		if(self.MousePositionX > self.position.x) then
			if(self.MousePositionY < self.maxPosY) then
				if(self.MousePositionY > self.position.y) then
					_G.CurrentButtonTouched = self.location;
					self.uiTransform:Scale(self.touchedScale.x,self.touchedScale.y,self.touchedScale.z);
					if(self.LEFTCLICK == true) then
						self.LEFTCLICK = false;
						OutputPrint("\n" .. self.NextStatePath);
						self.uiComp:FinalAnimationEnabled();
						self.enableButton = false;
						_G.State = self.NextStatePath;
						_G.PlayFinalAnimation = true;
						end
				end
			end
		end
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

Button.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnMouseMotion});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
end

return Button;