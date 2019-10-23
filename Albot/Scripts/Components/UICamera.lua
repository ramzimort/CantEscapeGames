
-- First approximation of a component script


UICamera = 
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

	-- Buttons
	CurrentButtonSelected = 0;
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
	
	OnKeyEvent():Bind({self, self.OnKey});

	OnMouseMotion():Bind({self, self.OnMouseMotion});
	
	OnMouseClick():Bind({self, self.OnMouseClick});

	
end

--Update called every tick
UICamera.Update = function(self, dt, owner) 

	_G.value = self.CurrentButtonSelected;
end

--Method
UICamera.OnKey = function(self, key, state)
	if(SCANCODE.W == key) then
		self.Forward = state;
		if(state == true) then
			if(self.CurrentButtonSelected > 0) then
				self.CurrentButtonSelected = self.CurrentButtonSelected -1;
			end
		end
	elseif(SCANCODE.S == key) then
		self.Backward = state;
		if(state == true) then
			if(self.CurrentButtonSelected < 5) then
				self.CurrentButtonSelected = self.CurrentButtonSelected + 1;
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