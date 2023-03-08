#pragma once
#include <glm.hpp>
#include <lua.hpp>

#include "LuaVec3.hpp"

class BezierCurve {
public:
	BezierCurve(
		const glm::vec3& start,
		const glm::vec3& p0,
		const glm::vec3& p1,
		const glm::vec3& p2,
		const glm::vec3& p3
	) : controlPoints{ p0, p1, p2, p3 }, start(start) {}

	glm::vec3 curvePoint(float t) const {
		glm::vec4 T{ t * t * t, t * t, t, 1.0f };
		glm::mat4x3 P{
		  controlPoints[0],
		  controlPoints[1],
		  controlPoints[2],
		  controlPoints[3]
		};

		glm::vec3 result = P * bezier_matrix_ * T + start;
		return result;
	}
	
	glm::vec3 controlPoints[4];
	glm::vec3 start;
private:

	glm::mat4 bezier_matrix_{
	  {-1.0f,  3.0f, -3.0f, 1.0f},
	  { 3.0f, -6.0f,  3.0f, 0.0f},
	  {-3.0f,  3.0f,  0.0f, 0.0f},
	  { 1.0f,  0.0f,  0.0f, 0.0f}
	};
};

namespace CSLua
{
	class LuaSpline
	{
	public:
		static void registerFuncs(lua_State* L)
		{
			lua_newtable(L);
			int tableIdx = lua_gettop(L);
			lua_pushvalue(L, tableIdx);
			lua_setglobal(L, "Spline");
			
			lua_pushcfunction(L, calculate);
			lua_setfield(L, -2, "calculate");

			luaL_newmetatable(L, "SplineMT");

			lua_pushstring(L, "__index");
			lua_pushcfunction(L, index);
			lua_settable(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, newindex);
			lua_settable(L, -3);
		}
		static void getMetatable(lua_State* L)
		{
			luaL_getmetatable(L, "SplineMT");
		}

		static int create(lua_State* L)
		{
			glm::vec3* p1 = (glm::vec3*)lua_touserdata(L, -4);
			glm::vec3* p2 = (glm::vec3*)lua_touserdata(L, -3);
			glm::vec3* p3 = (glm::vec3*)lua_touserdata(L, -2);
			glm::vec3* p4 = (glm::vec3*)lua_touserdata(L, -1);

			BezierCurve* ptr = (BezierCurve*)lua_newuserdata(L, sizeof(BezierCurve));
			new (ptr) BezierCurve{glm::vec3(0), *p1, *p2, *p3, *p4};
			getMetatable(L);
			lua_setmetatable(L, -2);
			return 1;
		}
		static int index(lua_State* L)
		{
			BezierCurve* orig = (BezierCurve*)lua_touserdata(L, -2);
			const char* index = lua_tostring(L, -1);
			if (strcmp(index, "p1") == 0)
			{
				LuaVec3::createUD(L, orig->controlPoints[0]);
				return 1;
			}
			if (strcmp(index, "v2") == 0)
			{
				LuaVec3::createUD(L, orig->controlPoints[1]);
				return 1;
			}
			if (strcmp(index, "v3") == 0)
			{
				LuaVec3::createUD(L, orig->controlPoints[2]);
				return 1;
			}
			if (strcmp(index, "v4") == 0)
			{
				LuaVec3::createUD(L, orig->controlPoints[3]);
				return 1;
			}
			if (strcmp(index, "start") == 0)
			{
				LuaVec3::createUD(L, orig->start);
				return 1;
			}
			lua_getglobal(L, "Spline");
			lua_pushstring(L, index);
			lua_rawget(L, -2);
			return 1;
		}
		static int newindex(lua_State* L)
		{
			BezierCurve* orig = (BezierCurve*)lua_touserdata(L, -3);
			const char* index = lua_tostring(L, -2);
			glm::vec3* set = (glm::vec3*)lua_touserdata(L, -1);
			if (strcmp(index, "p1") == 0)
			{
				orig->controlPoints[0] = *set;
				return 1;
			}
			if (strcmp(index, "v2") == 0)
			{
				orig->controlPoints[1] = *set;
				return 1;
			}
			if (strcmp(index, "v3") == 0)
			{
				orig->controlPoints[2] = *set;
				return 1;
			}
			if (strcmp(index, "v4") == 0)
			{
				orig->controlPoints[3] = *set;
				return 1;
			}
			if (strcmp(index, "start") == 0)
			{
				orig->start = *set;
				return 1;
			}
			throw std::runtime_error("Tried to modify non existant or protected Spline element");
		}

		static int calculate(lua_State* L)
		{
			BezierCurve* cam = (BezierCurve*)lua_touserdata(L, -2);
			float t = (float)lua_tonumber(L, -1);
			glm::vec3 res = cam->curvePoint(t);
			LuaVec3::createUD(L, res);
			return 1;
		}
	};
}
