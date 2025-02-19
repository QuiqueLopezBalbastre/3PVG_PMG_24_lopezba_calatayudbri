#include "ECS/System.hpp"
#include <iostream>
#include "glm/glm.hpp"
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // perspective
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
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

void RenderSystem::drawModel(const TransformComponent* transform, const RenderComponent* model, Program &program) {
	if (!model || !model->model) return;

	model->model->Draw(program);
}

void RenderSystem::UpdateTransformMatrix(TransformComponent& transform)
{
	
	transform.transform_matrix = glm::mat4(1.0f);
	transform.transform_matrix = glm::translate(transform.transform_matrix, transform.position);
	transform.transform_matrix  *= glm::mat4_cast(glm::quat(glm::radians(transform.rotation)));
	transform.transform_matrix = glm::scale(transform.transform_matrix, transform.scale);
	
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
void InputSystem::update(InputComponent* input, CameraComponent* camera, Input& inputManager, float deltaTime) {
	if (!input->active) return;

	input->keyReceived = inputManager.getCurrentlyPressedKey();

	// Procesar movimiento del ratón
	double mouseX, mouseY;
	inputManager.getMousePosition(mouseX, mouseY);

	static double lastX = mouseX;
	static double lastY = mouseY;

	float xOffset = mouseX - lastX;
	float yOffset = lastY - mouseY; // Invertido porque Y va de abajo hacia arriba

	lastX = mouseX;
	lastY = mouseY;

	// Actualizar ángulos de la cámara solo si el botón derecho está presionado
	if (inputManager.isMouseButtonPressed(inputManager.MOUSE_BUTTON_RIGHT)) {
		camera->yaw += xOffset * camera->mouseSensitivity;
		camera->pitch += yOffset * camera->mouseSensitivity;

		// Limitar el pitch para evitar que la cámara se voltee
		camera->pitch = glm::clamp(camera->pitch, -89.0f, 89.0f);

		// Actualizar los vectores de la cámara basados en los nuevos ángulos
		camera->updateCameraVectors();

		// Velocidad base de movimiento
		float currentSpeed = camera->movementSpeed;

		// Ajustar por deltaTime para movimiento consistente
		currentSpeed *= deltaTime;

		// Procesar entrada de teclado para movimiento
		if (input->keyReceived == Input::KEY_W)
			camera->position += camera->front * currentSpeed;
		if (input->keyReceived == Input::KEY_S)
			camera->position -= camera->front * currentSpeed;
		if (input->keyReceived == Input::KEY_A)
			camera->position -= camera->right * currentSpeed;
		if (input->keyReceived == Input::KEY_D)
			camera->position += camera->right * currentSpeed;
		if (input->keyReceived == Input::KEY_E)
			camera->position += camera->up * currentSpeed;
		if (input->keyReceived == Input::KEY_Q)
			camera->position -= camera->up * currentSpeed;
	}


	// Actualizar la matriz de vista después del movimiento
	camera->updateViewMatrix();
}
