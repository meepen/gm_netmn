## net_mn
### A Garry's Mod net message module for the menu state

---
Examples:
Using [gm_stringtable](https://github.com/danielga/gm_stringtable)

```lua
require "netmn"
require "stringtable"

local function getnet()
	local table = stringtable.Find "networkstring"
	if (not table) then
		return {}
	end

	return table:GetStrings()
end

function util.NetworkIDToString(id)
	return getnet()[id]
end

function util.NetworkStringToID(name)
	for id, str in pairs(getnet()) do
		if (str == name) then
			return id
		end
	end
end

function StartNetMessage(name)
	local pkt = GModPacket()
	pkt:WriteByte(0)
	pkt:WriteUInt(util.NetworkStringToID(name), 16)
	return pkt
end
```

```lua
StartNetMessage "net_name":WriteFloat(1):Send()
```