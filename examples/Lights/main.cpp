#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // perspective
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>

#include "Window.hpp"
#include "Input.hpp"
#include "ECS/Component.hpp"
#include "ECS/System.hpp"
#include "ECS/ECSManager.hpp"




int main() {
	glfwInit();
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	auto window = Window::make(1280, 1040, "LUQUI");
	if (nullptr == window->window) {
		return -1;
	}
	srand((unsigned int)time(NULL));
	window->setCurrentWindowActive();

	// Declaramos un gestor de input asociado a la ventana en activo.
	Input input(window->window);

	///// ----->SHADERS & PROGRAM<-----/////

#define GLSL(x) "#version 330\n"#x
	static const char* kExampleFragmentShader = GLSL(
		out vec4 FragColor;

	in vec2 TexCoords;

	uniform sampler2D texture_diffuse;
	uniform vec3 ambientLight; // Color de la luz ambiental
	uniform float ambientIntensity; // Intensidad de la luz ambiental

	void main()
	{
		vec4 textureColor = texture(texture_diffuse, TexCoords);
		if (textureColor.a < 0.1) { // Si no hay textura, usar un color base
			textureColor = vec4(0.8, 0.8, 0.8, 1.0); // Gris claro
		}
		vec3 finalColor = textureColor.rgb * ambientLight * ambientIntensity;
		FragColor = vec4(finalColor, textureColor.a);
		//FragColor = vec4(TexCoords, 0.0, 1.0); // ver las normales
	}
		);


#define GLSL(x) "#version 330\n"#x
	static const char* kExampleVertexShader = GLSL(
		layout(location = 0) in vec3 aPos;
	layout(location = 1) in vec3 aNormal;
	layout(location = 2) in vec2 aTexCoords;

	out vec2 TexCoords;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		TexCoords = aTexCoords;
		gl_Position = projection * view * model * vec4(aPos, 1.0);
	}
		);
	/** Creating shaders */
	Shader vertex = Shader();
	vertex.loadSource(Shader::ShaderType::kShaderType_Vertex, kExampleVertexShader, (unsigned int)strlen(kExampleVertexShader));
	vertex.compile();
	if (!vertex.get_isCompiled())
		return -1;
	Shader fragment = Shader();
	fragment.loadSource(Shader::ShaderType::kShaderType_Fragment, kExampleFragmentShader, (unsigned int)strlen(kExampleFragmentShader));
	fragment.compile();
	if (!fragment.get_isCompiled())
		return -1;

	/** Creating Program */
	Program program = Program();
	program.attach(&vertex);
	program.attach(&fragment);
	if (!program.link()) {
		std::cout << "Error al linkar el programa" << std::endl;
		return -1;
	}
	program.use();

	///////END OF PROGRAM & SHADERS/////

	// Creamos un gestor de entidades
	ECSManager ecsmanager;

	// Declaramos los sistemas que se utilizarán en el programa.
	RenderSystem renderSystem;
	InputSystem inputSystem;

	// Crear listas de componentes
	ecsmanager.addComponentType<TransformComponent>();
	ecsmanager.addComponentType<InputComponent>();
	ecsmanager.addComponentType<RenderComponent>();
	ecsmanager.addComponentType<LightComponent>();

	///////---->3D<------///////
//Model Entity
Entity modelEntity = ecsmanager.createEntity();

ecsmanager.editComponent<TransformComponent>(modelEntity, [](TransformComponent& transform) {
	transform.position = { 0.0f, 0.0f, 0.0f };
	transform.scale = { 1.0f, 1.0f, 1.0f };
	});

ecsmanager.editComponent<LightComponent>(modelEntity, [](LightComponent& light) {
	light.type = LightType::Ambient;
	light.color = glm::vec3(0.0f, 0.0f, 1.0f);
	light.intensity = 1.0f; // Intensidad de la luz ambiental
	});

ecsmanager.editComponent<RenderComponent>(modelEntity, [](RenderComponent& modelComp) {
	modelComp.model = std::make_shared<Model>("../data/Models/Alduin/Alduin.obj");
	});

ecsmanager.editComponent<InputComponent>(modelEntity, [](InputComponent& input) {
	input.active = true;
	});
			//////////////////////////////////

// Ciclo del juego
	while (!window->isOpen()) {
				///Rendering//
		glEnable(GL_COLOR_BUFFER_BIT);

		///* Render here */
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4x4 model, view, projection;
		model = glm::mat4(1.0f);
		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);

		auto modelTransform = ecsmanager.getComponent<TransformComponent>(modelEntity);
		if (modelTransform) {
			model = glm::mat4(1.0f);
			// Aplica las transformaciones en orden: escala, rotación, traslación
			model = glm::translate(model, modelTransform.value()->position);
			model = glm::rotate(model, glm::radians((float)glfwGetTime() * 10.0f), glm::vec3(0, 1, 0));
			model = glm::scale(model, modelTransform.value()->scale);
		}
		if (modelTransform) {
			glm::vec3 model_pos;
			model_pos.x = modelTransform.value()->position.x;
			model_pos.y = modelTransform.value()->position.y;
			model_pos.z = modelTransform.value()->position.z;

			view = glm::lookAt(
				glm::vec3(0.0f, 350.0f, 500.0f), // Posición de la cámara
				glm::vec3(-model_pos), // Punto al que mira
				glm::vec3(0.0f, 1.0f, 0.0f)  // Vector "up"
			);
		}
		projection = glm::perspective(glm::radians(90.0f), 1280.0f / 1040.0f,0.10f, 1000.f);

		glEnable(GL_TEXTURE_2D);
		GLuint model_loc = glGetUniformLocation(program.get_id(), "model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

		GLuint view_loc = glGetUniformLocation(program.get_id(), "view");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

		GLuint projection_loc = glGetUniformLocation(program.get_id(), "projection");
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));


		for (Entity entity = 0; entity < ecsmanager.get_nextEntity(); ++entity) {
			if (!ecsmanager.isEntityAlive(entity)) continue;
			// Obtener referencias opcionales a los componentes
			auto transformOpt = ecsmanager.getComponent<TransformComponent>(entity);
			
			auto inputComponentOpt = ecsmanager.getComponent<InputComponent>(entity);
			if (inputComponentOpt)
				inputSystem.update(inputComponentOpt.value(), transformOpt.value(), input);

			auto lightOpt = ecsmanager.getComponent<LightComponent>(modelEntity);

			if (lightOpt) {
				std::cout << "Light color: (" << lightOpt.value()->color.r << ", "
					<< lightOpt.value()->color.g << ", "
					<< lightOpt.value()->color.b << ")\n";
				std::cout << "Light intensity: " << lightOpt.value()->intensity << "\n";
			}


			if (lightOpt && lightOpt.value()->type == LightType::Ambient) {
				GLuint ambientLightLoc = glGetUniformLocation(program.get_id(), "ambientLight");
				GLuint ambientIntensityLoc = glGetUniformLocation(program.get_id(), "ambientIntensity");

				glUniform3f(ambientLightLoc, lightOpt.value()->color.r, lightOpt.value()->color.g, lightOpt.value()->color.b);
				glUniform1f(ambientIntensityLoc, lightOpt.value()->intensity);
			}
			//Dibujado del modelo//
			auto modelOpt = ecsmanager.getComponent<RenderComponent>(entity);
			if (transformOpt && modelOpt) {
				renderSystem.drawModel(transformOpt.value(), modelOpt.value(), program);
			}
		}

		//ImGui::Render();
		// Intercambiar buffers
		window->render();
		//ImGui::End();
	}

	window->~Window();
	glfwTerminate();
	return 0;
}
