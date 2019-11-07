
-- First approximation of a component script


UICameraSetting = 
{
	Enabled = true;
	-- World (State Change)
	world;
	-- Component
	uiComp;
	
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

	ButtonStartIndex;
	ButtonEndIndex;
	-- Buttons
	TotalButtons = 0;

	
	-- Objects
	TotalObjects = 0;
	-- States
	loadingState;
	LoadStateEnabled = false;
	
	-- Camera
	BaseWidth = 1280;
	BaseHeight = 720;
	Width = 1600;
	Height = 900;
	
	WindowResizeEnabled = false;
	
	-- GameObjectManager
	GOManager;
	GameObject;
	
	-- Affine Animation Data
	PlayFinalAnimation = false;
	
	-- Volume Slider
	SlidderValue = 0.0;
	

}


--Init called when comp is created
UICameraSetting.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});
	OnWindowSize():Bind({self, self.OnWindowSize});
	OnJoystickButton():Bind({self, self.OnJoystickButton});
	OnJoystickMotion():Bind({self, self.OnJoystickMotion});
end

--Begin called when obj has all comps
UICameraSetting.Begin = function(self, owner, goMgr)

	self.GameObject = owner;
	self.GOManager = goMgr;
	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	
	self.uiComp = owner:GetUiComp();
	if (self.uiComp == nil) then 
		OutputPrint("ERROR, UIComponent IS NIL\n");
	end
	
	local Camera = owner:GetCameraComp():GetCamera();
	if (Camera == nil) then 
		OutputPrint("ERROR, Camera is NIll NIL\n");
	end
	local widthHeight = Camera:GetWidthHeight();
	LOG("Width: " .. widthHeight.x .. "Height: " .. widthHeight.y .. "\n");
	self.Width = widthHeight.x;
	self.Height= widthHeight.y;
	
	self.TotalObjects = 19;
	
	
	self.TotalButtons = 5;
	
	_G.CurrentButtonTouched = 1;
	self.ButtonStartIndex = 1;
	self.ButtonEndIndex = self.TotalButtons;
	
	-- Set Volume Slider Value
	self.SlidderValue = 0.2;
	
	-- Loop used to set position and Scale according to Window
	local startPoint =  self.GameObject:GetId()- self.TotalObjects;
	local endPoint = startPoint + self.TotalObjects-1;
	 for i = startPoint,endPoint,1 
	 do 
		  local gameObject = self.GOManager:FindGameObjectById(i);
		
		--  Get WindoResize.lua if UI Object has it
		 local WindowResizeLUA = gameObject:GetCustomComp("WindowResize");
		 if (WindowResizeLUA ~= nil) then 
			 WindowResizeLUA:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
		 end
		-- Get SlideScroller.lua if UI Object has it
		local SlideScrollerLUA = gameObject:GetCustomComp("SlideScroller");
		if (SlideScrollerLUA ~= nil) then 
			if(gameObject:GetTag() == "VolumeSlideScroller") then
				SlideScrollerLUA:SetSliderValue(self.SlidderValue);
			end
		end
	 end
	 
	self.WindowResizeEnabled = false;
	
	self.BaseWidth = self.Width;
	self.BaseHeight = self.Height;
	
end

--Update called every tick
UICameraSetting.Update = function(self, dt, owner) 

	if(self.Enabled == false) then
		return;
	end
	if(self.LoadStateEnabled == true) then
		self.Enabled = false;
		
			local world = EventManager.Get();
			world:LoadState(false, self.loadingState);
			return;
	end
-----------------------------------------------------------------------------------------------------------------------	
	local finalAnimationObjectCountEnabled = 0;
	-- Loop all UI Objects
	local startPoint =  self.GameObject:GetId()- self.TotalObjects;
	local endPoint = startPoint + self.TotalObjects-1;

	for i = startPoint,endPoint,1 
	do 
		
		local gameObject = self.GOManager:FindGameObjectById(i);
		if (gameObject == nil) then
				 goto continue;
		end
		--OutputPrint("\n" .. gameObject:GetTag());
		--OutputPrint("\n" .. gameObject:GetId());
		-- Get AffineAnimation.lua if UI Object has it
		local AffineAnimationLUA = gameObject:GetCustomComp("AffineAnimation");
		if (AffineAnimationLUA ~= nil) then 
			if(AffineAnimationLUA:GetFinalAnimationState() == true) then
				finalAnimationObjectCountEnabled = finalAnimationObjectCountEnabled + 1;
			
			end
			if(self.PlayFinalAnimation == true) then
				AffineAnimationLUA:SetFinalAnimation();
			end
		end
		-- Get WindowResizeButton.lua if UI Object has it
		local ButtonWindowResizeLUA = gameObject:GetCustomComp("WindowResizeButton");
		if (ButtonWindowResizeLUA ~= nil) then 
			if (ButtonWindowResizeLUA:ReturnClicked() == true) then
				ButtonWindowResizeLUA:SetClicked(false);
				if(gameObject:GetTag() == "800x600Button") then
					self.Width = 800;
					self.Height = 600;
					OutputPrint("\n" .. gameObject:GetTag());
					self.WindowResizeEnabled = true;
					break;
				elseif(gameObject:GetTag() == "1024x768Button") then
					self.Width = 1024;
					self.Height = 768;
					OutputPrint("\n" .. gameObject:GetTag());
					self.WindowResizeEnabled = true;
					break;
				elseif(gameObject:GetTag() == "1280x720Button") then
					self.Width = 1280;
					self.Height = 720;
					OutputPrint("\n" .. gameObject:GetTag());
					self.WindowResizeEnabled = true;
					break;
				elseif(gameObject:GetTag() == "1280x960Button") then
					self.Width = 1280;
					self.Height = 960;
					OutputPrint("\n" .. gameObject:GetTag());
					self.WindowResizeEnabled = true;
					break;
				end
			end			
		
		end
		-- Get Button.lua if UI Object has it
		local ButtonLUA = gameObject:GetCustomComp("Button");
		if (ButtonLUA ~= nil) then 
			
			if(ButtonLUA:ReturnClicked() == true) then
				
				ButtonLUA:SetClicked(false);
				if(gameObject:GetTag() == "ApplyButton") then
					
					self.loadingState = "Assets\\Levels\\Menu.json";
				elseif (gameObject:GetTag() == "CloseButton") then
					self.loadingState = "Assets\\Levels\\Menu.json";
				end
				self.PlayFinalAnimation = true;				
			end
			
		end
		-- Get SlideScroller.lua if UI Object has it
		local SlideScrollerLUA = gameObject:GetCustomComp("SlideScroller");
		 if (SlideScrollerLUA ~= nil) then 
			if(gameObject:GetTag() == "VolumeSlideScroller") then
				self.SlidderValue = SlideScrollerLUA:GetSliderValue();
			end
			if(self.PlayFinalAnimation == true ) then
				SlideScrollerLUA:Disable();
			end
		 end
		-- Get DropDown.lua if UI Object has it
		local DropDownLUA = gameObject:GetCustomComp("DropDown");
		
		if (DropDownLUA ~= nil) then 	
			if(DropDownLUA:GetArrowClick() == true) then	 
				self.ButtonStartIndex = DropDownLUA:StartButtonIndex()-1;
				self.ButtonEndIndex = DropDownLUA:EndButtonIndex();
			else	
				self.ButtonStartIndex = 1;
				self.ButtonEndIndex = self.TotalButtons;
			end
			
			if(self.PlayFinalAnimation == true ) then
				DropDownLUA:Disable();
			end
		end
		
		
		
		::continue::
		
	end
	if(finalAnimationObjectCountEnabled >= self.TotalObjects) then
		
		self.LoadStateEnabled  = true;
	end
	if(self.Forward == true) then
		self.Forward = false;
		if(_G.CurrentButtonTouched  > self.ButtonStartIndex) then
				_G.CurrentButtonTouched = _G.CurrentButtonTouched -1;
		end
	end
	if(self.Backward == true) then
		self.Backward = false;
		if(_G.CurrentButtonTouched < self.ButtonEndIndex) then
				_G.CurrentButtonTouched = _G.CurrentButtonTouched + 1;
		end
	end

	if(self.WindowResizeEnabled == true) then
		self.WindowResizeEnabled = false;
		EventManager.Get():SetWindowSize(false, self.Width, self.Height);
		-- Loop used to set position and Scale according to Window
		local startPoint =  self.GameObject:GetId()- self.TotalObjects;
		local endPoint = startPoint + self.TotalObjects-1;
		for i = startPoint,endPoint,1 
		do 
		  local gameObject = self.GOManager:FindGameObjectById(i);
		
		--  Get WindoResize.lua if UI Object has it
		 local WindowResizeLUA = gameObject:GetCustomComp("WindowResize");
		 if (WindowResizeLUA ~= nil) then 
			 WindowResizeLUA:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
		 end
		
	 end
		self.BaseWidth = self.Width;
		self.BaseHeight = self.Height;
		
	end
	
end

--Method
UICameraSetting.OnKey = function(self, key, state)
	if(SCANCODE.W == key) then
		self.Forward = state;
	elseif(SCANCODE.S == key) then
		self.Backward = state;
	end
end

UICameraSetting.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
end

UICameraSetting.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end

UICameraSetting.OnWindowSize = function(self, Width, Height, xScale, yScale)
	self.Width = Width;
	self.Height = Height;
	--LOG("W: " .. Width .. " H: " .. Height .. "\n")
end
UICameraSetting.WindowResize = function(self, Width, Height)
	self.Width = Width;
	self.Height = Height;
	self.WindowResizeEnabled = true;
end

UICameraSetting.OnJoystickButton = function(self, joystickId, button, state)
	if(button == CONTROLLER.DUP) then
		self.Forward = state;
	elseif(button == CONTROLLER.DDOWN) then
		self.Backward = state;
	end
end

UICameraSetting.OnJoystickMotion = function(self, joystickId, axis, value)
	if(value < 0.2 and value > -0.2) then
		value = 0.0;
	end;
	if(axis == 1 and value > 0.9) then
		self.Backward = true;
	elseif(axis == 1 and value < -0.9) then
		self.Forward = true;
	else
		self.Forward = false;
		self.Backward = false;
	end
end

UICameraSetting.OnDestruction = function(self)
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
	OnKeyEvent():Unbind({self, self.OnKey});
	OnWindowSize():Unbind({self, self.OnWindowSize});
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
	OnJoystickMotion():Unbind({self, self.OnJoystickMotion});
end

return UICameraSetting;