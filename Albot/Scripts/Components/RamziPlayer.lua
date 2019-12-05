-- First approximation of a component script

 -- THIS IS THE ORIGINAL POSITION FOR THE PLAYER
 --"TransformComponent": {
 --    "Position": [
 --       0.0,
 --       0.5,
 --       48.0
 --    ],
 --    "Rotation": [
 --        0.0,
 --        0.0,
 --        0.0
 --    ],

RamziPlayer = 
{
	name = "RamziPlayer";

	-- Camera
	camera = nil;
	
	--Custom stuff
	animComp = nil;
	transformComp = nil;
	cubeTransformComp = nil;
	cubeOffset = Vector3.new(0.0, 1.0, 0.0);
	originalPosition = Vector3.new(0.0);
	rigidbodyComp = nil;
	
	walking = false;
	jumping = false;
	landing = false;
	falling = false;
	crouching = false;
	nearCrouch = false;
	dead = false;

	jumpDebounceTimer = -1.0;
	jumpDebounceTime  = 0.2;
	punching = false;
	punchWaitTimer = -1.0;
	punchWaitTime  = 1.0;
	isCrawling = false;	

	-- Movement
	analog = Vector3.new(0.0,0.0,0.0);
	prevVelY = 0;
	currVelY = 0;
	movespeed = 4;
	jumpSpeed = 6;
}


--Init called when comp is created
RamziPlayer.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKeyPressed});
	OnJoystickButton():Bind({self, self.OnJoystickButton});
	OnJoystickMotion():Bind({self, self.OnJoystickMotion});
end


--Begin called when obj has all comps
RamziPlayer.Begin = function(self, owner, goMgr)

	-- Cache the components
	self.animComp = owner:GetAnimationComp();
	self.transformComp = owner:GetTransformComp();
	self.rigidbodyComp = goMgr:FindGameObject("PlayerCube"):GetRigidbodyComp();
	self.cubeTransformComp = goMgr:FindGameObject("PlayerCube"):GetTransformComp();
	self.camera = goMgr:FindGameObject("lvleditor");
	
	self.originalPosition = Vector3.new(self.transformComp:GetPosition() + self.cubeOffset);
	self.cubeTransformComp:SetLocalPosition(self.originalPosition);
	self.cubeTransformComp:Scale(0.7, 2.0, 0.7);

	--Setup of the state machine
	self:AnimatorSetup();
	EventManager.Get():PlaySong(false, "Assets\\Songs\\Techno_3.mp3");
end

RamziPlayer.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnKeyPressed});
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
	OnJoystickMotion():Unbind({self, self.OnJoystickMotion});
end

--Method
RamziPlayer.OnKeyPressed = function(self, key, state)
	--Up-Down Movement
	if(SCANCODE.W == key) then
		if(state) then self.analog.z = -1.0
		else self.analog.z = 0.0 end
	end
	if(SCANCODE.S == key) then
		if(state) then self.analog.z = 1.0
		else self.analog.z = 0.0 end
	end
	if(SCANCODE.D == key) then
		if(state) then self.analog.x = 1.0
		else self.analog.x = 0.0 end
	end
	if(SCANCODE.A == key) then
		if(state) then self.analog.x = -1.0
		else self.analog.x = 0.0 end
	end

	--if(SCANCODE.E == key and state) then
	--	self.animComp:SetTrigger("Punch");
	--end
	--
	--if(SCANCODE.Q == key and state) then
	--	self.animComp:SetTrigger("Kick");
	--end
	--	
	--if(SCANCODE.R == key and state) then
	--	self.animComp:SetTrigger("Upper");
	--end
	--	
	--if(SCANCODE.ENTER == key and state) then
	--	self.animComp:SetTrigger("Crawl");
	--end
	--
	--if(SCANCODE.SPACE == key and state) then
	--	if (not self.jumping) then
	--		EventManager:Get():PlaySFX(false, "Assets\\SFX\\Jump.mp3");
	--		self.jumping = true;
	--		self.walking = false;
	--		local vel = self.rigidbodyComp:GetVelocity();
	--		vel.y = self.jumpSpeed;
	--		self.rigidbodyComp:SetVelocity(vel);
	--		self.animComp:SetTrigger("Jump");
	--	end
	--end
end

RamziPlayer.HandleCrouchTransition = function(self)
	if(self.crouching) then
		self.cubeTransformComp:Scale(0.7, 1.0, 2.0);
		self.cubeTransformComp:Translate(0.0, -0.5, 0.0);
		self.cubeOffset.y = 0.5;
		self.movespeed = self.movespeed / 4;
		self.animComp:SetTrigger("Crawl");
	else
		self.cubeTransformComp:Scale(0.7, 2.0, 0.7);
		self.cubeTransformComp:Translate(0.0, 0.5, 0.0);
		self.cubeOffset.y = 1;
		self.movespeed = self.movespeed * 4;
		self.animComp:SetTrigger("Crawl");
	end
end

RamziPlayer.OnJoystickButton = function(self, ID, key, state)
	if(CONTROLLER.A == key and state) then
		if (not self.falling and not self.landing and not self.crouching) then
			self.jumping = true;
			self.walking = false;
			self.animComp:SetTrigger("Jump");
			EventManager:Get():PlaySFX(false, "Assets\\SFX\\Jump.mp3");
			local vel = self.rigidbodyComp:GetVelocity();
			vel.y = self.jumpSpeed;
			self.cubeTransformComp:Translate(0.0, 0.01, 0.0);
			self.rigidbodyComp:SetVelocity(vel);
		end
	elseif(CONTROLLER.B == key and state and not self.falling and not self.landing and not self.walking) then
		if(self.nearCrouch and self.crouching) then return end;
		self.crouching = not self.crouching;
		self.HandleCrouchTransition(self);
	elseif(CONTROLLER.X == key and state and not self.falling and not self.landing and not self.walking and not self.crouching) then
		self.animComp:SetTrigger("Punch");
		--self.punching = true;
		--self.punchWaitTimer = self.punchWaitTime;
	elseif(CONTROLLER.Select == key and state) then 
		 EventManager.Get():LoadState(false, "Assets\\Levels\\Menu.json");
	elseif(CONTROLLER.Start == key and state) then
		self:HandleDeath(self);
	end
end

RamziPlayer.OnJoystickMotion = function(self, ID, axis, value)
	if(value < 0.2 and value > -0.2) then -- Debouncing small values
		 value = 0.0;
	else
	end
	if(axis == 0) then 
		self.analog.x = value;
	elseif(axis == 1) then 
		self.analog.z = value;
	else return
	end
end

RamziPlayer.HandleDeath = function(self)
	local script = self.camera:GetCustomComp("TppFollowController");
	--self.rigidbodyComp:SetVelocity(0.0, 0.0, 0.0);
	self.cubeTransformComp:SetLocalPosition(self.originalPosition);
	script.MinCoordinates = self.originalPosition + Vector3.new(-2, 0, -2);
	script.MaxCoordinates = self.originalPosition + Vector3.new(2, 1, 0);
	script.zoom = 0;
	script.Transform:SetLocalPosition(self.originalPosition.x, self.originalPosition.y + 4, self.originalPosition.z + 8);
	self.dead = false;
end


--Update called every tick
RamziPlayer.Update = function(self, dt, owner) 
	
	if (self.animComp == nil) then return end
	if (self.transformComp == nil) then return end
	if (self.rigidbodyComp == nil) then return end

	-- Update cube position if you die
	local position = self.transformComp:GetPosition();
	if(position.y < -4.0 or self.dead) then
		self:HandleDeath(self);
	end

	-- Update to Box Position
	local cubePosition = Vector3.new(self.cubeTransformComp:GetPosition());
	self.transformComp:SetLocalPosition(cubePosition - self.cubeOffset);
	
	--if(self.nearCrouch) then LOG("NEARCROUCH\n") end

	--GRAVITY (DOING THIS FOR GROUND COLLISION)
	local vel = self.rigidbodyComp:GetVelocity();
	local horizontalSpeed = (vel.x * vel.x) + (vel.z * vel.z);
	local verticalSpeed = Abs(vel.y);
	self.prevVelY = self.currVelY;
	self.currVelY = vel.y;
	local deltaVel = self.currVelY - self.prevVelY;
	
	if (self.landing) then 
		self.jumpDebounceTimer = self.jumpDebounceTimer - dt;
		vel.y = 0.0;
		self.rigidbodyComp:SetVelocity(vel);
		if(self.jumpDebounceTimer < 0.0) then
			self.landing = false;
		end
		return
		--LOG("Landing Timer: " .. self.jumpDebounceTimer .. "\n");
	end

	--Handle x-z displacement and rotation
	self:HandleMovement(self);
	local len2 = self.analog:len2();
	if(len2 > 0.01) then
		self:UpdateRotation();
	end
	
	-- Animation States
	if (vel.y < -5.0 and not self.falling and not self.landing) then
		self.falling = true;
		self.walking = false;
		self.animComp:SetTrigger("Jump");
		self.jumpDebounceTimer = self.jumpDebounceTime;
		--LOG("JUMP: " .. verticalSpeed .. "\n");
	elseif(self.jumping) then 
		self.jumping = false;
		self.falling = true;
	elseif (deltaVel > 0.0 and self.falling and not self.landing) then 
		self.walking = false;
		self.falling = false;
		self.jumpDebounceTimer = self.jumpDebounceTime;
		self.animComp:SetTrigger("Land");
		self.landing = true;
		EventManager:Get():PlaySFX(false, "Assets\\SFX\\Collision1.mp3");
		--LOG("Land: " .. deltaVel .. "\n");
	end

	local len2 = self.analog:len2();
	if(len2 < 0.01 and self.walking) then 
		self.walking = false; 
		self.animComp:SetTrigger("StopWalk");
		--LOG("StopWalk\n");
	elseif(len2 > 0.01 and not self.walking and not self.falling) then
		self.walking = true;
		self.animComp:SetTrigger("Walk");
		--LOG("Walk\n");
	end


end

RamziPlayer.HandleMovement = function(self)
	if(self.falling) then return end
	local mgt = self.movespeed;
	local currentVelocity = self.analog * mgt;
	currentVelocity.y = self.rigidbodyComp:GetVelocity().y;
	self.rigidbodyComp:SetVelocity(currentVelocity);
end

RamziPlayer.UpdateRotation = function(self)
	--Get current forward and target forward
	if(self.falling) then return end

	local currFwd = self.transformComp:GetForward();
	local tgtFwd = self.analog;
	currFwd:normalize();
	tgtFwd:normalize();

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
				self.transformComp:Rotate(0, 10.0, 0);
				self.cubeTransformComp:Rotate(0, 10.0, 0);
			elseif (yRot < 0.0) then
				self.transformComp:Rotate(0, -10.0, 0);
				self.cubeTransformComp:Rotate(0, -10.0, 0);
			end
		end
	end
end

--SOUND EFFECTS (for animation events)
RamziPlayer.OnStep = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Step.mp3");
end

RamziPlayer.OnKickTest01 = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Collision1.mp3");
end

RamziPlayer.OnKickTest02 = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Block.mp3");
end

RamziPlayer.OnKickWhoosh = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Whoosh_01.mp3");
end

RamziPlayer.OnKickWhoosh2 = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Whoosh_02.mp3");
end

RamziPlayer.AnimatorSetup = function(self)
	--STATES-------------------------------------------------------------
	local idle_State =		self.animComp:CreateState("idle", "IdleAnim");
	local atk01_State =		self.animComp:CreateState("atk01", "AttackAnim");
	local atk02_State =		self.animComp:CreateState("atk02", "KickAnim");
	local atk03_State =		self.animComp:CreateState("Upper", "UpperAnim");
	local walk_State =		self.animComp:CreateState("walk", "WalkAnim", 1.0);
	local jumpLoop_State =	self.animComp:CreateState("jumpLoop", "JumpLoopAnim");
	local crawl_State =		self.animComp:CreateState("Crawl", "CrawlAnim");


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

return RamziPlayer;
