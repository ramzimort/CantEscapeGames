
-- First approximation of a component script

SlideScroller = 
{	
	Enabled = true;
	-- Slider Base
	uiCompBase;
	uiBaseTransformComp;
	-- Slide Scroller Button1 Component
	Button1Obj;
	uiCompButton1;
	uiButton1TransformComp;
	Button1Index;
	ScaleButton1;
	TouchedScaleButton1;
	TouchedButton1 = false;
	ClickedButton1 = false;
	
	-- Slide Scroller Button2 Component
	Button2Obj;
	uiCompButton2;
	uiButton2TransformComp;
	Button2Index;
	ScaleButton2;
	TouchedScaleButton2;
	TouchedButton2 = false;
	ClickedButton2 = false;
	
	-- Slide Scroller Slider Component
	sliderObj;
	uiCompSlider;
	uiTransformCompSlider;
	SliderValue = 0.0;
	-- Slide Scroller GreenBackGround Component
	greenBackgroundObj;
	uiTransformCompGreenBackGround;

	
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
SlideScroller.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});
	OnJoystickButton():Bind({self, self.OnJoystickButton});

end

--Begin called when obj has all comps
SlideScroller.Begin = function(self, owner, goMgr)
	
	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	tag = owner:GetTag();

	-- Slider Base

	self.uiCompBase = owner:GetUiComp();
	if (self.uiCompBase == nil) then
		OutputPrint(">>> Slider Base UI Component not found\n");
		return;
	end

	self.uiBaseTransformComp = owner:GetTransformComp();
	if (self.uiBaseTransformComp == nil) then
		OutputPrint(">>> Slider Base Transform Component not found\n");
		return;
	end
	-- Button 1
	Button1Address = tag .. "ButtonLeft";
	self.Button1Obj = goMgr:FindGameObject(Button1Address);
	if (self.Button1Obj == nil) then
		OutputPrint(">>> GO  not found\n");
		return;
	end
	self.uiCompButton1 = self.Button1Obj:GetUiComp();
	if (self.uiCompButton1 == nil) then
		OutputPrint(">>> Button 1 UI Component not found\n");
		return;
	end
	self.uiButton1TransformComp = self.Button1Obj:GetTransformComp();
	if (self.uiButton1TransformComp == nil) then
		OutputPrint(">>> Button 1 Transform Component not found\n");
		return;
	end
	self.Button1Index = self.uiCompButton1:GetButtonIndex();
	local scale = Vector3.new(self.uiButton1TransformComp:GetScale());
	self.uiCompButton1:SetUnTouchedScale(scale);
	self.uiCompButton1:SetTouchedScale(scale*1.1);
	
	-- Button 2
	Button2Address = tag .. "ButtonRight";
	self.Button2Obj = goMgr:FindGameObject(Button2Address);
	if (self.Button2Obj == nil) then
		OutputPrint(">>> GO  not found\n");
		return;
	end
	self.uiCompButton2 = self.Button2Obj:GetUiComp();
	if (self.uiCompButton2 == nil) then
		OutputPrint(">>> Button 2 UI Component not found\n");
		return;
	end
	self.uiButton2TransformComp = self.Button2Obj:GetTransformComp();
	if (self.uiButton2TransformComp == nil) then
		OutputPrint(">>> Button 2 Transform Component not found\n");
		return;
	end
	self.Button2Index = self.uiCompButton2:GetButtonIndex();
	local scale = Vector3.new(self.uiButton2TransformComp:GetScale());
	self.uiCompButton2:SetUnTouchedScale(scale);
	self.uiCompButton2:SetTouchedScale(scale*1.1);
	
	-- Slider	
	SliderAddress = tag .. "Slider";
	self.sliderObj = goMgr:FindGameObject(SliderAddress);
	if (self.sliderObj == nil) then
		OutputPrint(">>> GO  not found\n");
		return;
	end
	
	self.uiCompSlider = self.sliderObj:GetUiComp();
	if (self.uiCompSlider == nil) then
		OutputPrint(">>> Slider UI Component not found\n");
		return;
	end
	
	self.uiTransformCompSlider = self.sliderObj:GetTransformComp();
	if (self.uiTransformCompSlider == nil) then
		OutputPrint(">>> Slider Transform Component not found\n");
		return;
	end	
	
	-- Green BackGround	
	GreenBackGroundAddress = tag .. "GreenBackGround";
	self.greenBackgroundObj = goMgr:FindGameObject(GreenBackGroundAddress);
	if (self.greenBackgroundObj == nil) then
		OutputPrint(">>> GO  not found\n");
		return;
	
	end
	
	self.uiTransformCompGreenBackGround = self.greenBackgroundObj:GetTransformComp();
	if (self.uiTransformCompGreenBackGround == nil) then
		OutputPrint(">>> Green BackGround Transform Component not found\n");
		return;
	end
	
end

--Update called every tick
SlideScroller.Update = function(self, dt, owner) 
	if(self.Enabled == false) then
		return;
	end

-- Set green BackGround scale to be between slider position and base position
	local greenBackGroundScale = Vector3.new(self.uiTransformCompGreenBackGround:GetScale());
	local sliderBasePos = Vector3.new(self.uiBaseTransformComp:GetPosition());
	local sliderPosition = Vector3.new(self.uiTransformCompSlider:GetPosition());
	greenBackGroundScale.x = sliderPosition.x - sliderBasePos.x; 
	self.uiTransformCompGreenBackGround:Scale(greenBackGroundScale.x,greenBackGroundScale.y,greenBackGroundScale.z);
	local greenBackGroundPos= Vector3.new(self.uiTransformCompGreenBackGround:GetPosition());

	-- Set Slider Posiiton
	local sliderBaseScale = Vector3.new(self.uiBaseTransformComp:GetScale());
	sliderPosition.x = sliderBasePos.x + self.SliderValue * sliderBaseScale.x;
	self.uiTransformCompSlider:SetLocalPosition(sliderPosition);
-- Button 1------------------------

	local positionButton1 = Vector3.new(self.uiButton1TransformComp:GetPosition());
	local scaleButton1 = Vector3.new(self.uiButton1TransformComp:GetScale());
	local maxPosXButton1 = scaleButton1.x + positionButton1.x;
	local maxPosYButton1 = scaleButton1.y + positionButton1.y;
	if(self.TouchedButton1 == true) then
		local touchedScale = Vector3.new(self.uiCompButton1:GetTouchedScale());
		self.uiButton1TransformComp:Scale(touchedScale);
	else
		local unTouchedScale = Vector3.new(self.uiCompButton1:GetUnTouchedScale());
		self.uiButton1TransformComp:Scale(unTouchedScale);
	end
	if(self.Button1Index == _G.CurrentButtonTouched) then
		self.TouchedButton1 = true;
		if(self.ENTER == true ) then
			self.ENTER = false;
			self.ClickedButton1 = true;
		end
	else
		self.TouchedButton1 = false;
	end
	if(self.MousePositionX < maxPosXButton1) then
		if(self.MousePositionX > positionButton1.x) then
			if(self.MousePositionY < maxPosYButton1) then
				if(self.MousePositionY > positionButton1.y) then
					self.TouchedButton1 = true;
					_G.CurrentButtonTouched = self.Button1Index;
					if(self.LEFTCLICK == true) then
						self.LEFTCLICK = false;
						self.ClickedButton1 = true;
					end
				end	
			end
		end
	end

	
	if(self.ClickedButton1 == true) then
		self.ClickedButton1 = false;
		if(self.SliderValue > 0.1) then
			self.SliderValue = self.SliderValue - 0.1;
			
		end
	end
	-- Button 2------------------------

	local positionButton2 = Vector3.new(self.uiButton2TransformComp:GetPosition());
	local scaleButton2 = Vector3.new(self.uiButton2TransformComp:GetScale());
	local maxPosXButton2 = scaleButton2.x + positionButton2.x;
	local maxPosYButton2 = scaleButton2.y + positionButton2.y;
	if(self.TouchedButton2 == true) then
		local touchedScale = Vector3.new(self.uiCompButton2:GetTouchedScale());
		self.uiButton2TransformComp:Scale(touchedScale);
	else
		local unTouchedScale = Vector3.new(self.uiCompButton2:GetUnTouchedScale());
		self.uiButton2TransformComp:Scale(unTouchedScale);
	end
	if(self.Button2Index == _G.CurrentButtonTouched) then
		self.TouchedButton2 = true;
		if(self.ENTER == true ) then
			self.ENTER = false;
			self.ClickedButton2 = true;
		end
	else
		self.TouchedButton2 = false;
	end
	if(self.MousePositionX < maxPosXButton2) then
		if(self.MousePositionX > positionButton2.x) then
			if(self.MousePositionY < maxPosYButton2) then
				if(self.MousePositionY > positionButton2.y) then
					self.TouchedButton2 = true;
					_G.CurrentButtonTouched = self.Button2Index;
					if(self.LEFTCLICK == true) then
						self.LEFTCLICK = false;
						self.ClickedButton2 = true;
					end
				end	
			end
		end
	end
	
	if(self.ClickedButton2 == true) then
		self.ClickedButton2 = false;
		if(self.SliderValue < 0.9) then
			self.SliderValue = self.SliderValue + 0.1;
			
		end
	end
	
end
--Method
SlideScroller.OnKey = function(self, key, state)
	if(SCANCODE.ENTER == key) then
		self.ENTER = state;
		
	end
end
SlideScroller.OnJoystickButton = function(self, joystickId, button, state)
	if(button == CONTROLLER.A) then
		self.ENTER = state;
	end
end
SlideScroller.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
end
SlideScroller.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end
SlideScroller.Disable = function(self)
   self.Enabled = false;
end
SlideScroller.GetSliderValue = function(self)
   return self.SliderValue;
end
SlideScroller.SetSliderValue = function(self, val)
   self.SliderValue = val;
end


SlideScroller.OnDestruction = function(self)
   	OnKeyEvent():Unbind({self, self.OnKey});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
end
return SlideScroller;