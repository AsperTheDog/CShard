#include "Manager.hpp"

#include <iostream>
#include "../ResourceManager.hpp"
#include "../Engine.hpp"
#include "components/LuaCam.hpp"
#include "components/LuaColl.hpp"
#include "components/LuaMod.hpp"
#include "components/LuaLi.hpp"
#include "components/LuaObj.hpp"
#include "components/LuaPhys.hpp"
#include "../../input/InputManager.hpp"

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

	void Manager::runCollideScript(uint32_t obj, uint32_t coll, uint32_t receiverObj, uint32_t receiverColl, uint32_t script)
	{
		GameObject* objPtr = ResourceManager::getObject(obj);
		lua_getglobal(L, "obj");
		GameObject** ud = (GameObject**)lua_touserdata(L, -1);
		*ud = objPtr;

		Component* compPtr = objPtr->getComponent(coll);
		lua_getglobal(L, "coll");
		Component** ud2 = (Component**)lua_touserdata(L, -1);
		*ud2 = compPtr;

		GameObject* objPtr2 = ResourceManager::getObject(receiverObj);
		lua_getglobal(L, "obj2");
		GameObject** ud3 = (GameObject**)lua_touserdata(L, -1);
		*ud3 = objPtr2;

		Component* compPtr2 = objPtr->getComponent(receiverColl);
		lua_getglobal(L, "coll2");
		Component** ud4 = (Component**)lua_touserdata(L, -1);
		*ud4 = compPtr2;

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
		uint32_t obj = lua_tonumber(L, -1);
		GameObject* objPtr = ResourceManager::getObject(obj);

		GameObject** ex = (GameObject**)lua_newuserdata(L, sizeof(GameObject*));
		*ex = objPtr;
		CSLua::LuaObj::getMetatable(L);
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
		uint32_t id = lua_tonumber(L, -1);
		lua_pushboolean(L, InputManager::triggeredThisFrame.contains(id));
		return 1;
	}

	void Manager::registerFunctions()
	{
		CSLua::Vec3::registerFuncs(L);
		CSLua::LuaCam::registerFuncs(L);
		CSLua::LuaColl::registerFuncs(L);
		CSLua::LuaLi::registerFuncs(L);
		CSLua::LuaMod::registerFuncs(L);
		CSLua::LuaObj::registerFuncs(L);
		CSLua::LuaPhys::registerFuncs(L);
	}
}
