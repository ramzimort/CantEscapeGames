
-- First approximation of a component script


UICameraProtoType3 = 
{
	Enabled = true;
	-- World (State Change)
	world;
	-- Component
	uiComp;
	
	-- GameObjectManager
	GOManager;
	GameObject;
	-- list of UI Object tag
	UITags = {};
	
	-- Health Slider
	HealthSliderLUA;
	
	-- Gas Slider
	GasSliderLUA;
	
}


--Init called when comp is created
UICameraProtoType3.Init = function(self)
	
	
end

--Begin called when obj has all comps
UICameraProtoType3.Begin = function(self, owner, goMgr)
	
	
	self.GameObject = owner;
	self.GOManager = goMgr;
	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	
	self.uiComp = owner:GetUiComp();
	if (self.uiComp == nil) then 
		OutputPrint("ERROR, UIComponent IS NIL\n");
	end
	
	-- Health Slider
	local HealthSliderTag = "HealthSlider";
	local healthSliderObj = goMgr:FindGameObject(HealthSliderTag);
	if (healthSliderObj == nil) then
		OutputPrint(">>> GO  not found\n");
		return;
	
	end
	 -- Get SlideScroller.lua if UI Object has itq
	self.HealthSliderLUA = healthSliderObj:GetCustomComp("Slider");
	 if (self.HealthSliderLUA  == nil) then 
		OutputPrint(">>> SliderComponent not found\n");
	 end
	
	-- Gas Slider
	local GasSliderTag = "GasSlider";
	local GasSliderObj = goMgr:FindGameObject(GasSliderTag);
	if (GasSliderObj == nil) then
		OutputPrint(">>> GO  not found\n");
		return;
	
	end
	 -- Get SlideScroller.lua if UI Object has itq
	self.GasSliderLUA = GasSliderObj:GetCustomComp("Slider");
	 if (self.GasSliderLUA  == nil) then 
		OutputPrint(">>> SliderComponent not found\n");
	 end
	
end

--Update called every tick
UICameraProtoType3.Update = function(self, dt, owner) 
	
	--self.HealthSliderLUA:SetSliderValue(0.2);
	--self.GasSliderLUA:SetSliderValue(0.8);
	
end

return UICameraProtoType3;