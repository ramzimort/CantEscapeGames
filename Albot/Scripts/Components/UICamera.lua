
-- First approximation of a component script


UICamera = 
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
	Width = 800;
	Height = 600;
	
	WindowResizeEnabled = false;
	
	-- GameObjectManager
	GOManager;
	GameObject;
	
	-- Affine Animation Data
	PlayFinalAnimation = false;
}


--Init called when comp is created
UICamera.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});
	OnWindowSize():Bind({self, self.OnWindowSize});
	
end

--Begin called when obj has all comps
UICamera.Begin = function(self, owner, goMgr)

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
	
	self.TotalObjects = self.uiComp:GetTotalObjects();
	
	
	self.TotalButtons = self.uiComp:GetTotalButtons();
	
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
		
		 local WindowResizeComponent = gameObject:GetCustomComp("WindowResize");
		 if (WindowResizeComponent ~= nil) then 
			 WindowResizeComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
		 end
		 -- Get Button.lua if UI Object has it
		 local ButtonComponent = gameObject:GetCustomComp("Button");
		 if (ButtonComponent ~= nil) then 
			 ButtonComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
		 end	
		 -- Get SlideScroller.lua if UI Object has itq
		 local SlideScrollerComponent = gameObject:GetCustomComp("SlideScroller");
		 if (SlideScrollerComponent ~= nil) then 
			 SlideScrollerComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
		 end
		 -- Get DropDown.lua if UI Object has it
		 local DropDownComponent = gameObject:GetCustomComp("DropDown");
		 if (DropDownComponent ~= nil) then 
			 DropDownComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
		 end
		 -- Get ButtonWindowResize.lua if UI Object has it
		 local ButtonWindowResizeComponent = gameObject:GetCustomComp("ButtonWindowResize");
		 if (ButtonWindowResizeComponent ~= nil) then 
			 ButtonWindowResizeComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
		 end
		 -- Get AffineAnimation.lua if UI Object has it
		 local AffineAnimationComponent = gameObject:GetCustomComp("AffineAnimation");
		 if (AffineAnimationComponent ~= nil) then 
			 AffineAnimationComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
		 end
		
	 end
	self.WindowResizeEnabled = false;
	
	self.BaseWidth = self.Width;
	self.BaseHeight = self.Height;
	
end

--Update called every tick
UICamera.Update = function(self, dt, owner) 

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
		--OutputPrint("\n" .. gameObject:GetTag());
		--OutputPrint("\n" .. gameObject:GetId());
		-- Get AffineAnimation.lua if UI Object has it
		local AffineAnimationComponent = gameObject:GetCustomComp("AffineAnimation");
		if (AffineAnimationComponent ~= nil) then 
			if(AffineAnimationComponent:GetFinalAnimationState() == true) then
				finalAnimationObjectCountEnabled = finalAnimationObjectCountEnabled + 1;
			end
			if(self.PlayFinalAnimation == true) then
				AffineAnimationComponent:SetFinalAnimation();
			end
			if(self.WindowResizeEnabled == true) then
				AffineAnimationComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
			end
		end
		
		-- Get Button.lua if UI Object has it
		local ButtonComponent = gameObject:GetCustomComp("Button");
		if (ButtonComponent ~= nil) then 
			
			if(ButtonComponent:ReturnClicked() == true) then
				-- Check the tag and use it to specify 
				self.loadingState = ButtonComponent:GetNextStateAddress();
				self.PlayFinalAnimation = true;
			end
			if(self.WindowResizeEnabled == true) then
				ButtonComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
			end
		
		end
		-- Get SlideScroller.lua if UI Object has it
		local SlideScrollerComponent = gameObject:GetCustomComp("SlideScroller");
		 if (SlideScrollerComponent ~= nil) then 
			 if(self.WindowResizeEnabled == true) then
				SlideScrollerComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
			end
		 end
		-- Get DropDown.lua if UI Object has it
		local DropDownComponent = gameObject:GetCustomComp("DropDown");
		
		if (DropDownComponent ~= nil) then 	
			if(DropDownComponent:GetArrowClick() == true) then	 
				self.ButtonStartIndex = DropDownComponent:StartButtonIndex()-1;
				self.ButtonEndIndex = DropDownComponent:EndButtonIndex();
			else	
				self.ButtonStartIndex = 1;
				self.ButtonEndIndex = self.TotalButtons;
			end		
			if(self.WindowResizeEnabled == true) then
				DropDownComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
			end
		end
		-- Get WindowResize.lua if UI Object has it
		if(self.WindowResizeEnabled == true) then
			local WindowResizeComponent = gameObject:GetCustomComp("WindowResize");
			if (WindowResizeComponent ~= nil) then 
				WindowResizeComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
			end
		end
		-- Get ButtonWindowResize.lua if UI Object has it
		local ButtonWindowResizeComponent = gameObject:GetCustomComp("ButtonWindowResize");
		if (ButtonWindowResizeComponent ~= nil) then 
			
			
			if(self.WindowResizeEnabled == true) then
				ButtonWindowResizeComponent:WindowResize(self.Width,self.Height, self.BaseWidth, self.BaseHeight);
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
		self.BaseWidth = self.Width;
		self.BaseHeight = self.Height;
		EventManager.Get():SetWindowSize(false, self.Width, self.Height);
	end
	
end

--Method
UICamera.OnKey = function(self, key, state)
	if(SCANCODE.W == key) then
		self.Forward = state;
	elseif(SCANCODE.S == key) then
		self.Backward = state;
	end
end

UICamera.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
end

UICamera.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end

UICamera.OnWindowSize = function(self, Width, Height, xScale, yScale)
	self.Width = Width;
	self.Height = Height;
	--LOG("W: " .. Width .. " H: " .. Height .. "\n")
end
UICamera.WindowResize = function(self, Width, Height)
	self.Width = Width;
	self.Height = Height;
	self.WindowResizeEnabled = true;
end

UICamera.OnDestruction = function(self)
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
	OnKeyEvent():Unbind({self, self.OnKey});
	OnWindowSize():Unbind({self, self.OnWindowSize});
end

return UICamera;