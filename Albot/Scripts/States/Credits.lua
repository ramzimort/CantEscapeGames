--TEMPLATE FOR A SCRIPTED STATE


Credits = 
{
	name = "Credits"
}

--OnCreate, called when creating this state
Credits.OnCreateState = function(self)
	
	OutputPrint("Created " .. self.name .. " state");

end


--YOU CAN ADD LOCAL METHODS LIKE THIS ONE
Credits.OnExitState = function(self) 

	OutputPrint("Exiting (destroying) " .. self.name .. " state");

end


--AT THE END, NEED TO ADD THIS LINE
return Credits;
