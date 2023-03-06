#include "Collider.hpp"

#include <gtx/transform.hpp>
#include <imgui.h>

#include "../../engine/lua/Manager.hpp"
#include "../../engine/Engine.hpp"
#include "../../device/graphics/GFramework.hpp"

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

glm::mat4 SphereCollider::getModelMatrix(PhysicalData& pData, glm::vec3& pos)
{
	glm::mat4 modelMat = glm::mat4(1);
	modelMat = glm::translate(modelMat, pData.pos + pos);
	modelMat = glm::scale(modelMat, pData.scale);
	modelMat = glm::scale(modelMat, glm::vec3(radius));

	return modelMat;
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

glm::mat4 OBBCollider::getModelMatrix(PhysicalData& pData, glm::vec3& pos)
{
	glm::mat4 modelMat = glm::mat4(1);
	modelMat = glm::translate(modelMat, pData.pos + pos);
	modelMat = glm::rotate(modelMat, glm::radians(pData.rot.x), {1, 0, 0});
	modelMat = glm::rotate(modelMat, glm::radians(pData.rot.y), {0, 1, 0});
	modelMat = glm::rotate(modelMat, glm::radians(pData.rot.z), {0, 0, 1});
	modelMat = glm::scale(modelMat, pData.scale);
	modelMat = glm::scale(modelMat, size);
	return modelMat;
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

std::tuple<glm::mat4, glm::mat4, glm::mat4> CapsuleCollider::getModelMatrix(PhysicalData& pData, glm::vec3& pos)
{
	glm::mat4 cylMat{1}, sphMat1{1}, sphMat2{1};
	glm::vec3 finalPos = pData.pos + pos;
	cylMat = glm::translate(cylMat, finalPos);
	cylMat = glm::rotate(cylMat, glm::radians(pData.rot.x), {1, 0, 0});
	cylMat = glm::rotate(cylMat, glm::radians(pData.rot.y), {0, 1, 0});
	cylMat = glm::rotate(cylMat, glm::radians(pData.rot.z), {0, 0, 1});
	cylMat = glm::scale(cylMat, {pData.scale.x, pData.scale.y * height, pData.scale.z});
	cylMat = glm::scale(cylMat, glm::vec3(radius, 1, radius));

	finalPos.y += pData.scale.y * height / 2;
	sphMat1 = glm::translate(sphMat1, finalPos);
	sphMat1 = glm::rotate(sphMat1, glm::radians(pData.rot.x), {1, 0, 0});
	sphMat1 = glm::rotate(sphMat1, glm::radians(pData.rot.y), {0, 1, 0});
	sphMat1 = glm::rotate(sphMat1, glm::radians(pData.rot.z), {0, 0, 1});
	sphMat1 = glm::scale(sphMat1, pData.scale);
	sphMat1 = glm::scale(sphMat1, glm::vec3(radius));
	
	finalPos.y -= pData.scale.y * height;
	sphMat2 = glm::translate(sphMat2, finalPos);
	sphMat2 = glm::rotate(sphMat2, glm::radians(pData.rot.x + 180), {1, 0, 0});
	sphMat2 = glm::rotate(sphMat2, glm::radians(pData.rot.y), {0, 1, 0});
	sphMat2 = glm::rotate(sphMat2, glm::radians(pData.rot.z), {0, 0, 1});
	sphMat2 = glm::scale(sphMat2, pData.scale);
	sphMat2 = glm::scale(sphMat2, glm::vec3(radius));

	return {cylMat, sphMat1, sphMat2};
}

void Collider::init()
{
	sphereMesh.commit(AssetPath::getPath(AssetPath::AssetType::OBJ, SPHERE_MESH_LOCATION));
	cubeMesh.commit(AssetPath::getPath(AssetPath::AssetType::OBJ, CUBE_MESH_LOCATION));
	meshCyl.commit(AssetPath::getPath(AssetPath::AssetType::OBJ, CAPSULE_CYL_MESH_LOCATION));
	meshSph.commit(AssetPath::getPath(AssetPath::AssetType::OBJ, CAPSULE_SPH_MESH_LOCATION));
}

Collider::Collider(ColliderType type): type(type), data{}
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

void Collider::renderSelection(PhysicalData& pData, Camera& cam)
{
	switch (type)
	{
	case COLLIDER_SPHERE:
		{
			glm::mat4 mat = data.sph.getModelMatrix(pData, position);
			GFramework::loadWireframeUniforms(cam, mat, pData);
			sphereMesh.renderAsSelection();
		}
		break;
	case COLLIDER_AABB:
		{
			glm::mat4 mat = data.aabb.getModelMatrix(pData, position);
			GFramework::loadWireframeUniforms(cam, mat, pData);
			cubeMesh.renderAsSelection();
		}
		break;
	case COLLIDER_CAPSULE:
		{
			auto [cylMat, sphMat1, sphMat2] = data.cap.getModelMatrix(pData, position);
			GFramework::loadWireframeUniforms(cam, cylMat, pData);
			meshCyl.renderAsSelection();
			GFramework::loadWireframeUniforms(cam, sphMat1, pData);
			meshSph.renderAsSelection();
			GFramework::loadWireframeUniforms(cam, sphMat2, pData);
			meshSph.renderAsSelection();
		}
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

void Collider::renderImGuiFields(std::string id)
{
	switch (type)
	{
	case COLLIDER_SPHERE:
		ImGui::PushItemWidth(118);
		ImGui::DragFloat(("Radius##Sphere" + id).c_str(), &this->data.sph.radius, 0.01f, 0.01f);
		ImGui::PopItemWidth();
		break;
	case COLLIDER_AABB:
		ImGui::DragFloat3(("Size##AABB" + id).c_str(), &this->data.aabb.size.x, 0.01f, 0.01f);
		break;
	case COLLIDER_CAPSULE:
		ImGui::PushItemWidth(118);
		ImGui::DragFloat(("Radius##Cap" + id).c_str(), &this->data.cap.radius, 0.01f, 0.01f);
		ImGui::DragFloat(("Height##Cap" + id).c_str(), &this->data.cap.height, 0.01f, 0.01f);
		ImGui::PopItemWidth();
		break;
	}
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
