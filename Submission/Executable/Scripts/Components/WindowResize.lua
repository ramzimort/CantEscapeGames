
-- First approximation of a component script
math = {}
WindowResize = 
{
	uiComp;
	uiTransform;

	Width;
	Height;
	
}

--Init called when comp is created
WindowResize.Init = function(self)

end

--Init called when comp is created
WindowResize.OnDestruction = function(self)

end

--Begin called when obj has all comps
WindowResize.Begin = function(self, owner, goMgr)

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
	
	
	
end

--Update called every tick
WindowResize.Update = function(self, dt, owner) 
	

	
end
WindowResize.WindowResize = function(self, Width, Height, BaseWidth, BaseHeight)
	
		self.Width = Width;
		self.Height = Height;
	
		local FinalPosition = Vector3.new(self.uiComp:GetFinalPosition());
		FinalPosition.x = FinalPosition.x/BaseWidth;
		FinalPosition.x = FinalPosition.x * self.Width;
		FinalPosition.y = FinalPosition.y/BaseHeight;
		FinalPosition.y = FinalPosition.y * self.Height;
		
		self.uiComp:SetFinalPosition(FinalPosition);
	
	----------------------------------------------------------------------------
		local InitialPosition = Vector3.new(self.uiComp:GetInitialPosition());
		InitialPosition.x = InitialPosition.x/BaseWidth;
		InitialPosition.x = InitialPosition.x * self.Width;
		InitialPosition.y = InitialPosition.y / BaseHeight;
		InitialPosition.y = InitialPosition.y * self.Height;
		
		self.uiComp:SetInitialPosition(InitialPosition);
	----------------------------------------------------------------------------
		local Scale = Vector3.new(self.uiTransform:GetScale());
		Scale.x = Scale.x/BaseWidth;
		Scale.x = Scale.x * self.Width;
		Scale.y = Scale.y/BaseHeight;
		Scale.y = Scale.y * self.Height;
		self.uiTransform:Scale(Scale);
	----------------------------------------------------------------------------
		local Position = Vector3.new(self.uiTransform:GetPosition());
		Position.x = Position.x  /BaseWidth;
		Position.x = Position.x * self.Width;
		Position.y = Position.y / BaseHeight;
		Position.y = Position.y * self.Height;
		self.uiTransform:SetLocalPosition(Position);
		
	----------------------------------------------------------------------------
		local TouchedScale = Vector3.new(self.uiComp:GetTouchedScale());
		TouchedScale.x = TouchedScale.x  /BaseWidth;
		TouchedScale.x = TouchedScale.x * self.Width;
		TouchedScale.y = TouchedScale.y / BaseHeight;
		TouchedScale.y = TouchedScale.y * self.Height;
		self.uiComp:SetTouchedScale(TouchedScale);
		
	----------------------------------------------------------------------------
		local UnTouchedScale = Vector3.new(self.uiComp:GetUnTouchedScale());
		UnTouchedScale.x = UnTouchedScale.x  /BaseWidth;
		UnTouchedScale.x = UnTouchedScale.x * self.Width;
		UnTouchedScale.y = UnTouchedScale.y / BaseHeight;
		UnTouchedScale.y = UnTouchedScale.y * self.Height;
		self.uiComp:SetUnTouchedScale(UnTouchedScale);
		
		
	
end

function Normalize(value)
	   
	 local mulVal = (value.x * value.x) + (value.y * value.y) + (value.z * value.z);
	  local magnitude = math.sqrt(mulVal);
	  local result = value/magnitude;
      return result;
 end
return WindowResize;