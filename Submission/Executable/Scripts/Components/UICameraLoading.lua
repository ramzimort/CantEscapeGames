
-- First approximation of a component script


UICameraLoading = 
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
UICameraLoading.Init = function(self)	
end

--Begin called when obj has all comps
UICameraLoading.Begin = function(self, owner, goMgr)
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

	self.TotalObjects = 2;
	
	
	
	
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
UICameraLoading.Update = function(self, dt, owner) 
	if(self.Enabled == false) then
		return;
	end
	
-----------------------------------------------------------------------------------------------------------------------	
end

--Method
UICameraLoading.OnKey = function(self, key, state)
	if(SCANCODE.W == key) then
		self.Forward = state;
	elseif(SCANCODE.S == key) then
		self.Backward = state;
	end
end

UICameraLoading.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
end

UICameraLoading.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end

UICameraLoading.OnWindowSize = function(self, Width, Height, xScale, yScale)
	self.Width = Width;
	self.Height = Height;
	--LOG("W: " .. Width .. " H: " .. Height .. "\n")
end

UICameraLoading.OnDestruction = function(self)
end

return UICameraLoading;