-- First approximation of a component script

test01Comp = 
{
	name = "test01Comp";

	--Custom variables you can add
	ranked = -1;
	isDumb = false;
	hashed = "_empty_";

	owner_temp = nil;

	shitPos = Vector3.new(0);
	shouldPrint = false;
}

--Init called when comp is created
test01Comp.Init = function(self)
	--OutputPrint("\nCOMP INIT - " .. self.name .. "\n");
end

--LOCAL Function (not a method, no self param). 
-- for it to be binded, it needs to be defined up here, 
-- or at least be declared as a local before the binding
local BindingFunction = function(msg, time)
	--OutputPrint("\n>>>>>>>> called from elsewhere -< " .. msg .. " >- at time " .. time .. ".\n");
	return 17;
end

--LOCAL Function (not a method, no self param)
local BindingFunction02 = function(obj, time, position)
	--OutputPrint("\n>>>>>>>> called from elsewhere -< " .. obj:GetTag() .. " >- at time " .. time .. ".\n");
	--OutputPrint("\n>>>>>>>> Vector param: -< " .. position.x .. ", " .. position.y .. ", " .. position.z .. " >-\n");
end


--Begin called when obj has all comps
test01Comp.Begin = function(self, owner, goMgr)

	-----------------------
	--  STATES  CHANGES  --
	-----------------------
	--local world = World.Get();
	--world.StateManager:SwitchState("");
	--world.StateManager:PushState("nameOfState");

	
	--OutputPrint("\n<<-- " .. self.name .. " BEGIN IN LUA -->>\n");

	if (owner == nil) then
		OutputPrint("ERROR, OWNER IS NIL\n");
		return;
	end
	--OutputPrint(">>> Owner's Tag: " .. owner:GetTag() .. "\n");
	self.owner_temp = owner;

	--INSTANTIATION ON SCRIPT------------------------------------------

	local albert = GameObject.Instantiate(owner:Manager());

	local transform = albert:AddTransformComp();
	transform:SetLocalPosition(Vector3.new(1, 2, 3));
	local testco02 = albert:AddCustomComp("test02Comp");
	--OutputPrint(">>> Comp2 call from instantiated GO: " .. testco02:ReturnWeirdString02() .. "\n");

	--local albertucks = GameObject.Instantiate(owner:Manager(), "Asset/Prefabs/AlbertSucks.json");

	--VECTOR OPERATION-------------------------------------------------

	local v1 = Vector3.new(-1);
	local v2 = Vector3.new(1);
	local v3 = v1 + v2;
	local v4 = v2 * 12;
	local dotted = v4:dot(v1);

	--OutputPrint(">> v1     :  " .. v1.x .. ", " .. v1.y .. ", " .. v1.z .. " \n");
	--OutputPrint(">> v2     :  " .. v2.x .. ", " .. v2.y .. ", " .. v2.z .. " \n");
	--OutputPrint(">> v1 + v2:  " .. v3.x .. ", " .. v3.y .. ", " .. v3.z .. " \n");
	--OutputPrint(">> v2 * 12:  " .. v4.x .. ", " .. v4.y .. ", " .. v4.z .. " \n");
	--OutputPrint(">> v4 � v1:  " .. dotted .. " \n");

	--GETTING ANOTHER CUSTOM COMP FROM SAME OBJ------------------------

	local Comp02 = owner:GetCustomComp("test02Comp");
	if (Comp02 ~= nil) then
		--OutputPrint(">>> Comp2 Call to method: " .. Comp02:ReturnWeirdString() .. "\n");
		--OutputPrint(">>> Comp2 hashed var: " .. Comp02.hashed .. "\n");
		--OutputPrint(">>> Comp2 ranked var: " .. Comp02.ranked .. "\n");
	end

	--FIND ANOTHER GAMEOBJ USING TAG-----------------------------------

	local gameobj01 = self.owner_temp;--goMgr:FindGameObject("albertobj3");
	if (gameobj01 == nil) then
		--OutputPrint(">>> GO with tag albertobj3 not found\n");
		return;
	end
	--OutputPrint(">>>>>>>>>> Go retrieved - TAG: " .. gameobj01:GetTag() .. " -\n");

	--GETTING OTHER GO COMPONENT AND BINDING TO ITS MULTICAST----------
	local otherGoComp02 = gameobj01:GetCustomComp("test02Comp");
	if (otherGoComp02 ~= nil) then
		--OutputPrint(">>>>>>>>>> Retrieved function from other go's comp02: " .. otherGoComp02:ReturnWeirdString() .. " -\n");

		--Multicast suscribe (2 ways)
		otherGoComp02.OnTimePassed = otherGoComp02.OnTimePassed + {self, self.BindingExample};
		otherGoComp02.OnTimePassed = otherGoComp02.OnTimePassed + {nil, BindingFunction};
		--otherGoComp02.OnTimePassed:suscribe( {self, self.BindingExample} );
		--otherGoComp02.OnTimePassed:suscribe( {nil, BindingFunction} );
		--otherGoComp02.OnTimePassed:unsuscribe({self, self.BindingExample});
		--otherGoComp02.OnTimePassed = otherGoComp02.OnTimePassed - {self, self.BindingExample};
	end

	--TRANSFORM--------------------------------------------------------
	local transform = owner:GetTransformComp();
	if (transform ~= nil) then
		--OutputPrint(">>> Transform component retrieval succesful!\n");
		local pos = transform:GetPosition();
		local rot = transform:GetRotation();
		local scale = transform:GetScale();
		--OutputPrint(">>> transform position :  " .. pos.x .. ", " .. pos.y .. ", " .. pos.z .. " \n");
		--OutputPrint(">>> transform rotation :  " .. rot.x .. ", " .. rot.y .. ", " .. rot.z .. " \n");
		--OutputPrint(">>> transform scale    :  " .. scale.x .. ", " .. scale.y .. ", " .. scale.z .. " \n");
		transform:Translate(Vector3.new(-1, -1, -1));
		--OutputPrint(">>> transform position :  " .. pos.x .. ", " .. pos.y .. ", " .. pos.z .. " \n");
	end

	local rigidbody = owner:GetRigidbodyComp();
	if (rigidbody ~= nil) then
		rigidbody.OnCollision:Bind({self, self.BindingExample});
	end
	--OutputPrint("<<-- COMP " .. self.name .. " BEGIN END -->>\n");
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

	if (self.shouldPrint == true) then
		--OutputPrint("\n(UPDATE) >>>>>>>> Vector shitPos: -< " .. self.shitPos.x .. ", " .. self.shitPos.y .. ", " .. self.shitPos.z .. " >-\n");
		self.shouldPrint = false;
	end

end

--Method
test01Comp.BindingExample = function(self, go01, go02)
	
	local rigidbody = go01:GetRigidbodyComp();
	
	if (rigidbody ~= nil) then

		--local vel = Vector3.new(0,10,0);
		--rigidbody:SetVelocity(vel);
	end

	return;
end

--Method
test01Comp.MulticastExample = function(self, gameobj, time, position)
	--OutputPrint("\n>>>> CPP multicast fired from script of GO owner: " .. self.owner_temp:GetTag() .. 
	--	". \nRecved param go with tag: -< " .. gameobj:GetId() .. " >- at time -< " .. time .. " >-\n");
	--OutputPrint("\n>>>>>>>> Vector param: -< " .. position.x .. ", " .. position.y .. ", " .. position.z .. " >-\n");
	
	self.shitPos = Vector3.new(position);

	self.shouldPrint = true;
	
	return 7;
end

test01Comp.OnDestruction= function(self)
end

return test01Comp;