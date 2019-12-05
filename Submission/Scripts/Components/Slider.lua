
-- First approximation of a component script

SlideScroller = 
{	
	Enabled = true;
	-- Slider Base
	uiCompBase;
	uiBaseTransformComp;
	
	-- Slide Scroller GreenBackGround Component
	greenBackgroundObj;
	uiTransformCompGreenBackGround;
	scaleGreenBackGround;

	
	SliderValue = 0.0;
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

	-- Slider Base

	self.uiCompBase = owner:GetUiComp();
	if (self.uiCompBase == nil) then
		OutputPrint(">>> Slider Base UI Component not found\n");
		return;
	end

	self.uiBaseTransformComp = owner:GetTransformComp();
	if (self.uiBaseTransformComp == nil) then
		OutputPrint(">>> Slider Base Transform Component not found\n");
		return;
	end
	
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
	
	self.scaleGreenBackGround = Vector3.new(self.uiTransformCompGreenBackGround:GetScale());
	
end

--Update called every tick
SlideScroller.Update = function(self, dt, owner) 

	if(self.Enabled == false) then
		return;
	end	
end
SlideScroller.Disable = function(self)
   self.Enabled = false;
end
SlideScroller.GetSliderValue = function(self)
   return self.SliderValue;
end
SlideScroller.SetSliderValue = function(self, val)

   self.SliderValue = val;		
   self.uiTransformCompGreenBackGround:Scale(self.scaleGreenBackGround.x * self.SliderValue,self.scaleGreenBackGround.y,self.scaleGreenBackGround.z);
   
   
end


SlideScroller.OnDestruction = function(self)
end

return SlideScroller;