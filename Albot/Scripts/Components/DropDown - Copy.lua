
-- First approximation of a component script

SlideScroller = 
{
	-- Component
	uiComp;
	uiAffineAnimationComp;
	uiTransformComp;
	position;
	scale;
	totalChildButtons;
	ChildButtonsObj = {};

	
	-- DropDown Button Component
	ButtonObj;
	uiCompButton;
	uiButtonAffineAnimationComp;
	uiButtonTransformComp;
	locationButton;
	positionButton;
	scaleButton;
	touchedScaleButton;
	maxPosXButton;
	maxPosYButton;
	buttonEnabled = false;
	
	

	-- Slide Scroller BackGround Component
	DropDownBackgroundObj;
	uiTransformCompDropDownBackBackGround;
	positionDropDownBackGround;
	scaleDropDownBackGround;
	
	
	
	
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
end

--Begin called when obj has all comps
SlideScroller.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	tag = owner:GetTag();

-- DropDown Slider
	
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
	
	-- Button 
	ButtonAddress = tag .. "Button";
	OutputPrint("\n" .. ButtonAddress);
	self.ButtonObj = goMgr:FindGameObject(ButtonAddress);
	if (self.ButtonObj == nil) then
		OutputPrint(">>> GO Button not found\n");
		return;
	end
	
	self.uiCompButton = self.ButtonObj:GetUiComp();
	if (self.uiCompButton == nil) then
		OutputPrint(">>> Button  UI Component not found\n");
		return;
	end
	
	self.uiButtonTransformComp = self.ButtonObj:GetTransformComp();
	if (self.uiButtonTransformComp == nil) then
		OutputPrint(">>> Button  Transform Component not found\n");
		return;
	end
	
	self.uiButtonAffineAnimationComp = self.ButtonObj:GetAffineAnimationComp();
	if (self.uiButtonAffineAnimationComp == nil) then
		OutputPrint(">>> Button  Affine Animation Component not found\n");
		return;
	end
	self.positionButton =  self.uiButtonAffineAnimationComp:GetFinalPosition();
	self.scaleButton = Vector3.new(self.uiButtonTransformComp:GetScale());
	self.touchedScaleButton = Vector3.new( self.scaleButton.x,self.scaleButton.y,self.scaleButton.z);
	self.touchedScaleButton = self.touchedScaleButton * 1.1;
	self.maxPosXButton = self.positionButton.x + self.scaleButton.x;
	self.maxPosYButton = self.positionButton.y + self.scaleButton.y;
	
--  Drop Down BackGround
	DropDownBackGroundAddress = tag .. "DropDownBackGround";
	self.dropDownBackGroundroundObj = goMgr:FindGameObject(DropDownBackGroundAddress);
	if (self.dropDowngroundObj == nil) then
		OutputPrint(">>> GO DropDownBackGround not found\n");
		return;
	end
	
	self.uiTransformCompDropDownBackGround = self.dropDowngroundObj:GetTransformComp();
	if (self.uiTransformCompDropDownBackGround == nil) then
		OutputPrint(">>> Green BackGround Transform Component not found\n");
		return;
	end
	self.scaleDropDownBackGround = self.uiTransformCompDropDownBackGround:GetScale();
	self.positionDropDownBackGround = self.uiTransformCompDropDownBackGround:GetFinalPosition();
	
end

--Update called every tick
SlideScroller.Update = function(self, dt, owner) 

	if(self.MousePositionX < self.maxPosXButton) then
		if(self.MousePositionX > self.positionButton.x) then
			if(self.MousePositionY < self.maxPosYButton) then
				if(self.MousePositionY > self.positionButton.y) then
					self.uiButtonTransformComp:Scale(self.touchedScaleButton.x,self.touchedScaleButton.y,self.touchedScaleButton.z);
					if(self.LEFTCLICK == true) then
						self.LEFTCLICK = false;
						if(self.buttonEnabled == false) then
							self.buttonEnabled = true;
							self.uiButtonTransformComp:SetLocalPosition( self.position);
						else
							self.buttonEnabled = false;
							self.uiButtonTransformComp:SetLocalPosition( self.positionDropDownBackGround);
						end
					end
				end
			else
				self.uiButtonTransformComp:Scale(self.scaleButton.x,self.scaleButton.y,self.scaleButton.z);
			end
			
		else
			self.uiButtonTransformComp:Scale(self.scaleButton.x,self.scaleButton.y,self.scaleButton.z);
		end
		
	else
		self.uiButtonTransformComp:Scale(self.scaleButton.x,self.scaleButton.y,self.scaleButton.z);
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

SlideScroller.OnDestruction = function(self, key, state)
	OnKeyEvent():Unbind({self, self.OnKey});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
end

return SlideScroller;