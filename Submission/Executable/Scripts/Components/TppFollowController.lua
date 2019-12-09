
-- First approximation of a component script

TppFollowController = 
{
	MinCoordinates = Vector3.new(-2, 0, -2);
	MaxCoordinates = Vector3.new(2, 1, 0);

	zoom = 0;
	minZoom = -2;
	maxZoom = 0;
	stepSize = 0.05;

	distance = 5.0;

	Transform = nil;
	Camera = nil;
	Player = nil;
}

--Init called when comp is created
TppFollowController.Init = function(self)
	OnMouseScroll():Bind({self, self.OnMouseScroll});
	OnJoystickButton():Bind({self, self.OnJoystickButton});
end

TppFollowController.OnMouseScroll = function(self, x, y)
	--LOG("X: " .. x .. " Y: " .. y .. "\n");
	local translation = Vector3.new(0.0, -y/8, -y/4);
	self.Transform:Translate(translation);
end

TppFollowController.OnJoystickButton = function(self, ID, key, state)
	if(CONTROLLER.Start == key and not state) then
		local position = self.Player:GetTransformComp():GetPosition();
		self.MinCoordinates = position + Vector3.new(-2, 0, -2);
		self.MaxCoordinates = position + Vector3.new(2, 1, 0);
		self.Transform:SetLocalPosition(position.x, position.y + 4, position.z + 8)
	end
end

TppFollowController.OnDestruction = function(self)
	OnMouseScroll():Unbind({self, self.OnMouseScroll});
	OnJoystickButton():Unbind({self, self.OnJoystickButton});
end

--Begin called when obj has all comps
TppFollowController.Begin = function(self, owner, goMgr)

	if (owner == nil) then return end

	self.Transform = owner:GetTransformComp();
	self.Camera = owner:GetCameraComp():GetCamera();

	self.Player = goMgr:FindGameObject("Player");
	local position = self.Player:GetTransformComp():GetPosition();
	self.MinCoordinates = position + Vector3.new(-2, 0, -2);
	self.MaxCoordinates = position + Vector3.new(2, 1, 0);
	self.zoom = 0.0;
	self.Transform:SetLocalPosition(position.x, position.y + 4, position.z + 8)
end

--Update called every tick
TppFollowController.Update = function(self, dt, owner) 
	local playerPosition = self.Player:GetTransformComp():GetPosition();

	-- Translate AABB Boundary as player moves towards edge
	local deltaMin = playerPosition - self.MinCoordinates;
	local deltaMax = playerPosition - self.MaxCoordinates;	
	if(deltaMin.x < 0.0) then 
		self.MinCoordinates.x = self.MinCoordinates.x + deltaMin.x;
		self.MaxCoordinates.x = self.MaxCoordinates.x + deltaMin.x;
		self.Transform:Translate(deltaMin.x, 0.0, 0.0);
	end
	if(deltaMax.x > 0.0) then 
		self.MinCoordinates.x = self.MinCoordinates.x + deltaMax.x;
		self.MaxCoordinates.x = self.MaxCoordinates.x + deltaMax.x;
		self.Transform:Translate(deltaMax.x, 0.0, 0.0);
	end
	if(deltaMin.y < 0.0) then 
		self.MinCoordinates.y = self.MinCoordinates.y + deltaMin.y;
		self.MaxCoordinates.y = self.MaxCoordinates.y + deltaMin.y;
		self.Transform:Translate(0.0, deltaMin.y, 0.0);
	end
	if(deltaMax.y > 0.0) then 
		self.MinCoordinates.y = self.MinCoordinates.y + deltaMax.y;
		self.MaxCoordinates.y = self.MaxCoordinates.y + deltaMax.y;
		self.Transform:Translate(0.0, deltaMax.y, 0.0);
	end
	if(deltaMin.z < 0.0) then 
		self.MinCoordinates.z = self.MinCoordinates.z + deltaMin.z;
		self.MaxCoordinates.z = self.MaxCoordinates.z + deltaMin.z;
		self.Transform:Translate(0.0, 0.0, deltaMin.z);

		if(self.zoom > self.minZoom) then
			self.zoom = self.zoom - self.stepSize;
			self.Transform:Translate(0.0, -self.stepSize/4, -self.stepSize);
		end

	end
	if(deltaMax.z > 0.0) then 
		self.MinCoordinates.z = self.MinCoordinates.z + deltaMax.z;
		self.MaxCoordinates.z = self.MaxCoordinates.z + deltaMax.z;
		self.Transform:Translate(0.0, 0.0, deltaMax.z);
		if(self.zoom < self.maxZoom) then
			self.zoom = self.zoom + self.stepSize;
			self.Transform:Translate(0.0, self.stepSize/4, self.stepSize);
		end
	end
end

return TppFollowController;