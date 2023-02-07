#pragma once
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vec3.hpp>

#include "../PhysicalData.hpp"

struct Component;
class GameObject;
struct Collider;

enum ColliderType
{
	COLLIDER_SPHERE,
	COLLIDER_AABB,
	COLLIDER_CAPSULE
};

class SphereCollider final
{
public:
	bool testCollisions(PhysicalData pData, Collider* other, PhysicalData otherPData);
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	float radius{};
};

class OBBCollider final
{
public:
	bool testCollisions(PhysicalData pData, Collider* other, PhysicalData otherPData);
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	glm::vec3 size{};
};

class CapsuleCollider final
{
public:
	bool testCollisions(PhysicalData pData, Collider* other, PhysicalData otherPData);
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	float height{};
	float radius{};
};

struct Collider
{
	union CollData
	{
		SphereCollider sph;
		OBBCollider aabb;
		CapsuleCollider cap;
	};

	uint32_t enterScript{};
	uint32_t insideScript{};
	uint32_t exitScript{};
	uint32_t outsideScript{};

	ColliderType type;
	CollData data;
	bool draw = false;
	bool isStatic = true;
	glm::vec3 position{ 0 };

	Collider();
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	bool testCollision(PhysicalData pData, Collider* other, PhysicalData otherPData);
};

struct ColliderStructNode
{
	Component* coll;
	GameObject* obj;

	bool operator==(const ColliderStructNode& other) const
	{
		return this->coll == other.coll && this->obj == other.obj;
	}
};

struct ColliderInteraction
{
	Component* c1;
	Component* c2;

	bool operator==(const ColliderInteraction& other) const
	{
		return (c1 == other.c1 && c2 == other.c2) || (c2 == other.c1 && c1 == other.c2);

	}
};

struct ColliderStructure
{
	static void init();

	static void add(GameObject* obj, Component* coll);
	static void remove(GameObject* obj, Component* coll);

	static void test();

	inline static std::unordered_set<ColliderStructNode> colliders;
	inline static std::unordered_map<ColliderInteraction, bool> lastColls;
};

template <class T>
void hash_combine(std::size_t& s, const T& v)
{
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

template <>
struct std::hash<ColliderInteraction>
{
	std::size_t operator()(const ColliderInteraction& s) const noexcept
	{
		std::size_t res = 0;
		if (s.c1 > s.c2)
		{
			hash_combine(res, s.c1);
			hash_combine(res, s.c2);
		}
		else
		{
			hash_combine(res, s.c2);
			hash_combine(res, s.c1);
		}
		return res;
	}
};