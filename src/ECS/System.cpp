#include "ECS/System.hpp"
#include <iostream>
#include <Input.hpp>

RenderSystem::RenderSystem()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Fondo gris oscuro
	// Configurar la matriz de proyección (ortográfica para simplicidad)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	// Configurar la matriz de modelo/vista
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderSystem::update(const std::vector<TransformComponent>& transforms, const std::vector<RenderComponent>& renders)
{
	if (transforms.size() != renders.size()) {
		std::cout << "Error: transforms y renders no tienen el mismo tamaño.";
		return;
	}

	for (size_t i = 0; i < renders.size(); ++i) {
		const TransformComponent& transform = transforms[i];
		const RenderComponent& render = renders[i];

		Entity entity = static_cast<Entity>(i);

		renderEntity(entity, transform, render);
	}
}

void RenderSystem::renderEntity(Entity entity, const TransformComponent& transform, const RenderComponent& render)
{
	std::cout << "Rendering entity " << entity << " at position ("
		<< transform.position.x << ", " << transform.position.y << ", " << transform.position.z
		<< ".\n";
}

void RenderSystem::drawModel(const TransformComponent* transform, const RenderComponent* model, Program program) {
	if (!model || !model->model) return;

	// Pasar la luz ambiental al shader
	GLuint ambientLightLoc = glGetUniformLocation(program.get_id(), "ambientLight");
	glUniform3f(ambientLightLoc, 0.5f, 0.5f, 0.5f); // Ejemplo de luz ambiental

	model->model->Draw(program);
}


void RenderSystem::drawShape(const TransformComponent* transform, const ShapeComponent* shape) {
    glPushMatrix();

    // Aplicar transformaciones 2D
    glTranslatef(transform->position.x, transform->position.y, transform->position.z);
    glRotatef(transform->rotation.z, 0.0f, 0.0f, 1.0f);  // Solo rotación en Z para 2D
    glScalef(transform->scale.x, transform->scale.y, 1.0f);

    // Dibujar la forma
    glColor4f(shape->color.x, shape->color.y, shape->color.z, shape->color.w);
    glBegin(GL_POLYGON);
    for (const Vec3& vertex : shape->vertices) {
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();

    glPopMatrix();
}


void AnimationSystem::update(TransformComponent* transform, AnimationComponent* anim, float deltaTime) {
	if (anim->active) {
		// Actualizar tiempo de animación
		anim->elapsedTime += deltaTime;
		if (anim->elapsedTime > anim->duration) {
			anim->elapsedTime = anim->duration; // Detener en el tiempo máximo
			return;
		}

		// Aplicar animación al transform usando glm::vec3
		transform->position = transform->position + (anim->translation * deltaTime);
		transform->rotation = transform->rotation + (anim->rotation * deltaTime);
		transform->scale = transform->scale + (anim->scale * deltaTime);
	}
}

void InputSystem::update(InputComponent* inputComponent, TransformComponent* transform, Input& input) {
	if (inputComponent->active) {
		inputComponent->keyReceived = input.getCurrentlyPressedKey();

		if (inputComponent->followingMouse) {
			double mouseX, mouseY;
			input.getMousePosition(mouseX, mouseY);
			inputComponent->mouseX = mouseX;
			inputComponent->mouseY = mouseY;
			transform->position = glm::vec3((float)inputComponent->mouseX, (float)inputComponent->mouseY, 0.0f);
		}
		else {
			// Para movimiento
			if (inputComponent->keyReceived == Input::KEY_W) {
				transform->position = transform->position + glm::vec3(0.0f, 0.25f * 0.016f, 0.0f);
			}
			if (inputComponent->keyReceived == Input::KEY_S) {
				transform->position = transform->position - glm::vec3(0.0f, 0.25f * 0.016f, 0.0f);
			}
			if (inputComponent->keyReceived == Input::KEY_D) {
				transform->position = transform->position + glm::vec3(0.25f * 0.016f, 0.0f, 0.0f);
			}
			if (inputComponent->keyReceived == Input::KEY_A) {
				transform->position = transform->position - glm::vec3(0.25f * 0.016f, 0.0f, 0.0f);
			}

			// Para rotaciÃ³n
			if (inputComponent->keyReceived == Input::KEY_E) {
				transform->rotation = transform->rotation - glm::vec3(0.0f, 0.0f, 25.0f * 0.016f);
			}
			if (inputComponent->keyReceived == Input::KEY_Q) {
				transform->rotation = transform->rotation + glm::vec3(0.0f, 0.0f, 25.0f * 0.016f);
			}

			// Para escala
			if (inputComponent->keyReceived == Input::KEY_Z) {
				transform->scale = transform->scale + glm::vec3(0.25f * 0.016f);
			}
			if (inputComponent->keyReceived == Input::KEY_X) {
				// Evitar escalas negativas
				if (transform->scale.x > 0.1f && transform->scale.y > 0.1f && transform->scale.z > 0.1f) {
					transform->scale = transform->scale - glm::vec3(0.25f * 0.016f);
				}
			}
		}
	}
}
