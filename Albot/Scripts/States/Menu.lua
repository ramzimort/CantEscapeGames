--TEMPLATE FOR A SCRIPTED STATE


Menu = 
{
	name = "Menu"
}

--OnCreate, called when creating this state
Menu.OnCreateState = function(self)
	local EventMgr = EventManager.Get();
	EventMgr:PushLoadedState(false);
	--EventMgr:PlaySong(false, "Assets\\Songs\\Main_Menu_2.mp3")
end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Menu.OnExitState = function(self) 
end

--AT THE END, NEED TO ADD THIS LINE
return Menu;
