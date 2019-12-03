
-- First approximation of a component script


UICameraProtoType1 = 
{
	Enabled = true;
	-- World (State Change)
	world;
	-- Component
	uiComp;
	
	-- MOUSE
	MousePositionX = 0;
	MousePositionY = 0;
	DeltaPositionX = 0;
	DeltaPositionY = 0;
	LEFTCLICK = false;
	RIGHTCLICK = false;
	
	-- KEYBOARD
	Forward = false;
	Backward = false;
	Left = false;
	Right = false;

	-- GameObjectManager
	GOManager;
	GameObject;
	-- list of UI Object tag
	UITags = {};
	
	-- Health Slider
	HealthSliderLUA;
	
}


--Init called when comp is created
UICameraProtoType1.Init = function(self)
	
	
end

--Begin called when obj has all comps
UICameraProtoType1.Begin = function(self, owner, goMgr)
	
	-- put tag values
	self.UITags[0] = "Pic1";
	
	
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
	OutputPrint(">>> \nReached\n");
	
end

--Update called every tick
UICameraProtoType1.Update = function(self, dt, owner) 
	
	self.HealthSliderLUA:SetSliderValue(0.2);
	
end

return UICameraProtoType1;