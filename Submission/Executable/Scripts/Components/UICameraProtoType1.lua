
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
	UICompOfImage = {};
	TotalImages = 0;
}


--Init called when comp is created
UICameraProtoType1.Init = function(self)
	
	
end

--Begin called when obj has all comps
UICameraProtoType1.Begin = function(self, owner, goMgr)
	
	
	
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
	
	
	-- put tag values
	self.UITags[0] = "UIBackgroundImage1";
	self.UITags[1] = "UIBackgroundImage2";
	self.UITags[2] = "UIBackgroundImage3";
	-- Total UI Objects Count
	self.TotalImages = 3;
	--
	for i = 0,self.TotalImages-1,1 
	do 
		local Tag = self.UITags[i];
		local GO = goMgr:FindGameObject(Tag);
		if (GO == nil) then
			OutputPrint(">>> GO  not found\n");
			return;
		end
		self.UICompOfImage[i] = GO:GetUiComp();
	end
	
	

end

--Update called every tick
UICameraProtoType1.Update = function(self, dt, owner) 
	
	
	
end

UICameraProtoType1.EnableImage = function(self, val) 
	
	self.UICompOfImage[val]:SetRenderEnable(true);
	
end
UICameraProtoType1.DisableImage = function(self, val) 
	
	self.UICompOfImage[val]:SetRenderEnable(false);
	
end
return UICameraProtoType1;