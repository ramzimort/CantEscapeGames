
-- First approximation of a component script


UICameraProtoType2 = 
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
	
	
}


--Init called when comp is created
UICameraProtoType2.Init = function(self)
	
	
end

--Begin called when obj has all comps
UICameraProtoType2.Begin = function(self, owner, goMgr)
	
	
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
	self.HealthSliderLUA:SetSliderValue(0.5);
	
	
end

--Update called every tick
UICameraProtoType2.Update = function(self, dt, owner) 
	
	
	
	
end

return UICameraProtoType2;