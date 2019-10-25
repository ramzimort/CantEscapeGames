-- First approximation of a component script

PlayerComp = 
{
	name = "PlayerComp";
	
	--Custom stuff
	nextAnim = "";
	animComp = nil;
	transformComp = nil;
	
	accel = Vector3.new(0,0,0);
	velocity = Vector3.new(0,0,0);

	walking = false;
	idle = true;
	rolling = false;
	
	rightPressed = false;
	leftPressed = false;
	upPressed = false;
	downPressed = false;
}


--Method
PlayerComp.OnKeyPressed = function(self, key, state)
	
	if (self.animComp == nil) then return end

	if(SCANCODE.UP == key) then
		self.upPressed = not self.upPressed;
	end
	if(SCANCODE.DOWN == key) then
		self.downPressed = not self.downPressed;
	end
	if(SCANCODE.LEFT == key) then
		self.leftPressed = not self.leftPressed;
	end	
	if(SCANCODE.RIGHT == key) then
		self.rightPressed = not self.rightPressed;
	end

	if (SCANCODE.SPACE == key) then 
		self.animComp:SwitchAnimation("AttackAnim", 5.0);
	end

end


--Init called when comp is created
PlayerComp.Init = function(self)
end


--Begin called when obj has all comps
PlayerComp.Begin = function(self, owner)

	OnKeyEvent():Bind({self, self.OnKeyPressed});

	self.animComp = owner:GetAnimationComp();
	self.transformComp = owner:GetTransformComp();

end


--Update called every tick
PlayerComp.Update = function(self, dt, owner) 

	if (self.animComp == nil) then return end
	if (self.transformComp == nil) then return end

	
	if (self.upPressed) then 
		self.accel.z = -5;
	end 
	if (self.downPressed) then 
		self.accel.z = 5;
	end 
	if (self.leftPressed) then 
		self.accel.x = -5;
	end 
	if (self.rightPressed) then 
		self.accel.x = 5;
	end 


	self.velocity = self.accel * dt;
	local accelSub = self.accel * 0.125;
	self.accel = self.accel - accelSub;

	--Animation
	local speed = self.velocity:len();
	if (speed > 0.01 and not self.walking) then 
		
		self.idle = false;
		self.walking = true;
		self.animComp:SwitchAnimation("WalkAnim", 5.0);

	elseif (speed <= 0.01 and not self.idle) then 

		self.velocity = Vector3.new(0);
		self.idle = true;
		self.walking = false;
		self.animComp:SwitchAnimation("IdleAnim", 5.0);

	end

	--Movement
	self.transformComp:Translate(self.velocity);
	OutputPrint("\n>>>> TRANSLATING: <" .. self.velocity.x .. ", " .. self.velocity.y .. ", " .. self.velocity.z .. ">");

end


return PlayerComp;
