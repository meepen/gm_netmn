#include "GarrysMod/Lua/Interface.h"
#include <memory>
#include <unordered_map>

#define GModPacket_Functions(fn) \
		fn(__gc) \
		fn(__tostring) \
		fn(WriteUInt) \
		fn(WriteInt) \
		fn(WriteFloat) \
		fn(WriteDouble) \
		fn(WriteBool) \
		fn(WriteByte) \
		fn(WriteBit) \
		fn(WriteString) \
		fn(WriteData) \
		fn(WriteVector) \
		fn(WriteAngle) \
		fn(WriteNormal) \
		fn(Send)

#define GModPacket_Function_Run(x) LUA_FUNCTION(x);
namespace netmn {
	namespace GModPacket {
		class Class;
		extern std::unordered_map<GarrysMod::Lua::ILuaBase*, int> type_map;
		void Open(GarrysMod::Lua::ILuaBase* LUA);
		void Close(GarrysMod::Lua::ILuaBase* LUA);


		Class* New(GarrysMod::Lua::ILuaBase* LUA);
		Class* Get(GarrysMod::Lua::ILuaBase *LUA, int stack);

		LUA_FUNCTION(GModPacket);

		GModPacket_Functions(GModPacket_Function_Run)
	};
};