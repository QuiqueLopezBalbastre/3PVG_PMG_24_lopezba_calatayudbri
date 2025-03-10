project "LUQUI"

    kind "StaticLib"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { "include", "deps/assimp/include", "deps/glew/include",  "deps/glm", "deps/stb", "data", "deps/imgui"} 
    libdirs { "deps/assimp/lib", "deps/glew/lib"}
    -- UseLibs {"glload", "freeglut"}
    conan_config_lib()
    files {
        "premake5.lua",
        "src/build/conanfile.txt",
        "src/build/conan.lua",
        -- "src/stdafx.cpp", "src/stdafx.hpp",
        "src/WindowSystem.cpp", "include/WindowSystem.hpp",
        "src/Window.cpp", "include/Window.hpp",
        "src/Figure.cpp", "include/Figure.hpp",
        "src/Input.cpp", "include/Input.hpp",
        "src/Shader.cpp", "include/Shader.hpp",
        "src/Program.cpp", "include/Program.hpp",
        "src/ModelLoader/Model.cpp", "include/ModelLoader/Model.hpp",
        "src/ModelLoader/Mesh.cpp", "include/ModelLoader/Mesh.hpp",
        "include/common.hpp", "src/stb_image.cpp",
        -- "src/Color.cpp", "include/Color.hpp",
        "src/JobSystem.cpp", "include/JobSystem.hpp",
        "src/Scripting.cpp", "include/Scripting.hpp",
        "src/Imgui.cpp", "include/Imgui.hpp",
        "src/ECS/Component.cpp", "include/ECS/Component.hpp",
        "src/ECS/System.cpp", "include/ECS/System.hpp",
        "src/ECS/ECSManager.cpp", "include/ECS/ECSManager.hpp",

        "deps/imgui/imgui.h",  "deps/imgui/imgui.cpp",
        "deps/imgui/imgui_impl_opengl3.h", "deps/imgui/imgui_impl_opengl3.cpp",
        "deps/imgui/imgui_impl_glfw.h", "deps/imgui/imgui_impl_glfw.cpp",
        "deps/imgui/imgui_tables.cpp", "deps/imgui/imgui_draw.cpp", "deps/imgui/imgui_widgets.cpp",
        "deps/imgui/imgui_stdlib.h", "deps/imgui/imgui_stdlib.cpp",
    }