#pragma once
#include <cstdint>

#include "lua.hpp"

namespace CSLua
{
	class LuaColl
	{
	public:
		static void registerFuncs(lua_State* L)
		{
			lua_newtable(L);
			int tableIdx = lua_gettop(L);
			lua_pushvalue(L, tableIdx);
			lua_setglobal(L, "Collider");

			luaL_newmetatable(L, "ColliderMT");

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, index);
			lua_settable(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);
		}
		static void getMetatable(lua_State* L)
		{
			luaL_getmetatable(L, "ColliderMT");
		}

		static int index(lua_State* L)
		{
			
			return 1;
		}
		static int newindex(lua_State* L)
		{
			
			return 1;
		}
	};
}
#pragma once
