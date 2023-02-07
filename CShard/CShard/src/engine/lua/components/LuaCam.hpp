#pragma once
#include "lua.hpp"
#include "LuaVec3.hpp"
#include "../../../device/graphics/GFramework.hpp"

namespace CSLua
{
	class LuaCam
	{
	public:
		static void registerFuncs(lua_State* L)
		{
			lua_newtable(L);
			int tableIdx = lua_gettop(L);
			lua_pushvalue(L, tableIdx);
			lua_setglobal(L, "Camera");
			
			lua_pushcfunction(L, move);
			lua_setfield(L, -2, "move");
			lua_pushcfunction(L, anchor);
			lua_setfield(L, -2, "anchor");
			lua_pushcfunction(L, setActive);
			lua_setfield(L, -2, "setActive");

			luaL_newmetatable(L, "CameraMT");

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, index);
			lua_settable(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);
		}
		static void getMetatable(lua_State* L)
		{
			luaL_getmetatable(L, "CameraMT");
		}
		
		static int move(lua_State* L)
		{
			Camera* orig = *(Camera**)lua_touserdata(L, -2);
			glm::vec3* newVec = (glm::vec3*)lua_touserdata(L, -1);
			orig->pos += *newVec;
			return 1;
		}
		static int anchor(lua_State* L)
		{
			Camera* orig = *(Camera**)lua_touserdata(L, -2);
			glm::vec3* newVec = (glm::vec3*)lua_touserdata(L, -1);
			orig->lookAt(glm::normalize(*newVec - orig->pos));
			return 1;
		}

		static int index(lua_State* L)
		{
			Camera* orig = *(Camera**)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);
			if (strcmp(index, "pos") == 0)
			{
				LuaVec3::createUD(L, orig->pos);
				return 1;
			}
			if (strcmp(index, "dir") == 0)
			{
				LuaVec3::createUD(L, orig->dir);
				return 1;
			}
			if (strcmp(index, "up") == 0)
			{
				LuaVec3::createUD(L, orig->worldUp);
				return 1;
			}
			if (strcmp(index, "fov") == 0)
			{
				lua_pushnumber(L, orig->FOV);
				return 1;
			}
			if (strcmp(index, "near") == 0)
			{
				lua_pushnumber(L, orig->nearPlane);
				return 1;
			}
			if (strcmp(index, "far") == 0)
			{
				lua_pushnumber(L, orig->farPlane);
				return 1;
			}
			lua_getglobal(L, "Camera");
			lua_pushstring(L, index);
			lua_rawget(L, -2);
			return 1;
		}
		static int newindex(lua_State* L)
		{
			Camera* orig = *(Camera**)lua_touserdata(L, -3);
			const char* index = lua_tostring(L, -2);
			if (strcmp(index, "pos") == 0)
			{
				glm::vec3* vec = *(glm::vec3**)lua_touserdata(L, -1);
				orig->move(*vec);
				return 1;
			}
			if (strcmp(index, "dir") == 0)
			{
				glm::vec3* vec = *(glm::vec3**)lua_touserdata(L, -1);
				orig->lookAt(*vec);
				return 1;
			}
			if (strcmp(index, "up") == 0)
			{
				glm::vec3* vec = *(glm::vec3**)lua_touserdata(L, -1);
				orig->changeWorldUp(*vec);
				return 1;
			}
			if (strcmp(index, "fov") == 0)
			{
				orig->changeLense((float)lua_tonumber(L, -1));
				return 1;
			}
			if (strcmp(index, "near") == 0)
			{
				orig->changeLense(orig->FOV, (float)lua_tonumber(L, -1));
				return 1;
			}
			if (strcmp(index, "far") == 0)
			{
				orig->changeLense(orig->FOV, orig->nearPlane, (float)lua_tonumber(L, -1));
				return 1;
			}
			lua_getglobal(L, "Camera");
			lua_pushstring(L, index);
			lua_rawget(L, -2);
			return 1;
		}

		static int setActive(lua_State* L)
		{
			Camera* cam = *(Camera**)lua_touserdata(L, -1);
			cam->updateAspectRatio((float)GFramework::viewPortSize.x / (float)GFramework::viewPortSize.y);
			Engine::activeCam = cam;
			return 1;
		}
	};
}


