#include "gmodpacket.h"
#include "tier1/bitbuf.h"
#include <array>
#include "tier1/interface.h"

using namespace netmn;
using namespace GarrysMod::Lua;

std::unordered_map<ILuaBase*, int> GModPacket::type_map;

class IVEngineClient;

class GModPacket::Class {
public:
	Class() {
		write.StartWriting(data.data(), data.size());
	}

public:
	bf_write write;
	std::array<char, 0xffff> data;
};

using luaL_ref = struct {
	const char* name;
	int (*function)(lua_State*);
};

#define entry(n) {#n, GModPacket:: n },
static luaL_ref library[] = {
	GModPacket_Functions(entry)
	{nullptr, nullptr}
};

void GModPacket::Open(ILuaBase* LUA) {
	GModPacket::type_map[LUA] = LUA->CreateMetaTable("GModPacket");
	LUA->Push(-1);
	LUA->SetField(-2, "__index");

	for (luaL_ref* func = library; func->name; func++) {
		LUA->PushCFunction(func->function);
		LUA->SetField(-2, func->name);
	}

	LUA->Pop(1);

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->PushCFunction(GModPacket::GModPacket);
		LUA->SetField(-2, "GModPacket");
	LUA->Pop(1);
}

void GModPacket::Close(ILuaBase* LUA) {
	GModPacket::type_map.erase(LUA);
}

GModPacket::Class* GModPacket::New(ILuaBase* LUA) {
	LUA->CreateMetaTable("GModPacket");
	GModPacket::Class data;
	LUA->PushUserType_Value(data, GModPacket::type_map[LUA]);
	auto ret = Get(LUA, -1);
	LUA->Remove(LUA->Top() - 1);
	return ret;
}

GModPacket::Class* GModPacket::Get(ILuaBase* LUA, int stack) {
	auto dat = LUA->GetUserType<GModPacket::Class>(stack, GModPacket::type_map[LUA]);
	if (!dat) {
		LUA->ArgError(stack, "expected GModPacket");
	}

	return dat;
}


int GModPacket::__gc__Imp(ILuaBase* LUA) {
	return 0;
}

int GModPacket::__tostring__Imp(ILuaBase* LUA) {
	LUA->PushString("GModPacket");
	return 1;
}

int GModPacket::WriteUInt__Imp(ILuaBase* LUA) {
	GModPacket::Get(LUA, 1)->write.WriteUBitLong(LUA->CheckNumber(2), LUA->CheckNumber(3));
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteInt__Imp(ILuaBase* LUA) {
	GModPacket::Get(LUA, 1)->write.WriteBitLong(LUA->CheckNumber(2), LUA->CheckNumber(3), true);
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteFloat__Imp(ILuaBase* LUA) {
	GModPacket::Get(LUA, 1)->write.WriteFloat(LUA->CheckNumber(2));
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteDouble__Imp(ILuaBase* LUA) {
	double num = LUA->CheckNumber(2);
	GModPacket::Get(LUA, 1)->write.WriteBits(&num, sizeof(num) * 8);
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteBool__Imp(ILuaBase* LUA) {
	GModPacket::Get(LUA, 1)->write.WriteOneBit(LUA->GetBool(2) ? 1 : 0);
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteByte__Imp(ILuaBase* LUA) {
	GModPacket::Get(LUA, 1)->write.WriteByte(LUA->CheckNumber(2));
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteBit__Imp(ILuaBase* LUA) {
	GModPacket::Get(LUA, 1)->write.WriteByte(LUA->CheckNumber(2));
	GModPacket::Get(LUA, 1);
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteString__Imp(ILuaBase* LUA) {
	GModPacket::Get(LUA, 1)->write.WriteString(LUA->CheckString(2));
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteData__Imp(ILuaBase* LUA) {
	LUA->CheckString(2);
	unsigned int len;
	auto data = LUA->GetString(2, &len);
	if (LUA->IsType(3, Type::Number)) {
		auto newlen = LUA->CheckNumber(3);
		if (newlen <= len && newlen > 0) {
			len = newlen;
		}
	}
	GModPacket::Get(LUA, 1)->write.WriteBytes(data, len);
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteVector__Imp(ILuaBase* LUA) {
	GModPacket::Get(LUA, 1)->write.WriteBitVec3Coord(*LUA->GetUserType<Vector>(2, Type::Vector));
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteAngle__Imp(ILuaBase* LUA) {
	GModPacket::Get(LUA, 1)->write.WriteBitAngles(*LUA->GetUserType<QAngle>(2, Type::Angle));
	LUA->Push(1);
	return 1;
}

int GModPacket::WriteNormal__Imp(ILuaBase* LUA) {
	GModPacket::Get(LUA, 1)->write.WriteBitVec3Normal(*LUA->GetUserType<Vector>(2, Type::Vector));
	LUA->Push(1);
	return 1;
}

int GModPacket::Send__Imp(ILuaBase* LUA) {
	auto fn = Sys_GetFactory("engine.dll"); 
	int ret;
	auto engine = (IVEngineClient *)fn("VEngineClient015", &ret);
	if (!engine) {
		return 0;
	}

	auto data = Get(LUA, 1);
	bool reliable = true;
	if (LUA->Top() > 1)
		reliable = LUA->GetBool(2);

	/*CEngineClient::GMOD_SendToServer*/
	typedef void(__thiscall* OriginalFn)(IVEngineClient* ths, const void* data, unsigned int size, bool reliable);
	OriginalFn((*(void ***)engine)[133])(engine, data->write.m_pData, data->write.GetNumBitsWritten(), reliable);
	return 0;
}

int GModPacket::GModPacket__Imp(ILuaBase* LUA) {
	GModPacket::New(LUA);
	return 1;
}