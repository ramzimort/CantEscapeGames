
-- First approximation of a component script

ButtonWindowResize = 
{

	-- Component
	uiComp;
	uiTransform;
	
	-- Data
	Width;
	Height;
	ButtonIndex;
	Enabled = false;
	Scale;
	TouchedScale;
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
ButtonWindowResize.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});
end

--Begin called when obj has all comps
ButtonWindowResize.Begin = function(self, owner, goMgr)
	
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
	
	self.Scale = Vector3.new(self.uiTransform:GetScale());
	self.TouchedScale = Vector3.new( self.uiTransform:GetScale())*1.1;
	self.Width = self.uiComp:GetWidth();
	self.Height = self.uiComp:GetHeight();
	OutputPrint("\n" .. self.Width);
	
end

--Update called every tick
ButtonWindowResize.Update = function(self, dt, owner) 
	
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
			local increment = self.ButtonIndex - ArrowButtonIndex;
			NewPosition.y = NewPosition.y + self.Scale.y * ( increment);
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
		
		local position = Vector3.new(self.uiTransform:GetPosition());
		local scale = Vector3.new(self.uiTransform:GetScale());
		local maxPosX = scale.x + position.x;
		local maxPosY = scale.y + position.y;
		if(self.Touched == true) then
			self.uiTransform:Scale(self.TouchedScale.x,self.TouchedScale.y,self.TouchedScale.z);
		else
			self.uiTransform:Scale(self.Scale.x,self.Scale.y,self.Scale.z);
		end
		if(self.ButtonIndex == _G.CurrentButtonTouched) then
			self.Touched = true;
			if(self.ENTER == true ) then
				self.ENTER = false;
				self.Clicked = true;
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
						self.uiTransform:Scale(self.TouchedScale.x,self.TouchedScale.y,self.TouchedScale.z);
						if(self.LEFTCLICK == true) then
							self.LEFTCLICK = false;
							self.Clicked = true;
							
							
						end
					end	
				end
			end
		end
	if(self.Clicked == true) then
		self.Clicked = false;
		
		  if (DropDownComponent ~= nil) then 
			  DropDownComponent:SetArrowClick(true);
			  local parentNameUIObject = DropDownComponent:GetParentName();
			
			  local parentGameObject = self.GoManager:FindGameObject(parentNameUIObject);
			  DropDownComponent:SetCurrentChildButtonSelect(self.ButtonIndex);
			  local UICameraComponent = parentGameObject:GetCustomComp("UICamera");
			  if (UICameraComponent == nil) then 
				  OutputPrint("ERROR, UICamera.lua not Found\n");
			  end
			  UICameraComponent:WindowResize(self.Width,self.Height);
		  end
		
	end
	
	
end
--Method
ButtonWindowResize.OnKey = function(self, key, state)
	if(SCANCODE.ENTER == key) then
		self.ENTER = state;
	end
end

ButtonWindowResize.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
end

ButtonWindowResize.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end

ButtonWindowResize.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnMouseMotion});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
end

ButtonWindowResize.Disable = function(self)
   self.Enabled = false;
end
ButtonWindowResize.WindowResize = function(self, Width, Height, BaseWidth, BaseHeight)
	self.Scale.x = (self.Scale.x / BaseWidth) * Width;
	self.Scale.y = (self.Scale.y / BaseHeight) * Height;
	self.TouchedScale.x = (self.TouchedScale.x / BaseWidth) * Width;
	self.TouchedScale.y = (self.TouchedScale.y / BaseHeight) * Height;
end
return ButtonWindowResize;