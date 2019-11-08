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


--Init called when comp is created
test02Comp.Init = function(self)
end


--Begin called when obj has all comps
test02Comp.Begin = function(self, owner)
    
	local rigidbody = owner:GetRigidbodyComp();
	if (rigidbody ~= nil) then
		rigidbody.OnCollision:Bind({self, self.BindingExample});
	end

end



--Update called every tick
test02Comp.Update = function(self, dt, owner) 

	self.timeElapsed = self.timeElapsed + dt;

end


-- Test local stuff
test02Comp.BindingExample = function(self, obj01, obj02)

	local rb1 = obj01:GetRigidbodyComp();
	local rb2 = obj02:GetRigidbodyComp();
	--OutputPrint("01-----\n");
	if (rb1 ~= nil and rb2 ~=nil) then
	
		--OutputPrint(">> Obj " .. obj01:GetTag() .. " is colliding against Obj name: " .. obj02:GetTag() .. ", " .. rb2.CollisionMask .. " \n");
		--OutputPrint(">> COLLISION_MASK.PLAYER: " .. COLLISION_MASK.PLAYER .. " \n");
		--OutputPrint("-----------------");
		if (rb2.CollisionMask == COLLISION_MASK.PLAYER) then
			--OutputPrint("03-----\n");
			local vel = Vector3.new(0,10,0);
			rb1:SetVelocity(vel);
			EventManager:Get():PlaySFX(false, "Assets\\SFX\\Collision1.mp3");
		end
	end
end


return test02Comp;
