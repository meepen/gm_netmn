#include "GarrysMod/Lua/Interface.h"
#include "gmodpacket.h"
#include <cstring>

GMOD_MODULE_OPEN() {
	netmn::GModPacket::Open(LUA);
	return 0;
}

GMOD_MODULE_CLOSE() {
	netmn::GModPacket::Close(LUA);
	return 0;
}