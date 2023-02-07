#pragma once

#include <cstdint>
#include <lua.hpp>

#include "LuaPhys.hpp"

namespace CSLua
{
	class LuaObj
	{
	public:
		static void registerFuncs(lua_State* L)
		{
			lua_newtable(L);
			int tableIdx = lua_gettop(L);
			lua_pushvalue(L, tableIdx);
			lua_setglobal(L, "Object");

			lua_pushcfunction(L, getComponent);
			lua_setfield(L, -2, "component");

			luaL_newmetatable(L, "ObjectMT");

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, index);
			lua_settable(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);
		}
		static void getMetatable(lua_State* L)
		{
			luaL_getmetatable(L, "ObjectMT");
		}

		static int getComponent(lua_State* L)
		{
			GameObject* obj = *(GameObject**)lua_touserdata(L, -2);
			uint32_t id = (uint32_t)lua_tonumber(L, -1);

			Component* comp = obj->getComponent(id);
			if (!comp)
			{
				lua_pushnil(L);
				return 1;
			}
			switch(comp->type)
			{
			case COMPONENT_CAMERA:
			{
				Camera** ud = (Camera**)lua_newuserdata(L, sizeof(Camera*));
				*ud = &comp->value.cam;
				LuaCam::getMetatable(L);
				lua_setmetatable(L, -2);
				break;
			}
			case COMPONENT_COLLIDER:
			{
				ColliderBase** ud = (ColliderBase**)lua_newuserdata(L, sizeof(ColliderBase*));
				*ud = &comp->value.coll;
				LuaColl::getMetatable(L);
				lua_setmetatable(L, -2);
				break;
			}
			case COMPONENT_MODEL:
			{
				Model** ud = (Model**)lua_newuserdata(L, sizeof(Model*));
				*ud = &comp->value.mod;
				LuaMod::getMetatable(L);
				lua_setmetatable(L, -2);
				break;
			}
			case COMPONENT_SCRIPT:
				std::cout << "WARNING: Tried to get script component (ID: " + std::to_string(id) + ") from Object. Null is returned instead\n";
				lua_pushnil(L);
				break;
			case COMPONENT_LIGHT:
			{
				Light** ud = (Light**)lua_newuserdata(L, sizeof(Light*));
				*ud = &comp->value.li;
				LuaLi::getMetatable(L);
				lua_setmetatable(L, -2);
				break;
			}
			case COMPONENT_BACKGROUND:
				std::cout << "Tried to get background component (ID: " + std::to_string(id) + ") from Object. Null is returned instead\n";
				lua_pushnil(L);
				break;
			}
			return 1;
		}

		static int index(lua_State* L)
		{
			GameObject* orig = *(GameObject**)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);
			if (strcmp(index, "show") == 0)
			{
				lua_pushboolean(L, orig->show);
				return 1;
			}
			if (strcmp(index, "lightCount") == 0)
			{
				lua_pushnumber(L, orig->lightCount);
				return 1;
			}
			if (strcmp(index, "transform") == 0)
			{
				PhysicalData** ud = (PhysicalData**)lua_newuserdata(L, sizeof(PhysicalData*));
				*ud = &orig->modelData;
				LuaPhys::getMetatable(L);
				lua_setmetatable(L, -2);
				return 1;
			}
			lua_getglobal(L, "Object");
			lua_pushstring(L, index);
			lua_rawget(L, -2);
			return 1;
		}
		static int newindex(lua_State* L)
		{
			GameObject* orig = *(GameObject**)lua_touserdata(L, -3);
			const char* index = lua_tostring(L, -2);
			if (strcmp(index, "show") == 0)
			{
				orig->show = lua_toboolean(L, -1);
				return 1;
			}
			throw std::runtime_error("Tried to modify non existant or protected Object element");
		}
	};
}


