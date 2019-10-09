--TEMPLATE FOR A SCRIPTED COMPONENT


--THIS TABLE HAS TO BE NAMED SAME AS THE SCRIPT
ExampleComp = 
{
	name = "test02Comp",

	--HERE YOU CAN ADD VARIABLES FOR THE SCRIPT
	age = 32,
	code = "qwerty"
}


--Init called when comp is created and ovewritten
ExampleComp.Init = function(self)

	-- Can access variables via self (like this in cpp)
	OutputPrint("COMP INIT -" .. self.name .. "\n");

end


--Begin called when all objs and components have been created
ExampleComp.Begin = function(self, owner)

	OutputPrint("COMP BEGIN IN LUA - " .. self.name .. "\n");

end


--Update called every tick
ExampleComp.Update = function(self, dt, owner) 

	--Every tick

end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
ExampleComp.LocalMethod01 = function(self, a, b) 

	return a * b;

end


--AT THE END, NEED TO ADD THIS LINE
return ExampleComp;
