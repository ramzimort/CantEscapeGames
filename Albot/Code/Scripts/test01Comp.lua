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
	
	----OutputPrint("COMP INIT - " .. self.name .. "\n");

end


--Begin called when obj has all comps
test01Comp.Begin = function(self, owner)

	----OutputPrint("COMP BEGIN IN LUA - " .. self.name .. "\n");

	if (owner == nil) then
		----OutputPrint("FAILED, OWNER IS NIL\n");
		return;
	end
	
	----OutputPrint(">>> " .. owner:GetTag() .. "\n");

	--local testComp = owner:GetTestComp();
	--OutputPrint(">>> " .. testComp.testName .. "\n");

	--local test02Comp = owner:GetCustomComp("test02Comp");
	--OutputPrint(">>> " .. test02Comp.GetName() .. "\n");

end


--Update called every tick
test01Comp.Update = function(self, dt, owner) 

	----OutputPrint("UPDATE - " .. self.name .. " - DT: " .. dt .. 
	----" - RANK: " .. self.ranked .. ", Hash: " .. self.hashed .. "\n");

end