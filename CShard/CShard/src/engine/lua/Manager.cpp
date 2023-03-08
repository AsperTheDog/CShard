#include "Manager.hpp"

#include <iostream>
#include "../ResourceManager.hpp"
#include "../Engine.hpp"
#include "components/LuaCam.hpp"
#include "components/LuaColl.hpp"
#include "components/LuaMod.hpp"
#include "components/LuaLi.hpp"
#include "components/LuaObj.hpp"
#include "../../input/InputManager.hpp"
#include "components/LuaPost.hpp"

namespace CSLua
{
	void Manager::init()
	{
		L = luaL_newstate();
		luaL_openlibs(L);

		registerFunctions();

		GameObject** ex = (GameObject**)lua_newuserdata(L, sizeof(GameObject*));
		*ex = nullptr;
		CSLua::LuaObj::getMetatable(L);
		lua_setmetatable(L, -2);

		lua_setglobal(L, "obj");

		ex = (GameObject**)lua_newuserdata(L, sizeof(GameObject*));
		*ex = nullptr;
		CSLua::LuaObj::getMetatable(L);
		lua_setmetatable(L, -2);

		lua_setglobal(L, "obj2");

		Component** comp = (Component**)lua_newuserdata(L, sizeof(Component*));
		*comp = nullptr;
		CSLua::LuaColl::getMetatable(L);
		lua_setmetatable(L, -2);

		lua_setglobal(L, "coll");

		comp = (Component**)lua_newuserdata(L, sizeof(Component*));
		*comp = nullptr;
		CSLua::LuaColl::getMetatable(L);
		lua_setmetatable(L, -2);

		lua_setglobal(L, "coll2");

		lua_pushcfunction(L, lua_getObject);
		lua_setglobal(L, "getObject");
		lua_pushcfunction(L, lua_getdt);
		lua_setglobal(L, "getdt");
		lua_pushcfunction(L, lua_getPost);
		lua_setglobal(L, "getPost");
		lua_pushcfunction(L, lua_checkInput);
		lua_setglobal(L, "getInput");
	}

	void Manager::reset()
	{
		lua_close(L);
		Manager::init();
	}

	void Manager::runObjScript(uint32_t obj, uint32_t script)
	{
		GameObject* objPtr = ResourceManager::getObject(obj);
		lua_getglobal(L, "obj");
		GameObject** ud = (GameObject**)lua_touserdata(L, -1);
		*ud = objPtr;

		try {
			if (!ResourceManager::isValidScript(script)) throw std::runtime_error("Tried to run invalid id script: " + std::to_string(script));
			ResourceManager::getScript(script)->execute(L);
		}
		catch (std::runtime_error& e)
		{
			std::cout << "[LUA: " << script << "]: " << e.what() << "\n";
		}
	}

	void Manager::runCollideScript(GameObject* obj, Component* coll, GameObject* receiverObj, Component* receiverColl, uint32_t script)
	{
		lua_getglobal(L, "obj");
		GameObject** ud = (GameObject**)lua_touserdata(L, -1);
		*ud = obj;

		lua_getglobal(L, "coll");
		Component** ud2 = (Component**)lua_touserdata(L, -1);
		*ud2 = coll;

		lua_getglobal(L, "obj2");
		GameObject** ud3 = (GameObject**)lua_touserdata(L, -1);
		*ud3 = receiverObj;

		lua_getglobal(L, "coll2");
		Component** ud4 = (Component**)lua_touserdata(L, -1);
		*ud4 = receiverColl;

		try {
			if (!ResourceManager::isValidScript(script)) throw std::runtime_error("Tried to run invalid id script: " + std::to_string(script));
			ResourceManager::getScript(script)->execute(L);
		}
		catch (std::runtime_error& e)
		{
			std::cout << "[LUA: " << script << "]: " << e.what() << "\n";
		}
	}

	int Manager::lua_getObject(lua_State* L)
	{
		uint32_t obj = (uint32_t)lua_tonumber(L, -1);
		GameObject* objPtr = ResourceManager::getObject(obj);
		if (!objPtr)
		{
			lua_pushnil(L);
			return 1;
		}

		GameObject** ex = (GameObject**)lua_newuserdata(L, sizeof(GameObject*));
		*ex = objPtr;
		CSLua::LuaObj::getMetatable(L);
		lua_setmetatable(L, -2);

		return 1;
	}

	int Manager::lua_getPost(lua_State* L)
	{
		uint32_t pst = (uint32_t)lua_tonumber(L, -1);
		PostEffect* postPtr = ResourceManager::getPost(pst);
		if (!postPtr)
		{
			lua_pushnil(L);
			return 1;
		}

		PostEffect** ex = (PostEffect**)lua_newuserdata(L, sizeof(PostEffect*));
		*ex = postPtr;
		CSLua::LuaPost::getMetatable(L);
		lua_setmetatable(L, -2);

		return 1;
	}

	int Manager::lua_getdt(lua_State* L)
	{
		lua_pushnumber(L, Engine::dt);
		return 1;
	}

	int Manager::lua_checkInput(lua_State* L)
	{
		const char* id = lua_tostring(L, -1);
		lua_pushboolean(L, InputManager::triggeredThisFrame.contains(id));
		return 1;
	}

	void Manager::registerFunctions()
	{
		CSLua::LuaVec3::registerFuncs(L);
		CSLua::LuaCam::registerFuncs(L);
		CSLua::LuaColl::registerFuncs(L);
		CSLua::LuaLi::registerFuncs(L);
		CSLua::LuaMod::registerFuncs(L);
		CSLua::LuaObj::registerFuncs(L);
		CSLua::LuaPhys::registerFuncs(L);
		CSLua::LuaPost::registerFuncs(L);
	}
}
