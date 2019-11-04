
-- First approximation of a component script

AffineAnimation = 
{
	Enabled = true;
	-- Animation Data
	InitailAnimationEnabled = false;
	FinalAnimationEnabled = false;
	FinalAnimationEnd = false;
	-- Component
	uiComp;
	uiTransform;

	
	-- Data
	position;
	initialPosition;
	finalPosition;
	initialScale;
	finalScale;
	scale;
	rotation;
	initialRotation;
	finalRotation;
	rotationRate;
	touchedScale;
	location;
	deltaTime;
	Velocity;

	-- Bool Values
	xTranslateInitial = false;
	yTranslateInitial = false;
	xTranslateFinal = false;
	yTranslateFinal = false;
	
	rotateInitial = false;
	rotateFinal = false;
	
	
}

--Init called when comp is created
AffineAnimation.Init = function(self)

end

--Init called when comp is created
AffineAnimation.OnDestruction = function(self)

end

--Begin called when obj has all comps
AffineAnimation.Begin = function(self, owner, goMgr)

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
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
	
	
	

	self.deltaTime = self.uiComp:GetDeltaTime();
	
	
	self.initialPosition = Vector3.new(self.uiComp:GetInitialPosition());
	self.finalPosition = Vector3.new(self.uiComp:GetFinalPosition());
	
	
	--self.uiComp:InitialAnimationEnabled();
	
	self.rotation = self.uiComp:GetInitialRotation();
	self.initialRotation = self.uiComp:GetInitialRotation();
	self.finalRotation = self.uiComp:GetFinalRotation();
	self.rotationRate = self.uiComp:GetRotationRate();
	

	self.InitailAnimationEnabled = true;
end

--Update called every tick
AffineAnimation.Update = function(self, dt, owner) 
	if(self.Enabled == false) then
		--return;
	end
	self.initialPosition = Vector3.new(self.uiComp:GetInitialPosition());
	self.finalPosition = Vector3.new(self.uiComp:GetFinalPosition());
	if(self.deltaTime < 0.0) then
		self.InitailAnimationEnabled = false;
		self.FinalAnimationEnd = true;
		return;
	end
	
	local position = Vector3.new(self.uiTransform:GetPosition());
	local rotation = Vector3.new(self.uiTransform:GetRotation());
	local velocity = Vector3.new(self.uiComp:GetVelocity());
	self.Velocity = velocity;
	-- Animation code
	--------------------------
	if(self.InitailAnimationEnabled == true) then
		
		
		-- X axis Compare
		if(self.xTranslateInitial == false) then
			local compare = position.x - self.finalPosition.x;
			if(compare < 0) then
				compare = -compare ;
			end
		
			if(compare < self.deltaTime) then
			
				position.x = self.finalPosition.x;	
				self.xTranslateInitial = true;	
			
			else
				position.x = position.x + velocity.x * self.deltaTime;	
			end	
		end
		
		-- Y axis Compare
		if(self.yTranslateInitial == false) then
			local compare = position.y - self.finalPosition.y;
			if(compare < 0) then
				compare = -compare ;
			end
			if(compare < self.deltaTime) then
				position.y = self.finalPosition.y;	
				self.yTranslateInitial = true;	
			else
				position.y = position.y + velocity.y * self.deltaTime;	
			end
		end
		-- Rotation
		if(self.rotateInitial == false) then
			if( self.rotation <= 0.0) then
				self.rotation = 0.0;	
				self.rotateInitial = true;	
			else
				self.rotation = self.rotation + self.rotationRate ;
			end
		end
	end
---------------------------------------------------------------	
	if(self.FinalAnimationEnabled == true and self.FinalAnimationEnd == false ) then
		
		
		-- X axis Compare
		if(self.xTranslateFinal == false) then
			local compare = position.x - self.initialPosition.x;
			if(compare < 0) then
				compare = -compare ;
			end
			if(compare <= self.deltaTime) then
				position.x = self.initialPosition.x;
				self.xTranslateFinal = true;
			else
				position.x = position.x - velocity.x * self.deltaTime;	
			end	
		end
		-- Y axis Compare
		if(self.yTranslateFinal == false) then
			local compare = position.y - self.initialPosition.y;
			if(compare < 0) then
				compare = -compare ;
			end
			if(compare <= self.deltaTime) then
				position.y = self.initialPosition.y;		
				self.yTranslateFinal = true;
			
			else
				position.y = position.y - velocity.y * self.deltaTime;	
			end
		end
		-- Rotation
		if(self.rotateFinal == false) then
			if( self.rotation >= self.initialRotation) then
				self.rotation = 0.0;	
				self.rotateFinal = true;	
			else
				self.rotation = self.rotation - self.rotationRate ;
			
			end
		end
	end
-------------------------------------------------------------------------------	
	if(self.xTranslateInitial == true and self.yTranslateInitial == true and self.rotateInitial == true) then
		self.xTranslateInitial = false;
		self.yTranslateInitial = false;
		self.InitailAnimationEnabled = false;
		
	end
	if(self.xTranslateFinal == true and self.yTranslateFinal == true and self.rotateFinal == true) then
		self.FinalAnimationEnd = true;
	end
	--------------------------------------------------------------------------
	
	if(self.InitailAnimationEnabled == true or self.FinalAnimationEnabled == true ) then
		self.uiTransform:SetLocalRotation(0.0,0.0,self.rotation);
		self.uiTransform:SetLocalPosition(position.x, position.y, position.z);
	end
end

AffineAnimation.SetFinalAnimation = function(self)
   self.FinalAnimationEnabled = true;
   self.InitailAnimationEnabled = false;
end
AffineAnimation.GetFinalAnimationState = function(self)
   return self.FinalAnimationEnd;
end
AffineAnimation.Disable = function(self)
   self.Enabled = false;
end
AffineAnimation.GetxTranslateFinal = function(self)
  return self.xTranslateFinal;
end
AffineAnimation.GetyTranslateFinal = function(self)
  return self.yTranslateFinal;
end
AffineAnimation.GetRotateFinal = function(self)
  return self.rotateFinal;
end
AffineAnimation.WindowResize = function(self, Width, Height, BaseWidth, BaseHeight)
	self.initialPosition.x = (self.initialPosition.x / BaseWidth) * Width;
	self.initialPosition.y = (self.initialPosition.y / BaseHeight) * Height;
	self.finalPosition.x = (self.finalPosition.x / BaseWidth) * Width;
	self.finalPosition.y = (self.finalPosition.y / BaseHeight) * Height;
end
return AffineAnimation;