
-- First approximation of a component script

Text = 
{
	-- Component
	uiComp;
	
	textUnTouchedScale = Vector3.new(0.0,0.0,0.0);
	textTouchedScale = Vector3.new(0.0,0.0,0.0);

	-- Text Data
	text = "";
}

--Init called when comp is created
Text.Init = function(self)
	
end

--Begin called when obj has all comps
Text.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	self.uiComp = owner:GetUiComp();
	if (self.uiComp == nil) then 
		OutputPrint("ERROR, UIComponent IS NIL\n");
	end
	
	
	self.textUnTouchedScale = Vector3.new(self.uiComp:GetTextScale());
	self.textTouchedScale = Vector3.new(self.textUnTouchedScale*1.1);
	
	
end

--Update called every tick
Text.Update = function(self, dt, owner) 
	
end
--Method

Text.OnDestruction = function(self)
	
end
Text.Touched = function(self)
	self.uiComp:SetTextScale(self.textTouchedScale);
end
Text.UnTouched = function(self)
	self.uiComp:SetTextScale(self.textUnTouchedScale);
end
Text.SetText = function(self, val)
	self.text = self.uiComp:SetText(val);
end
Text.Enabled = function(self)
   self.Enabled = true;
end
return Text;