
-- First approximation of a component script

SlideScroller = 
{
	-- Component
	uiComp;
	uiAffineAnimationComp;
	uiTransformComp;
	position;
	scale;

	
	-- Slide Scroller Button1 Component
	Button1Obj;
	uiCompButton1;
	uiButton1AffineAnimationComp;
	uiButton1TransformComp;
	locationButton1;
	positionButton1;
	scaleButton1;
	touchedScaleButton1;
	maxPosXButton1;
	maxPosYButton1;
	
	-- Slide Scroller Button2 Component
	Button2Obj;
	uiCompButton2;
	uiButton2AffineAnimationComp;
	uiButton2TransformComp;
	locationButton2;
	positionButton2;
	scaleButton2;
	touchedScaleButton2;
	maxPosXButton2;
	maxPosYButton2;
	
	-- Slide Scroller Slider Component
	sliderObj;
	uiCompSlider;
	uiTransformCompSlider;
	uiSliderAffineAnimationComp;
	positionSlider;
	scaleSlider;
	noramlizedValue;
	
	-- Slide Scroller GreenBackGround Component
	greenBackgroundObj;
	uiTransformCompGreenBackGround;
	scaleGreenBackGround;

	
	-- Data
	touchedScale;
	
	
	
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

end

--Begin called when obj has all comps
SlideScroller.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	tag = owner:GetTag();
	
	OnKeyEvent():Bind({self, self.OnKey});

	OnMouseMotion():Bind({self, self.OnMouseMotion});
	
	OnMouseClick():Bind({self, self.OnMouseClick});
	
	
-- Scroll Slider
	
	self.uiComp = owner:GetUiComp();
	if (self.uiComp == nil) then 
		OutputPrint("ERROR, UIComponent IS NIL\n");
	end
	
	self.uiTransformComp = owner:GetTransformComp();
	if (self.uiTransformComp == nil) then 
		OutputPrint("ERROR, uiTransformComp IS NIL\n");
	end
	
	self.uiAffineAnimationComp = owner:GetAffineAnimationComp();
	if (self.uiAffineAnimationComp == nil) then 
		OutputPrint("ERROR, Ui Affine Animation IS NIL\n");
	end
	self.position =  self.uiAffineAnimationComp:GetFinalPosition();
	self.scale = Vector3.new(self.uiTransformComp:GetScale());
	
	-- Button 1
	Button1Address = tag .. "Button1";
	
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
	
	self.uiButton1AffineAnimationComp = self.Button1Obj:GetAffineAnimationComp();
	if (self.uiButton1AffineAnimationComp == nil) then
		OutputPrint(">>> Button 1 Affine Animation Component not found\n");
		return;
	end
	self.locationButton1 = self.uiCompButton1:GetLocation();
	self.positionButton1 =  self.uiButton1AffineAnimationComp:GetFinalPosition();
	self.scaleButton1 = Vector3.new(self.uiButton1TransformComp:GetScale());
	self.touchedScaleButton1 = Vector3.new( self.scaleButton1.x,self.scaleButton1.y,self.scaleButton1.z);
	self.touchedScaleButton1 = self.touchedScaleButton1 * 1.1;
	self.maxPosXButton1 = self.positionButton1.x + self.scaleButton1.x;
	self.maxPosYButton1 = self.positionButton1.y + self.scaleButton1.y;
	
	-- Button 2
	Button2Address = tag .. "Button2";
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
	self.uiButton2AffineAnimationComp = self.Button2Obj:GetAffineAnimationComp();
	if (self.uiButton2AffineAnimationComp == nil) then
		OutputPrint(">>> Button 2 Affine Animation Component not found\n");
		return;
	end
	
	self.locationButton2 = self.uiCompButton2:GetLocation();
	self.positionButton2 =  self.uiButton2AffineAnimationComp:GetFinalPosition();
	self.scaleButton2 = Vector3.new(self.uiButton2TransformComp:GetScale());
	
	self.touchedScaleButton2 = Vector3.new( self.scaleButton2.x,self.scaleButton2.y,self.scaleButton2.z);
	self.touchedScaleButton2 = self.touchedScaleButton2 * 1.1;
	self.maxPosXButton2 = self.positionButton2.x + self.scaleButton2.x;
	self.maxPosYButton2 = self.positionButton2.y + self.scaleButton2.y;
	
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
	self.uiSliderAffineAnimationComp = self.sliderObj:GetAffineAnimationComp();
	if (self.uiSliderAffineAnimationComp == nil) then
		OutputPrint(">>> Slider Affine Animation Component not found\n");
		return;
	end
	
	self.positionSlider = self.uiSliderAffineAnimationComp:GetFinalPosition();
	self.scaleSlider = self.uiTransformCompSlider:GetScale();
	
	

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
	self.scaleGreenBackGround = self.uiTransformCompGreenBackGround:GetScale();
	
end

--Update called every tick
SlideScroller.Update = function(self, dt, owner) 

-- Button 1------------------------
	if(self.locationButton1 == _G.CurrentButtonTouched) then
		self.uiButton1TransformComp:Scale(self.touchedScaleButton1.x,self.touchedScaleButton1.y,self.touchedScaleButton1.z);
		if(self.ENTER == true) then
			if(self.positionSlider.x > self.position.x ) then
				self.positionSlider.x = self.positionSlider.x - 1.0;
			else
				self.positionSlider.x = self.position.x;
			end
		end
	else
		
		self.uiButton1TransformComp:Scale(self.scaleButton1.x,self.scaleButton1.y,self.scaleButton1.z);
	end
	
	if(self.MousePositionX < self.maxPosXButton1) then
		if(self.MousePositionX > self.positionButton1.x) then
			if(self.MousePositionY < self.maxPosYButton1) then
				if(self.MousePositionY > self.positionButton1.y) then
					_G.CurrentButtonTouched = self.locationButton1;
					self.uiButton1TransformComp:Scale(self.touchedScaleButton1.x,self.touchedScaleButton1.y,self.touchedScaleButton1.z);
					if(self.LEFTCLICK == true) then
						if(self.positionSlider.x > self.position.x ) then
							self.positionSlider.x = self.positionSlider.x - 1.0;
						else
							self.positionSlider.x = self.position.x;
						end
					end
				end
			end
		end
	end
-- Button 2------------------------
	if(self.locationButton2 == _G.CurrentButtonTouched) then
		self.uiButton2TransformComp:Scale(self.touchedScaleButton2.x,self.touchedScaleButton2.y,self.touchedScaleButton2.z);
		if(self.ENTER == true) then
			compare = self.position.x + self.scale.x;
			if(self.positionSlider.x  < compare - self.scaleSlider.x- self.scaleSlider.x) then
				self.positionSlider.x = self.positionSlider.x + 1.0;
			else
				self.positionSlider.x = compare - self.scaleSlider.x - self.scaleSlider.x;
			end
		end
	else
		
		self.uiButton2TransformComp:Scale(self.scaleButton2.x,self.scaleButton2.y,self.scaleButton2.z);
	end
	if(self.MousePositionX < self.maxPosXButton2) then
		if(self.MousePositionX > self.positionButton2.x) then
			if(self.MousePositionY < self.maxPosYButton2) then
				if(self.MousePositionY > self.positionButton2.y) then
					_G.CurrentButtonTouched = self.locationButton2;
					self.uiButton2TransformComp:Scale(self.touchedScaleButton2.x,self.touchedScaleButton2.y,self.touchedScaleButton2.z);
					if(self.LEFTCLICK == true) then
						compare = self.position.x + self.scale.x ;
						
						if(self.positionSlider.x  < compare - self.scaleSlider.x- self.scaleSlider.x) then
							
							self.positionSlider.x = self.positionSlider.x + 1.0;
						else
							self.positionSlider.x = compare - self.scaleSlider.x - self.scaleSlider.x;
							
						end
					end
				end
			end
		end
	end
	if (_G.PlayFinalAnimation == false) then
		self.noramlizedValue = (self.positionSlider.x - self.position.x)/ (self.scale.x);
		self.uiCompSlider:SetSliderValue(self.noramlizedValue);
		self.uiTransformCompSlider:SetLocalPosition(self.positionSlider.x,self.positionSlider.y,self.positionSlider.z);
		self.scaleGreenBackGround.x = self.positionSlider.x - self.position.x;
		self.uiTransformCompGreenBackGround:Scale(self.scaleGreenBackGround);
	end
	
end
--Method
SlideScroller.OnKey = function(self, key, state)
	if(SCANCODE.ENTER == key) then
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
return SlideScroller;