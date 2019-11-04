
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
	
	local normalizeFinalPosition = Vector3.new(self.uiComp:GetFinalPosition());
		normalizeFinalPosition.x = normalizeFinalPosition.x/BaseWidth;
		normalizeFinalPosition.x = normalizeFinalPosition.x * self.Width;
		normalizeFinalPosition.y = normalizeFinalPosition.y/BaseHeight;
		normalizeFinalPosition.y = normalizeFinalPosition.y * self.Height;
		
		self.uiComp:SetFinalPosition(normalizeFinalPosition);
	
	----------------------------------------------------------------------------
		local normalizeInitialPosition = Vector3.new(self.uiComp:GetInitialPosition());
		normalizeInitialPosition.x = normalizeInitialPosition.x/BaseWidth;
		normalizeInitialPosition.x = normalizeInitialPosition.x * self.Width;
		normalizeInitialPosition.y = normalizeInitialPosition.y / BaseHeight;
		normalizeInitialPosition.y = normalizeInitialPosition.y * self.Height;
		
		self.uiComp:SetInitialPosition(normalizeInitialPosition);
	----------------------------------------------------------------------------
		local normalizeScale = Vector3.new(self.uiTransform:GetScale());
		normalizeScale.x = normalizeScale.x/BaseWidth;
		normalizeScale.x = normalizeScale.x * self.Width;
		normalizeScale.y = normalizeScale.y/BaseHeight;
		normalizeScale.y = normalizeScale.y * self.Height;
		self.uiTransform:Scale(normalizeScale);
	----------------------------------------------------------------------------
		local normalizePosition = Vector3.new(self.uiTransform:GetPosition());
		normalizePosition.x = normalizePosition.x  /BaseWidth;
		normalizePosition.x = normalizePosition.x * self.Width;
		normalizePosition.y = normalizePosition.y / BaseHeight;
		normalizePosition.y = normalizePosition.y * self.Height;
		self.uiTransform:SetLocalPosition(normalizePosition);
	
end

function Normalize(value)
	   
	 local mulVal = (value.x * value.x) + (value.y * value.y) + (value.z * value.z);
	  local magnitude = math.sqrt(mulVal);
	  local result = value/magnitude;
      return result;
 end
return WindowResize;