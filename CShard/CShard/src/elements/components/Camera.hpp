#pragma once

#include <fstream>
#include <glm.hpp>

struct PhysicalData;

class Camera
{
public:
	Camera();
	
	glm::mat4 getViewMatrix(PhysicalData& parent);
	void updateViewMatrix(PhysicalData& parent);
	glm::mat4 getProjMatrix();
	void updateProjMatrix();

	void move(glm::vec3 pos);
	void lookAt(glm::vec3 dir);
	void changeWorldUp(glm::vec3 worldUp);
	void changeLense(float FOV, float nearP = -1, float farP = -1);
	void updateAspectRatio(float aspectRatio);
	
	void serialize(std::ofstream& wf);
	void deserialize(std::ifstream& wf);

	glm::vec3 pos{};
	glm::vec3 dir{};
	glm::vec3 worldUp{};
	glm::vec3 right{};

	float FOV{};
	float nearPlane{};
	float farPlane{};
	float aspectRatio{};
	bool hasChangedView = false;
	bool hasChangedProj = false;
private:
	glm::mat4 viewMatrix{};
	glm::mat4 projMatrix{};
};

