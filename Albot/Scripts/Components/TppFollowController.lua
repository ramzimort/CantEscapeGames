
-- First approximation of a component script

TppFollowController = 
{
	MinCoordinates = Vector3.new(-5, -20, -5);
	MaxCoordinates = Vector3.new(5, 20, 0);

	distance = 5.0;

	Transform = nil;
	Camera = nil;
	Player = nil;
}

--Init called when comp is created
TppFollowController.Init = function(self)
	OnMouseScroll():Bind({self, self.OnMouseScroll});
end

TppFollowController.OnMouseScroll = function(self, x, y)
	--LOG("X: " .. x .. " Y: " .. y .. "\n");
	local translation = Vector3.new(0.0, -y/8, -y/4);
	self.Transform:Translate(translation);
end

TppFollowController.OnDestruction = function(self)
	OnMouseScroll():Unbind({self, self.OnMouseScroll});
end

--Begin called when obj has all comps
TppFollowController.Begin = function(self, owner, goMgr)

	if (owner == nil) then return end

	self.Transform = owner:GetTransformComp();
	self.Camera = owner:GetCameraComp():GetCamera();

	self.Player = goMgr:FindGameObject("Player");
	local position = self.Player:GetTransformComp():GetPosition();
	self.MinCoordinates = position + self.MinCoordinates;
	self.MaxCoordinates = position + self.MaxCoordinates;
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
		LOG("LEFT\n");
	end
	if(deltaMin.y < 0.0) then 
		self.MinCoordinates.y = self.MinCoordinates.y + deltaMin.y;
		self.MaxCoordinates.y = self.MaxCoordinates.y + deltaMin.y;
		self.Transform:Translate(0.0, deltaMin.y, 0.0);
		LOG("Down\n");
	end
	if(deltaMin.z < 0.0) then 
		self.MinCoordinates.z = self.MinCoordinates.z + deltaMin.z;
		self.MaxCoordinates.z = self.MaxCoordinates.z + deltaMin.z;
		self.Transform:Translate(0.0, 0.0, deltaMin.z);
		LOG("Forward\n");
	end
	if(deltaMax.x > 0.0) then 
		self.MinCoordinates.x = self.MinCoordinates.x + deltaMax.x;
		self.MaxCoordinates.x = self.MaxCoordinates.x + deltaMax.x;
		self.Transform:Translate(deltaMax.x, 0.0, 0.0);
		LOG("Right\n");
	end
	if(deltaMax.y > 0.0) then 
		self.MinCoordinates.y = self.MinCoordinates.y + deltaMax.y;
		self.MaxCoordinates.y = self.MaxCoordinates.y + deltaMax.y;
		self.Transform:Translate(0.0, deltaMax.y, 0.0);
		LOG("Up\n");
	end
	if(deltaMax.z > 0.0) then 
		self.MinCoordinates.z = self.MinCoordinates.z + deltaMax.z;
		self.MaxCoordinates.z = self.MaxCoordinates.z + deltaMax.z;
		self.Transform:Translate(0.0, 0.0, deltaMax.z);
		LOG("Back\n");
	end
end

return TppFollowController;