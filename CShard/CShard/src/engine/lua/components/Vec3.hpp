#pragma once

#include <lua.hpp>
#include <stdexcept>
#include <string>
#include <vec3.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtx/string_cast.hpp>
#include <geometric.hpp>

namespace CSLua
{
	class Vec3
	{
	public:
		static void registerFuncs(lua_State* L)
		{
			lua_newtable(L);
			int tableIdx = lua_gettop(L);
			lua_pushvalue(L, tableIdx);
			lua_setglobal(L, "Vec3");

			lua_pushcfunction(L, create);
			lua_setfield(L, -2, "new");
			lua_pushcfunction(L, add);
			lua_setfield(L, -2, "add");
			lua_pushcfunction(L, sub);
			lua_setfield(L, -2, "sub");
			lua_pushcfunction(L, mul);
			lua_setfield(L, -2, "mul");
			lua_pushcfunction(L, div);
			lua_setfield(L, -2, "div");
			lua_pushcfunction(L, unm);
			lua_setfield(L, -2, "invert");
			lua_pushcfunction(L, len);
			lua_setfield(L, -2, "length");
			lua_pushcfunction(L, normalize);
			lua_setfield(L, -2, "norm");
			lua_pushcfunction(L, rotate);
			lua_setfield(L, -2, "rotate");

			luaL_newmetatable(L, "Vec3MT");

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, index);
			lua_settable(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);

			lua_pushstring(L, "__tostring");
			lua_pushcfunction(L, tostring);
			lua_settable(L, -3);

			lua_pushstring(L, "__add");
			lua_pushcfunction(L, add);
			lua_settable(L, -3);

			lua_pushstring(L, "__sub");
			lua_pushcfunction(L, sub);
			lua_settable(L, -3);

			lua_pushstring(L, "__mul");
			lua_pushcfunction(L, mul);
			lua_settable(L, -3);

			lua_pushstring(L, "__div");
			lua_pushcfunction(L, div);
			lua_settable(L, -3);

			lua_pushstring(L, "__unm");
			lua_pushcfunction(L, unm);
			lua_settable(L, -3);

			lua_pushstring(L, "__len");
			lua_pushcfunction(L, len);
			lua_settable(L, -3);
		}

		static void createUD(lua_State* L, glm::vec3 elem)
		{
			glm::vec3* ptr = (glm::vec3*)lua_newuserdata(L, sizeof(glm::vec3));
			*ptr = elem;
			luaL_getmetatable(L, "Vec3MT");
			assert(lua_istable(L, -1));
			lua_setmetatable(L, -2);
		}

		static int create(lua_State* L)
		{
			float x = (float)lua_tonumber(L, -3);
			float y = (float)lua_tonumber(L, -2);
			float z = (float)lua_tonumber(L, -1);

			createUD(L, glm::vec3(x, y, z));
			return 1;
		}
		static int add(lua_State* L)
		{
			if (lua_isnumber(L, -1))
			{
				float other = (float)lua_tonumber(L, -1);
				glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -2);
				glm::vec3 tmp = *orig + other;
				createUD(L, tmp);
				return 1;
			}
			if (lua_isuserdata(L, -1))
			{
				glm::vec3* other = (glm::vec3*)lua_touserdata(L, -1);
				glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -2);
				glm::vec3 tmp = *orig + *other;
				createUD(L, tmp);
				return 1;
			}
			throw std::runtime_error("invalid argument in operator add for ");
		}
		static int sub(lua_State* L)
		{
			if (lua_isnumber(L, -1))
			{
				float other = (float)lua_tonumber(L, -1);
				glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -2);
				glm::vec3 tmp = *orig - other;
				createUD(L, tmp);
				return 1;
			}
			if (lua_isuserdata(L, -1))
			{
				glm::vec3* other = (glm::vec3*)lua_touserdata(L, -1);
				glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -2);
				glm::vec3 tmp = *orig - *other;
				createUD(L, tmp);
				return 1;
			}
			throw std::runtime_error("invalid argument in operator sub for ");
		}
		static int mul(lua_State* L)
		{
			if (lua_isnumber(L, -1))
			{
				float other = (float)lua_tonumber(L, -1);
				glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -2);
				glm::vec3 tmp = *orig * other;
				createUD(L, tmp);
				return 1;
			}
			if (lua_isuserdata(L, -1))
			{
				glm::vec3* other = (glm::vec3*)lua_touserdata(L, -1);
				glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -2);
				glm::vec3 tmp = *orig * *other;
				createUD(L, tmp);
				return 1;
			}
			throw std::runtime_error("invalid argument in operator mul for ");
		}
		static int div(lua_State* L)
		{
			if (lua_isnumber(L, -1))
			{
				float other = (float)lua_tonumber(L, -1);
				glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -2);
				glm::vec3 tmp = *orig / other;
				createUD(L, tmp);
				return 1;
			}
			if (lua_isuserdata(L, -1))
			{
				glm::vec3* other = (glm::vec3*)lua_touserdata(L, -1);
				glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -2);
				glm::vec3 tmp = *orig / *other;
				createUD(L, tmp);
				return 1;
			}
			throw std::runtime_error("invalid argument in operator div for ");
		}
		static int unm(lua_State* L)
		{
			glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -1);
			glm::vec3 tmp = (*orig) * -1.f;
			createUD(L, tmp);
			return 1;
		}
		static int len(lua_State* L)
		{
			glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -1);
			float length = glm::length(*orig);
			lua_pushnumber(L, length);
			return 1;
		}
		static int normalize(lua_State* L)
		{
			glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -1);
			glm::vec3 tmp = glm::normalize(*orig);
			createUD(L, tmp);
			return 1;
		}
		static int rotate(lua_State* L)
		{
			glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -2);
			glm::vec3* toRot = (glm::vec3*)lua_touserdata(L, -1);
			glm::vec3 tmp = *orig;
			tmp = glm::rotate(tmp, toRot->x, glm::vec3(1, 0, 0));
			tmp = glm::rotate(tmp, toRot->y, glm::vec3(0, 1, 0));
			tmp = glm::rotate(tmp, toRot->z, glm::vec3(0, 0, 1));
			createUD(L, tmp);
			return 1;
		}

		static int index(lua_State* L)
		{
			glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);
			if (strcmp(index, "x") == 0)
			{
				lua_pushnumber(L, orig->x);
				return 1;
			}
			if (strcmp(index, "y") == 0)
			{
				lua_pushnumber(L, orig->y);
				return 1;
			}
			if (strcmp(index, "z") == 0)
			{
				lua_pushnumber(L, orig->z);
				return 1;
			}
			lua_getglobal(L, "Vec3");
			lua_pushstring(L, index);
			lua_rawget(L, -2);
			return 1;
		}
		static int newindex(lua_State* L)
		{
			glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -3);
			const char* index = lua_tostring(L, -2);
			if (strcmp(index, "x") == 0)
			{
				orig->x = (float)lua_tonumber(L, -1);
				return 1;
			}
			if (strcmp(index, "y") == 0)
			{
				orig->y = (float)lua_tonumber(L, -1);
				return 1;
			}
			if (strcmp(index, "z") == 0)
			{
				orig->z = (float)lua_tonumber(L, -1);
				return 1;
			}
			throw std::runtime_error("Tried to modify non existant or protected Vec3 element");
		}
		static int tostring(lua_State* L)
		{
			glm::vec3* orig = (glm::vec3*)lua_touserdata(L, -1);
			lua_pushstring(L, glm::to_string(*orig).c_str());
			return 1;
		}
	};
}
