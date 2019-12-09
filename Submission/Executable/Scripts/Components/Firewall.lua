Firewall = 
{
	name = "Firewall";
	
	--Custom stuff
	triggerComp = nil;
	PlayerScript = nil;

}

Firewall.Init = function(self)
end

Firewall.Begin = function(self, owner, goMgr)
	self.triggerComp = owner:GetTriggerComp();

	self.triggerComp.OnEnter:Bind({self, self.OnEnter});
	self.Player = goMgr:FindGameObject("Player"):GetCustomComp("RamziPlayer");
end

Firewall.Update = function(self, dt, owner) 
end

Firewall.OnEnter = function(self, gameObj1, gameObj2)
	local triggerComp1 = gameObj1:GetTriggerComp();
	local triggerComp2 = gameObj2:GetTriggerComp();

	if (triggerComp1 == nil or triggerComp2 == nil) then
		return;
	end

	if (gameObj2:GetTag() == "PlayerCube") then
			self.Player.dead = true;
	end
end

Firewall.OnDestruction = function(self)
	self.triggerComp.OnEnter:Unbind({self, self.OnEnter})
end

return Firewall;