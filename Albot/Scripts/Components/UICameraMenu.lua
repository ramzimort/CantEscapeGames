
-- First approximation of a component script


UICameraMenu = 
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

	
	-- Buttons
	TotalButtons = 0;
	ButtonStartIndex;
	ButtonEndIndex;
	
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
	
	-- GameObjectManager
	GOManager;
	GameObject;
	
	-- Affine Animation Data
	PlayFinalAnimation = false;
}


--Init called when comp is created
UICameraMenu.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});
	OnWindowSize():Bind({self, self.OnWindowSize});
	
end

--Begin called when obj has all comps
UICameraMenu.Begin = function(self, owner, goMgr)

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
	-- Get 
	local Camera = owner:GetCameraComp():GetCamera();
	if (Camera == nil) then 
		OutputPrint("ERROR, Camera is NIll NIL\n");
	end
	local widthHeight = Camera:GetWidthHeight();
	LOG("Width: " .. widthHeight.x .. "Height: " .. widthHeight.y .. "\n");
	self.Width = widthHeight.x;
	self.Height= widthHeight.y;

	self.TotalObjects = 10;
	self.TotalButtons = 6;
	
	_G.CurrentButtonTouched = 1;
	self.ButtonStartIndex = 1;
	self.ButtonEndIndex = self.TotalButtons;
	
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

--Update called every tick
UICameraMenu.Update = function(self, dt, owner) 

	if(self.Enabled == false) then
		return;
	end
	if(self.LoadStateEnabled == true) then
		self.Enabled = false;
		local world = EventManager.Get();
		world:PushState(false, self.loadingState);
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
		-- Get AffineAnimation.lua if UI Object has it
		local AffineAnimationLUA = gameObject:GetCustomComp("AffineAnimation");
		if (AffineAnimationLUA ~= nil) then 
			if(AffineAnimationLUA:GetFinalAnimationState() == true) then
				
				if(gameObject:GetTag() == "Stars") then
					finalAnimationObjectCountEnabled = finalAnimationObjectCountEnabled + 1;
				else
					finalAnimationObjectCountEnabled = finalAnimationObjectCountEnabled + 1;
				end
			else
			
			end
			if(self.PlayFinalAnimation == true) then
				AffineAnimationLUA:SetFinalAnimation();
			end
		end
		
		-- Get Button.lua if UI Object has it
		local ButtonLUA = gameObject:GetCustomComp("Button");
		if (ButtonLUA ~= nil) then 
			if(ButtonLUA:ReturnClicked() == true) then
				-- Check the tag and use it to specify 
				if(gameObject:GetTag() == "AnimationsButton") then
					self.loadingState = "Assets\\Levels\\Loading.json";
				elseif (gameObject:GetTag() == "GraphicsButton") then
					--self.loadingState = "Assets\\Levels\\GraphicsButton.json";
				elseif (gameObject:GetTag() == "PhysicsButton") then
					--self.loadingState = "Assets\\Levels\\PhysicsButton.json";
				elseif (gameObject:GetTag() == "SettingButton") then
					self.loadingState = "Assets\\Levels\\Setting.json";
				elseif (gameObject:GetTag() == "CreditsButton") then
					self.loadingState = "Assets\\Levels\\Credits.json";
				elseif (gameObject:GetTag() == "ExitButton") then
					self.loadingState = "Assets\\Levels\\Exit.json";
				end

				self.PlayFinalAnimation = true;
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
	
end

--Method
UICameraMenu.OnKey = function(self, key, state)
	if(SCANCODE.W == key) then
		self.Forward = state;
	elseif(SCANCODE.S == key) then
		self.Backward = state;
	end
end

UICameraMenu.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
end

UICameraMenu.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end

UICameraMenu.OnWindowSize = function(self, Width, Height, xScale, yScale)
	self.Width = Width;
	self.Height = Height;
	--LOG("W: " .. Width .. " H: " .. Height .. "\n")
end

UICameraMenu.OnDestruction = function(self)
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
	OnKeyEvent():Unbind({self, self.OnKey});
	OnWindowSize():Unbind({self, self.OnWindowSize});
end

return UICameraMenu;