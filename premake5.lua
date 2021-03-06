workspace "Andromeda"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Andromeda"
	location "Andromeda"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Source/**.hpp"
	}

	includedirs
	{
		"Dependencies/GLEW",
		"Dependencies/GLFW",
		"Dependencies/ASSIMP/include",
		"%{prj.name}/Source",
		"%{prj.name}/Source/**"
	}
	
	libdirs
	{
		"Dependencies/GLEW",
		"Dependencies/GLFW",
		"Dependencies/ASSIMP/lib"
	}
	
	links {
		"glew32s.lib",
		"glfw3.lib",
		"opengl32.lib",
		"zlibstaticd.lib",
		"IrrXMLd.lib",
		"assimpd.lib"
	}
	
	cppdialect "C++17"
	staticruntime "off"
	systemversion "latest"
	
	defines
	{
		"GLEW_BUILD",
		"GLFW_INCLUDE_NONE"
	}
	
	filter "system:not windows"
		
		defines
		{
			"AD_PLATFORM_OTHER"
		}

	filter "system:windows"

		defines
		{
			"AD_PLATFORM_WINDOWS"
		}
		
	filter {}

	filter "configurations:Debug"
		defines "AD_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "AD_RELEASE"
		optimize "On"

	filter {}