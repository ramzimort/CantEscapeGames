-- First approximation of a component script

test01Comp = 
{
	name = "test01Comp",

	--Custom variables you can add
	ranked = -1,
	isDumb = false,
	hashed = "_empty_"
}

--Init called when comp is created
test01Comp.Init = function(self)
	
	OutputPrint("COMP INIT - " .. self.name .. "\n");

end

--LOCAL Function (not a method, no self param). 
-- for it to be binded, it needs to be defined up here, 
-- or at least be declared as a local before the binding
local BindingFunction = function(msg, time)
	OutputPrint("\n>>>>>>>> called from elsewhere -< " .. msg .. " >- at time " .. time .. ".\n");
	return 17;
end


--Begin called when obj has all comps
test01Comp.Begin = function(self, owner, goMgr)

	OutputPrint("COMP BEGIN IN LUA - " .. self.name .. "\n");

	if (owner == nil) then
		OutputPrint("FAILED, OWNER IS NIL\n");
		return;
	end

	--[[-------------------------

	EventManager.bind({self, &asd});

	EventMgr:

	-----------------------------

	albert = goMgr.find("albert");

	rgdbdy = albert.getRigidbodyComp();

	if (rgdbdy) then 
		rgdbdy.OnEnter.Bind(self.HitME);
	end

	-----------------------------

	input = owner:getInputComp();

	input.OnKeyDown.bind(self, self.OnKeyWhatever);

	---------------------------]]

	albert = GameObject.Instantiate(owner:Manager());

	transform = albert:AddTransformComp();
	transform:SetLocalPosition(Vector3.new(1, 2, 3));
	
	testco02 = albert:AddCustomComp("test02Comp");
	OutputPrint(testco02:ReturnWeirdString02());

	-----------------------------------------------------------

	OutputPrint(">>> TAG: " .. owner:GetTag() .. "\n");
	
	local v1 = Vector3.new(-1);
	local v2 = Vector3.new(1);
	local v3 = v1 + v2;
	local v4 = v2 * 12;
	--local v5 = 12 * v2;
	local dotted = v4:dot(v1);

	OutputPrint(">> v1     :  " .. v1.x .. ", " .. v1.y .. ", " .. v1.z .. " \n");
	OutputPrint(">> v2     :  " .. v2.x .. ", " .. v2.y .. ", " .. v2.z .. " \n");
	OutputPrint(">> v1 + v2:  " .. v3.x .. ", " .. v3.y .. ", " .. v3.z .. " \n");
	OutputPrint(">> v2 * 12:  " .. v4.x .. ", " .. v4.y .. ", " .. v4.z .. " \n");
	----OutputPrint(">> 12 * v2:  " .. v5.x .. ", " .. v5.y .. ", " .. v5.z .. " \n");
	OutputPrint(">> v4 ° v1:  " .. dotted .. " \n");

	
	local Comp02 = owner:GetCustomComp("test02Comp");
	if (Comp02 ~= nil) then
		OutputPrint(">>> Comp2 Call to method: " .. Comp02:ReturnWeirdString() .. "\n");
		OutputPrint(">>> Comp2 hashed var: " .. Comp02.hashed .. "\n");
		OutputPrint(">>> Comp2 ranked var: " .. Comp02.ranked .. "\n");
	end

	--Try to get an external GO
	local gameobj01 = goMgr:FindGameObject("Andres");
	if (gameobj01 == nil) then
		OutputPrint(">>> GO with tag Andres not found\n");
		return;
	end

	--GETTING OTHER GO's COMPONENT AND BINDING TO MULTICAST
	OutputPrint(">>>>>>>>>> Go retrieved - TAG: " .. gameobj01:GetTag() .. " -\n");
	local otherGoComp02 = gameobj01:GetCustomComp("test02Comp");
	OutputPrint(">>>>>>>>>> Retrieved function from other go's comp02: " .. otherGoComp02:ReturnWeirdString() .. " -\n");

	--Multicast suscribe (2 ways)
	otherGoComp02.OnTimePassed = otherGoComp02.OnTimePassed + {self, self.BindingExample};
	otherGoComp02.OnTimePassed = otherGoComp02.OnTimePassed + {nil, BindingFunction};
	--otherGoComp02.OnTimePassed:suscribe( {self, self.BindingExample} );
	--otherGoComp02.OnTimePassed:suscribe( {nil, BindingFunction} );

	--Multicast, unsuscribe
	--otherGoComp02.OnTimePassed:unsuscribe({self, self.BindingExample});
	--otherGoComp02.OnTimePassed = otherGoComp02.OnTimePassed - {self, self.BindingExample};

	--TRANSFORM
	local transform = owner:GetTransformComp();
	if (transform ~= nil) then
		OutputPrint(">>> Transform component retrieval succesful!\n");
		local pos = transform:GetPosition();
		local rot = transform:GetRotation();
		local scale = transform:GetScale();
		OutputPrint(">>> transform position :  " .. pos.x .. ", " .. pos.y .. ", " .. pos.z .. " \n");
		OutputPrint(">>> transform rotation :  " .. rot.x .. ", " .. rot.y .. ", " .. rot.z .. " \n");
		OutputPrint(">>> transform scale    :  " .. scale.x .. ", " .. scale.y .. ", " .. scale.z .. " \n");
		transform:Translate(Vector3.new(-1, -1, -1));
		OutputPrint(">>> transform position :  " .. pos.x .. ", " .. pos.y .. ", " .. pos.z .. " \n");
	end


end


--Update called every tick
test01Comp.Update = function(self, dt, owner) 

	if (owner:GetTag() == "Andres") then
		--OutputPrint("\n\nUPDATE - Andres got rank up. Andres is a little shit");
		self.ranked = self.ranked + 1;
	end

	--OutputPrint("\nUPDATE - GO tag: " .. owner:GetTag() .. " - ScriptName: " .. 
	--self.name .. " - DT: " .. dt .. " - RANK: " .. self.ranked .. ", Hash: " .. 
	--self.hashed .. "\n");

end

--Method
test01Comp.BindingExample = function(self, msg, time)
	OutputPrint("\n>>>>>>>> called from elsewhere -< " .. msg .. " >- at time " .. time .. "by " .. self.hashed .. "\n");
	return 7;
end

return test01Comp;