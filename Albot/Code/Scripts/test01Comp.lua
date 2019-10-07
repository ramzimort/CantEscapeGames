-- First approximation of a component script

test01Comp = 
{
	name = "test01Comp",

	--Custom stuff
	ranked = -1,
	isDumb = false,
	hashed = "_empty_"
}

--Init called when comp is created
test01Comp.Init = function(self)
	
	OutputPrint("COMP INIT - " .. self.name .. "\n");

end


--Begin called when obj has all comps
test01Comp.Begin = function(self, owner, goMgr)

	OutputPrint("COMP BEGIN IN LUA - " .. self.name .. "\n");

	if (owner == nil) then
		OutputPrint("FAILED, OWNER IS NIL\n");
		return;
	end
	
	OutputPrint(">>> TAG: " .. owner:GetTag() .. "\n");

	local testComp = owner:GetTestComp();
	if (testComp ~= nil) then
		OutputPrint(">>> " .. testComp.testName .. "\n");
	end
	
	local Comp02 = owner:GetCustomComp("test02Comp");
	if (Comp02 ~= nil) then
		OutputPrint(">>> Comp2 Call to method: " .. Comp02:ReturnWeirdString() .. "\n");
		OutputPrint(">>> Comp2 hashed var: " .. Comp02.hashed .. "\n");
		OutputPrint(">>> Comp2 ranked var: " .. Comp02.ranked .. "\n");
	end

	--Try to get an external GO
	local gameobj01 = goMgr:FindGameObjectById(0);
	if (gameobj01 == nil) then
		OutputPrint(">>> GO with id 0 not found\n");
		return;
	end

	OutputPrint(">>>>>>>>>> Go retrieved - TAG: " .. gameobj01:GetTag() .. " -\n");

	local otherGoComp02 = gameobj01:GetCustomComp("test02Comp");
	OutputPrint(">>>>>>>>>> Retrieved function from other go's comp02: " .. otherGoComp02:ReturnWeirdString() .. " -\n");

end


--Update called every tick
test01Comp.Update = function(self, dt, owner) 

	if (owner:GetTag() == "Andres") then
		OutputPrint("\n\nUPDATE - Andres got rank up. Andres is a little shit");
		self.ranked = self.ranked + 1;
	end

	OutputPrint("\nUPDATE - GO tag: " .. owner:GetTag() .. " - ScriptName: " .. 
	self.name .. " - DT: " .. dt .. " - RANK: " .. self.ranked .. ", Hash: " .. 
	self.hashed .. "\n");

end


return test01Comp;