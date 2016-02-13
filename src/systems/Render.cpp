#include "systems/Render.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Systems
{
	using namespace kult;

	void Render(Shader* shader, Camera* camera)
	{
		shader->Use();
		shader->SetUniform("projview", camera->GetProjectionView());
		shader->SetUniform("tex", 0);

		for (auto &id : join<Component::Position, Component::Render>()) {
			auto& renderData = get<Component::Render>(id);
			auto& positionData = get<Component::Position>(id);

			glm::mat4 model = glm::translate(positionData.pos);
			model *= glm::mat4_cast(positionData.rot);
			model = glm::scale(model, positionData.scale);

			shader->SetUniform("model", model);
			shader->SetUniform("scale", positionData.scale);
			shader->SetUniform("scaleuv", renderData.scaleUv);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderData.texture->GetTexture());
			
			glBindVertexArray(renderData.mesh->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, renderData.mesh->GetVertexCount());
		}
	}
}
