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

float deltaTime = 0.0f; // Tiempo entre frames
float lastFrameTime = 0.0f; // Tiempo del frame anterior

// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
		}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
		}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
		}();
	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void PrintShaderValues(Program program)
{
	GLint numUniforms = 0;
	glGetProgramiv(program.get_id(), GL_ACTIVE_UNIFORMS, &numUniforms);

	std::cout << "Uniforms activos en el shader:\n";
	for (GLint i = 0; i < numUniforms; i++) {
		char name[256];
		GLsizei length;
		GLint size;
		GLenum type;
		glGetActiveUniform(program.get_id(), i, sizeof(name), &length, &size, &type, name);
		std::cout << "  - " << name << " (size: " << size << ", type: " << type << ")\n";

		// Si es "ambientLight", obtenemos su valor y lo imprimimos
		if (std::strcmp(name, "LightType") == 0) {
			GLint location = glGetUniformLocation(program.get_id(), name);
			if (location != -1) {
				GLfloat ambientLight[3]; // Vec3
				glGetUniformfv(program.get_id(), location, ambientLight);
				std::cout << "    > LightType: ("
					<< ambientLight[0] << ", "
					")\n";
			}
		}

	}
}

float near_plane = 1.0f, far_plane = 7.5f;

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			 // bottom face
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 // top face
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
unsigned int planeVBO;
unsigned int planeVAO;
void renderScene(Program& shader)
{
	//// floor
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//shader.setmat4("model", model);
	//glBindVertexArray(planeVAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
	shader.setmat4("model", model);
	renderCube();
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setmat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setmat4("model", model);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.setmat4("model", model);
	renderCube();
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	auto window = Window::make(1024, 1024, "LUQUI");
	if (nullptr == window->window) {
		return -1;
	}
	srand((unsigned int)time(NULL));
	window->setCurrentWindowActive();
	glewInit();
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(message_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	// Declaramos un gestor de input asociado a la ventana en activo.
	Input input(window->window);

	// set up vertex data (and buffer(s)) and configure vertex attributes
   // ------------------------------------------------------------------
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};
	// plane VAO
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	/** Creating shaders */
	Shader shadow_vertex = Shader();
	if (!shadow_vertex.loadFromFile(Shader::ShaderType::kShaderType_Vertex, "../data/Shaders/shadow_vertex.vs")) {
		std::cerr << "Error al cargar el vertex shader desde archivo." << std::endl;
		return -2;
	}
	if (!shadow_vertex.compile()) {
		std::cerr << "Error al compilar el vertex shader." << std::endl;
		return -3;
	}

	Shader shadow_fragment = Shader();
	if (!shadow_fragment.loadFromFile(Shader::ShaderType::kShaderType_Fragment, "../data/Shaders/shadow_fragment.fs")) {
		std::cerr << "Error al cargar el fragment shader desde archivo." << std::endl;
		return -4;
	}
	if (!shadow_fragment.compile()) {
		std::cerr << "Error al compilar el fragment shader." << std::endl;
		return -5;
	}

	Shader vertex = Shader();
	if (!vertex.loadFromFile(Shader::ShaderType::kShaderType_Vertex, "../data/Shaders/vertex.vs")) {
		std::cerr << "Error al cargar el vertex shader desde archivo." << std::endl;
		return -2;
	}
	if (!vertex.compile()) {
		std::cerr << "Error al compilar el vertex shader." << std::endl;
		return -3;
	}

	Shader fragment = Shader();
	if (!fragment.loadFromFile(Shader::ShaderType::kShaderType_Fragment, "../data/Shaders/fragment.fs")) {
		std::cerr << "Error al cargar el fragment shader desde archivo." << std::endl;
		return -4;
	}
	if (!fragment.compile()) {
		std::cerr << "Error al compilar el fragment shader." << std::endl;
		return -5;
	}

	Shader quad_vertex = Shader();
	if (!quad_vertex.loadFromFile(Shader::ShaderType::kShaderType_Vertex, "../data/Shaders/debug_shader_vertex.vs")) {
		std::cerr << "Error al cargar el vertex shader desde archivo." << std::endl;
		return -2;
	}
	if (!quad_vertex.compile()) {
		std::cerr << "Error al compilar el vertex shader." << std::endl;
		return -3;
	}

	Shader quad_fragment = Shader();
	if (!quad_fragment.loadFromFile(Shader::ShaderType::kShaderType_Fragment, "../data/Shaders/debug_shader_fragment.fs")) {
		std::cerr << "Error al cargar el fragment shader desde archivo." << std::endl;
		return -4;
	}
	if (!quad_fragment.compile()) {
		std::cerr << "Error al compilar el fragment shader." << std::endl;
		return -5;
	}

	/** Creating Program */
	Program program = Program();
	program.attach(&vertex);
	program.attach(&fragment);
	if (!program.link()) {
		std::cout << "Error al linkar el programa" << std::endl;
		return -4;
	}

	Program shadow_program = Program();
	shadow_program.attach(&shadow_fragment);
	shadow_program.attach(&shadow_vertex);
	if (!shadow_program.link()) {
		std::cout << "Error al linkar el programa sombra" << std::endl;
		return -4;
	}

	/** Creating Program */
	Program quad_program = Program();
	quad_program.attach(&quad_vertex);
	quad_program.attach(&quad_fragment);
	if (!quad_program.link()) {
		std::cout << "Error al linkar el programa" << std::endl;
		return -4;
	}

	///////END OF PROGRAM & SHADERS/////

	// Creamos un gestor de entidades
	ECSManager ecsmanager;

	// Declaramos los sistemas que se utilizar�n en el programa.
	RenderSystem renderSystem;
	InputSystem inputSystem;

	// Crear listas de componentes
	ecsmanager.addComponentType<TransformComponent>();
	ecsmanager.addComponentType<InputComponent>();
	ecsmanager.addComponentType<RenderComponent>();
	ecsmanager.addComponentType<LightComponent>();
	ecsmanager.addComponentType<CameraComponent>();

	//Camera Entity
	Entity CameraEntity = ecsmanager.createEntity();
	ecsmanager.addComponent<CameraComponent>(CameraEntity);
	ecsmanager.editComponent<CameraComponent>(CameraEntity, [](CameraComponent& camera) {
		camera.position = glm::vec3(0.0f, 0.0f, 0.0f); // Nueva posición
		camera.updateViewMatrix(); // Actualizar la matriz de vista
		camera.updateProjectionMatrix();
		});
	ecsmanager.addComponent<InputComponent>(CameraEntity);

	ecsmanager.editComponent<InputComponent>(CameraEntity, [](InputComponent& input) {
		input.active = true;
		input.followingMouse = true;
		});
	ecsmanager.addComponent<TransformComponent>(CameraEntity);
	ecsmanager.editComponent<TransformComponent>(CameraEntity, [](TransformComponent& tr) {
		tr.position = { 0.0f, 0.0f, 5.0f };
		});

	// Crear una entidad para la luz
	Entity lightEntity = ecsmanager.createEntity();

	//ecsmanager.editComponent<LightComponent>(lightEntity, [](LightComponent& light) {
	//	light.type = LightType::Point; // Tipo de luz (Point Light)
	//	light.color = glm::vec3(1.0f, 0.0f, 0.0f); // Color de la luz (RGB)
	//	light.position = glm::vec3(20.0f, 100.0f, 0.0f); // Posición de la luz
	//	light.intensity = 2.0f; // Intensidad de la luz
	//	light.radius = 250.0f; // Radio de influencia de la luz
	//	});

	ecsmanager.editComponent<LightComponent>(lightEntity, [](LightComponent& light) {
		light.type = LightType::Directional; // Tipo de luz (Spotlight)
		light.color = glm::vec3(1.0f, 1.0f, 1.0f); // Color de la luz (blanco)
		light.position = glm::vec3(-2.0f, 4.0f, -1.0f); // Posición de la luz
		light.direction = glm::vec3(0.0f, -1.0f, 0.0f); // Dirección de la luz
		light.intensity = 2.0f; // Intensidad de la luz
		//light.cutoff = glm::cos(glm::radians(12.5f)); // Ángulo de corte interior (12.5 grados)
		//light.outerCutoff = glm::cos(glm::radians(17.5f)); // Ángulo de corte exterior (17.5 grados)
		});

	// Configurar la luz como Point Light
	//Entity lightEntity2 = ecsmanager.createEntity();
	//ecsmanager.editComponent<LightComponent>(lightEntity2, [](LightComponent& light) {
	//	light.type = LightType::Point; // Tipo de luz (Point Light)
	//	light.color = glm::vec3(0.0f, 1.0f, 0.0f); // Color de la luz (RGB)
	//	light.position = glm::vec3(0.0f, 100.0f, 0.0f); // Posición de la luz
	//	light.intensity = 2.0f; // Intensidad de la luz
	//	light.radius = 250.0f; // Radio de influencia de la luz
	//	});

	//Entity lightEntity3 = ecsmanager.createEntity();
	//ecsmanager.editComponent<LightComponent>(lightEntity3, [](LightComponent& light) {
	//	light.type = LightType::Point; // Tipo de luz (Point Light)
	//	light.color = glm::vec3(0.0f, 0.0f, 1.0f); // Color de la luz (RGB)
	//	light.position = glm::vec3(50.0f, 100.0f, 0.0f); // Posición de la luz
	//	light.intensity = 2.0f; // Intensidad de la luz
	//	light.radius = 250.0f; // Radio de influencia de la luz
	//	});

	

	auto mesh = std::make_shared<Model>("../data/Models/Alduin/Alduin.obj");

	// Crear la primera entidad para el modelo 1
	Entity modelEntity1 = ecsmanager.createEntity();

	ecsmanager.editComponent<TransformComponent>(modelEntity1, [](TransformComponent& transform) {
		transform.position = {0.0f, 0.0f, 0.0f }; // Posición del primer modelo
		transform.scale = { 1.0f, 1.0f, 1.0f };
		});

	ecsmanager.editComponent<RenderComponent>(modelEntity1, [&](RenderComponent& modelComp) {
		modelComp.model = mesh; // Cargar el primer modelo
		});
	// Crear la segunda entidad para el modelo 2
	Entity modelEntity2 = ecsmanager.createEntity();

	ecsmanager.editComponent<TransformComponent>(modelEntity2, [](TransformComponent& transform) {
		transform.position = { 200.0f, -300.0f, 50.0f }; // Posición del segundo modelo
		transform.scale = { 1.0f, 1.0f, 1.0f };

		transform.rotation = { 0.0f,45.0f,0.0f };
		});

	ecsmanager.editComponent<RenderComponent>(modelEntity2, [&](RenderComponent& modelComp) {
		modelComp.model = mesh; // Cargar el segundo modelo
		});
	//////////////////////////////////
	program.CreateShadowMap();

// Ciclo del juego
	while (!window->isOpen()) {
		float currentFrameTime = glfwGetTime();

		// Calcular el delta time
		deltaTime = currentFrameTime - lastFrameTime;

		// Actualizar el tiempo del frame anterior
		lastFrameTime = currentFrameTime;

		float rotation = glm::radians((float)glfwGetTime() * 100.0f);
		//glEnable(GL_COLOR_BUFFER_BIT);
		for (Entity entity = 1; entity < ecsmanager.get_nextEntity(); ++entity)
		{
			if (auto transformOpt = ecsmanager.getComponent<TransformComponent>(entity))
			{
				transformOpt.value()->rotation.y = rotation;
				RenderSystem::UpdateTransformMatrix(*transformOpt.value());
			}
		}
		// Limpiar la pantalla
		glClearColor(0.4, 0.4, 0.4, 1.0);
		glFrontFace(GL_CCW);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(true);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
		

		program.use();
		glm::mat4x4 model, view, projection;
		model = glm::mat4(1.0f);
		view = glm::mat4(1.0f);
		projection = glm::mat4(1.0f);

		glm::mat4 lightProjection, lightView, lightSpaceMatrix;

		auto cameraComponent = ecsmanager.getComponent<CameraComponent>(CameraEntity);


		// Gestion de camara
		auto inputCameraComponentOpt = ecsmanager.getComponent<InputComponent>(CameraEntity);
		if (cameraComponent) {
			glm::vec3 cameraPosition = cameraComponent.value()->position;

			if (inputCameraComponentOpt)
				inputSystem.update(inputCameraComponentOpt.value(), cameraComponent.value(), input, deltaTime);
			// Usa las matrices de la cámara del componente
			cameraComponent.value()->updateViewMatrix();
			cameraComponent.value()->updateProjectionMatrix();
			view = cameraComponent.value()->view;
			projection = cameraComponent.value()->projection;
			program.setVec3("cameraPos", cameraPosition);

			printf("%f | %f | %f \n", cameraComponent.value()->position.x, cameraComponent.value()->position.y, cameraComponent.value()->position.z);
		}
		auto camera = ecsmanager.getComponent<CameraComponent>(CameraEntity).value();
		// Pasar las matrices de vista y proyección al shader
		if (camera) {

			program.setmat4("view", view);
			/*GLuint viewLoc = glGetUniformLocation(program.get_id(), "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));*/

			program.setmat4("projection", projection);
			/*GLuint projectionLoc = glGetUniformLocation(program.get_id(), "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));*/

		}

		for (Entity Light_entity = 1; Light_entity < ecsmanager.get_nextEntity(); ++Light_entity) {
			// Configurar la luz en el shader
			auto lightOpt = ecsmanager.getComponent<LightComponent>(Light_entity);
			if (!ecsmanager.isEntityAlive(Light_entity)) continue;
			if (!lightOpt.has_value()) continue;
			program.use();
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,program.get_depthMap());
			program.setInt("shadowMap", 1);
			program.unuse();
			shadow_program.use();
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane,far_plane);
			lightView = glm::lookAt(lightOpt.value()->position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			lightSpaceMatrix = lightProjection * lightView;
			shadow_program.setmat4("lightSpaceMatrix", lightSpaceMatrix);

			glViewport(0, 0, program.get_SHADOW_WIDTH(), program.get_SHADOW_HEIGHT());
			glBindFramebuffer(GL_FRAMEBUFFER, program.get_depthMapFBO());
			glClear(GL_DEPTH_BUFFER_BIT);
			/*for (int i = 1; i < mesh->get_LoadedTextures().size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->get_LoadedTextures().at(i).id);
				
			}*/
			renderSystem.RenderScene(ecsmanager, shadow_program, model, view, projection);
			renderScene(shadow_program);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glViewport(0, 0, 1024, 1024);
			program.use();
			if (lightOpt && lightOpt.value()->type == LightType::Point) {
				program.setVec3("pointLightColor", lightOpt.value()->color);
				program.setVec3("pointLightPosition", lightOpt.value()->position);
				program.setFloat("pointLightRadius", lightOpt.value()->radius);
				program.setInt("LightType", static_cast<int>(lightOpt.value()->type));
			}
			if (lightOpt && lightOpt.value()->type == LightType::Spot) {
				program.setVec3("spotlightColor", lightOpt.value()->color);
				program.setVec3("spotlightPosition", lightOpt.value()->position);
				program.setVec3("spotlightDirection", lightOpt.value()->direction);
				program.setFloat("spotlightIntensity", lightOpt.value()->intensity);
				program.setFloat("spotlightCutoff", lightOpt.value()->cutoff);
				program.setFloat("spotlightOuterCutoff", lightOpt.value()->outerCutoff);
				program.setInt("LightType", static_cast<int>(lightOpt.value()->type));
			}
			if (lightOpt && lightOpt.value()->type == LightType::Directional) {
				program.setVec3("directionalLightColor", lightOpt.value()->color);
				program.setVec3("directionalLightDirection", lightOpt.value()->direction);
				program.setFloat("directionalLightIntensity", lightOpt.value()->intensity);
				program.setVec3("directionLightPosition", lightOpt.value()->position);
				program.setInt("LightType", static_cast<int>(lightOpt.value()->type));
			}

			// Renderizar todas las entidades
			program.setmat4("lightSpaceMatrix", lightSpaceMatrix);

			renderSystem.RenderScene(ecsmanager, program, model, view, projection);
			renderScene(program);
			glBlendFunc(GL_ONE, GL_ONE);
			glDepthMask(false);
		}
		glDepthMask(true);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
		program.unuse();
		// render Depth map to quad for visual debugging
  // ---------------------------------------------
		quad_program.use();
		quad_program.setFloat("near_plane", near_plane);
		quad_program.setFloat("far_plane", far_plane);
		quad_program.setInt("depthMap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, program.get_depthMap());
		//renderQuad();
		
		// Intercambiar buffers

		window->render();
	}

	window->~Window();
	glfwTerminate();
	return 0;
}
