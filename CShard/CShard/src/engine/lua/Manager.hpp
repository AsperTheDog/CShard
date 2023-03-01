#pragma once

#include <cstdint>
#include <lua.hpp>

#include "../../elements/GameObject.hpp"

namespace CSLua
{
	class Manager
	{
	public:
		static void init();
		static void reset();

		static void runObjScript(uint32_t obj, uint32_t script);
		static void runCollideScript(GameObject* obj, Component* coll, GameObject* receiverObj, Component* receiverColl, uint32_t script);
	private:
		static int lua_getObject(lua_State* L);
		static int lua_getPost(lua_State* L);
		static int lua_getdt(lua_State* L);
		static int lua_checkInput(lua_State* L);

		static void registerFunctions();

		inline static lua_State* L;
	};

}


