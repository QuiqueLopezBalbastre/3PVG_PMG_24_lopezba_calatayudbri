#include "Imgui.hpp"
#include <iostream>
#include <string>
#include <vector>


#include "GL/glew.h"
#include "../deps/imgui/imgui.h"
#include "../deps/imgui/imgui_impl_glfw.h"
#include "../deps/imgui/imgui_impl_opengl3.h"
#include "../deps/imgui/imgui_stdlib.h"


// Constructor: Initialize ImGui context and set up GLFW and OpenGL backends
LuquiImgui::LuquiImgui(Window* window) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window->window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Initialize selected entity to an invalid value
	selectedEntity = 0;
}

// Destructor: Clean up ImGui context and backends
LuquiImgui::~LuquiImgui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

// Start a new ImGui frame
void LuquiImgui::NewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

// Create all entity-related windows
void LuquiImgui::EntityWindows(ECSManager& ecsManager) {
	EntitySelector(ecsManager);
	EntityProperties(ecsManager);

	// Get camera component if available
	auto cameraComponent = ecsManager.getComponent<CameraComponent>(selectedEntity);
	if (cameraComponent.has_value()) {
		CameraProperties(*cameraComponent.value());
	}
}

// Create entity selector window
void LuquiImgui::EntitySelector(ECSManager& ecsManager) {
	ImGui::Begin("Selector de entidades");

	// Show all entities up to the next entity ID
	Entity nextEntity = ecsManager.get_nextEntity();
	std::string entityName;
	for (Entity i = 1; i < nextEntity; i++) {
		if (ecsManager.isEntityAlive(i)) {
			auto name_ = ecsManager.getComponent<NameComponent>(i).value()->name;
			if (ecsManager.getComponent<NameComponent>(i).has_value() && name_ != " ")
			{
				entityName = ecsManager.getComponent<NameComponent>(i).value()->name;
			}
			else
			{
				entityName = "Entity " + std::to_string(i);
			}
			if (ImGui::Selectable(entityName.c_str(), selectedEntity == i)) {
				selectedEntity = i;
			}
		}
	}

	// Add button to create a new entity
	if (ImGui::Button("Crear entidad")) {
		Entity newEntity = ecsManager.createEntity();
		selectedEntity = newEntity;
	}

	ImGui::End();
}

// Create entity properties window
void LuquiImgui::EntityProperties(ECSManager& ecsManager) {
	ImGui::Begin("Inspector de propiedades");

	if (selectedEntity > 0 && ecsManager.isEntityAlive(selectedEntity)) {
		ImGui::Text("Entity ID: %u", selectedEntity);

		// Get transform component
		auto transform = ecsManager.getComponent<TransformComponent>(selectedEntity);
		if (transform.has_value()) {
			if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::DragFloat3("Position", &transform.value()->position.x, 0.1f);
				ImGui::DragFloat3("Rotation", &transform.value()->rotation.x, 1.0f);
				ImGui::DragFloat3("Scale", &transform.value()->scale.x, 0.1f);
				if (ImGui::Button("Apply", ImVec2(150.0f, 50.0f)))
					ecsManager.editComponent<TransformComponent>(selectedEntity, [&transform](TransformComponent& tr) {tr.position = transform.value()->position; });
			}
		}

		// Get render component
		auto render = ecsManager.getComponent<RenderComponent>(selectedEntity);
		if (render.has_value()) {
			if (ImGui::CollapsingHeader("Render Component", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Checkbox("Active", &render.value()->active);
				if (ImGui::Button("	Set mesh")) {
					ImGui::OpenPopup("mesh_popup");
				}

				if (ImGui::BeginPopup("mesh_popup")) {
					ImGui::Text("Seleccionar modelo:");
					// Variable para almacenar el índice seleccionado (si es necesario)
					static int selectedIndex = -1;

					// Recorrer el vector y mostrar cada elemento como seleccionable
					for (int i = 0; i < ecsManager.resources.size(); i++) {
						if (ImGui::Selectable(ecsManager.resources[i]->get_name().c_str(), selectedIndex == i)) {
							selectedIndex = i;

							ecsManager.editComponent<RenderComponent>(selectedEntity, [&ecsManager, i](RenderComponent& renderComp) {
								renderComp.model = ecsManager.resources[i];
								});
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::EndPopup();
				}
				// Display model info if available
				if (render.value()->model) {
					ImGui::Text("Model loaded: Yes");
				}
				else {
					ImGui::Text("Model loaded: No");
				}
			}
		}

		// Get shape component
		auto shape = ecsManager.getComponent<ShapeComponent>(selectedEntity);
		if (shape.has_value()) {
			if (ImGui::CollapsingHeader("Shape Component", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Checkbox("Active", &shape.value()->active);

				const char* shapeTypes[] = { "Square", "Triangle", "Circle", "Cube", "Sphere" };
				int currentType = static_cast<int>(shape.value()->type);
				if (ImGui::Combo("Shape Type", &currentType, shapeTypes, IM_ARRAYSIZE(shapeTypes))) {
					shape.value()->type = static_cast<ShapeType>(currentType);
				}

				ImGui::ColorEdit4("Color", &shape.value()->color.x);
				ImGui::Text("Vertices: %zu", shape.value()->vertices.size());
			}
		}

		// Get animation component
		auto animation = ecsManager.getComponent<AnimationComponent>(selectedEntity);
		if (animation.has_value()) {
			if (ImGui::CollapsingHeader("Animation Component", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Checkbox("Active", &animation.value()->active);
				ImGui::DragFloat3("Translation", &animation.value()->translation.x, 0.01f);
				ImGui::DragFloat3("Rotation", &animation.value()->rotation.x, 0.1f);
				ImGui::DragFloat3("Scale", &animation.value()->scale.x, 0.01f);
				ImGui::DragFloat("Duration", &animation.value()->duration, 0.1f, 0.1f, 10.0f);
				ImGui::ProgressBar(animation.value()->elapsedTime / animation.value()->duration);
			}
		}

		// Get input component
		auto input = ecsManager.getComponent<InputComponent>(selectedEntity);
		if (input.has_value()) {
			if (ImGui::CollapsingHeader("Input Component", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Checkbox("Active", &input.value()->active);
				ImGui::Checkbox("Follow Mouse", &input.value()->followingMouse);
				ImGui::Text("Last Key: %d", input.value()->keyReceived);
			}
		}


		static std::string editingName;
		auto nameComponent = ecsManager.getComponent<NameComponent>(selectedEntity);

		if (ImGui::CollapsingHeader("Entity Name", ImGuiTreeNodeFlags_DefaultOpen)) {
			static Entity lastEntity = 0;
			if (lastEntity != selectedEntity) {
				editingName = nameComponent.value()->name;
				lastEntity = selectedEntity;
			}

			ImGui::InputText("Entity name", &editingName);

			if (ImGui::Button("Change Name")) {
				ecsManager.editComponent<NameComponent>(selectedEntity, [](NameComponent& entityName) {
					entityName.name = editingName;
					});
			}
		}
		// Add component button
		if (ImGui::Button("Add component")) {
			ImGui::OpenPopup("add_component_popup");
		}

		if (ImGui::BeginPopup("add_component_popup")) {
			ImGui::Text("Seleccionar componente:");



			if (!transform.has_value() && ImGui::Selectable("Transform Component")) {
				ecsManager.addComponent<TransformComponent>(selectedEntity);
				ecsManager.editComponent<TransformComponent>(selectedEntity, [](TransformComponent& c) {
					c.active = true;
					c.position = { 0.0f, 0.0f, 0.0f };
					c.rotation = { 0.0f, 0.0f, 0.0f };
					c.scale = { 1.0f, 1.0f, 1.0f };
					});
			}

			if (!render.has_value() && ImGui::Selectable("Render Component")) {
				ecsManager.addComponent<RenderComponent>(selectedEntity);
				ecsManager.editComponent<RenderComponent>(selectedEntity, [](RenderComponent& c) {
					c.active = true;
					c.model = nullptr;
					});
			}

			if (!shape.has_value() && ImGui::Selectable("Shape Component")) {
				ecsManager.addComponent<ShapeComponent>(selectedEntity);
				ecsManager.editComponent<ShapeComponent>(selectedEntity, [](ShapeComponent& c) {
					c = createSquare(1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
					});
			}

			if (!animation.has_value() && ImGui::Selectable("Animation Component")) {
				ecsManager.addComponent<AnimationComponent>(selectedEntity);
				ecsManager.editComponent<AnimationComponent>(selectedEntity, [](AnimationComponent& c) {
					c.active = true;
					c.translation = { 0.0f, 0.0f, 0.0f };
					c.rotation = { 0.0f, 0.0f, 0.0f };
					c.scale = { 0.0f, 0.0f, 0.0f };
					c.duration = 1.0f;
					c.elapsedTime = 0.0f;
					});
			}

			if (!input.has_value() && ImGui::Selectable("Input Component")) {
				ecsManager.addComponent<InputComponent>(selectedEntity);
				ecsManager.editComponent<InputComponent>(selectedEntity, [](InputComponent& c) {
					c.active = true;
					c.followingMouse = false;
					c.keyReceived = 0;
					});
			}

			ImGui::EndPopup();
		}
	}
	else {
		ImGui::Text("No entity selected");
	}

	ImGui::End();
}

// Create camera properties window
void LuquiImgui::CameraProperties(CameraComponent& camera) {
	ImGui::Begin("Inspector de la cámara principal");

	ImGui::Checkbox("Active", &camera.active);

	if (ImGui::CollapsingHeader("Position & Orientation", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
		ImGui::DragFloat("Yaw", &camera.yaw, 1.0f);
		ImGui::DragFloat("Pitch", &camera.pitch, 1.0f);

		if (ImGui::Button("Update Vectors")) {
			camera.updateCameraVectors();
		}
	}

	if (ImGui::CollapsingHeader("Projection Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
		bool updated = false;
		updated |= ImGui::SliderFloat("FOV", &camera.fov, 30.0f, 120.0f);
		updated |= ImGui::SliderFloat("Near Plane", &camera.nearPlane, 0.01f, 10.0f);
		updated |= ImGui::SliderFloat("Far Plane", &camera.farPlane, 10.0f, 1000.0f);
		updated |= ImGui::SliderFloat("Aspect Ratio", &camera.aspectRatio, 0.5f, 2.0f);

		if (updated) {
			camera.updateProjectionMatrix();
		}
	}

	if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::SliderFloat("Movement Speed", &camera.movementSpeed, 1.0f, 100.0f);
		ImGui::SliderFloat("Sprint Multiplier", &camera.sprintMultiplier, 1.0f, 5.0f);
		ImGui::SliderFloat("Mouse Sensitivity", &camera.mouseSensitivity, 0.01f, 1.0f);
	}

	ImGui::End();
}

// Render ImGui windows and finalize the frame
void LuquiImgui::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Entity LuquiImgui::GetSelectedEntity() const {
	return selectedEntity;
}

void LuquiImgui::SetSelectedEntity(Entity entity) {
	selectedEntity = entity;
}