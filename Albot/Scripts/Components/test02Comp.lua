-- First approximation of a component script

test02Comp = 
{
	name = "test02Comp",
	
	--Custom stuff
	timeElapsed = 0.0,
	OnTimePassed = multicast()
}


--Init called when comp is created
test02Comp.Init = function(self)

	OutputPrint("COMP INIT -" .. self.name .. "\n");

end


--Begin called when obj has all comps
test02Comp.Begin = function(self, owner)

	OutputPrint("\nCOMP BEGIN IN LUA - " .. self.name .. "\n");

	OutputPrint("Amount of binded funcs: " .. #self.OnTimePassed.suscribers);

end


--Update called every tick
test02Comp.Update = function(self, dt, owner) 

	self.timeElapsed = self.timeElapsed + dt;



	if (self.timeElapsed >= 5.0) then
		self.timeElapsed = 0.0;
		if (self.OnTimePassed == nil) then
			OutputPrint("\nMulticast var is nil!\n");
		else

			---- OutputPrint("\n5 seconds alarm!\n");
			---- OutputPrint("Amount of binded funcs: " .. #self.OnTimePassed);	
			---- --# to call the delegate simply        --THIS IS SELF
			---- count, returnvals = self.OnTimePassed("LUA MULTICAST FIRED", dt);
			---- --# count is the total number of functions ran and returnvals is a table of return vals by pcall:
			---- OutputPrint("Number of functions called: " .. count);
			---- OutputPrint(", Return val: " .. returnvals[1][2] .. "\n");
			---- --self.SpawnBullet(owner);
			---- OutputPrint("\n5 After the multicast fired\n");

		end
	end

end


-- Test local stuff
test02Comp.ReturnWeirdString = function(self)

	return "JAJAJAJAAJ";

end


-- Test local stuff
test02Comp.ReturnWeirdString02 = function(self)

	return "FUUUUUUUUUUUUUUUUUCK ALBERT!!!!!!!!!!!";

end


--[[ Test local stuff
test02Comp.SpawnBullet = function(self, owner)

	OutputPrint("\nBULLET SPAWNED\n");
	
	--Option01 : with descriptor----------------------------
	desc = GameObjectDesc.new();
	desc.setupFunction = function(go)  
		
		comp01 = go:addCustomComp("test01Comp.lua");
		comp01.hashed = "SHUPA-SHUP";
		comp01.init();

		transform = go:AddTransformComponent();
		transform.position = Vector3(0, 0, 1);
		transform.init();

		go:Begin();
	end
	owner:Instantiate(desc);

	--Option02 : With cloning-------------------------------
	local bulletGo = GameObject.new(); --Imagine this go has a lot of components
	owner:Instantiate(bulletGo);

	--Option03 : With prefab name (uses factory)------------
	owner:Instantiate("Bullet.json");

end
--]]

return test02Comp;
