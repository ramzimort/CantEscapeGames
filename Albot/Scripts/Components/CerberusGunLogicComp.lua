dofile("Scripts\\Helper\\ShooterRailPrototype_Common.lua")

CerberusGunLogicComp = 
{
	spawnGameobjPrefabDir = "Assets\\Prefabs\\Projectiles\\ProjectilePlayer.json";
	gameobjManager = nil;
	transformComp = nil;
	playerGameObj = nil;
	playerTransformComp = nil;
	playerCameraComp = nil;
	playerCamera = nil;
	positionRelativeToPlayer = Vector3.new(0.0);
	currentRotationVector = Vector3.new(0.0);
	--gun variables
	LEFTCLICK = false;
	--RIGHTCLICK = false;
	onCooldown = false;
	cooldown = 0.0;
	maxCooldown = 0.2;
	bulletSpeed = 50.0;
};
--Init called when obj has all comps
CerberusGunLogicComp.Init  = function(self)
	OnMouseClick():Bind({self, self.OnMouseClick});
	self.positionRelativeToPlayer.x = 0.0;
	self.positionRelativeToPlayer.y = -0.25;
	self.positionRelativeToPlayer.z = -0.6;

	self.currentRotationVector.x = -90.0;
	self.currentRotationVector.y = 0.0;
	self.currentRotationVector.z = 0.0;
end


--Begin called when on that frame all objects has been instiated
CerberusGunLogicComp.Begin = function(self, owner, gameobjManager)
	self.gameobjManager = gameobjManager;
	self.transformComp = owner:GetTransformComp();
	self.transformComp:Scale(0.009, 0.009, 0.009);
	self.playerGameObj = gameobjManager:FindGameObject("player01");
	if(self.playerGameObj == nil) then
		return;
	end
	self.playerCameraComp = self.playerGameObj:GetCameraComp();
	self.playerCamera = self.playerCameraComp:GetCamera();
	self.playerTransformComp = self.playerGameObj:GetTransformComp();
end

--Update called every tick
CerberusGunLogicComp.Update = function(self, dt, owner)
	if(self.playerGameObj == nil) then
		return;
	end
	local playerPosition = self.playerTransformComp:GetPosition();

	local cameraUp = self.playerCamera:GetUp();
	local cameraForward = self.playerCamera:GetForward();
	local cameraRight = self.playerCamera:GetRight();

	local basisUp = PiecewiseProd(self.positionRelativeToPlayer, cameraUp);
	local basisForward = PiecewiseProd(self.positionRelativeToPlayer, cameraForward);
	local basisRight = PiecewiseProd(self.positionRelativeToPlayer, cameraRight);

	local ownerRelativeTranslationMatrix = CreateTranslation(self.positionRelativeToPlayer);
	local ownerRotationMatrix = CreateRotationMatrixFromDegrees(self.currentRotationVector.x, 
		self.currentRotationVector.y, self.currentRotationVector.z);


	self.transformComp:SetLocalPosition(playerPosition);

	local invViewMatrix = self.playerCamera:GetInvViewMatrixCopy();
	local zeroVector = Vector3.new(0.0);
	invViewMatrix:Translation(zeroVector);
	local newRotationMatrix = ownerRotationMatrix * ownerRelativeTranslationMatrix *  invViewMatrix;
	local cameraDegRotationVector = MatrixToDegreeEulerAngles(invViewMatrix);
	self.transformComp:SetLocalRotationMatrix(newRotationMatrix)

	if (self.onCooldown) then
		self.cooldown = self.cooldown + dt;			
		if (self.cooldown > self.maxCooldown) then
			self.cooldown = 0.0
			self.onCooldown = false
		end
	end

	if (self.LEFTCLICK) then
		if (not self.onCooldown) then
			--OutputPrint("\nPEW!\n");
			local newSpawnedProjectile = GameObject.Instantiate(self.gameobjManager, self.spawnGameobjPrefabDir);
			local projectileTransform = newSpawnedProjectile:GetTransformComp();
			projectileTransform:SetLocalPosition(playerPosition + cameraForward);
			projectileTransform:SetLocalRotation(cameraDegRotationVector.x + 180.0, cameraDegRotationVector.y, cameraDegRotationVector.z);
			local projectileRigidbody = newSpawnedProjectile:GetRigidbodyComp();
			local pojectileVelocity = cameraForward * self.bulletSpeed;
			projectileRigidbody:SetVelocity(pojectileVelocity);
			self.onCooldown = true;

			--SHAKE THE GUN HERE

		end
	--else
		--OutputPrint("\nSTILL ON COOLDOWN\n");
	end
end

--Method
CerberusGunLogicComp.OnKey = function(self, key, state)
	--[[if(SCANCODE.W == key) then
		self.Forward = state;
	elseif(SCANCODE.S == key) then
		self.Backward = state;
	elseif(SCANCODE.A == key) then
		self.Left = state;
	elseif(SCANCODE.D == key) then
		self.Right = state;
	end]]
end

CerberusGunLogicComp.OnMouseMotion = function(self, position, deltaposition)
	--[[self.MousePositionX = position.x;
	self.MousePositionY = position.y;
	self.DeltaPositionX = deltaposition.x;
	self.DeltaPositionY = deltaposition.y;]]
end

CerberusGunLogicComp.OnMouseClick = function(self, button, state)
	if(button == 1) then
		self.LEFTCLICK = state;
	--[[elseif(button == 2) then
		self.RIGHTCLICK = state;]]
	end
end

CerberusGunLogicComp.OnDestruction = function(self)
	--[[OnKeyEvent():Unbind({self, self.OnKey});
	OnMouseMotion():Unbind({self, self.OnMouseMotion});]]
	OnMouseClick():Unbind({self, self.OnMouseClick});
end

return CerberusGunLogicComp;