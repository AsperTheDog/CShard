#pragma once
#include <unordered_set>
#include <vec3.hpp>

#include "Camera.hpp"

#include "Script.hpp"
#include "../../device/graphics/Mesh.hpp"

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
	bool testCollisions(PhysicalData& pData, Collider* other, PhysicalData& otherPData);
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);
	glm::mat4 getModelMatrix(PhysicalData& pData, glm::vec3& pos);

	float radius{};
};

class OBBCollider final
{
public:
	bool testCollisions(PhysicalData& pData, Collider* other, PhysicalData& otherPData);
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);
	glm::mat4 getModelMatrix(PhysicalData& pData, glm::vec3& pos);

	glm::vec3 size{};
};

class CapsuleCollider final
{
public:
	bool testCollisions(PhysicalData& pData, Collider* other, PhysicalData& otherPData);
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);
	std::tuple<glm::mat4, glm::mat4, glm::mat4> getModelMatrix(PhysicalData& pData, glm::vec3& pos);

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

	uint32_t scripts[4] = {0, 0, 0, 0};
	int tempScripts[4] = {0, 0, 0, 0};

	ColliderType type;
	bool draw = false;
	bool isStatic = true;
	glm::vec3 position{ 0 };
	CollData data;

	static void init();

	Collider(ColliderType type);
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	void renderSelection(PhysicalData& pData, Camera& cam);
	bool testCollision(PhysicalData& pData, Collider* other, PhysicalData& otherPData);
	void renderImGuiFields(std::string id);

	inline static Mesh sphereMesh{};
	inline static Mesh cubeMesh{};
	inline static Mesh meshCyl{};
	inline static Mesh meshSph{};
};

class CollisionNode
{
public:
	CollisionNode(GameObject* obj, Component* coll) : obj(obj), coll(coll){}
	ScriptType updateCollisionHistory(Component* otherColl, bool isInside);

	GameObject* obj;
	Component* coll;

	std::unordered_set<Component*> collsInside;
};

class CollisionStructure
{
public:
	static void addCollider(GameObject* obj, Component* coll);
	static void removeCollider(Component* coll);

	static void testCollisions();

	inline static std::vector<CollisionNode> colliders;
};