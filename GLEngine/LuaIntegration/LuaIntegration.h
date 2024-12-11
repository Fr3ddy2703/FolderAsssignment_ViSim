#pragma once
#include "../Components/Entity/Entity.h"
// include Lua, assumes it is local to this file
extern "C"
{
#include "../Libraries/lua54/include/lua.h"
#include "../Libraries/lua54/include/lauxlib.h"
#include "../Libraries/lua54/include/lualib.h"
}

// Link to lua library
#ifdef _WIN32
#pragma comment(lib, "Libraries/lua54/lua54.lib")
#endif

class LuaIntegration
{
public:
	lua_State* L;
	void lua();

	static int createEntity(lua_State* _L);

	std::vector<Entity> entities;

};

