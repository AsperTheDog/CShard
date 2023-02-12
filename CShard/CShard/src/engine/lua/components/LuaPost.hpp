#pragma once
#include <cstring>
#include <lua.hpp>
#include <stdexcept>

#include "../../../device/graphics/PostEffects.hpp"

class PostEffect;

namespace CSLua
{
	class LuaPost
	{
	public:
		static void registerFuncs(lua_State* L)
		{
			lua_newtable(L);
			int tableIdx = lua_gettop(L);
			lua_pushvalue(L, tableIdx);
			lua_setglobal(L, "PostEffect");

			luaL_newmetatable(L, "PostMT");

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, index);
			lua_settable(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);
		}
		static void getMetatable(lua_State* L)
		{
			luaL_getmetatable(L, "PostMT");
		}

		static int index(lua_State* L)
		{
			PostEffect* orig = *(PostEffect**)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);
			if (strcmp(index, "intensity") == 0)
			{
				switch(orig->getType())
				{
				case POST_FILMGRAIN: 
					lua_pushnumber(L, ((FilmGrain*)orig)->intensity);
					break;
				case POST_FADETOBLACK: 
					lua_pushnumber(L, ((BlackFade*)orig)->mult);
					break;
				case POST_DEPTHBUFFER: 
				case POST_PIXELATE: 
				case POST_ATMOSPHERICFOG:
					lua_pushnil(L);
				}
				return 1;
			}
			if (strcmp(index, "subsample") == 0)
			{
				if (orig->getType() != POST_PIXELATE) lua_pushnil(L);
				else lua_pushnumber(L, ((Pixelate*)orig)->subsample);
				return 1;
			}
			if (strcmp(index, "fogColor") == 0 && orig->getType() == POST_ATMOSPHERICFOG)
			{
				glm::vec3* col = (glm::vec3*)lua_touserdata(L, -1);
				((Atmospheric*)orig)->color = *col;
				return 1;
			}
			if (strcmp(index, "fogDensity") == 0 && orig->getType() == POST_ATMOSPHERICFOG)
			{
				((Atmospheric*)orig)->density = (float)lua_tonumber(L, -1);
				return 1;
			}
			lua_getglobal(L, "PostEffect");
			lua_pushstring(L, index);
			lua_rawget(L, -2);
			return 1;
		}
		static int newindex(lua_State* L)
		{
			PostEffect* orig = *(PostEffect**)lua_touserdata(L, -3);
			const char* index = lua_tostring(L, -2);
			if (strcmp(index, "intensity") == 0)
			{
				switch(orig->getType())
				{
				case POST_FILMGRAIN:
					((FilmGrain*)orig)->intensity = (float)lua_tonumber(L, -1);
					break;
				case POST_FADETOBLACK: 
					((BlackFade*)orig)->mult = (float)lua_tonumber(L, -1);
					break;
				case POST_DEPTHBUFFER: 
				case POST_PIXELATE: 
				case POST_ATMOSPHERICFOG:
					throw std::runtime_error("Tried to modify non existant or protected Post element");
				}
				return 1;
			}
			if (strcmp(index, "subsample") == 0)
			{
				if (orig->getType() == POST_PIXELATE) ((Pixelate*)orig)->subsample = (int)lua_tonumber(L, -1);
				else throw std::runtime_error("Tried to modify non existant or protected Post element");
				return 1;
			}
			if (strcmp(index, "fogColor") == 0)
			{
				if (orig->getType() == POST_ATMOSPHERICFOG)
				{
					glm::vec3* col = (glm::vec3*)lua_touserdata(L, -1);
					((Atmospheric*)orig)->color = *col;
				}
				else throw std::runtime_error("Tried to modify non existant or protected Post element");
				return 1;
			}
			if (strcmp(index, "fogDensity") == 0)
			{
				if (orig->getType() == POST_ATMOSPHERICFOG) ((Atmospheric*)orig)->density = (float)lua_tonumber(L, -1);
				else throw std::runtime_error("Tried to modify non existant or protected Post element");
				return 1;
			}
			throw std::runtime_error("Tried to modify non existant or protected Object element");
		}
	};
}
