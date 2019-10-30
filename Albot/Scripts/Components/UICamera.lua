
-- First approximation of a component script


UICamera = 
{
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
	-- Objects
	TotalObjects = 0;
	-- States
	loadingState;
}


--Init called when comp is created
UICamera.Init = function(self)

end

--Begin called when obj has all comps
UICamera.Begin = function(self, owner, goMgr)

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
	
	OnKeyEvent():Bind({self, self.OnKey});

	OnMouseMotion():Bind({self, self.OnMouseMotion});
	
	OnMouseClick():Bind({self, self.OnMouseClick});
	
	_G.CurrentButtonTouched = 1;

	_G.FinalAnimationCount = 0;
end

--Update called every tick
UICamera.Update = function(self, dt, owner) 


	
	if(self.TotalObjects <=  _G.FinalAnimationCount) then
		self.loadingState = _G.State;
		
		World.Get():PushStateEvent(false, self.loadingState);
	end
	
	
end

--Method
UICamera.OnKey = function(self, key, state)
	if(SCANCODE.W == key) then
		self.Forward = state;
		if(state == true) then
			if(_G.CurrentButtonTouched > 1) then
				_G.CurrentButtonTouched = _G.CurrentButtonTouched -1;
			end
		end
	elseif(SCANCODE.S == key) then
		self.Backward = state;
		
		if(state == true) then
			
			if(_G.CurrentButtonTouched < self.TotalButtons) then
				_G.CurrentButtonTouched = _G.CurrentButtonTouched + 1;
			end
		end
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

return UICamera;