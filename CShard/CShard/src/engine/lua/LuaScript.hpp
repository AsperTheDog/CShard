#pragma once
#include <lua.hpp>
#include <stdexcept>
#include <string>

#include "../AssetPath.hpp"

class LuaScript
{
public:
	LuaScript() = default;

	void commit(const AssetPath& source)
	{
		this->isTracked = source.isTracked;
		this->source = source.path;
		this->name = source.path.filename().string();
		this->reload();
	}

	void execute(lua_State* L)
	{
		execLua(code, L);
	}

	void reload()
	{
		std::ifstream file(this->source);
		code = std::string(std::istreambuf_iterator(file), std::istreambuf_iterator<char>());
	}

	void rename(std::filesystem::path& newName)
	{
		this->source = newName;
		this->name = newName.filename().string();
	}

	std::filesystem::path source;
	std::string name;
	bool isTracked = false;

	std::string code{};
private:

	static bool execLua(const std::string& code, lua_State* st)
	{
		int r = luaL_dostring(st, code.c_str());
		if (r != LUA_OK)
		{
			throw std::runtime_error(lua_tostring(st, -1));
		}
		return r == LUA_OK;
	}

	static bool callLua(lua_State* st, uint32_t argnum, uint32_t retnum, uint32_t something)
	{
		int r = lua_pcall(st, argnum, retnum, something);
		if (r != LUA_OK)
		{
			throw std::runtime_error(lua_tostring(st, -1));
		}
		return r == LUA_OK;
	}
};
