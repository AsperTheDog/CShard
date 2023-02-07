#include "Collider.hpp"

#include "../../engine/lua/Manager.hpp"

bool SphereCollider::testCollisions(PhysicalData pData, Collider* other, PhysicalData otherPData)
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

bool OBBCollider::testCollisions(PhysicalData pData, Collider* other, PhysicalData otherPData)
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

bool CapsuleCollider::testCollisions(PhysicalData pData, Collider* other, PhysicalData otherPData)
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
	wf.write((char*) &enterScript, sizeof(enterScript));
	wf.write((char*) &insideScript, sizeof(insideScript));
	wf.write((char*) &exitScript, sizeof(exitScript));
	wf.write((char*) &outsideScript, sizeof(outsideScript));	
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
	wf.read((char*) &enterScript, sizeof(enterScript));
	wf.read((char*) &insideScript, sizeof(insideScript));
	wf.read((char*) &exitScript, sizeof(exitScript));
	wf.read((char*) &outsideScript, sizeof(outsideScript));	
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

bool Collider::testCollision(PhysicalData pData, Collider* other, PhysicalData otherPData)
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

void ColliderStructure::init()
{
	
}

void ColliderStructure::add(GameObject* obj, Component* coll)
{
	colliders.push_back({coll, obj});
}

void ColliderStructure::remove(GameObject* obj, Component* coll)
{
	colliders.push_back({coll, obj});
}

void ColliderStructure::test()
{
	for (auto& collNode : colliders)
	{
		if (collNode.coll->value.coll.isStatic) continue;
		PhysicalData newData = collNode.obj->modelData;
		newData.pos += collNode.coll->value.coll.position;
		for (auto& coll : colliders)
		{
			PhysicalData newOtherData = coll.obj->modelData;
			newOtherData.pos += coll.coll->value.coll.position;
			CSLua::Manager::runCollideScript(
				collNode.obj, collNode.coll, 
				coll.obj, coll.coll, 
				collNode.coll->value.coll.enterScript);
		}
	}
}
