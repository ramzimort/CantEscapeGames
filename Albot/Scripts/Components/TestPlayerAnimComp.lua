-- First approximation of a component script

TestPlayerAnimComp = 
{
	name = "TestPlayerAnimComp";
	
	--Custom stuff
	animComp = nil;
	transformComp = nil;
	
	walking = false;
	jumping = false;

	accel = Vector3.new(0,0,0);
	velocity = Vector3.new(0,0,0);
	targetFwd = Vector3.new(0,0,0);
	
	rightPressed = false;
	leftPressed = false;
	upPressed = false;
	downPressed = false;
	spacePressed = false;
}


--Method
TestPlayerAnimComp.OnKeyPressed = function(self, key, state)
	
	if (self.animComp == nil) then 
		return 
	end

	if(SCANCODE.E == key) then
		self.walking = false;
		self.animComp:SetTrigger("Punch");
	
	elseif(SCANCODE.Q == key) then
		self.walking = false;
		self.animComp:SetTrigger("Kick");
		
	elseif(SCANCODE.R == key) then
		self.walking = false;
		self.animComp:SetTrigger("Upper");
		
	elseif(SCANCODE_ENTER == key) then
		self.animComp:SetTrigger("Crawl");

	elseif(SCANCODE.SPACE == key) then
		if (not self.jumping) then
			self.jumping = true;
			self.animComp:SetTrigger("Jump");
		else 
			self.jumping = false;
			self.animComp:SetTrigger("Land");
		end
	end

	
	if(SCANCODE.UP == key) then
		self.upPressed = not self.upPressed;
		self.targetFwd.x = 0.0;
		self.targetFwd.z = 1.0;
	end
	if(SCANCODE.DOWN == key) then
		self.downPressed = not self.downPressed;
		self.targetFwd.x = 0.0;
		self.targetFwd.z = -1.0;
	end		
	if(SCANCODE.RIGHT == key) then
		self.rightPressed = not self.rightPressed;
		self.targetFwd.x = 1.0;
		self.targetFwd.z = 0.0;
	end
	if(SCANCODE.LEFT == key) then
		self.leftPressed = not self.leftPressed;
		self.targetFwd.x = -1.0;
		self.targetFwd.z = 0.0;
	end

end

TestPlayerAnimComp.OnJoystickButton = function(self, ID, key, state)
	if (self.animComp == nil) then return end

	if (self.animComp == nil) then 
		return 
	end

	if(CONTROLLER.A == key) then
		self.walking = false;
		self.animComp:SetTrigger("Punch");
	
	elseif(CONTROLLER.B == key) then
		self.walking = false;
		self.animComp:SetTrigger("Kick");
		
	elseif(CONTROLLER.X == key) then
		self.walking = false;
		self.animComp:SetTrigger("Upper");

	elseif(CONTROLLER.Y == key) then
		if (not self.jumping) then
			self.jumping = true;
			self.animComp:SetTrigger("Jump");
		else 
			self.jumping = false;
			self.animComp:SetTrigger("Land");
		end
	end

	
	if(CONTROLLER.DUP == key) then
		self.upPressed = not self.upPressed;
		self.targetFwd.x = 0.0;
		self.targetFwd.z = 1.0;
	end
	if(CONTROLLER.DDOWN == key) then
		self.downPressed = not self.downPressed;
		self.targetFwd.x = 0.0;
		self.targetFwd.z = -1.0;
	end		
	if(CONTROLLER.DRIGHT == key) then
		self.rightPressed = not self.rightPressed;
		self.targetFwd.x = 1.0;
		self.targetFwd.z = 0.0;
	end
	if(CONTROLLER.DLEFT == key) then
		self.leftPressed = not self.leftPressed;
		self.targetFwd.x = -1.0;
		self.targetFwd.z = 0.0;
	end
end


--Init called when comp is created
TestPlayerAnimComp.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKeyPressed});
	OnJoystickButton():Bind({self, self.OnJoystickButton});
end


--Begin called when obj has all comps
TestPlayerAnimComp.Begin = function(self, owner)

	-- Cache the components
	self.animComp = owner:GetAnimationComp();
	self.transformComp = owner:GetTransformComp();

	--Set the forward Vec initial val
	self.targetFwd = self.transformComp:GetForward();

	-- Create states
	local idle_State =		self.animComp:CreateState("idle", "IdleAnim");
	local atk01_State =		self.animComp:CreateState("atk01", "AttackAnim");
	local atk02_State =		self.animComp:CreateState("atk02", "KickAnim");
	local atk03_State =		self.animComp:CreateState("Upper", "UpperAnim");
	local walk_State =		self.animComp:CreateState("walk", "WalkAnim", 1.5);
	local jumpLoop_State =	self.animComp:CreateState("jumpLoop", "JumpLoopAnim");
	local jumpEnd_State =	self.animComp:CreateState("jumpEnd", "JumpEndAnim");
	local crawl_State =		self.animComp:CreateState("Crawl", "CrawlAnim");

	-- Create transitions
	idle_State:SetTransition(atk01_State, 5.0, {"Punch"});
	idle_State:SetTransition(atk02_State, 5.0, {"Kick"});
	idle_State:SetTransition(atk03_State, 5.0, {"Upper"});
	idle_State:SetTransition(jumpLoop_State, 10.0, {"Jump"});
	idle_State:SetTransition(walk_State, 5.0, {"Walk"});
	idle_State:SetTransition(crawl_State, 10.0, {"Crawl"});
	
	atk01_State:SetTransition(idle_State, 10.0, {});
	atk02_State:SetTransition(idle_State, 10.0, {});
	atk03_State:SetTransition(idle_State, 10.0, {});
	
	jumpLoop_State:SetTransition(jumpEnd_State, 10.0, {"Land"});
	jumpEnd_State:SetTransition(idle_State, 5.0, {});
	jumpEnd_State:SetTransition(walk_State, 5.0, {"Walk"});
	crawl_State:SetTransition(idle_State, 10.0, {"Crawl"});
	
	walk_State:SetTransition(idle_State, 5.0, {"StopWalk"});
	walk_State:SetTransition(atk01_State, 5.0, {"Punch"});
	walk_State:SetTransition(atk02_State, 5.0, {"Kick"});
	walk_State:SetTransition(atk03_State, 5.0, {"Upper"});

	--Starting state
	self.animComp:SetEntryState(idle_State);



	--Animation events
	self.animComp:AddAnimEvent("KickAnim", 26, {self, self.OnKickTest01});
	self.animComp:AddAnimEvent("KickAnim", 20, {self, self.OnKickWhoosh});
	self.animComp:AddAnimEvent("KickAnim", 50, {self, self.OnKickTest02});
	
	self.animComp:AddAnimEvent("JumpEndAnim", 10, {self, self.OnKickTest01});
	
	self.animComp:AddAnimEvent("WalkAnim", 5, {self, self.OnKickTest01});
	self.animComp:AddAnimEvent("WalkAnim", 20, {self, self.OnKickTest02});
	self.animComp:AddAnimEvent("WalkAnim", 35, {self, self.OnKickTest01});
	self.animComp:AddAnimEvent("WalkAnim", 40, {self, self.OnKickTest02});

	self.animComp:AddAnimEvent("UpperAnim", 20, {self, self.OnKickWhoosh});
	self.animComp:AddAnimEvent("UpperAnim", 40, {self, self.OnKickWhoosh2});
	self.animComp:AddAnimEvent("UpperAnim", 50, {self, self.OnKickWhoosh});
	self.animComp:AddAnimEvent("UpperAnim", 54, {self, self.OnKickWhoosh2});
	self.animComp:AddAnimEvent("UpperAnim", 64, {self, self.OnKickWhoosh2});
	self.animComp:AddAnimEvent("UpperAnim", 74, {self, self.OnKickWhoosh});
	

end


--Update called every tick
TestPlayerAnimComp.Update = function(self, dt, owner) 
	
	if (self.animComp == nil) then return end
	if (self.transformComp == nil) then return end

	--Acceleration when pressed
	if (self.upPressed) then 
		self.accel.z = -7;
	end 
	if (self.downPressed) then 
		self.accel.z = 7;
	end 
	if (self.leftPressed) then 
		self.accel.x = -7;
	end 
	if (self.rightPressed) then 
		self.accel.x = 7;
	end 

	--Update rotation
	self:UpdateRotation();

	--Physics calcs
	self.velocity = self.accel * dt;
	local accelSub = self.accel * 0.125;
	self.accel = self.accel - accelSub;

	--Animation
	local speed = self.velocity:len();
	if (speed > 0.01 and not self.walking) then 
		self.walking = true;
		self.animComp:SetTrigger("Walk");
	elseif (speed <= 0.01 and self.walking) then 
		self.walking = false;
		self.animComp:SetTrigger("StopWalk");

	end


	--Movement
	self.transformComp:Translate(self.velocity);
end


TestPlayerAnimComp.UpdateRotation = function(self)
	
	--Get current forward and target forward
	local currFwd = self.transformComp:GetForward();
	local tgtFwd = self.targetFwd;

	--Make the calculations
	local PI = 3.14159;
	local dot = currFwd:dot(tgtFwd);
	
	local absdot = dot
	if (dot < 0.0) then
		absdot = -dot;
	end

	if (1.0 - absdot > 0.01) then --math.abs() 

		local rad_angle = Acos(dot);
		local yRot = (rad_angle  * 180.0) / PI;
		local thressholdAngle = 0.01;
		if ((yRot) > thressholdAngle or yRot < -thressholdAngle) then --math.abs()
		
			local right = self.transformComp:GetRight();
			local rightdot = right:dot(tgtFwd);
			if ( rightdot < 0.0) then
				yRot = -yRot;
			end

			if (yRot > 0.0) then
				self.transformComp:Rotate(0, 3.0, 0);
			elseif (yRot < 0.0) then
				self.transformComp:Rotate(0, -3.0, 0);
			end

		end
	end
	--]]
end


TestPlayerAnimComp.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnKeyPressed});
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
end


TestPlayerAnimComp.OnKickTest01 = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Collision1.mp3");
	--self.transformComp:Scale(0.075, 0.075, 0.075);
end

TestPlayerAnimComp.OnKickTest02 = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Block.mp3");
	--self.transformComp:Scale(0.05, 0.05, 0.05);
end

TestPlayerAnimComp.OnKickWhoosh = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Whoosh_01.mp3");
end

TestPlayerAnimComp.OnKickWhoosh2 = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Whoosh_02.mp3");
end


return TestPlayerAnimComp;
