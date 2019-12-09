
-- First approximation of a component script

RunnerManagerComp = 
{
	--Keep track of how much the stage has moved forward
	elapsedTime = 0.0;
	elapsedDist = 0.0;
	distanceSinceLastSpawn = 0.0;

	scrollSpeed = 20.0;
	spawnDistance = 400.0;
	objects = {};
	scrolling = false;
	playerGO = nil;
	deadzone = nil;
	transformComp = nil;

	spawnAmountAdditionOffset = 0;
	counter = 0;
	
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
	self:SpawnNewFloor(owner, -self.spawnDistance);
	self:SpawnNewFloor(owner, -2*self.spawnDistance);

	--Spawn obstacles
	self:Spawn_Obstacle_set(owner, -self.spawnDistance, -2*self.spawnDistance);

	--Spawn collectables
	self:Spawn_Collectables_set(owner, -40, -self.spawnDistance);
	self:Spawn_Collectables_set(owner, -self.spawnDistance, -2*self.spawnDistance);
	self:Spawn_Collectables_coins(owner, -40, -self.spawnDistance);
	self:Spawn_Collectables_coins(owner, -self.spawnDistance, -2*self.spawnDistance);

	--Bind to the deadzone's multicast'
	local dzone = goMgr:FindGameObject("DeadZone");
	if (dzone ~= nil) then
		self.deadzone = dzone;
		local triggercomp = self.deadzone:GetTriggerComp();
		triggercomp.OnEnter:Bind({self, self.CheckIfGameOver});
	end
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
	self.distanceSinceLastSpawn = self.distanceSinceLastSpawn + dt * self.scrollSpeed;

	local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
	playerComp.Meters = self.elapsedDist;


	
	--Procedural stuff-----------------------------------------
	if (self.distanceSinceLastSpawn >= self.spawnDistance) then
		
		--Every 400 meters, double increase scroll speed by 50%
		self.scrollSpeed = 1.3*self.scrollSpeed;
		self.spawnDistance = 1.3*self.spawnDistance;

		--Augment counter. Every x of these increments, increment offset
		local iters = 1;
		self.counter = self.counter + 1;
		if (self.counter % iters == 0) then 
			self.spawnAmountAdditionOffset = self.spawnAmountAdditionOffset + 1;
		end

		OutputPrint("SPAWNING FLOOR!! \n");
		self:SpawnNewFloor(owner, -2*self.spawnDistance);

		self:Spawn_Obstacle_set(owner, -self.spawnDistance, -2*self.spawnDistance);
	    self:Spawn_Collectables_set(owner, -self.spawnDistance, -2*self.spawnDistance);
		self:Spawn_Collectables_coins(owner, -self.spawnDistance, -2*self.spawnDistance);

		OutputPrint("elapsedDist: " .. self.elapsedDist .. "\n");
		self.distanceSinceLastSpawn = 0.0;
	end

	
	--Using scroll speed and list of objs, make them all scroll towards positive z axis
	--Later, we can add a direction of scroll (for a more dynamic look)
	--Also, acceleration can be added
	if (self.scrolling) then 
		local moveDir = Vector3.new(0, 0, 1);
		local vel = moveDir * self.scrollSpeed * dt;

		--Move stage objects
		for key,value in ipairs(self.objects) do
			--Assume, Value is the gameobject
			--OutputPrint("Iterating go with tag: " .. value:GetTag() .. "\n");
			local transform = value:GetTransformComp();
			if (transform ~= nil) then 
				transform:Translate(vel);

				--if (value:GetTag() ~= "Ground01" and transform:GetPosition().z > 100.0) then
				--	value:Destroy();
				--end
			end
		end

		--Move player
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
	
	if (go1 == self.deadzone and go2 == self.playerGO or (go2 == self.deadzone and go1 == self.playerGO)) then
		local playerComp = self.playerGO:GetCustomComp("TestPlayerAnimComp");
		playerComp:OnEnterDeadzone();
	end

end


RunnerManagerComp.Spawn_Collectables_set = function(self, owner, zmin, zmax)
	
	local maxStageHalfDepth = 65;
	local offset = maxStageHalfDepth + 0;
	local num = 4 + self.spawnAmountAdditionOffset;
	local deltaDepth = ((zmin - offset) - (zmax + maxStageHalfDepth + 10)) / num;

	local i = 0;
	while (i < num) do 
		local xr = Range(-30, 30);
		local yr = Range(10, 30);
		local zr = (zmin - maxStageHalfDepth - 65) - deltaDepth * i;
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
	
	local pickableComp = go:GetCustomComp("RunnerPickableComp");
	pickableComp.playerGO = self.playerGO;
end


RunnerManagerComp.Spawn_Collectables_coins = function(self, owner, zmin, zmax)
	
	local maxStageHalfDepth = 65;
	local num = 2 + self.spawnAmountAdditionOffset / 2;
	local deltaDepth = ((zmin - maxStageHalfDepth - 65) - (zmax + maxStageHalfDepth + 15)) / num;

	local i = 0;
	while (i < num) do 
		local xr = Range(-30, 30);
		local yr = Range(15, 30);
		local zr = (zmin - maxStageHalfDepth - 65) - deltaDepth * i;
		
		local delta = 5.0;
		--WIDTH
		--self:Spawn_CoinSet(owner, xr - delta, yr - delta, zr - 2*delta);
		self:Spawn_CoinSet(owner, xr - delta, yr - 0, zr - 2*delta);
		self:Spawn_CoinSet(owner, xr - delta, yr + delta, zr - 2*delta);
		--self:Spawn_CoinSet(owner, xr - 0, yr - delta, zr - 2*delta);
		self:Spawn_CoinSet(owner, xr - 0, yr - 0, zr - 2*delta);
		self:Spawn_CoinSet(owner, xr - 0, yr + delta, zr - 2*delta);
		--self:Spawn_CoinSet(owner, xr + delta, yr - delta, zr - 2*delta);
		self:Spawn_CoinSet(owner, xr + delta, yr - 0, zr - 2*delta);
		self:Spawn_CoinSet(owner, xr + delta, yr + delta, zr - 2*delta);
		--
		--self:Spawn_CoinSet(owner, xr - delta, yr - delta, zr - delta);
		self:Spawn_CoinSet(owner, xr - delta, yr - 0, zr - delta);
		self:Spawn_CoinSet(owner, xr - delta, yr + delta, zr - delta);
		--self:Spawn_CoinSet(owner, xr - 0, yr - delta, zr - delta);
		self:Spawn_CoinSet(owner, xr - 0, yr - 0, zr - delta);
		self:Spawn_CoinSet(owner, xr - 0, yr + delta, zr - delta);
		--self:Spawn_CoinSet(owner, xr + delta, yr - delta, zr - delta);
		self:Spawn_CoinSet(owner, xr + delta, yr - 0, zr - delta);
		self:Spawn_CoinSet(owner, xr + delta, yr + delta, zr - delta);
		--
		--self:Spawn_CoinSet(owner, xr - delta, yr - delta, zr - 0);
		self:Spawn_CoinSet(owner, xr - delta, yr - 0, zr - 0);
		self:Spawn_CoinSet(owner, xr - delta, yr + delta, zr - 0);
		--self:Spawn_CoinSet(owner, xr - 0, yr - delta, zr - 0);
		self:Spawn_CoinSet(owner, xr - 0, yr - 0, zr - 0);
		self:Spawn_CoinSet(owner, xr - 0, yr + delta, zr - 0);
		--self:Spawn_CoinSet(owner, xr + delta, yr - delta, zr - 0);
		self:Spawn_CoinSet(owner, xr + delta, yr - 0, zr - 0);
		self:Spawn_CoinSet(owner, xr + delta, yr + delta, zr - 0);
		--
		--self:Spawn_CoinSet(owner, xr - delta, yr - delta, zr + delta);
		self:Spawn_CoinSet(owner, xr - delta, yr - 0, zr + delta);
		self:Spawn_CoinSet(owner, xr - delta, yr + delta, zr + delta);
		--self:Spawn_CoinSet(owner, xr - 0, yr - delta, zr + delta);
		self:Spawn_CoinSet(owner, xr - 0, yr - 0, zr + delta);
		self:Spawn_CoinSet(owner, xr - 0, yr + delta, zr + delta);
		--self:Spawn_CoinSet(owner, xr + delta, yr - delta, zr + delta);
		self:Spawn_CoinSet(owner, xr + delta, yr - 0, zr + delta);
		self:Spawn_CoinSet(owner, xr + delta, yr + delta, zr + delta);
		--
		--self:Spawn_CoinSet(owner, xr - delta, yr - delta, zr + 2*delta);
		self:Spawn_CoinSet(owner, xr - delta, yr - 0, zr + 2*delta);
		self:Spawn_CoinSet(owner, xr - delta, yr + delta, zr + 2*delta);
		--self:Spawn_CoinSet(owner, xr - 0, yr - delta, zr + 2*delta);
		self:Spawn_CoinSet(owner, xr - 0, yr - 0, zr + 2*delta);
		self:Spawn_CoinSet(owner, xr - 0, yr + delta, zr + 2*delta);
		--self:Spawn_CoinSet(owner, xr + delta, yr - delta, zr + 2*delta);
		self:Spawn_CoinSet(owner, xr + delta, yr - 0, zr + 2*delta);
		self:Spawn_CoinSet(owner, xr + delta, yr + delta, zr + 2*delta);
		
		i = i+1;
	end
end


RunnerManagerComp.Spawn_CoinSet = function(self, owner, x, y, z) 
	local go = GameObject.Instantiate(owner:Manager(), "Assets\\Prefabs\\RunnerLevel_Coin.json");
	local transform = go:GetTransformComp();
	local trigger = go:GetTriggerComp();
	local spawnPos = Vector3.new(x, y, z);
	transform:SetLocalPosition(spawnPos);
	self.objects[#self.objects + 1] = go;
	
	local pickableComp = go:GetCustomComp("RunnerCoinComp");
	pickableComp.playerGO = self.playerGO;
end



RunnerManagerComp.Spawn_Obstacle_set = function(self, owner, zmin, zmax)
	
	local maxStageHalfDepth = 65;
	local offset = maxStageHalfDepth + 10;
	local num = 3 + self.spawnAmountAdditionOffset; -- Number of depth subdivissions
	local deltaDepth = ( (zmin - offset) - (zmax + maxStageHalfDepth + 65) ) / num;

	local i = 0;
	while (i <= num) do 
		local xr = Range(-50, 50);
		local yr = Range(0, 50);
		local zr = (zmin - maxStageHalfDepth - 65) - deltaDepth * i;
		self:Spawn_Obstacle1(owner, xr, yr, zr);
		i = i+1;
	end
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