
-- First approximation of a component script

JoseController = 
{	
	initialZoom = 0.0;
	idealZoom = 0.0;

	initialPosition = Vector3.new(0.0);
	movement_amount = Vector2.new(0.0);
	MoveSpeed = 10.0;

	ZoomInEnabled = false;
	target = nil;
	zoomSpeed = 0.0;

	Transform = nil;
	Camera = nil;
}

--Init called when comp is created
JoseController.Init = function(self)
end

JoseController.ActivateSlowZoom = function(self, target)
	self.ZoomInEnabled = true;
	self.target = Vector3.new(target);
	self.zoomSpeed = 0.5;
end

--Begin called when obj has all comps
JoseController.Begin = function(self, owner, goMgr)
	self.Transform = owner:GetTransformComp();
	self.Camera = owner:GetCameraComp():GetCamera();
	
	self.initialPosition = Vector3.new(self.Transform:GetPosition());
end

--Update called every tick
JoseController.Update = function(self, dt, owner) 

	if (self.ZoomInEnabled) then 
		--Camera offset
		--local camPos = self.Camera:GetCameraPosition();
		--local offset = self.target - camPos;
		--local lookDir = Vector3.new(offset.x, offset.y, offset.z);
		--lookDir:normalize();
		--
		----Calculate current dis
		--local dist = offset:len();
		--OutputPrint("Dist: " .. dist .. "\n");
		--dist = dist - dt * self.zoomSpeed;
		--camPos = self.target + Vector3.new(lookDir.x * dist, lookDir.y * dist, lookDir.z * dist);
		--
		----Translate the camera
		--self.Camera:SetCameraPosition(camPos.x, camPos.y, camPos.z);
		--self.Camera:SetLook(lookDir);
	else
		--Camera offset
		local offset = Vector3.new(0, 30.0, 75.0);
		local camPos = self.initialPosition + offset;
		local lookDir = Vector3.new(-offset.x, -offset.y, -offset.z);
		lookDir:normalize();

		--Translate the camera
		self.Camera:SetCameraPosition(camPos.x, camPos.y, camPos.z);
		self.Camera:SetLook(lookDir);
	end
end


JoseController.OnDestruction = function(self)
end

return JoseController;