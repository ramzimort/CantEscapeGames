-- First approximation of a component script

test02Comp = 
{
	name = "test02Comp",
	
	--Custom stuff
	ranked = 230,
	isDumb = true
}


--Init called when comp is created
test02Comp.Init = function(self)

	OutputPrint("COMP INIT -" .. self.name .. "\n");

end


--Begin called when obj has all comps
test02Comp.Begin = function(self, owner)

	OutputPrint("COMP BEGIN IN LUA - " .. self.name .. "\n");

end


--Update called every tick
test02Comp.Update = function(self, dt, owner) 

	OutputPrint("\n\nUPDATE - " .. self.name .. " - " .. self:ReturnWeirdString() .. " - DT: " .. dt .. "\n");

end


-- Test local stuff
test02Comp.ReturnWeirdString = function(self)

	return "JAJAJAJAAJ";

end

return test02Comp;
