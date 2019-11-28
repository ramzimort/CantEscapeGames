-- First approximation of a component script

TestPlayerAnimComp = 
{
	name = "TestPlayerAnimComp";
	
	--Custom stuff
	animComp = nil;
	transformComp = nil;
	
	walking = false;
	jumping = false;
	falling = false;
	landing = false;
	isCrawling = false;
	landingTime = 0.0;
	yFloor = 0.0;

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

	if(SCANCODE.E == key and state) then
		self.animComp:SetTrigger("Punch");
	end
	
	if(SCANCODE.Q == key and state) then
		self.animComp:SetTrigger("Kick");
	end
		
	if(SCANCODE.R == key and state) then
		self.animComp:SetTrigger("Upper");
	end
		
	if(SCANCODE.ENTER == key and state) then
		if (self.isCrawling or (not self.walking and not self.jumping)) then
			self.isCrawling = not self.isCrawling;
			self.animComp:SetTrigger("Crawl");
		end
	end

	if(SCANCODE.SPACE == key and state) then
		if (not self.jumping and not self.isCrawling) then
			EventManager:Get():PlaySFX(false, "Assets\\SFX\\Jump.mp3");
			self.jumping = true;
			self.walking = false;
			self.yFloor = self.transformComp:GetPosition().y;
			self.accel.y = 60;
			self.animComp:SetTrigger("Jump");
		end
	end
	

	--Up-Down Movement
	if(SCANCODE.UP == key) then
		self.upPressed = state;
	end
	if(SCANCODE.DOWN == key) then
		self.downPressed = state;
	end
	if(SCANCODE.RIGHT == key) then
		self.rightPressed = state;
	end
	if(SCANCODE.LEFT == key) then
		self.leftPressed = state;
	end
end


TestPlayerAnimComp.OnJoystickButton = function(self, ID, key, state)
	
	if (self.animComp == nil) then 
		return 
	end

	if(CONTROLLER.A == key and state) then
		self.animComp:SetTrigger("Punch");
	end
	
	if(CONTROLLER.B == key and state) then
		self.animComp:SetTrigger("Kick");
	end
		
	if(CONTROLLER.X == key and state) then
		self.animComp:SetTrigger("Upper");
	end

	if(CONTROLLER.Y == key and state) then
		if (not self.jumping) then
			self.walking = false;
			self.jumping = true;
			self.animComp:SetTrigger("Jump");
			self.accel.y = 60;
			self.yFloor = self.transformComp:GetPosition().y;
		end
	end

	
	if(CONTROLLER.DUP == key) then
		self.upPressed = state;
	end
	if(CONTROLLER.DDOWN == key) then
		self.downPressed = state;
	end
	if(CONTROLLER.DRIGHT == key) then
		self.rightPressed = state;
	end
	if(CONTROLLER.DLEFT == key) then
		self.leftPressed = state;
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

	--Setup of the state machine
	self:AnimatorSetup();

end


--Update called every tick
TestPlayerAnimComp.Update = function(self, dt, owner) 
	
	if (self.animComp == nil) then return end
	if (self.transformComp == nil) then return end


	--GRAVITY FAKING
	if (self.falling) then
		self.accel.y = -20;
	end
	if (self.landing) then
		self.landingTime = self.landingTime - dt;
		if (self.landingTime < 0.0) then
			self.landingTime = 0.0;
			self.landing = false;
		end
	end


	--Handle x-z displacement
	if (not self.isCrawling) then
		self:HandleMovement(self.landing, self.isJumping);
	end

	--Rotation to face input shit
	if (self.targetFwd.x ~= 0 or self.targetFwd.y ~= 0 or self.targetFwd.z ~= 0) then
		self:UpdateRotation(self.isCrawling);
	end

	--Physics integration and movement
	self.velocity = self.accel * dt;
	local accelSub = self.accel * 0.125;
	self.accel = self.accel - accelSub;
	self.transformComp:Translate(self.velocity);
	--owner:GetRigidbodyComp():SetVelocity(self.velocity);

	--GRAVITY FAKING-------------------------
	if (self.jumping and not self.falling) then
		if (self.velocity.y <= 0.05) then
			self.falling = true;
		end
	elseif(self.jumping and self.falling) then
		--Apply gravity stuff
		local yDisp = self.transformComp:GetPosition().y - self.yFloor; 
		if (yDisp < 0) then 

			--Fire animation change and correct position
			self.animComp:SetTrigger("Land");
			self.transformComp:Translate(0, -yDisp, 0);
			
			--Dont keep falling and set flags to false
			self.accel.y = 0;
			self.falling = false;
			self.jumping = false;

			--Set landing time. It wont move until stops landing
			self.landingTime = 0.20; --Half a second
			self.landing = true;

			--Also stop moving in the xz dir until landing ends
			self.walking = false;
			self.accel.x = 0.0;
			self.accel.z = 0.0;

			EventManager:Get():PlaySFX(false, "Assets\\SFX\\Step.mp3");
		end
	end--------------------------------------

	
	--WALKING Animation control
	local xz_vel = Vector2.new(self.velocity.x, self.velocity.z);
	local speed = xz_vel:len();
	if (speed > 0.3 and not self.walking and not self.jumping) then
		self.walking = true;
		self.animComp:SetTrigger("Walk");
	elseif (speed == 0 and self.walking) then 
		self.walking = false;
		self.animComp:SetTrigger("StopWalk");
	end
end



TestPlayerAnimComp.HandleMovement = function(self, isLanding, isJumping)
	
	--Acceleration when pressed UP OR DOWN
	if (self.upPressed) then 
		self.accel.z = -1;
		self.targetFwd.z = -1.0;
	elseif (self.downPressed) then 
		self.accel.z = 1;
		self.targetFwd.z = 1.0;
	else
		self.accel.z = 0;
		self.targetFwd.z = 0;
	end
	
	--Acceleration when pressed RIGHT OR LEFT
	if (self.leftPressed) then 
		self.accel.x = -1;
		self.targetFwd.x = -1.0;
	elseif (self.rightPressed) then 
		self.accel.x = 1;
		self.targetFwd.x = 1.0;
	else 
		self.accel.x = 0;
		self.targetFwd.x = 0; 
	end

	--Normalize and apply the magnitude
	local mgt = 20.0;
	if (isLanding or isJumping) then
		mgt = 0.5 * mgt;
	end
	local xzAccel = Vector2.new(self.accel.x, self.accel.z);
	xzAccel:normalize();
	self.accel = Vector3.new( mgt*xzAccel.x, self.accel.y, mgt*xzAccel.y);
end



TestPlayerAnimComp.UpdateRotation = function(self, isCrawling)
	
	--Get current forward and target forward
	local currFwd = self.transformComp:GetForward();
	local tgtFwd = self.targetFwd;
	currFwd:normalize();
	tgtFwd:normalize();

	local mgt = 10.0;

	--Make the calculations
	local PI = 3.14159265;
	local dot = currFwd:dot(tgtFwd);

	if (Abs(dot) <= 1.0) then
		local rad_angle = Acos(dot);
		local yRot = (rad_angle  * 180.0) / PI;
		local thressholdAngle = 5.0;
		if ((yRot) > thressholdAngle or yRot < -thressholdAngle) then --math.abs()
		
			local right = self.transformComp:GetRight();
			local rightdot = right:dot(tgtFwd);
			if ( rightdot < 0.0) then
				yRot = -yRot;
			end

			if (yRot > 0.0) then
				self.transformComp:Rotate(0, mgt, 0);
			elseif (yRot < 0.0) then
				self.transformComp:Rotate(0, -mgt, 0);
			end

		end
	end
end

TestPlayerAnimComp.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnKeyPressed});
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
end


TestPlayerAnimComp.CrawlForward = function(self)
	
	if (self.transformComp ~= nil) then

		--Normalize and apply the magnitude
		local forward = self.transformComp:GetForward();
		local mgt = 2.0;
		local xzAccel = Vector2.new(forward.x, forward.z);
		xzAccel:normalize();
		self.accel = Vector3.new( mgt*xzAccel.x, self.accel.y, mgt*xzAccel.y);
	
	end
end


--SOUND EFFECTS (for animation events)
TestPlayerAnimComp.OnStep = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Step.mp3");
end

TestPlayerAnimComp.OnKickTest01 = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Collision1.mp3");
end

TestPlayerAnimComp.OnKickTest02 = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Block.mp3");
end

TestPlayerAnimComp.OnKickWhoosh = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Whoosh_01.mp3");
end

TestPlayerAnimComp.OnKickWhoosh2 = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Whoosh_02.mp3");
end

TestPlayerAnimComp.OnCrawlPlaySFX = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Crawl.mp3");
end



TestPlayerAnimComp.AnimatorSetup = function(self)
	--STATES-------------------------------------------------------------
	local idle_State =		self.animComp:CreateState("idle", "IdleAnim");
	local atk01_State =		self.animComp:CreateState("atk01", "AttackAnim");
	local atk02_State =		self.animComp:CreateState("atk02", "KickAnim");
	local atk03_State =		self.animComp:CreateState("Upper", "UpperAnim");
	local walk_State =		self.animComp:CreateState("walk", "WalkAnim", 1.0);
	local jumpLoop_State =	self.animComp:CreateState("jumpLoop", "JumpLoopAnim");
	local crawl_State =		self.animComp:CreateState("Crawl", "CrawlAnim", 0.5);


	--TRANSITIONS--------------------------------------------------------
	--Transitions from idle_state
	idle_State:SetTransition(atk01_State, 5.0, {"Punch"});
	idle_State:SetTransition(atk02_State, 5.0, {"Kick"});
	idle_State:SetTransition(atk03_State, 5.0, {"Upper"});
	idle_State:SetTransition(jumpLoop_State, 5.0, {"Jump"});
	idle_State:SetTransition(walk_State, 5.0, {"Walk"});
	idle_State:SetTransition(crawl_State, 5.0, {"Crawl"});
	
	--Transitions from atk01_state
	atk01_State:SetTransition(idle_State, 5.0, {});
	atk01_State:SetTransition(walk_State, 5.0, {"Walk"});
	atk01_State:SetTransition(atk01_State, 5.0, {"Punch"});
	atk01_State:SetTransition(atk02_State, 5.0, {"Kick"});
	atk01_State:SetTransition(atk03_State, 5.0, {"Upper"});
	atk01_State:SetTransition(jumpLoop_State, 5.0, {"Jump"});
	atk01_State:SetTransition(crawl_State, 5.0, {"Crawl"});

	--Transitions from atk02_state
	atk02_State:SetTransition(idle_State, 5.0, {});
	atk02_State:SetTransition(walk_State, 5.0, {"Walk"});
	atk02_State:SetTransition(atk01_State, 5.0, {"Punch"});
	atk02_State:SetTransition(atk02_State, 5.0, {"Kick"});
	atk02_State:SetTransition(atk03_State, 5.0, {"Upper"});
	atk02_State:SetTransition(jumpLoop_State, 5.0, {"Jump"});
	atk02_State:SetTransition(crawl_State, 5.0, {"Crawl"});
	
	--Transitions from atk03_state
	atk03_State:SetTransition(idle_State, 5.0, {});
	atk03_State:SetTransition(walk_State, 5.0, {"Walk"});
	atk03_State:SetTransition(atk03_State, 5.0, {"Upper"});
	atk03_State:SetTransition(atk01_State, 5.0, {"Punch"});
	atk03_State:SetTransition(atk02_State, 5.0, {"Kick"});
	atk03_State:SetTransition(atk03_State, 5.0, {"Upper"});
	atk03_State:SetTransition(jumpLoop_State, 5.0, {"Jump"});
	atk03_State:SetTransition(crawl_State, 5.0, {"Crawl"});

	--Transitions from Jumploop_State
	jumpLoop_State:SetTransition(idle_State, 5.0, {"Land"});
	
	--Transitions from Crawl_state
	crawl_State:SetTransition(idle_State, 5.0, {"Crawl"});

	--Transitions from walk_State	
	walk_State:SetTransition(idle_State, 5.0, {"StopWalk"});
	walk_State:SetTransition(atk01_State, 5.0, {"Punch"});
	walk_State:SetTransition(atk02_State, 5.0, {"Kick"});
	walk_State:SetTransition(atk03_State, 5.0, {"Upper"});
	walk_State:SetTransition(jumpLoop_State, 5.0, {"Jump"});


	--STARTING STATE FOR THE STAGTE MACHINE------------------------------
	self.animComp:SetEntryState(idle_State);


	--ANIMATION EVENTS---------------------------------------------------
	--Animation events for kickAnim
	self.animComp:AddAnimEvent("KickAnim", 26, {self, self.OnKickTest01});
	self.animComp:AddAnimEvent("KickAnim", 20, {self, self.OnKickWhoosh});
	self.animComp:AddAnimEvent("KickAnim", 54, {self, self.OnStep});
	
	--Animation events for kickAnim
	--self.animComp:AddAnimEvent("CrawlAnim", 1, {self, self.OnCrawlPlaySFX});
	self.animComp:AddAnimEvent("CrawlAnim", 2,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 4,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 6,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 8,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 10,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 12,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 14,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 16,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 18,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 20,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 22,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 24,  {self, self.CrawlForward});
	----
	--self.animComp:AddAnimEvent("CrawlAnim", 31, {self, self.OnCrawlPlaySFX});
	self.animComp:AddAnimEvent("CrawlAnim", 32,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 34,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 36,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 38,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 40,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 42,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 44,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 46,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 48,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 50,  {self, self.CrawlForward});
	self.animComp:AddAnimEvent("CrawlAnim", 52,  {self, self.CrawlForward});
	
	--Animation events for walk anim
	----self.animComp:AddAnimEvent("WalkAnim", 5, {self, self.OnStep});
	----self.animComp:AddAnimEvent("WalkAnim", 20, {self, self.OnStep});
	----self.animComp:AddAnimEvent("WalkAnim", 35, {self, self.OnStep});
	----self.animComp:AddAnimEvent("WalkAnim", 40, {self, self.OnStep});

	--Animation eventgs for walk anim (but different from above walk anim, this is more like a run)
	self.animComp:AddAnimEvent("WalkAnim", 8, {self, self.OnStep});
	self.animComp:AddAnimEvent("WalkAnim", 19, {self, self.OnStep});
	
	--Animation events for Upper anim
	self.animComp:AddAnimEvent("UpperAnim", 20, {self, self.OnKickWhoosh});
	self.animComp:AddAnimEvent("UpperAnim", 40, {self, self.OnKickWhoosh2});
	self.animComp:AddAnimEvent("UpperAnim", 50, {self, self.OnKickWhoosh});
	self.animComp:AddAnimEvent("UpperAnim", 54, {self, self.OnKickWhoosh2});
	self.animComp:AddAnimEvent("UpperAnim", 64, {self, self.OnKickWhoosh2});
	self.animComp:AddAnimEvent("UpperAnim", 74, {self, self.OnKickWhoosh});
end



return TestPlayerAnimComp;
