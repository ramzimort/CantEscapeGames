-- First approximation of a component script

test02Comp = 
{
	name = "test02Comp";
	
	--Custom stuff
	timeElapsed = 0.0;
	OnTimePassed = multicast();

	nextAnim = "";
	animComp = nil;
}


--Method
test02Comp.OnKey = function(self, key, state)
	
	if (self.animComp == nil) then return end

	if(SCANCODE.I == key) then
	
		self.nextAnim = "DanceAnim";
		self.animComp:SwitchAnimation(self.nextAnim, 100.0);
	
	elseif(SCANCODE.O == key) then
	
		self.nextAnim = "StopAnim";
		self.animComp:SwitchAnimation(self.nextAnim, 100.0);

	elseif(SCANCODE.P == key) then
	
		self.nextAnim = "WalkAnim";
		self.animComp:SwitchAnimation(self.nextAnim, 100.0);

	elseif(SCANCODE.U == key) then
	
		self.nextAnim = "IdleAnim";
		self.animComp:SwitchAnimation(self.nextAnim, 100.0);
	end
end


--Init called when comp is created
test02Comp.Init = function(self)
end


--Begin called when obj has all comps
test02Comp.Begin = function(self, owner)

	self.animComp = owner:GetAnimationComp();

end



--Update called every tick
test02Comp.Update = function(self, dt, owner) 

	self.timeElapsed = self.timeElapsed + dt;

	--[[
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
	--]]

end


-- Test local stuff
test02Comp.ReturnWeirdString = function(self)

	--return "JAJAJAJAAJ";

end


-- Test local stuff
test02Comp.ReturnWeirdString02 = function(self)

	--return "FUUUUUUUUUUUUUUUUUCK ALBERT!!!!!!!!!!!";

end

test02Comp.OnDestruction = function(self)
end


return test02Comp;
