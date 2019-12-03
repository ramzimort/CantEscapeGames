
-- First approximation of a component script

RunnerManagerComp = 
{
	--Keep track of how much the stage has moved forward
	elapsedTime = 0.0;
	elapsedDist = 0.0;
	timeToSpawnFloor = 0.0;
	nextTimeGoal = 0.0;

	scrollSpeed = 0.25;
	objects = {};
	scrolling = false;
	playerGO = nil;
	deadzone = nil;
	transformComp = nil;
	
	--Variable made so the multicasts are set after the first frame
	frameNum = 0;
}


--Key bindings for keyboard
RunnerManagerComp.OnKey = function(self, key, state)
	--if(SCANCODE.W == key) then
	--elseif(SCANCODE.S == key) then
	--elseif(SCANCODE.A == key) then
	--elseif(SCANCODE.D == key) then
	--end
end


--Init called when comp is created
RunnerManagerComp.Init = function(self)
	OnKeyEvent():Bind({self, self.OnKey});
end


--Begin called when obj has all comps
RunnerManagerComp.Begin = function(self, owner, goMgr)
	
	self.transformComp = owner:GetTransformComp();

	--Add player to the list
	local player = goMgr:FindGameObject("Player");
	if (player ~= nil) then
		--Player should be treated different than rest of stage
		--self.objects[#self.objects + 1] = player;
		self.playerGO = player;

		--Add multicast so player can toggle stage scrolling
		local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
		playerComp.OnTogglingScrolling:suscribe({self, self.ToggleScrolling});
	end

	--Initial part of the stage (not procedural)
	local go1 = goMgr:FindGameObject("Ground01");
	if (go1 ~= nil) then
		self.objects[#self.objects + 1] = go1;
		
		local groundTrigger = go1:GetTriggerComp();
		local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
		groundTrigger.OnExit:Bind({playerComp, playerComp.OnLeaveFloor});
	end

	--Start level already generating the far away floor
	--OutputPrint("SPAWNING FLOOR!! \n");
	self:SpawnNewFloor(owner, -400);
	self:SpawnNewFloor(owner, -800);

	--Spawn obstacles
	self:Spawn_Obstacle_set(owner, -400, -800);

	--Spawn collectables
	self:Spawn_Collectables_set(owner, -40, -400);
	self:Spawn_Collectables_set(owner, -400, -800);

	--Bind to the deadzone's multicast'
	local dzone = goMgr:FindGameObject("DeadZone");
	if (dzone ~= nil) then
		self.deadzone = dzone;

		local triggercomp = self.deadzone:GetTriggerComp();
		triggercomp.OnEnter:Bind({self, self.CheckIfGameOver});
	end

	--Setting up some crap
	self.timeToSpawnFloor = 30.0; -- 15 worked well also
	self.nextTimeGoal = 35.0;	  --  5 worked well also
end


--Update called every tick
RunnerManagerComp.Update = function(self, dt, owner) 
	
	--This is so pop time doesnt get screwed by getting hit
	if (not self.scrolling) then 
		return;
	end

	--Advance elapsed time
	self.elapsedTime = self.elapsedTime + dt;
	self.elapsedDist = self.elapsedDist + dt * self.scrollSpeed;


	--Procedural stuff-----------------------------------------
	if (self.elapsedTime > self.nextTimeGoal) then
		
		OutputPrint("SPAWNING FLOOR!! \n");
		self:SpawnNewFloor(owner, -800);

		self:Spawn_Obstacle_set(owner, -350, -800);
	    self:Spawn_Collectables_set(owner, -350, -800);

		OutputPrint("elapsedDist: " .. self.elapsedDist .. "\n");
		self.nextTimeGoal = self.elapsedTime + self.timeToSpawnFloor;
	end
	--Procedural stuff-----------------------------------------



	--Using scroll speed and list of objs, make them all scroll towards positive z axis
	--Later, we can add a direction of scroll (for a more dynamic look)
	--Also, acceleration can be added
	if (self.scrolling) then 
		local moveDir = Vector3.new(0, 0, 1);
		local vel = moveDir * self.scrollSpeed;

		--Move stage objects
		for key,value in ipairs(self.objects) do
			--Assume, Value is the gameobject
			--OutputPrint("Iterating go with tag: " .. value:GetTag() .. "\n");
			local transform = value:GetTransformComp();
			if (transform ~= nil) then 
				transform:Translate(vel);
			end
		end

		--Move player
		local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
		if (not playerComp.jumping and not playerComp.falling and not playerComp.flying) then 
			--Basically, if grounded, move with the stage
			local transform = self.playerGO:GetTransformComp();
			if (transform ~= nil) then 
				transform:Translate(vel);
			end
		end
	end

end


--Called when the component is gonna be destroyed
RunnerManagerComp.OnDestruction = function(self)
	OnKeyEvent():Unbind({self, self.OnKey});
end


--Called for toggling scrolling
RunnerManagerComp.ToggleScrolling = function(self)
	OutputPrint("RunnerManagerComp.ToggleScrolling - CALLED\n");
	self.scrolling = not self.scrolling;
end


RunnerManagerComp.CheckIfGameOver = function(self, go1, go2)
	
	if (go1 == self.deadzone and go2 == self.playerGO) then
		OutputPrint("PLAYER IN DEADZONE!!!!\n");
		local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
		playerComp:OnEnterDeadzone();
	end

end


RunnerManagerComp.Spawn_Collectables_set = function(self, owner, zmin, zmax)
	
	local maxStageHalfDepth = 65;
	local num = 4;
	local deltaDepth = ((zmin - maxStageHalfDepth) - (zmax + maxStageHalfDepth)) / num;

	OutputPrint("ASDAD: " .. deltaDepth .. "\n");

	local i = 0;
	while (i < num) do 
		local xr = Range(-30, 30);
		local yr = Range(10, 30);
		local zr = (zmin - maxStageHalfDepth) - deltaDepth * i;
		self:Spawn_GasTank(owner, xr, yr, zr);
		i = i+1;
	end
end


RunnerManagerComp.Spawn_GasTank = function(self, owner, x, y, z) 
	local go = GameObject.Instantiate(owner:Manager(), "Assets\\Prefabs\\RunnerLevel_GasTank.json");
	local transform = go:GetTransformComp();
	local trigger = go:GetTriggerComp();
	local spawnPos = Vector3.new(x, y, z);
	transform:SetLocalPosition(spawnPos);
	self.objects[#self.objects + 1] = go;
	
	local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
	trigger.OnEnter:Bind({playerComp, playerComp.OnGasTank});
end


RunnerManagerComp.Spawn_Obstacle_set = function(self, owner, zmin, zmax)
	
	local maxStageHalfDepth = 65;
	local num = 4; -- Number of depth subdivissions
	local deltaDepth = ( (zmin - maxStageHalfDepth) - (zmax + maxStageHalfDepth) ) / num;

	local i = 0;
	while (i < num) do 
		local xr = Range(-50, 50);
		local yr = Range(0, 50);
		local zr = (zmin - maxStageHalfDepth) - deltaDepth * i;
		self:Spawn_Obstacle1(owner, xr, yr, zr);
		i = i+1;
	end
	
	---xr = Range(-50, 50);
	---yr = Range(0, 60);
	---zr = zr - Range(80, 120);
	---self:Spawn_Obstacle1(owner, xr, yr, zr);
	---
	---xr = Range(-50, 50);
	---yr = Range(0, 60);
	---zr = zr - Range(80, 120);
	---self:Spawn_Obstacle1(owner, xr, yr, zr);
end


RunnerManagerComp.Spawn_Obstacle1 = function(self, owner, x, y, z) 
	local go = GameObject.Instantiate(owner:Manager(), "Assets\\Prefabs\\RunnerLvl_Obstacle1.json");
	local transform = go:GetTransformComp();
	local spawnPos = Vector3.new(x, y, z);
	transform:SetLocalPosition(spawnPos);
	self.objects[#self.objects + 1] = go;
end


RunnerManagerComp.SpawnNewFloor = function(self, owner, spawnDepth) 
	
	--Instantiate floor piece
	local stageGO = GameObject.Instantiate(owner:Manager(), "Assets\\Prefabs\\RunnerLevel_Ground.json");
	local transform = stageGO:GetTransformComp();
	local trigger = stageGO:GetTriggerComp();
	local rigidbody = stageGO:GetRigidbodyComp();

	--Add to scrolling list
	self.objects[#self.objects + 1] = stageGO;

	--local spawnPos
	local spawnPos = Vector3.new(0.0, 0.0, spawnDepth);
	local yval = Range(5, 60);
	local depth = Range(70, 130);

	--Override values
	transform:SetLocalPosition(spawnPos);
	transform:Scale(75.0, yval, depth);
	trigger:SetScale(Vector3.new(75.0, yval + 5, depth));
	trigger:SetOffset(Vector3.new(0.0, 0.0, 0.0));
		
	--Player multicast
	local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
	trigger.OnExit:Bind({playerComp, playerComp.OnLeaveFloor});
	rigidbody.OnCollision:Bind({playerComp, playerComp.OnCollisionGround});

	--Instantiate second obj-------------------------------------------------------------------------------------
	local bouncerGO = GameObject.Instantiate(owner:Manager(), "Assets\\Prefabs\\RunnerLevel_GroundBouncer.json");
	local bTransform = bouncerGO:GetTransformComp();
	local bTrigger = bouncerGO:GetTriggerComp();

	--Add to scrolling
	self.objects[#self.objects + 1] = bouncerGO;

	--Override values
	bTransform:SetLocalPosition(spawnPos);
	bTransform:Scale(76.0, yval-5, depth + 1); -- x and z, 5 more. y, 10 less.
	bTrigger:SetScale(Vector3.new(76.0, yval-5, depth + 1));
	bTrigger:SetOffset(Vector3.new(0.0, 0.0, 0.0));
		
	--Player multicast
	local bouncerComp = bouncerGO:GetCustomComp("RunnerGround");
	bouncerComp.playerGO = self.playerGO;
end


return RunnerManagerComp;