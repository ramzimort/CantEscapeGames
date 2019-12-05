
-- First approximation of a component script

shooter_fpscontroller = 
{
	-- Rotation (Mouse/Joystick)
	MousePositionX = 0;
	MousePositionY = 0;
	DeltaPositionX = 0;
	DeltaPositionY = 0;
	Rotation = Vector3.new(0.0);
	RotationMultiplier = 10.0;
	RotationSpeed = 20.0;
	LEFTCLICK = false;
	RIGHTCLICK = false;
	RotationEnabled = true;
	
	triggerComp = nil;

	movement_amount = Vector2.new(0.0);
	MoveSpeed = 10.0;
	Transform = nil;
	Camera = nil;
	runGame = -1;
	ownerGameObj = nil;
	life = 15;
	maxLife = 15;

	isPlaySound = false;
	healthSliderUIGameObj = nil;
	healthSliderUILuaComp = nil;

	-- Audio
	--shootingSFX = "Assets\\SFX\\Jump.mp3"
	--walking = "Assets\\SFX\\Step.mp3"
	-- takingDamage =  "Assets\\SFX\\Jump.mp3"
	--loose = "Assets\\SFX\\Jump.mp3"
	--win = "Assets\\SFX\\Jump.mp3"
}

--Init called when comp is created
shooter_fpscontroller.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	OnMouseClick():Bind({self, self.OnMouseClick});

end

--Begin called when obj has all comps
shooter_fpscontroller.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	self.healthSliderUIGameObj = goMgr:FindGameObject("HealthSlider");
	self.healthSliderUILuaComp = self.healthSliderUIGameObj:GetCustomComp("Slider");
	EventManager.Get():PlaySong(false, "Assets\\Songs\\ShooterMusic.wav");
	self.ownerGameObj = owner;
	self.Transform = owner:GetTransformComp();
	self.Camera = owner:GetCameraComp():GetCamera();
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.triggerComp.OnExit:Bind({self, self.OnExit});
end

--Update called every tick
shooter_fpscontroller.Update = function(self, dt, owner) 
	self.healthSliderUILuaComp:SetSliderValue(self.life / self.maxLife);
	
	if (self.life < 1.0) then
		OutputPrint("GAME OVER!!!");
		EventManager:Get():PlaySFX(false, "Assets\\SFX\\PlayerDies.wav");
		--EventManager:Get():StopSong(false);
		local world = EventManager.Get();
		world:LoadState(false, "Assets\\Levels\\Menu.json");
		self.life = 100; -- NOTE: until we restart level, REMOVE later (this is for sound not to loop over)
	end
	
	local position = self.Transform:GetPosition();
	local movement = self.movement_amount * self.MoveSpeed * dt;
	local strafe = self.Camera:GetRight() * movement.x;
	local forward = self.Camera:GetForward() * movement.y;
	position = position + strafe + forward;

	local rotation = self.Rotation;
	if (self.RIGHTCLICK) then
		rotation.x = -1.0*self.DeltaPositionY;
		rotation.y = -1.0*self.DeltaPositionX;
		self.DeltaPositionX = 0.0;
		self.DeltaPositionY = 0.0;
		rotation = rotation * dt * self.RotationSpeed;
		self.Transform:Rotate(rotation.x, rotation.y, 0.0);
		self.Rotation.x = 0.0;
		self.Rotation.y = 0.0;
		self.Rotation.z = 0.0;
	end
	if(self.runGame ~= 1) then
		self.Transform:SetLocalPosition(position.x, position.y, position.z);
	end
end

shooter_fpscontroller.OnEnter = function(self, gameObj1, gameObj2)
	
	local transform1 = gameObj1:GetTransformComp();
	local transform2 = gameObj2:GetTransformComp();
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if(transform1 == nil or transform2 == nil or triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	local collisionMask1 = triggerComp1:GetCollisionMask();
	local collisionMask2 = triggerComp2:GetCollisionMask();
	if (collisionMask2 == CollisionMask.ENEMY_PROJ) then
		self.life = self.life - 1;
		EventManager:Get():PlaySFX(false, "Assets\\SFX\\PlayerTakesDamage.wav");
	end

	--SPAWN PARTICLES
end

shooter_fpscontroller.OnExit = function(self, gameObj1, gameObj2)
end

--Method
shooter_fpscontroller.OnKey = function(self, key, state)
	local delta = 0.0;
	if(state) then
		delta = 1.0;
	end
	
	if(SCANCODE.W == key) then
		self.movement_amount.y = delta;
	elseif(SCANCODE.S == key) then
		self.movement_amount.y = -1.0*delta;
	elseif(SCANCODE.A == key) then
		self.movement_amount.x = -1.0*delta;
	elseif(SCANCODE.D == key) then
		self.movement_amount.x = delta;
	elseif(SCANCODE.L == key) then
		self.life = 0;
	elseif(SCANCODE.Z == key and state ~= true) then
		self.runGame = self.runGame * -1;
		local followPathCurvesComp = self.ownerGameObj:GetFollowCurvesPathComp();
		if(self.runGame == 1) then
			followPathCurvesComp:SetEnableMotionAlongPath(true);
		else
			followPathCurvesComp:SetEnableMotionAlongPath(false);
		end
	elseif(SCANCODE.ESCAPE == key and state) then
		 EventManager.Get():LoadState(false, "Assets\\Levels\\Menu.json");
	end
end

shooter_fpscontroller.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
	self.DeltaPositionX = deltaposition.x;
	self.DeltaPositionY = deltaposition.y;
end

shooter_fpscontroller.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	elseif(button == 3) then
		self.RIGHTCLICK = state;
	end
end


shooter_fpscontroller.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnKey});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});
	OnMouseClick():Unbind({self, self.OnMouseClick});
	
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter});
	self.triggerComp.OnExit:Unbind({self, self.OnExit});
end

shooter_fpscontroller.Draw = function(self, dt, owner, appRenderer)
	local cameraForward = self.Camera:GetForward();
	cameraForward = Vector3.new(cameraForward.x, 0.0, cameraForward.z);
	local offset = cameraForward * 25.0;
	local position = self.Transform:GetPosition();
	position = Vector3.new(position.x, 0.0, position.z);
	local finalFocusPos = position + offset;
	appRenderer:GetMomentShadowMap():SetFocusPoint(finalFocusPos);
	--how to draw text, if this fail that means no Fonts resource is loaded
	--appRenderer:RegisterTextFontInstance("Shooter fps controller", FONT_TYPE.COURIER_NEW, 
		--Vector2.new(0.0, 0.0), Vector3.new(1.0, 1.0, 0.0), Vector3.new(1.0, 1.0, 1.0), 0.0);
end

return shooter_fpscontroller;