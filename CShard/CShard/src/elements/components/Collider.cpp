#include "Collider.hpp"

#include "../../engine/lua/Manager.hpp"

bool SphereCollider::testCollisions(PhysicalData& pData, Collider* other, PhysicalData& otherPData)
{
	return false;
}

void SphereCollider::serialize(std::ofstream& wf)
{
	wf.write((char*) &radius, sizeof(radius));
}

void SphereCollider::deserialize(std::ifstream& wf)
{
	wf.read((char*) &radius, sizeof(radius));
}

bool OBBCollider::testCollisions(PhysicalData& pData, Collider* other, PhysicalData& otherPData)
{
	return false;
}

void OBBCollider::serialize(std::ofstream& wf)
{
	wf.write((char*) &size, sizeof(size));
}

void OBBCollider::deserialize(std::ifstream& wf)
{
	wf.read((char*) &size, sizeof(size));
}

bool CapsuleCollider::testCollisions(PhysicalData& pData, Collider* other, PhysicalData& otherPData)
{
	return false;
}

void CapsuleCollider::serialize(std::ofstream& wf)
{
	wf.write((char*) &height, sizeof(height));
	wf.write((char*) &radius, sizeof(radius));
}

void CapsuleCollider::deserialize(std::ifstream& wf)
{
	wf.read((char*) &height, sizeof(height));
	wf.read((char*) &radius, sizeof(radius));
}

Collider::Collider(): type(COLLIDER_SPHERE), data{}
{

}

void Collider::serialize(std::ofstream& wf)
{
	wf.write((char*) &type, sizeof(type));
	wf.write((char*) &position, sizeof(position));
	wf.write((char*) &draw, sizeof(draw));
	wf.write((char*) &isStatic, sizeof(isStatic));
	wf.write((char*) &scripts, sizeof(scripts));
	switch(type)
	{
	case COLLIDER_SPHERE:
		data.sph.serialize(wf);
		break;
	case COLLIDER_AABB:
		data.aabb.serialize(wf);
		break;
	case COLLIDER_CAPSULE:
		data.cap.serialize(wf);
		break;
	}
}

void Collider::deserialize(std::ifstream& wf)
{
	wf.read((char*) &type, sizeof(type));
	wf.read((char*) &position, sizeof(position));
	wf.read((char*) &draw, sizeof(draw));
	wf.read((char*) &isStatic, sizeof(isStatic));
	wf.read((char*) &scripts, sizeof(scripts));
	for (int i = 0; i < 4; ++i) tempScripts[i] = scripts[i];

	switch(type)
	{
	case COLLIDER_SPHERE:
		data.sph.deserialize(wf);
		break;
	case COLLIDER_AABB:
		data.aabb.deserialize(wf);
		break;
	case COLLIDER_CAPSULE:
		data.cap.deserialize(wf);
		break;
	}
}

bool Collider::testCollision(PhysicalData& pData, Collider* other, PhysicalData& otherPData)
{
	switch (type)
	{
	case COLLIDER_SPHERE:
		return data.sph.testCollisions(pData, other, otherPData);
	case COLLIDER_AABB:
		return data.aabb.testCollisions(pData, other, otherPData);
	case COLLIDER_CAPSULE:
		return data.cap.testCollisions(pData, other, otherPData);
	}
	return false;
}

ScriptType CollisionNode::updateCollisionHistory(Component* otherColl, bool isInside)
{
	bool old = collsInside.contains(otherColl);
	ScriptType type;
	if (old && isInside)
	{
		type = SCRIPT_COLLISION_IN;
	}
	else if (!old && !isInside)
	{
		type =  SCRIPT_COLLISION_OUT;
	}
	else if (!old && isInside)
	{
		type =  SCRIPT_COLLISION_ENTER;
		collsInside.insert(otherColl);
	}
	else // old && !isInside
	{
		type = SCRIPT_COLLISION_OUT;
		collsInside.erase(otherColl);
	}
	return type;
}

void CollisionStructure::addCollider(GameObject* obj, Component* coll)
{
	colliders.emplace_back(obj, coll);
}

void CollisionStructure::removeCollider(Component* coll)
{
	for (auto it = colliders.begin(); it != colliders.end(); ++it)
	{
		if (it->coll != coll) continue;
		colliders.erase(it);
		break;
	}
}

void CollisionStructure::testCollisions()
{
	for (auto it = colliders.begin(); it != colliders.end(); ++it)
	{
		if (it->coll->value.coll.isStatic) continue;
		PhysicalData itData = it->obj->modelData;
		itData.pos += it->coll->value.coll.position;
		for (auto it2 = colliders.begin(); it2 != colliders.end(); ++it2)
		{
			PhysicalData itData2 = it2->obj->modelData;
			itData2.pos += it2->coll->value.coll.position;
			bool isInside = it->coll->value.coll.testCollision(itData, &it2->coll->value.coll, itData2);
			ScriptType type = it->updateCollisionHistory(it2->coll, isInside);
			CSLua::Manager::runCollideScript(
				it->obj, it->coll, 
				it2->obj, it2->coll, 
				it->coll->value.coll.scripts[type]);
		}
	}
}
