#pragma once
#include <cstdint>

#include "lua.hpp"
#include "LuaPhys.hpp"

namespace CSLua
{
	class LuaMod
	{
	public:
		static void registerFuncs(lua_State* L)
		{
			lua_newtable(L);
			int tableIdx = lua_gettop(L);
			lua_pushvalue(L, tableIdx);
			lua_setglobal(L, "Model");

			luaL_newmetatable(L, "ModelMT");

			lua_pushcfunction(L, reload);
			lua_setfield(L, -2, "reload");

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, index);
			lua_settable(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);
		}
		static void getMetatable(lua_State* L)
		{
			luaL_getmetatable(L, "ModelMT");
		}

		static int reload(lua_State* L)
		{
			Model* orig = *(Model**)lua_touserdata(L, -1);
			orig->changeMesh();
			orig->changeTexture();
			return 1;
		}

		static int index(lua_State* L)
		{
			Model* orig = *(Model**)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);
			if (strcmp(index, "mesh") == 0)
			{
				lua_pushnumber(L, orig->meshID);
				return 1;
			}
			if (strcmp(index, "texture") == 0)
			{
				lua_pushnumber(L, orig->textureID);
				return 1;
			}
			if (strcmp(index, "transform") == 0)
			{
				PhysicalData** ud = (PhysicalData**)lua_newuserdata(L, sizeof(PhysicalData*));
				*ud = &orig->data;
				LuaPhys::getMetatable(L);
				lua_setmetatable(L, -2);
				return 1;
			}
			lua_getglobal(L, "Model");
			lua_pushstring(L, index);
			lua_rawget(L, -2);
			return 1;
		}
		static int newindex(lua_State* L)
		{
			Model* orig = *(Model**)lua_touserdata(L, -3);
			const char* index = lua_tostring(L, -2);
			if (strcmp(index, "mesh") == 0)
			{
				orig->tempMeshID = (int)lua_tonumber(L, -1);
				return 1;
			}
			if (strcmp(index, "texture") == 0)
			{
				orig->tempTexID = (int)lua_tonumber(L, -1);
				return 1;
			}
			throw std::runtime_error("Tried to modify non existant or protected Model element");
		}
	};
}
