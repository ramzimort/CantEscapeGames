-- First approximation of a component script

test02Comp = 
{
	name = "test02Comp"
}

--Init called when comp is created
test02Comp.Init = function(self)

	----OutputPrint("COMP INIT -" .. self.name .. "\n");

end


--Begin called when obj has all comps
test02Comp.Begin = function(self, owner)

	----OutputPrint("COMP BEGIN IN LUA - " .. self.name .. "\n");

end


--Update called every tick
test02Comp.Update = function(self, dt, owner) 

	----OutputPrint("UPDATE - " .. self.name .. " - DT: " .. dt .. "\n");

end