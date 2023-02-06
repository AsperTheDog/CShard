#pragma once
#pragma once
#include "lua.hpp"

namespace CSLua
{
	class LuaLi
	{
	public:
		static void registerFuncs(lua_State* L)
		{
			lua_newtable(L);
			int tableIdx = lua_gettop(L);
			lua_pushvalue(L, tableIdx);
			lua_setglobal(L, "Light");
			
			lua_pushcfunction(L, move);
			lua_setfield(L, -2, "move");

			luaL_newmetatable(L, "LightMT");

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, index);
			lua_settable(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);

			lua_pushstring(L, "__tostring");
			lua_pushcfunction(L, tostring);
			lua_settable(L, -3);
		}
		static void getMetatable(lua_State* L)
		{
			luaL_getmetatable(L, "LightMT");
		}
		
		static int move(lua_State* L)
		{
			Light* orig = *(Light**)lua_touserdata(L, -2);
			glm::vec3* newVec = (glm::vec3*)lua_touserdata(L, -1);
			orig->position += *newVec;
			return 1;
		}

		static int index(lua_State* L)
		{
			Light* orig = *(Light**)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);
			if (strcmp(index, "pos") == 0)
			{
				LuaVec3::createUD(L, orig->position);
				return 1;
			}
			if (strcmp(index, "color") == 0)
			{
				LuaVec3::createUD(L, orig->color);
				return 1;
			}
			if (strcmp(index, "constant") == 0)
			{
				lua_pushnumber(L, orig->constant);
				return 1;
			}
			if (strcmp(index, "linear") == 0)
			{
				lua_pushnumber(L, orig->linear);
				return 1;
			}
			if (strcmp(index, "quadratic") == 0)
			{
				lua_pushnumber(L, orig->quadratic);
				return 1;
			}
			lua_getglobal(L, "Light");
			lua_pushstring(L, index);
			lua_rawget(L, -2);
			return 1;
		}
		static int newindex(lua_State* L)
		{
			Light* orig = *(Light**)lua_touserdata(L, -3);
			const char* index = lua_tostring(L, -2);
			if (strcmp(index, "pos") == 0)
			{
				glm::vec3* set = (glm::vec3*)lua_touserdata(L, -1);
				orig->position = *set;
				return 1;
			}
			if (strcmp(index, "color") == 0)
			{
				glm::vec3* set = (glm::vec3*)lua_touserdata(L, -1);
				orig->color = *set;
				return 1;
			}
			if (strcmp(index, "constant") == 0)
			{
				orig->constant = (float)lua_tonumber(L, -1);
				return 1;
			}
			if (strcmp(index, "linear") == 0)
			{
				orig->linear = (float)lua_tonumber(L, -1);
				return 1;
			}
			if (strcmp(index, "quadratic") == 0)
			{
				orig->quadratic = (float)lua_tonumber(L, -1);
				return 1;
			}
			throw std::runtime_error("Tried to modify non existant or protected Model element");
		}
		static int tostring(lua_State* L)
		{
			
			return 1;
		}
	};
}
