#include "systems/PointRender.h"

#include "components/Freelook.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Systems
{
	using namespace kult;

	void PointRender(AssetManager::Shader* shader, kult::entity camera, glm::ivec2 screenSize, glm::f32 fov, glm::f32 near, glm::f32 far)
	{
		auto& cameraPositionData = get<Component::Position>(camera);
		auto& cameraData = get<Component::Freelook>(camera);

		glm::mat4 projection = glm::perspective(glm::radians(fov), (glm::f32)screenSize.x / screenSize.y, near, far);
		glm::vec3 lookDirection = cameraPositionData.rot * glm::vec3(1, 0, 0);
		glm::vec3 up = cameraPositionData.rot * glm::vec3(0, 0, 1);
		glm::mat4 view = glm::lookAt(cameraPositionData.pos, cameraPositionData.pos + lookDirection, up);

		shader->Use();
		shader->SetUniform("projview", projection * view);

		for (auto &id : join<Component::Position, Component::PointRender>()) {
			auto& renderData = get<Component::PointRender>(id);
			auto& positionData = get<Component::Position>(id);

			glm::mat4 model;
			model = glm::translate(model, positionData.pos);
			model *= glm::mat4_cast(positionData.rot);
			model = glm::scale(model, positionData.scale);

			shader->SetUniform("model", model);
			shader->SetUniform("scale", positionData.scale);

			glBindVertexArray(renderData.mesh->GetVAO());
			glDrawArrays(GL_POINTS, 0, renderData.mesh->GetVertexCount());
		}
	}
}
