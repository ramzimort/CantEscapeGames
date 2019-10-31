
-- First approximation of a component script

DropDown = 
{
	-- Component
	uiComp;
	uiAffineAnimationComp;
	uiTransformComp;
	position;
	scale;
	totalChildButtons = 0;
	currentChildButtonSelected = 4;
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
	uiAffineAnimationCompDropDownBackBackGround;
	uiTransformCompDropDownBackBackGround;
	positionDropDownBackGround;
	scaleDropDownBackGround;
	
	-- Child Button
	InitalPostionChildButton = Vector3.new(-1000,-1000,0);
	
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
DropDown.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});
end

--Begin called when obj has all comps
DropDown.Begin = function(self, owner, goMgr)
	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	tag = owner:GetTag();

-- DropDown 
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
	self.totalChildButtons = self.uiComp:GetChildButtonCount();
	
	-- Button 
	ButtonAddress = tag .. "Button";
	
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
	DropDownBackGroundAddress = tag .. "BackGround";
	self.DropDownBackgroundObj = goMgr:FindGameObject(DropDownBackGroundAddress);
	if (self.DropDownBackgroundObj == nil) then
		OutputPrint(">>> GO DropDownBackGround not found\n");
		return;
	end
	
	self.uiTransformCompDropDownBackGround = self.DropDownBackgroundObj:GetTransformComp();
	if (self.uiTransformCompDropDownBackGround == nil) then
		OutputPrint(">>> Drop Down BackGround Transform Component not found\n");
		return;
	end
	self.uiAffineAnimationCompDropDownBackBackGround = self.DropDownBackgroundObj:GetAffineAnimationComp();
	if (self.uiAffineAnimationCompDropDownBackBackGround == nil) then
		OutputPrint(">>> Button  Affine Animation Component not found\n");
		return;
	end
	
	self.scaleDropDownBackGround = Vector3.new(self.uiTransformCompDropDownBackGround:GetScale());
	self.positionDropDownBackGround = Vector3.new(self.uiAffineAnimationCompDropDownBackBackGround:GetFinalPosition());

	-- Child Button
	for count = 1,self.totalChildButtons,1 do 
	
			ChildButtonAddress = tag .. "ChildButton" .. tostring(count);
			self.ChildButtonsObj[count] =  goMgr:FindGameObject(ChildButtonAddress);
			if (self.ChildButtonsObj[count]  == nil) then
				OutputPrint(">>> GO Child Button not found\n");
				return;
			end
			
		
	end
end

--Update called every tick
DropDown.Update = function(self, dt, owner) 

	if(self.MousePositionX < self.maxPosXButton) then
		if(self.MousePositionX > self.positionButton.x) then
			if(self.MousePositionY < self.maxPosYButton) then
				if(self.MousePositionY > self.positionButton.y) then
					self.uiButtonTransformComp:Scale(self.touchedScaleButton.x,self.touchedScaleButton.y,self.touchedScaleButton.z);
					if(self.LEFTCLICK == true) then
						self.LEFTCLICK = false;
						if(self.buttonEnabled == false) then
							self.buttonEnabled = true;
							
								local pos = Vector3.new(self.position);
								pos.x = self.positionDropDownBackGround.x;
								local scaleVal = Vector3.new(self.scale);
								scaleVal.y = scaleVal.y * (self.totalChildButtons-1)
								pos.y = pos.y + self.scale.y -5;
								self.uiTransformCompDropDownBackGround:Scale(scaleVal);
								self.uiTransformCompDropDownBackGround:SetLocalPosition( pos );
							
							local incr = 0;
							for counter = 1,self.totalChildButtons,1 do 
								
								local uiComponent = self.ChildButtonsObj[counter]:GetUiComp();  
								if (uiComponent == nil) then
									OutputPrint(">>> Drop Down Child Button UI Component not found\n");
									return;
								end
								
								local uiAffineAnimationComponent = self.ChildButtonsObj[counter]:GetAffineAnimationComp(); 
								if (uiAffineAnimationComponent == nil) then
									OutputPrint(">>> Drop Down Child Button Affine Animation Component not found\n");
									return;
								end
								local uiTransformComponent = self.ChildButtonsObj[counter]:GetTransformComp(); 
								if (uiTransformComponent == nil) then
									OutputPrint(">>> Drop Down  Child Button Transform Component not found\n");
									return;
								end
								self.InitalPostionChildButton = Vector3.new(uiAffineAnimationComponent:GetFinalPosition());
								if(self.currentChildButtonSelected == uiComponent:GetChildButtonLocation()) then
									local  posButton = Vector3.new(self.position);
									uiTransformComponent:SetLocalPosition( posButton);
									incr = incr +1;
								else
									local  posButton = Vector3.new(self.position);
									posButton.x = posButton.x - 5;
									posButton.y = posButton.y + self.scale.y * (counter-incr) -5;
									uiTransformComponent:SetLocalPosition( posButton.x,posButton.y,posButton.z);
								end
		
							end
							
						else
							self.buttonEnabled = false;
							self.uiButtonTransformComp:Scale(self.scaleButton.x,self.scaleButton.y,self.scaleButton.z);
							
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
	
	if(self.buttonEnabled == false) then
		
		if (_G.PlayFinalAnimation == false) then
			self.uiTransformCompDropDownBackGround:SetLocalPosition(self.positionDropDownBackGround);
			for counter = 1,self.totalChildButtons,1 do 
				local uiTransformComponent = self.ChildButtonsObj[counter]:GetTransformComp(); 
				if (uiTransformComponent == nil) then
					OutputPrint(">>> Drop Down  Child Button Transform Component not found\n");
					return;
				end
				uiTransformComponent:SetLocalPosition( self.InitalPostionChildButton);
			end
		end
	else
		for counter = 1,self.totalChildButtons,1 do 
			local uiTransformComponent = self.ChildButtonsObj[counter]:GetTransformComp(); 
			if (uiTransformComponent == nil) then
				OutputPrint(">>> Drop Down  Child Button Transform Component not found\n");
				return;
			end
			local uiAffineAnimationComponent = self.ChildButtonsObj[counter]:GetAffineAnimationComp(); 
			if (uiAffineAnimationComponent == nil) then
				OutputPrint(">>> Drop Down Child Button Affine Animation Component not found\n");
				return;
			end
			
		end
	end
end

--Method
DropDown.OnKey = function(self, key, state)
	if(SCANCODE.ENTER == key) then
		self.ENTER = state;
	end
end
DropDown.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
end
DropDown.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 2) then
		self.RIGHTCLICK = state;
	end
end

DropDown.OnDestruction = function(self, key, state)
	OnKeyEvent():Unbind({self, self.OnKey});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
end

return DropDown;