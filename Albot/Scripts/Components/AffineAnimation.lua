
-- First approximation of a component script

AffineAnimation = 
{

	
	-- Component
	uiComp;
	uiTransform;
	cameraComp;
	
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
	positveVelocity;
	negativeVelocity;
	initialAnimationEnable = false;
	finalAnimationEnable = false;

	-- Bool Values
	xTranslateInitial = false;
	yTranslateInitial = false;
	xTranslateFinal = false;
	yTranslateFinal = false;
	
	rotateInitial = false;
	rotateFinal = false;
	
	
	-- Death
	ComponentDeath = false;
	
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
	
	
	self.initialPosition = Vector3.new(self.uiComp:GetInitialPosition());
	
	self.position = Vector3.new(self.uiComp:GetInitialPosition());
	self.finalPosition = Vector3.new(self.uiComp:GetFinalPosition());
	
	self.deltaTime = self.uiComp:GetDeltaTime();
	self.positveVelocity = Vector3.new(self.uiComp:GetVelocity());
	
	self.negativeVelocity = self.uiComp:GetVelocity();
	self.negativeVelocity = self.negativeVelocity * (-1);
	self.initialAnimationEnable = true;
	
	self.scale = Vector3.new(self.uiTransform:GetScale());
	self.uiComp:InitialAnimationEnabled();
	
	self.rotation = self.uiComp:GetInitialRotation();
	self.initialRotation = self.uiComp:GetInitialRotation();
	self.finalRotation = self.uiComp:GetFinalRotation();
	self.rotationRate = self.uiComp:GetRotationRate();
	
	OutputPrint("Reached\n" );
	
end

--Update called every tick
AffineAnimation.Update = function(self, dt, owner) 
	
	if(self.ComponentDeath == true) then
		return;
	end
	
	-- Animation code
	--------------------------
	if(self.uiComp:GetInitailAnimationState() == true) then
		
		-- X axis Compare
		compare = self.position.x - self.finalPosition.x;
		if(compare < 0) then
			compare = compare * (-1);
		end
		
		if(compare < self.deltaTime) then
			
			self.position.x = self.finalPosition.x;	
			self.xTranslateInitial = true;	
			
		else
			self.position.x = self.position.x + self.positveVelocity.x * self.deltaTime;	
		end	
		-- Y axis Compare
		compare = self.position.y - self.finalPosition.y;
		if(compare < 0) then
			compare = compare * (-1);
		end
		if(compare < self.deltaTime) then
			self.position.y = self.finalPosition.y;	
			self.yTranslateInitial = true;	
		else
			self.position.y = self.position.y + self.positveVelocity.y * self.deltaTime;	
		end
		-- Rotation
		if( self.rotation <= 0.0) then
			self.rotation = 0.0;	
			self.rotateInitial = true;	
		else
			self.rotation = self.rotation + self.rotationRate ;
			
		end
	end
---------------------------------------------------------------	
	if(self.uiComp:GetFinalAnimationState() == true or _G.PlayFinalAnimation == true) then
		
		
		-- X axis Compare
		compare = self.position.x - self.initialPosition.x;
		
		if(compare < 0) then
			compare = compare * (-1);
		end

		if(compare <= self.deltaTime) then
			
			self.position.x = self.initialPosition.x;
			if (self.xTranslateFinal == false)	then		
				self.xTranslateFinal = true;
			end
		else
			
			self.position.x = self.position.x - self.positveVelocity.x * self.deltaTime;	
		end	
		-- Y axis Compare
		compare = self.position.y - self.initialPosition.y;
		if(compare < 0) then
			compare = compare * (-1);
		end
		if(compare <= self.deltaTime) then
			self.position.y = self.initialPosition.y;
			if (self.yTranslateFinal == false)	then		
				self.yTranslateFinal = true;
			end			
			
			
		else
			self.position.y = self.position.y - self.positveVelocity.y * self.deltaTime;	
		end
		-- Rotation
		if( self.rotation >= self.initialRotation) then
			self.rotation = 0.0;	
			self.rotateFinal = true;	
		else
			self.rotation = self.rotation - self.rotationRate ;
			
		end
	end
-------------------------------------------------------------------------------	
	if(self.xTranslateInitial == true and self.yTranslateInitial == true and self.rotateInitial == true) then
		self.uiComp:InitialAnimationDisabled();
		self.xTranslateInitial = false;
		self.yTranslateInitial = false;
		
	end
	if(self.xTranslateFinal == true and self.yTranslateFinal == true and self.rotateFinal == true) then
		self.uiComp:FinalAnimationDisabled();
		self.ComponentDeath = true;
		
		_G.FinalAnimationCount =  _G.FinalAnimationCount + 1;
	end
	--------------------------------------------------------------------------
	
	if(self.uiComp:GetInitailAnimationState() == true or self.uiComp:GetFinalAnimationState() == true or _G.PlayFinalAnimation == true) then
		self.uiTransform:SetLocalRotation(0.0,0.0,self.rotation);
		self.uiTransform:SetLocalPosition(self.position.x, self.position.y, self.position.z);
	end
end

return AffineAnimation;