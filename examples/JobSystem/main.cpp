#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <future>
#include <memory>
#include <utility>

#include "WindowSystem.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Figure.hpp"
#include "JobSystem.hpp"
#include "ModelLoader/Model.hpp"
#include "ModelLoader/Mesh.hpp"

// Shader source code moved to separate constants for clarity
const char* const kExampleVertexShader = R"(
#version 330
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* const kExampleFragmentShader = R"(
#version 330
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture_diffuse;

void main() {
    FragColor = texture(texture_diffuse, TexCoords);
}
)";

// Enhanced ModelManager with proper memory management
class ModelManager {
public:
    ModelManager(const std::vector<std::string>& paths)
        : modelPaths(paths), currentModelIndex(0), isLoading(false) {
        // Initialize with first model
        if (!modelPaths.empty()) {
            currentModel = std::make_unique<Model>(modelPaths[0]);
            currentModel->finalizeModel();
        }
    }

    // Prevent copying to avoid memory issues
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    // Synchronous loading (for KEY_D)
    void loadModelSync() {
        std::lock_guard<std::mutex> lock(modelMutex);
        currentModelIndex = (currentModelIndex + 1) % modelPaths.size();

        // Create new model before destroying the old one
        auto newModel = std::make_unique<Model>(modelPaths[currentModelIndex]);
        newModel->finalizeModel();

        // Only replace current model after new one is ready
        currentModel = std::move(newModel);
    }

    // Asynchronous loading (for KEY_RIGHT)
    void loadModelAsync(JobSystem& js) {
        std::lock_guard<std::mutex> lock(modelMutex);
        if (!isLoading) {
            currentModelIndex = (currentModelIndex + 1) % modelPaths.size();
            isLoading = true;

            // Store the new model separately while it loads
            pendingModel = std::make_unique<Model>(modelPaths[currentModelIndex], js);
        }
    }

    void update() {
        std::lock_guard<std::mutex> lock(modelMutex);
        if (isLoading && pendingModel && pendingModel->isDataLoaded()) {
            try {
                // Create GL resources for the pending model
                pendingModel->createGLResources();

                // Only replace current model after new one is fully ready
                currentModel = std::move(pendingModel);
                pendingModel.reset(); // Explicitly cleanup pending model
                isLoading = false;
            }
            catch (const std::exception& e) {
                std::cerr << "Error during model update: " << e.what() << std::endl;
                pendingModel.reset();
                isLoading = false;
            }
        }
    }

    // Cambiamos la firma para aceptar Program& en lugar de const Program&
    void draw(Program& program) {
        std::lock_guard<std::mutex> lock(modelMutex);
        if (!isLoading && currentModel) {
            try {
                currentModel->Draw(program);
            }
            catch (const std::exception& e) {
                std::cerr << "Error during model drawing: " << e.what() << std::endl;
            }
        }
    }

private:
    std::vector<std::string> modelPaths;
    std::unique_ptr<Model> currentModel;
    std::unique_ptr<Model> pendingModel;  // Holds model while loading
    size_t currentModelIndex;
    bool isLoading;
    std::mutex modelMutex;  // Protects shared resources
};

int main(int argc, char** argv) {
    // Initialize GLFW and create window
    glfwInit();
    auto window = Window::make(1000, 800, "LUQUI");
    if (nullptr == window->window) {
        return -1;
    }
    window->setCurrentWindowActive();

    // Initialize job system for async loading
    JobSystem js((unsigned int)(std::thread::hardware_concurrency() * 0.5f));

    // Create and compile shaders
    Shader vertex;
    vertex.loadSource(Shader::ShaderType::kShaderType_Vertex,
        kExampleVertexShader,
        (unsigned int)(strlen(kExampleVertexShader)));
    if (!vertex.compile()) {
        std::cout << "Failed to compile vertex shader" << std::endl;
        return -1;
    }

    Shader fragment;
    fragment.loadSource(Shader::ShaderType::kShaderType_Fragment,
        kExampleFragmentShader,
        (unsigned int)(strlen(kExampleFragmentShader)));
    if (!fragment.compile()) {
        std::cout << "Failed to compile fragment shader" << std::endl;
        return -1;
    }

    // Create and link shader program
    Program program;  // Aquí quitamos el 'const' para que sea modificable
    program.attach(&vertex);
    program.attach(&fragment);
    if (!program.link()) {
        std::cout << "Failed to link shader program" << std::endl;
        return -1;
    }
    program.use();

    // Setup model paths and manager
    std::vector<std::string> modelPaths = {
        "../data/Models/cube/cube.obj",
        "../data/Models/cube/wheelchair.obj",
        "../data/Models/Alduin/Alduin.obj"
    };
    ModelManager modelManager(modelPaths);

    Input input(window->window);

    // Main render loop
    while (!window->isOpen()) {
        window->clear();

        // Handle input for different loading methods
        if (input.wasKeyJustPressed(Input::KEY_D)) {
            modelManager.loadModelSync();  // Synchronous loading
        }
        if (input.wasKeyJustPressed(Input::KEY_RIGHT)) {
            modelManager.loadModelAsync(js);  // Asynchronous loading
        }

        // Setup OpenGL state
        glEnable(GL_COLOR_BUFFER_BIT);
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        // Setup transformation matrices
        glm::mat4 model = glm::rotate(glm::mat4(1.0f),
            glm::radians((float)glfwGetTime() * 10.0f),
            glm::vec3(0, 1, 0));
        glm::mat4 view = glm::translate(glm::mat4(1.0f),
            glm::vec3(0.0f, -2.0f, -15.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            640.0f / 480.0f,
            0.1f, 500.f);

        // Update uniforms
        glUniformMatrix4fv(glGetUniformLocation(program.get_id(), "model"),
            1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(program.get_id(), "view"),
            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(program.get_id(), "projection"),
            1, GL_FALSE, glm::value_ptr(projection));

        // Update and draw current model
        modelManager.update();
        modelManager.draw(program);  // Aquí pasamos 'program' sin 'const'

        window->render();
    }

    // Cleanup
    js.~JobSystem();
    window->~Window();
    glfwTerminate();
    return 0;
}