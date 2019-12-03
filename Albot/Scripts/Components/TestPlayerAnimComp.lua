-- First approximation of a component script

TestPlayerAnimComp = 
{
	name = "TestPlayerAnimComp";
	
	--Custom stuff
	ownerGO = nil;
	animComp = nil;
	transformComp = nil;
	rigidbodyComp = nil;
	triggerComp = nil;

	--Stuff to avoid the game in the first couple of frames
	frameNum = 0;
	BeginScroll = false;

	--For now, special flag. Not to use for anything other than gravity
	grounded = false;
	grounded_prevFrame = false;

	--Joystick crap 
	axisMovement = Vector2.new(0.0);
	usingJoystick = false;

	--Other normal flags
	death = false;
	inDash = false;
	walking = false;
	flying = false;
	jumping = false;
	falling = false;
	landing = false;
	knockback = false;
	isCrawling = false;
	readyForFlight = false;
	triggerDash = false;

	--HP data
	maxHP = 0.0;
	currentHP = 0.0;
	dead = false;

	--Gas data
	gasBar = nil;
	maxGas = 0.0;
	currentGas = 0.0;
	burnSpeed = 0.0;
	recoverySpeed = 0.0;
	spentThisFrame = 0.0;

	--Time stuff
	landingTime = 0.0;
	knockbackTime = 1.0;
	knockbackElapsed = 0.0;
	originalRot = Vector2.new(0.0);
	jumpTime = 0.0;
	thressholdBeforeActivatingFlight = 0.0;
	yFloor = 0.0;

	--Physics stuff
	accel = Vector3.new(0,0,0);
	velocity = Vector3.new(0,0,0);
	targetFwd = Vector3.new(0,0,0);
	
	--Multicasts for communication between scripts
	OnTogglingScrolling = multicast();

	propellerForce = Vector3.new(0.0, 0.0, 0.0);
	fakeMass = nil;
	
	rightPressed = false;
	leftPressed = false;
	upPressed = false;
	downPressed = false;
	spacePressed = false;
}


--Method
TestPlayerAnimComp.OnKeyPressed = function(self, key, state)
	
	--If being knowckedback, dont listen to input
	if (self.knockback or self.death) then 
		return 
	end
		
	if(SCANCODE.V == key and state) then
		if (self.flying or self.falling or self.jumping) then
			if (self.currentGas >= (0.3 * self.maxGas)) then
				self.currentGas = self.currentGas - (0.3 * self.maxGas);
				self.animComp:SetTrigger("Dash");
				self.falling = false;
				self.inDash = true;
				--self.triggerDash = true;
			end
		end
	end

	if(SCANCODE.SPACE == key) then
		self.spacePressed = state;
		OutputPrint("Space pressed\n");
		if (not state and self.flying and not self.inDash) then
				self.flying = false;
				self.falling = true;
				self.readyForFlight = false;
				OutputPrint("FALLING ACTIVATED\n");
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


--Method
TestPlayerAnimComp.OnJoystickButton = function(self, joystickId, button, state)
	
	--Just so flag gets to true and then remains
	if (self.usingJoystick == false) then 
		self.usingJoystick = true;
	end
	
	--If being knowckedback, dont listen to input
	if (self.knockback or self.death) then 
		return 
	end

	if(button == CONTROLLER.Select and state) then	
		local EventMgr = EventManager.Get();
		EventMgr:LoadState(false, "Assets\\Levels\\Menu.json");
	end
	
	if(button == CONTROLLER.X and state) then
		if (self.flying or self.falling or self.jumping) then
			if (self.currentGas >= (0.3 * self.maxGas)) then
				self.currentGas = self.currentGas - (0.3 * self.maxGas);
				OutputPrint("ACTIVATING DASH!!! - GRAVITY TURNED OFF\n");
				self.animComp:SetTrigger("Dash");
				self.inDash = true;
				self.falling = false;
				--self.triggerDash = true;
			end	
		end
	end
	
	
	if(button == CONTROLLER.Y and state) then
		self.OnTogglingScrolling();
	end
	
	if(button == CONTROLLER.A) then
		self.spacePressed = state;
		OutputPrint("A button pressed\n");
		if (not state and self.flying and not self.inDash) then
			self.flying = false;
			self.falling = true;
			self.readyForFlight = false;
			OutputPrint("FALLING ACTIVATED\n");
		end
	end
end

--Method
TestPlayerAnimComp.OnJoystickMotion = function(self, joystickId, axis, value)
	
	if (self.usingJoystick == false) then 
		self.usingJoystick = true;
	end

	--Thresshold deadzone
	if(value < 0.2 and value > -0.2) then
		value = 0.0;
	end;

	--x axis
	if(axis == 0) then
		self.axisMovement.x = value;
	end
	
	--y axis
	if(axis == 1) then
		self.axisMovement.y = value;
	end
end


--Init called when comp is created
TestPlayerAnimComp.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKeyPressed});
	OnJoystickButton():Bind({self, self.OnJoystickButton});
	OnJoystickMotion():Bind({self, self.OnJoystickMotion});
end


--Begin called when obj has all comps
TestPlayerAnimComp.Begin = function(self, owner)

	-- Cache the components
	self.animComp = owner:GetAnimationComp();
	self.transformComp = owner:GetTransformComp();
	self.rigidbodyComp = owner:GetRigidbodyComp();
	self.triggerComp = owner:GetTriggerComp();

	--Cache owner
	self.ownerGO = owner;

	--Multicast for onCollision
	self.rigidbodyComp.OnCollision:Bind({self, self.OnCollisionGround});

	--Set physical quantities
	self.fakeMass = 30.0;
	self.thressholdBeforeActivatingFlight = 0.3;

	--Set the forward Vec initial val
	self.targetFwd = self.transformComp:GetForward();

	--Setup of the state machine
	self:AnimatorSetup();

	--Starting values for runner level
	self.falling = true;
	self.death = false;

	--Hp settings
	self.maxHP = 3.0;
	self.currentHP = self.maxHP;

	--TEMPORARY - gas bar settings
	local gasbar = owner:Manager():FindGameObject("GasBar");
	if (gasbar ~= nil) then 
		self.gasBar = gasbar;
	end
	self.maxGas = 100.0;
	if (self.currentGas == nil) then OutputPrint("WTF-0\n"); 
	else OutputPrint("1>> " .. self.currentGas .. "\n"); end
	self.currentGas = 100.0;
	if (self.currentGas == nil) then OutputPrint("WTF-1\n"); 
	else OutputPrint("2>> " .. self.currentGas .. "\n"); end
	self.burnSpeed = 10.0;
	self.recoverySpeed = 5.0;

end


--Update called every tick
TestPlayerAnimComp.Update = function(self, dt, owner) 
	
	if (self.death) then
		return; 
	end

	if (self.frameNum < 5) then 
		self.frameNum = self.frameNum + 1;
		return;
	end

	--knockback lag
	if (self.knockback) then
		
		self.knockbackElapsed = self.knockbackElapsed - dt;
		self.transformComp:Rotate(30.0 * dt, 0.0, 0.0);
		
		if (self.knockbackElapsed < 0.0) then
			self.knockback = false;
			self.OnTogglingScrolling();
			self.falling = true;
			
			self.transformComp:SetLocalRotation(self.originalRot.x, 0.0, self.originalRot.z);

			OutputPrint("KNOCKBACK END!\n");
			--Weird bug, but this flag setting fixes it
			self.spacePressed = false;
			--if (self.readyForFlight) then OutputPrint(">>asd01\n"); end
			--if (self.spacePressed) then OutputPrint(">>asd02\n"); end
		end
	end
	--if (self.spacePressed) then OutputPrint("asd02\n"); end

	--If space is pressed
	if (self.spacePressed and not self.flying and not self.falling and not self.jumping and not self.knockback) then
		self:BeginJump();
		self.thressholdBeforeActivatingFlight = 0.4;
	elseif (self.currentGas >= (0.01*self.maxGas) and self.spacePressed and self.readyForFlight and 
			(self.flying or self.falling or self.jumping) and not self.knockback ) then
		self.flying = true;
		self.falling = false;
		self.jumping = false;
		self.propellerForce = 300.0;
		self.accel.y = self.propellerForce / self.fakeMass;
		--OutputPrint("FLYING!!\n");
	elseif(self.currentGas < (0.01*self.maxGas)) then
		self.flying = false;
		self.falling = true;
		self.jumping = false;
		self.accel.y = self.accel.y + self.fakeMass * 0.25;
	end

	--Gas spending
	if (self.flying) then
		self.currentGas = self.currentGas - self.burnSpeed * dt;
		if (self.currentGas < 0.0) then 
			self.currentGas = 0.0;
		end
	elseif (not self.jumping and not self.falling) then 
		self.currentGas = self.currentGas + self.recoverySpeed * dt;
		if (self.currentGas > self.maxGas) then 
			self.currentGas = self.maxGas;
		end
	end
	--GAS UI
	if (self.gasBar ~= nil) then 
		local gasPercent = self.currentGas / self.maxGas;
		local t = self.gasBar:GetTransformComp();
		t:Scale(gasPercent*20.0, 2.0, 1.0);
		--t:Translate(self.spentThisFrame, 0, 0);
	end

	--If spacePressed, check for when the timer goes to zero
	if (self.spacePressed and not self.knockback) then 
		self.thressholdBeforeActivatingFlight = self.thressholdBeforeActivatingFlight - dt;
		if (self.thressholdBeforeActivatingFlight < 0.0) then
			self.readyForFlight = true;
		end
	end

	-- Should always activate this when not in ground	
	if (self.falling) then
		self.accel.y = self.accel.y - self.fakeMass * 0.85;
		if (self.accel.y < -self.fakeMass * 0.85) then 
			self.accel.y = -self.fakeMass * 0.85;
		end
		--OutputPrint("GRAVITY ACTIVE\n");
	end

	--Landing lag -- BOUNCING LAG NOW
	if (self.landing) then
		self.landingTime = self.landingTime - dt;
		if (true) then --self.landingTime < 0.0) then
			--Experiment
			local currentVel = self.rigidbodyComp:GetVelocity();
			currentVel.y = 0.0;
			self.rigidbodyComp:SetVelocity(currentVel)			
			self.landingTime = 0.0;
			self.landing = false;
			self.animComp:SetTrigger("Land");
		end
	end

	--jumping time
	if (self.jumping and not self.inDash) then
		self.jumpTime = self.jumpTime - dt;
		if (self.jumpTime < 0.0) then		
			self.jumpTime = 0.0;
			self.jumping = false;
			self.falling = true;
			OutputPrint("After jump time, activate falling!\n");
		end
	end

	--hack for always having vel zero when  grounded
	if (not self.falling and not self.flying and not self.jumping) then 
		--Experiment
		local currentVel = self.rigidbodyComp:GetVelocity();
		currentVel.y = 0.0;
		currentVel.x = 0.0;--------------------------------------------******************
		currentVel.z = 0.0;--------------------------------------------******************
		--OutputPrint("Setting velocity to zero!!\n");
		self.rigidbodyComp:SetVelocity(currentVel)			
	end


	--Handle x-z displacement
	if (not self.isCrawling) then
		self:HandleMovement(self.landing, self.isflying);
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
	--Apply constraints (in x-y-z)
	local position = self.transformComp:GetPosition();
	local xThresshold = 30;
	local zThresshold = 40;
	if(position.x < -xThresshold) then
		position.x = -xThresshold;
	elseif (position.x > xThresshold) then
		position.x = xThresshold;
	end
	if(position.z < -(zThresshold + 180)) then
		position.z = -(zThresshold + 180);
	elseif (position.z > zThresshold) then
		position.z = zThresshold;
	end
	self.transformComp:SetLocalPosition(position);
	

	--Print current acceleration
	--OutputPrint("VEL: <" .. self.velocity.x .. ", " .. self.velocity.y .. ", " .. self.velocity.z .. "> \n");


	--WALKING Animation control
	local xz_vel = Vector2.new(self.velocity.x, self.velocity.z);
	local speed = xz_vel:len();
	if (speed > 0.2 and not self.walking and not self.flying and not self.falling and not self.jumping) then
		--For when using joystick, only pay attention to speed if the joytick axis are actually being used
		if (self.usingJoystick) then
			if (self.axisMovement.x ~= 0 or self.axisMovement.y ~= 0) then 
				self.walking = true;
				self.animComp:SetTrigger("Walk");
			end
		--For the non joystick, I dont really care
		else
			self.walking = true;
			self.animComp:SetTrigger("Walk");
		end
	elseif (speed < 0.1 and self.walking) then 
		self.walking = false;
		self.animComp:SetTrigger("StopWalk");
	end
end


TestPlayerAnimComp.BeginJump = function(self)
	EventManager:Get():PlaySFX(false, "Assets\\SFX\\Jump.mp3");
	self.falling = false;
	self.walking = false;
	self.yFloor = self.transformComp:GetPosition().y;
	self.propellerForce = 1000.0;
	self.accel.y = self.propellerForce / self.fakeMass;
	self.animComp:SetTrigger("Jump");

	self.jumping = true;
	self.jumpTime = 0.5;
end


--This is only for ground objs
--As you spawn them, bind to their triggers
--When they are destroyed, no need to unbind really
TestPlayerAnimComp.OnLeaveFloor = function(self, go01, go02)
	OutputPrint("EXIT FLOOR!! *******************************\n");
	if (not self.knockback and not self.jumping and not self.flying and not self.falling) then
		self.falling = true;
		-- TODO - add animation switch
		self.animComp:SetTrigger("Jump");
	end
end

TestPlayerAnimComp.OnEnterFloor = function(self, go01, go02)
		OutputPrint("ENTER FLOOR!! *******************************\n");
end



TestPlayerAnimComp.OnEnterDeadzone = function(self, go01, go02)
		OutputPrint("ENTER Deadzone!! *******************************\n");
		self.falling = false;
		self.death = true;
		-- TODO - add animation switch
		self.animComp:SetTrigger("Gameover");
		self.OnTogglingScrolling();
		
		self.accel.x = 0.0;
		self.accel.y = 0.0;
		self.accel.z = 0.0;

		--Make camera start zoom in
		--local camController = self.ownerGO:GetCustomComp("JoseController");
		--camController:ActivateSlowZoom(self.transformComp:GetPosition());
end


TestPlayerAnimComp.ApplyKnockback = function(self, go01, go02)
	
	OutputPrint("KNOCKBACK********************\n");
	self.OnTogglingScrolling();
	self.knockbackElapsed = self.knockbackTime;

	--Reset movement stuff so it doesnt move
	self.knockback = true;
	self.originalRot = Vector3.new(self.transformComp:GetRotation());

	self.jumping = false;
	self.falling = false;
	self.flying = false;
	self.readyForFlight = false;

	--And reset the acceleration values so it only focuses on the new accel
	self.accel.x = 0.0;
	self.accel.y = 0.0;
	self.accel.z = 0.0;

	self:TakeDamage(1.0);

	local xz_vel = Vector2.new(self.velocity.x, self.velocity.z);
	local speed = xz_vel:len();
	local mgt = 100.0 + 200.0 * speed;

	local xzFwd = Vector2.new(0.0, 1.0);
	self.accel = self.accel + Vector3.new(mgt*xzFwd.x, -1.0, mgt*xzFwd.y);
end


TestPlayerAnimComp.ApplyKnockback_2 = function(self, go01, go02)
	
	--- OutputPrint("KNOCKBACK2********************\n");
	self.OnTogglingScrolling();
	self.knockbackElapsed = self.knockbackTime;
	--- 
	--- --Reset movement stuff so it doesnt move
	self.knockback = true;
	self.originalRot = Vector3.new(self.transformComp:GetRotation());

	self.jumping = false;
	self.falling = false;
	self.flying = false;
	self.readyForFlight = false;

	self:TakeDamage(1.0);

	--And reset the acceleration values so it only focuses on the new accel
	self.accel.x = 0.0;
	self.accel.y = 0.0;
	self.accel.z = 0.0;

	local xz_vel = Vector2.new(self.velocity.x, self.velocity.z);
	local speed = xz_vel:len();
	local mgt = 100.0 + 200.0 * speed;

	local fwd = self.transformComp:GetForward();
	local xz_fwd = Vector2.new(-fwd.x, -fwd.z);
	self.accel = self.accel + Vector3.new(mgt*xz_fwd.x, 0.0, mgt*xz_fwd.y);
end


TestPlayerAnimComp.OnGasTank = function(self, go1, go2)
	self.currentGas = self.currentGas + 20;

	local t = go1:GetTransformComp();
	t:Translate(-100, -100, 0);
end


TestPlayerAnimComp.DashForward02 = function(self)
	local elevation = 5.0;
	local currFwd = self.transformComp:GetForward();
	local mgt = 22.0;
	local xzFwd = Vector2.new(currFwd.x, currFwd.z);
	xzFwd:normalize();
	self.accel = self.accel + Vector3.new(mgt*xzFwd.x, elevation, mgt*xzFwd.y);
	OutputPrint("DASHING\n");
end


TestPlayerAnimComp.DashForward03 = function(self)
	local elevation = 5.0;
	local currFwd = self.transformComp:GetForward();
	local mgt = 10.0;
	local xzFwd = Vector2.new(currFwd.x, currFwd.z);
	xzFwd:normalize();
	self.accel = self.accel + Vector3.new(mgt*xzFwd.x, elevation, mgt*xzFwd.y);
	OutputPrint("DASHING\n");
end


TestPlayerAnimComp.EndDash = function(self) 
	if (not self.falling) then
		OutputPrint("END DASH!!!!!!!!!!!!!!!!!!!!!\n")
		self.falling = true;
		self.inDash = false;
	else
		OutputPrint("END DASH, BUT WAS FALLING ALREADY!!!!!!!!!!!!!!!!!!!!!\n")
	end
end



TestPlayerAnimComp.HandleMovement = function(self, isLanding, isflying)
    
	if (self.knockback) then 
		return; 
	end

	local refAccel = Vector3.new(0);

	--Acceleration when pressed UP OR DOWN
	if (self.upPressed) then 
		refAccel.z = -1;
		self.targetFwd.z = -1.0;
	elseif (self.downPressed) then 
		refAccel.z = 1;
		self.targetFwd.z = 1.0;
	else
		refAccel.z = 0;
		self.targetFwd.z = 0;
	end
	
	--Acceleration when pressed RIGHT OR LEFT
	if (self.leftPressed) then 
		refAccel.x = -1;
		self.targetFwd.x = -1.0;
	elseif (self.rightPressed) then 
		refAccel.x = 1;
		self.targetFwd.x = 1.0;
	else 
		refAccel.x = 0;
		self.targetFwd.x = 0; 
	end
	--]]
	
	if (self.usingJoystick) then
		refAccel = Vector3.new(self.axisMovement.x, 0.0, self.axisMovement.y);
		self.targetFwd = Vector3.new(self.axisMovement.x, 0.0, self.axisMovement.y);
	end

	--Normalize and apply the magnitude
	local mgt = 3.4;
	if (isLanding or isflying) then
		mgt = 0.5 * mgt;
	end
	local xzAccel = Vector2.new(refAccel.x, refAccel.z);
	xzAccel:normalize();
	self.accel = self.accel + Vector3.new(mgt*xzAccel.x, 0.0, mgt*xzAccel.y);
end



TestPlayerAnimComp.OnCollisionGround = function(self, go01, go02)
	
	local rgb2 = go02:GetRigidbodyComp();
	
	--When colliding against ground, stop falling
	if (self.falling and rgb2:GetCollisionMask() == CollisionMask.STATIC_OBJ) then
	
		OutputPrint("COLLISION WITH GROUND!!! -------------------------*********\n");

		--This only will happen at the start of the level
		if (self.BeginScroll == false) then
			OutputPrint("BeginScroll!!!\n");
			self.BeginScroll = true;
			--FIRE TOGGLE SCROLL MULTICAST
			self.OnTogglingScrolling();
			OutputPrint("OnTogglingScrolling MULTICAST FIRED!!! -------------------------*********\n");
		end

		--Dont keep falling and set flags to false
		self.accel.y = 0;
		self.falling = false;
		self.flying = false; --ERASE

		--Set landing time. It wont move until stops landing
		self.landingTime = 0.20; --Half a second
		self.landing = true;

		--Also stop moving in the xz dir until landing ends
		self.walking = false;
		self.accel.x = 0.0;
		self.accel.z = 0.0;

		EventManager:Get():PlaySFX(false, "Assets\\SFX\\Step.mp3");
	end

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
	OnJoystickMotion():Unbind({self, self.OnJoystickMotion});

	--Multicast for onCollision
	self.rigidbodyComp.OnCollision:Unbind({self, self.OnCollisionGround});
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
	local walk_State =		self.animComp:CreateState("walk", "WalkAnim", 1.0);
	local jumpLoop_State =	self.animComp:CreateState("jumpLoop", "JumpLoopAnim");
	local dash_state =		self.animComp:CreateState("dash", "DashAnim", 2.5);
	local crawl_State =		self.animComp:CreateState("Crawl", "CrawlAnim", 0.5);
	local atk03_State =		self.animComp:CreateState("Upper", "UpperAnim");
	local knock_state =		self.animComp:CreateState("knockback", "KnockbackAnim", 4.0);
	local death_state =		self.animComp:CreateState("death", "deathAnim", 1.0);


	--TRANSITIONS--------------------------------------------------------
	--Transitions from idle_state
	idle_State:SetTransition(atk03_State, 5.0, {"Upper"});
	idle_State:SetTransition(jumpLoop_State, 5.0, {"Jump"});
	idle_State:SetTransition(walk_State, 5.0, {"Walk"});
	idle_State:SetTransition(crawl_State, 5.0, {"Crawl"});
	idle_State:SetTransition(idle_State, 5.0, {"Land"});
	idle_State:SetTransition(idle_State, 5.0, {"StopWalk"});-------////////********
	idle_State:SetTransition(death_state, 5.0, {"Gameover"});-------////////********
	
	--Transitions from dash_state
	dash_state:SetTransition(jumpLoop_State, 2.5, {});

	--Transitions from knock_state
	knock_state:SetTransition(jumpLoop_State, 2.5, {});
	
	--Transitions from atk03_state
	atk03_State:SetTransition(idle_State, 5.0, {});
	atk03_State:SetTransition(walk_State, 5.0, {"Walk"});
	atk03_State:SetTransition(atk03_State, 5.0, {"Upper"});
	atk03_State:SetTransition(atk03_State, 5.0, {"Upper"});
	atk03_State:SetTransition(jumpLoop_State, 5.0, {"Jump"});
	atk03_State:SetTransition(crawl_State, 5.0, {"Crawl"});

	--Transitions from Jumploop_State
	jumpLoop_State:SetTransition(idle_State, 2.5, {"Land"});
	jumpLoop_State:SetTransition(dash_state, 2.5, {"Dash"});
	jumpLoop_State:SetTransition(knock_state, 2.5, {"Knockback"});
	jumpLoop_State:SetTransition(death_state, 5.0, {"Gameover"});-------////////********

	--Transitions from walk_State	
	walk_State:SetTransition(idle_State, 5.0, {"StopWalk"});
	walk_State:SetTransition(atk03_State, 5.0, {"Upper"});
	walk_State:SetTransition(jumpLoop_State, 5.0, {"Jump"});
	walk_State:SetTransition(idle_State, 5.0, {"Land"});------------//****
	walk_State:SetTransition(walk_State, 5.0, {"Walk"});-------////////********
	walk_State:SetTransition(death_state, 5.0, {"Gameover"});-------////////********



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

	--Dash Animation
	self.animComp:AddAnimEvent("DashAnim", 1,  {self, self.DashForward02});
	self.animComp:AddAnimEvent("DashAnim", 3,  {self, self.DashForward02});
	self.animComp:AddAnimEvent("DashAnim", 5,  {self, self.DashForward02});
	self.animComp:AddAnimEvent("DashAnim", 7,  {self, self.DashForward02});
	self.animComp:AddAnimEvent("DashAnim", 9,  {self, self.DashForward02});
	self.animComp:AddAnimEvent("DashAnim", 11,  {self, self.DashForward02});
	self.animComp:AddAnimEvent("DashAnim", 13,  {self, self.DashForward02});
	self.animComp:AddAnimEvent("DashAnim", 15,  {self, self.DashForward02});
	self.animComp:AddAnimEvent("DashAnim", 17,  {self, self.DashForward03});
	self.animComp:AddAnimEvent("DashAnim", 19,  {self, self.DashForward03});
	self.animComp:AddAnimEvent("DashAnim", 21,  {self, self.DashForward03});
	self.animComp:AddAnimEvent("DashAnim", 23,  {self, self.DashForward03});
	self.animComp:AddAnimEvent("DashAnim", 25,  {self, self.DashForward03});
	self.animComp:AddAnimEvent("DashAnim", 27,  {self, self.DashForward03});
	--Experiment-------------------------------------------------------
	self.animComp:AddAnimEndEvent("DashAnim", {self, self.EndDash});

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

	--Anim event for Gameover
	self.animComp:AddAnimEndEvent("deathAnim", {self, self.OnDeathAnimEnd});
end


TestPlayerAnimComp.OnDeathAnimEnd = function(self) 
	local EventMgr = EventManager.Get();
	EventMgr:LoadState(false, "Assets\\Levels\\Menu.json");
end


TestPlayerAnimComp.TakeDamage = function(self, amount) 
	
	--Update val
	self.currentHP = self.currentHP - amount;
	if (self.currentHP <= 0) then
		self.death = true;
		self.animComp:SetTrigger("Gameover");

		self.falling = true;-------------------***
		self.OnTogglingScrolling();------------***
	end

	--TODO Update visual for Hp
	-- (...)
end


return TestPlayerAnimComp;