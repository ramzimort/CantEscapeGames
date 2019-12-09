SwatEnemyLogicComp = 
{
	gameobjManager = nil;
	animComp = nil;
};

--Init called when obj has all comps
SwatEnemyLogicComp.Init  = function(self)
	
end

--Begin called when on that frame all objects has been instiated
SwatEnemyLogicComp.Begin = function(self, owner, gameobjManager)
	self.gameobjManager = gameobjManager;
	self.animComp = owner:GetAnimationComp();
	self:AnimatorSetup();
end

--Update called every tick
SwatEnemyLogicComp.Update = function(self, dt, owner)
	
end

SwatEnemyLogicComp.OnDestruction = function(self)
	
end

SwatEnemyLogicComp.AnimatorSetup = function(self)
	
	--STATES-
	local idle_State =		self.animComp:CreateState("idle", "ShootAnim");

	--STARTING STATE FOR THE STAGTE MACHINE
	self.animComp:SetEntryState(idle_State);
end


return SwatEnemyLogicComp;