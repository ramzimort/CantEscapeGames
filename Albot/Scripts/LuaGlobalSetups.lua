-- Here we will define some global functions we wanna be able to access from every other script


-- Use this to copy when trying to make each go 
-- have their own copy of the script's table
function deepcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopy(orig_key)] = deepcopy(orig_value)
        end
        setmetatable(copy, deepcopy(getmetatable(orig)))
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
end


function multicast()
        
	return setmetatable
	( 
		--TABLE we return
		{
			suscribers = {},

			suscribe = function(self, entry)
				local caller = entry[1];
				local funct = entry[2];
				if self and funct then
					
					--Avoid duplicates
					for i = 1, #self.suscribers do
						local caller2 = self.suscribers[i][1];
						local method2 = self.suscribers[i][2];
						if caller2 == caller and method2 == func then
							return;
						end
					end
					
					--If not present, add it to the table
					self.suscribers[#self.suscribers + 1] = entry;

				else
					OutputPrint("\nError - table(self), usertype, function expected, got " 
						.. type(self) .. ", " .. type(entry[1]) .. ", " .. type(entry[2]) .. "\n");
				end

				return self;
			end,
			
			unsuscribe = function(self, entry)
				if self and entry then

					for i = 1, #self.suscribers do	
						local caller = self.suscribers[i][1];
						local method = self.suscribers[i][2];
						if caller == entry[1] and method == entry[2] then
							self.suscribers[i] = nil;
						end
					end

				else
					OutputPrint("Error - table(self), table expected, got " 
						.. type(self) .. ", " .. type(entry) .. "\n");
				end
			end,
		},
	
		--Metatable
		{
			__call = function(self, ...)
				if type(self.suscribers) == "table" then
					local c, r = 1, {};
					for key, val in pairs(self.suscribers) do
						if type(val) == "table" then
					
							if (val[1] ~= nil) and (type(val[2]) == "function") then
							
								r[c] = { pcall(val[2], val[1], ...) };
								c = c + 1;

							elseif type(val[2]) == "function" then
								
								r[c] = { pcall(val[2], ...) };
								c = c + 1;

							end
						end
					end
					return c - 1, r;
				end
			end,

			__add = function(self, entry)
				return self:suscribe(entry);
			end,   
				
			__sub = function(self, entry)
				return self:unsuscribe(entry);
			end,
		}
	)
end