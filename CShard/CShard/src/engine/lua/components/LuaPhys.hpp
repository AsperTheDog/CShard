#pragma once
#include "lua.hpp"
#include "LuaVec3.hpp"
#include "../../../elements/PhysicalData.hpp"

namespace CSLua
{
	class LuaPhys
	{
	public:
		static void registerFuncs(lua_State* L)
		{
			lua_newtable(L);
			int tableIdx = lua_gettop(L);
			lua_pushvalue(L, tableIdx);
			lua_setglobal(L, "Transform");
			
			lua_pushcfunction(L, move);
			lua_setfield(L, -2, "move");
			lua_pushcfunction(L, rotate);
			lua_setfield(L, -2, "rotate");

			luaL_newmetatable(L, "TransformMT");

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, index);
			lua_settable(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);
		}
		static void getMetatable(lua_State* L)
		{
			luaL_getmetatable(L, "TransformMT");
		}
		
		static int move(lua_State* L)
		{
			PhysicalData* orig = *(PhysicalData**)lua_touserdata(L, -2);
			glm::vec3* newVec = (glm::vec3*)lua_touserdata(L, -1);
			orig->pos += *newVec;
			return 1;
		}
		static int rotate(lua_State* L)
		{
			PhysicalData* orig = *(PhysicalData**)lua_touserdata(L, -2);
			glm::vec3* toRot = (glm::vec3*)lua_touserdata(L, -1);
			orig->rot = glm::rotate(orig->rot, toRot->x, glm::vec3(1, 0, 0));
			orig->rot = glm::rotate(orig->rot, toRot->y, glm::vec3(0, 1, 0));
			orig->rot = glm::rotate(orig->rot, toRot->z, glm::vec3(0, 0, 1));
			return 1;
		}

		static int index(lua_State* L)
		{
			PhysicalData* orig = *(PhysicalData**)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);
			if (strcmp(index, "pos") == 0)
			{
				LuaVec3::createUD(L, orig->pos);
				return 1;
			}
			if (strcmp(index, "rot") == 0)
			{
				LuaVec3::createUD(L, orig->rot);
				return 1;
			}
			if (strcmp(index, "scale") == 0)
			{
				LuaVec3::createUD(L, orig->scale);
				return 1;
			}
			if (strcmp(index, "albedo") == 0)
			{
				LuaVec3::createUD(L, orig->albedo);
				return 1;
			}
			lua_getuservalue(L, 1);
			lua_pushvalue(L, 2);
			lua_gettable(L, -2);
			if (lua_isnil(L, -1))
			{
				lua_getglobal(L, "Transform");
				lua_pushstring(L, index);
				lua_rawget(L, -2);
			}
			return 1;
		}
		static int newindex(lua_State* L)
		{
			PhysicalData* orig = *(PhysicalData**)lua_touserdata(L, -3);
			const char* index = lua_tostring(L, -2);
			glm::vec3* set = (glm::vec3*)lua_touserdata(L, -1);
			orig->changed = true;
			if (strcmp(index, "pos") == 0)
			{
				orig->pos = *set;
				return 1;
			}
			if (strcmp(index, "rot") == 0)
			{
				orig->rot = *set;
				return 1;
			}
			if (strcmp(index, "scale") == 0)
			{
				orig->scale = *set;
				return 1;
			}
			if (strcmp(index, "albedo") == 0)
			{
				orig->albedo = *set;
				return 1;
			}
			lua_getuservalue(L, 1);	//1
			lua_pushvalue(L, 2);	//2
			lua_pushvalue(L, 3);	//3
			lua_settable(L, -3);	//1[2] = 3
		}
	};
}
