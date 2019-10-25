
-- First approximation of a component script

Button = 
{
	-- Component
	uiComp;
	uiTransform;
	
	-- Data
	position;
	scale;
	loacation;
	maxPosX;
	minPosX;
	maxPosY;
	minPosY;
	clipSpacePos;
	ndcSpacePos;
	windowSpacePos;
	clipSpaceScaleMax;
	ndcSpaceScaleMax;
	windowSpaceScaleMax;
	clipSpaceScaleMin;
	ndcSpaceScaleMin;
	windowSpaceScaleMin;
	
	
	-- MOUSE
	MousePositionX = 0;
	MousePositionY = 0;
	LEFTCLICK = false;
	RIGHTCLICK = false;
	
	-- KEYBOARD
	Up = false;
	Down = false;	
	
	--UI Camera
	cameraObj;
	cameraComp;
	camera;
	projectionMatrix;
	viewProjectionMatrix;
	viewMatrix;
	screenWidth;
	screenHeight;
	
}

--Init called when comp is created
Button.Init = function(self)

end

--Begin called when obj has all comps
Button.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	
	
	OnMouseMotion():Bind({self, self.OnMouseMotion});
	
	self.cameraObj = goMgr:FindGameObject("UIObject1");
	if (self.cameraObj == nil) then
		OutputPrint(">>> GO with tag UIObject1 not found\n");
		return;
	end
	
	self.cameraComp = self.cameraObj:GetCameraComp();
	if (self.cameraComp == nil) then
		OutputPrint(">>> Camera Component not found\n");
		return;
	end

	self.camera = self.cameraComp:GetCamera();
	if (self.camera == nil) then
		OutputPrint(">>> Camera  not found\n");
		return;
	end
	
	
	self.uiComp = owner:GetUiComp();
	if (self.uiComp == nil) then 
		OutputPrint("ERROR, UIComponent IS NIL\n");
	end
	
	self.uiTransform = owner:GetTransformComp();
	if (self.uiTransform == nil) then 
		OutputPrint("ERROR, UITransform IS NIL\n");
	end
	
	
	self.loacation = self.uiComp:GetLocation();

	--self.maxPosX = self.position.x + self.scale.x;
	--self.minPosX = self.position.x - self.scale.x;
	--self.maxPosY = self.position.y + self.scale.y;
	--self.minPosY = self.position.y - self.scale.y;
	
end

--Update called every tick
Button.Update = function(self, dt, owner) 

	--self.viewProjectionMatrix = self.camera:GetViewProjectionMatrix();
	self.projectionMatrix = self.camera:GetProjectionMatrix();
	self.viewMatrix = self.camera:GetViewMatrix();
	self.viewProjectionMatrix = self.projectionMatrix * self.viewMatrix;
	self.screenWidth = self.camera:GetScreenWidth();
	self.screenHeight = self.camera:GetScreenHeight();
	
	self.position = self.uiTransform:GetPosition();
	self.scale = self.uiTransform:GetScale();
	
	self.clipSpacePos = Vector4.new();
	self.clipSpaceScaleMax = Vector4.new();
	self.clipSpaceScaleMin = Vector4.new();
	
	self.clipSpacePos.x = self.viewProjectionMatrix.a00 * self.position.x + self.viewProjectionMatrix.a01 * self.position.y + self.viewProjectionMatrix.a02 * self.position.z + self.viewProjectionMatrix.a03 * 1.0;
	self.clipSpacePos.y = self.viewProjectionMatrix.a11 * self.position.x + self.viewProjectionMatrix.a12 * self.position.y + self.viewProjectionMatrix.a13 * self.position.z + self.viewProjectionMatrix.a14 * 1.0;
	self.clipSpacePos.z = self.viewProjectionMatrix.a21 * self.position.x + self.viewProjectionMatrix.a22 * self.position.y + self.viewProjectionMatrix.a23 * self.position.z + self.viewProjectionMatrix.a24 *  1.0;
	self.clipSpacePos.w = self.viewProjectionMatrix.a31 * self.position.x + self.viewProjectionMatrix.a32 * self.position.y + self.viewProjectionMatrix.a33 * self.position.z + self.viewProjectionMatrix.a34 *  1.0;
	
	self.clipSpaceScaleMax.x = self.viewProjectionMatrix.a00 * (self.scale.x+self.position.x) + self.viewProjectionMatrix.a01 * (self.scale.y+self.position.y) + self.viewProjectionMatrix.a02 * (self.scale.z+self.position.z) + self.viewProjectionMatrix.a03 * 1.0;
	self.clipSpaceScaleMax.y = self.viewProjectionMatrix.a11 * (self.scale.x+self.position.x) + self.viewProjectionMatrix.a12 * (self.scale.y+self.position.y) + self.viewProjectionMatrix.a13 * (self.scale.z+self.position.z) + self.viewProjectionMatrix.a14 * 1.0;
	self.clipSpaceScaleMax.z = self.viewProjectionMatrix.a21 * (self.scale.x+self.position.x) + self.viewProjectionMatrix.a22 * (self.scale.y+self.position.y) + self.viewProjectionMatrix.a23 * (self.scale.z+self.position.z) + self.viewProjectionMatrix.a24 *  1.0;
	self.clipSpaceScaleMax.w = self.viewProjectionMatrix.a31 * (self.scale.x+self.position.x) + self.viewProjectionMatrix.a32 * (self.scale.y+self.position.y) + self.viewProjectionMatrix.a33 * (self.scale.z+self.position.z) + self.viewProjectionMatrix.a34 *  1.0;
	
	self.clipSpaceScaleMin.x = self.viewProjectionMatrix.a00 * (self.position.x - self.scale.x) + self.viewProjectionMatrix.a01 * (self.position.y - self.scale.y) + self.viewProjectionMatrix.a02 * (self.position.z - self.scale.z) + self.viewProjectionMatrix.a03 * 1.0;
	self.clipSpaceScaleMin.y = self.viewProjectionMatrix.a11 * (self.position.x - self.scale.x) + self.viewProjectionMatrix.a12 * (self.position.y - self.scale.y) + self.viewProjectionMatrix.a13 * (self.position.z - self.scale.z) + self.viewProjectionMatrix.a14 * 1.0;
	self.clipSpaceScaleMin.z = self.viewProjectionMatrix.a21 * (self.position.x - self.scale.x) + self.viewProjectionMatrix.a22 * (self.position.y - self.scale.y) + self.viewProjectionMatrix.a23 * (self.position.z - self.scale.z) + self.viewProjectionMatrix.a24 *  1.0;
	self.clipSpaceScaleMin.w = self.viewProjectionMatrix.a31 * (self.position.x - self.scale.x) + self.viewProjectionMatrix.a32 * (self.position.y - self.scale.y) + self.viewProjectionMatrix.a33 * (self.position.z - self.scale.z) + self.viewProjectionMatrix.a34 *  1.0;
	
	self.ndcSpacePos = Vector3.new();
	self.ndcSpaceScaleMax = Vector3.new();
	self.ndcSpaceScaleMin = Vector3.new();
	
	self.ndcSpacePos.x = self.clipSpacePos.x ;
	self.ndcSpacePos.y = self.clipSpacePos.y ;
	self.ndcSpacePos.z = self.clipSpacePos.z ;
	
	self.ndcSpaceScaleMax.x = self.clipSpaceScaleMax.x / self.clipSpaceScaleMax.w;
	self.ndcSpaceScaleMax.y = self.clipSpaceScaleMax.y / self.clipSpaceScaleMax.w;
	self.ndcSpaceScaleMax.z = self.clipSpaceScaleMax.z / self.clipSpaceScaleMax.w;
	
	self.ndcSpaceScaleMin.x = self.clipSpaceScaleMin.x / self.clipSpaceScaleMin.w;
	self.ndcSpaceScaleMin.y = self.clipSpaceScaleMin.y / self.clipSpaceScaleMin.w;
	self.ndcSpaceScaleMin.z = self.clipSpaceScaleMin.z / self.clipSpaceScaleMin.w;
	
	self.windowSpacePos = Vector2.new();
	self.windowSpaceScaleMax = Vector2.new();
	self.windowSpaceScaleMin = Vector2.new();
	
	self.windowSpacePos.x = ((self.ndcSpacePos.x + 1.0)/2.0)* self.screenWidth; 
	self.windowSpacePos.y = ((self.ndcSpacePos.y + 1.0)/2.0)* self.screenHeight; 
	
	self.windowSpaceScaleMax.x = ((self.ndcSpaceScaleMax.x + 1.0)/2.0)* self.screenWidth; 
	self.windowSpaceScaleMax.y = ((self.ndcSpaceScaleMax.y + 1.0)/2.0)* self.screenHeight; 
	
	self.windowSpaceScaleMin.x = ((self.ndcSpaceScaleMin.x + 1.0)/2.0)* self.screenWidth; 
	self.windowSpaceScaleMin.y = ((self.ndcSpaceScaleMin.y + 1.0)/2.0)* self.screenHeight; 
	
	OutputPrint("\n var:\n\n" .. self.loacation);
	
	if(self.loacation == _G.value) then
		self.uiTransform:Scale(1.2,1.2,1.2);
	else
		self.uiTransform:Scale(1.0,1.0,1.0);
	end
	--OutputPrint("\n var:\n\n" .. self.windowSpaceScaleMax.y);
	
	-- self.maxPosX = self.windowSpacePos.x + self.windowSpaceScale.x;
	-- self.minPosX = self.windowSpacePos.x - self.windowSpaceScale.x;
	-- self.maxPosY = self.windowSpacePos.y + self.windowSpaceScale.y;
	-- self.minPosY = self.windowSpacePos.y - self.windowSpaceScale.y;

	-- if(self.MousePositionX < self.maxPosX) then
		-- if(self.MousePositionX > self.minPosX) then
			-- if(self.MousePositionY < self.maxPosY) then
				-- if(self.MousePositionY > self.minPosY) then
					-- self.uiComp:IsTriggerd();
				-- end
			-- end
		-- end
	-- else
		-- self.uiComp:IsNotTriggered();
	-- end
	
		
	
end

Button.OnMouseMotion = function(self, position, deltaposition)
	self.MousePositionX = position.x;
	self.MousePositionY = position.y;
end

return Button;