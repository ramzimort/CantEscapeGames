
-- First approximation of a component script

Button = 
{
	-- MOUSE
	MousePositionX = 0;
	MousePositionY = 0;
	LEFTCLICK = false;
	RIGHTCLICK = false;
	
	-- KEYBOARD
	Up = false;
	Down = false;
	
	
}

--Init called when comp is created
Button.Init = function(self)

end

--Begin called when obj has all comps
Button.Begin = function(self, owner, goMgr)

	

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	
	local uicomp = owner:GetUiComp();
	if (uicomp ~= nil) then 
		uicomp.OnButtonClick:Bind({self, self.OnMouseClick});
	end

	
end

--Update called every tick
Button.Update = function(self, dt, owner) 

	
	
end



return Button;