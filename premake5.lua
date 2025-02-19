conan = {}
configs = {'Debug','Release','RelWithDebInfo'}
    for i = 1,3 do

    include("build/deps/"..configs[i].."/conanbuildinfo.premake.lua")
    conan[configs[i]] = {}
    local cfg = conan[configs[i]]
    cfg["build_type"] = conan_build_type
    cfg["arch"] = conan_arch
    cfg["includedirs"] = conan_includedirs
    cfg["libdirs"] = conan_libdirs
    cfg["bindirs"] = conan_bindirs
    cfg["libs"] = conan_libs
    cfg["system_libs"] = conan_system_libs
    cfg["defines"] = conan_defines
    cfg["cxxflags"] = conan_cxxflags
    cfg["cflags"] = conan_cflags
    cfg["sharedlinkflags"] = conan_sharedlinkflags
    cfg["exelinkflags"] = conan_exelinkflags
    cfg["frameworks"] = conan_frameworks

    end

function conan_config_exec()

configs = {'Debug','Release','RelWithDebInfo'}
    for i = 1,3 do

    local cfg = conan[configs[i]]
    filter("configurations:"..configs[i])

    linkoptions { cfg["exelinkflags"] }
    includedirs{ cfg["includedirs"] }
    libdirs{ cfg["libdirs"] }
    links{ cfg["libs"] }
    links{ cfg["system_libs"] }
    links{ cfg["frameworks"] }
    links{ cfg["assimp"] }
    
    defines{ cfg["defines"] }

    filter{}

    end

end

function conan_config_lib()

configs = {'Debug','Release','RelWithDebInfo'}
    for i = 1,3 do

    local cfg = conan[configs[i]]
    filter("configurations:"..configs[i])

    linkoptions { cfg["sharedlinkflags"] }
    includedirs{ cfg["includedirs"] }
    defines{ cfg["defines"] }

    filter{}

    end

end


workspace "Motor"

configurations { "Debug", "Release", "RelWithDebInfo" }
architecture "x64"
location "build"
cppdialect "c++17"
startproject "Window"
filter "configurations:Debug"
defines { "DEBUG" }
symbols "On"
runtime "Debug"

filter "configurations:Release"
defines { "NDEBUG" }
optimize "On"
runtime "Release"

filter "configurations:RelWithDebInfo"

defines { "NDEBUG" }
optimize "On"
runtime "Release"
symbols "On"

filter {}

project "Motor"

    kind "StaticLib"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { "include", "deps/assimp/include", "deps/glew/include",  "deps/glm", "deps/stb", "data"} 
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
        -- "src/ImguiPanel.cpp", "include/ImguiPanel.hpp",
        "src/ECS/Component.cpp", "include/ECS/Component.hpp",
        "src/ECS/System.cpp", "include/ECS/System.hpp",
        "src/ECS/ECSManager.cpp", "include/ECS/ECSManager.hpp",
    }

project "Window"

    kind "ConsoleApp"
    language "C++"
    targetdir "build/%{prj.name}/%{cfg.buildcfg}"
    includedirs "include"
    links "Motor"
    conan_config_exec("Debug")
    conan_config_exec("Release")
    conan_config_exec("RelWithDebInfo")
    debugargs { _MAIN_SCRIPT_DIR .. "/src/data" }
    files "examples/Window/main.cpp"

project "Triangle"

    kind "WindowedApp"
    language "C++"
    targetdir "build/%{prj.name}/%{cfg.buildcfg}"
    includedirs "include"
    links "Motor"
    conan_config_exec("Debug")
    conan_config_exec("Release")
    conan_config_exec("RelWithDebInfo")
    debugargs { _MAIN_SCRIPT_DIR .. "/src/data" }
    files "examples/Triangle/main.cpp"

project "OBJLoader"

    kind "ConsoleApp"
    language "C++"
    targetdir "build/%{prj.name}/%{cfg.buildcfg}"
    includedirs "include"
    links "Motor"
    conan_config_exec("Debug")
    conan_config_exec("Release")
    conan_config_exec("RelWithDebInfo")
    debugargs { _MAIN_SCRIPT_DIR .. "/src/data" }
    files "examples/OBJLoader/main.cpp"

project "JobSystem"

    kind "ConsoleApp"
    language "C++"
    targetdir "build/%{prj.name}/%{cfg.buildcfg}"
    includedirs "include"
    links "Motor"
    conan_config_exec("Debug")
    conan_config_exec("Release")
    conan_config_exec("RelWithDebInfo")
    debugargs { _MAIN_SCRIPT_DIR .. "/src/data" }
    files "examples/JobSystem/main.cpp"


project "ECS"

    kind "ConsoleApp"
    language "C++"
    targetdir "build/%{prj.name}/%{cfg.buildcfg}"
    includedirs "include"
    links "Motor"
    conan_config_exec("Debug")
    conan_config_exec("Release")
    conan_config_exec("RelWithDebInfo")
    debugargs { _MAIN_SCRIPT_DIR .. "/src/data" }
    files "examples/ECS/main.cpp"

project "Lights"

    kind "ConsoleApp"
    language "C++"
    targetdir "build/%{prj.name}/%{cfg.buildcfg}"
    includedirs "include"
    links "Motor"
    conan_config_exec("Debug")
    conan_config_exec("Release")
    conan_config_exec("RelWithDebInfo")
    debugargs { _MAIN_SCRIPT_DIR .. "/src/data" }
    files "examples/Lights/main.cpp"

project "Camera"

    kind "ConsoleApp"
    language "C++"
    targetdir "build/%{prj.name}/%{cfg.buildcfg}"
    includedirs "include"
    links "Motor"
    conan_config_exec("Debug")
    conan_config_exec("Release")
    conan_config_exec("RelWithDebInfo")
    debugargs { _MAIN_SCRIPT_DIR .. "/src/data" }
    files "examples/Camera/main.cpp"