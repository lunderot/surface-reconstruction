#include "systems/Render.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Systems
{
	using namespace kult;

	void Render(Shader* shader, Camera* camera)
	{
		shader->Use();
		shader->SetUniform("projview", camera->GetProjectionView());
		shader->SetUniform("tex", 0);

		for (auto &id : join<Component::Position, Component::Render>()) {
			Mesh* mesh = get<Component::Render>(id).mesh;
			Texture* texture = get<Component::Render>(id).texture;

			glm::vec3 pos = kult::get<Component::Position>(id).pos;
			glm::vec3 rot = kult::get<Component::Position>(id).rot;

			glm::mat4 m = glm::translate(pos);
			m = glm::rotate(m, rot.x, glm::vec3(1, 0, 0));
			m = glm::rotate(m, rot.y, glm::vec3(0, 1, 0));

			shader->SetUniform("model", m);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
			
			glBindVertexArray(mesh->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertexCount());
		}
	}
}
