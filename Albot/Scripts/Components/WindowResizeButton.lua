
-- First approximation of a component script

WindowResizeButton = 
{

	-- Component
	uiComp;
	uiTransform;
	
	-- Data
	Width;
	Height;
	ButtonIndex;
	Enabled = false;
	enableButton = false;
	PositionOnEnabled = false;
	
	-- Trigger/Touch
	Touched = false;
	Clicked = false;
	
	-- Game Object/ Manager
		GoManager;

	-- MOUSE
	MousePositionX = 0;
	MousePositionY = 0;
	LEFTCLICK = false;
	RIGHTCLICK = false;
	ENTER = false;

	-- KEYBOARD
	Up = false;
	Down = false;	
	

}

--Init called when comp is created
WindowResizeButton.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});
	OnJoystickButton():Bind({self, self.OnJoystickButton});
end

--Begin called when obj has all comps
WindowResizeButton.Begin = function(self, owner, goMgr)
	
	self.GoManager = goMgr;
	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
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
	self.ButtonIndex = self.uiComp:GetButtonIndex();
	
	local scale = Vector3.new(self.uiTransform:GetScale());
	self.uiComp:SetUnTouchedScale(scale);
	self.uiComp:SetTouchedScale(scale*1.1);
	
end

--Update called every tick
WindowResizeButton.Update = function(self, dt, owner) 
	
	local parentName = self.uiComp:GetParentName();
	
	local gameObject = self.GoManager:FindGameObject(parentName);
	
	local parentTransform = gameObject:GetTransformComp();
	if(gameObject == nil) then
		OutputPrint("ERROR, Parent Game Object not Found\n");
	end
	local DropDownComponent = gameObject:GetCustomComp("DropDown");
	if (DropDownComponent == nil) then 
			OutputPrint("ERROR, DropDown.lua not Found\n");
	end

	if(self.uiComp:GetRenderEnable() == true) then
		if(DropDownComponent:GetArrowClick() == true) then
		
			local ArrowButtonIndex = DropDownComponent:GetButtonArrowIndex();
			local NewPosition = Vector3.new(parentTransform:GetPosition());
			local scale = Vector3.new(self.uiComp:GetTouchedScale());
			local increment = self.ButtonIndex - ArrowButtonIndex;
			NewPosition.y = NewPosition.y + scale.y * ( increment );
			self.uiTransform:SetLocalPosition(NewPosition); 
			self.Enabled = true;
		else
			if(DropDownComponent:GetCurrentChildButtonSelect() == self.ButtonIndex) then
				local parentPosition = Vector3.new(parentTransform:GetPosition());
				local position = Vector3.new(self.uiTransform:GetPosition());
				position.y = parentPosition.y;
				self.uiTransform:SetLocalPosition(position);
				self.uiComp:SetRenderEnable(true);
				return;
			else
				-- local parentPosition = Vector3.new(parentTransform:GetPosition());
				-- parentPosition.y = -100;
				-- self.uiTransform:SetLocalPosition(parentPosition);
				return;
			end
		end
	else
		if(DropDownComponent:GetCurrentChildButtonSelect() == self.ButtonIndex) then
			self.uiComp:SetRenderEnable(true);
		end
	end
	if(self.Enabled == false) then
		return;
	end
		
		if(self.Clicked == true) then
			--self.Clicked = false;
		end
		
		local position = Vector3.new(self.uiTransform:GetPosition());
		local scale = Vector3.new(self.uiComp:GetUnTouchedScale());
		local NewPosition = Vector3.new(parentTransform:GetPosition());
		local scale = Vector3.new(self.uiComp:GetTouchedScale());
		local ArrowButtonIndex = DropDownComponent:GetButtonArrowIndex();
		local increment = self.ButtonIndex - ArrowButtonIndex;
		NewPosition.y = NewPosition.y + scale.y * ( increment );
		local maxPosX = scale.x + NewPosition.x;
		local maxPosY = scale.y + NewPosition.y;
		if(self.Touched == true) then
			local touchedScale = Vector3.new(self.uiComp:GetTouchedScale());
			self.uiTransform:Scale(touchedScale);
		else
			local unTouchedScale = Vector3.new(self.uiComp:GetUnTouchedScale());
			self.uiTransform:Scale(unTouchedScale);
		end
		if(self.ButtonIndex == _G.CurrentButtonTouched) then
			self.Touched = true;
			if(self.ENTER == true ) then
				self.ENTER = false;
				self.Clicked = true;
				DropDownComponent:SetArrowClick(true);
				DropDownComponent:SetCurrentChildButtonSelect(self.ButtonIndex);
			end
		else
			self.Touched = false;
		end
		if(self.MousePositionX < maxPosX) then
			if(self.MousePositionX > position.x) then
				if(self.MousePositionY < maxPosY) then
					if(self.MousePositionY > position.y) then
						self.Touched = true;
						_G.CurrentButtonTouched = self.ButtonIndex;
						if(self.LEFTCLICK == true) then
							self.LEFTCLICK = false;
							self.Clicked = true;
							if (DropDownComponent ~= nil) then 
								DropDownComponent:SetArrowClick(true);
								DropDownComponent:SetCurrentChildButtonSelect(self.ButtonIndex);
								OutputPrint("\n" .. position.y);
							end
							
						end
					end	
				end
			end
		end

	
	
end
--Method
WindowResizeButton.OnKey = function(self, key, state)
	if(SCANCODE.ENTER == key) then
		self.ENTER = state;
	end
end

WindowResizeButton.OnJoystickButton = function(self, joystickId, button, state)
	if(button == CONTROLLER.A) then
		self.ENTER = state;
	end
end

WindowResizeButton.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
end

WindowResizeButton.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end

WindowResizeButton.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnMouseMotion});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
end
WindowResizeButton.ReturnClicked = function(self)
   return self.Clicked;
end
WindowResizeButton.SetClicked = function(self, val)
    self.Clicked = false;
end
WindowResizeButton.Disable = function(self)
   self.Enabled = false;
end

return WindowResizeButton;