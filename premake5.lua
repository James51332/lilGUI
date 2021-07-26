workspace "littleGUI"
  location "build"
  architecture "x86_64"
  language "C++"
  cppdialect "C++17"
  staticruntime "On"
  startproject "testGUI"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

  flags
  {
    "MultiProcessorCompile"
  }

project "littleGUI"
  location "build"
  kind "StaticLib"
  
  targetdir "build/littleGUI/bin"
  objdir "build/littleGUI/bin-obj"

  files
  {
    "src/**.cpp",
    "src/**.h"
  }

  filter "configurations:Debug"
    runtime "Debug"
    defines "LITTLE_DEBUG"
    symbols "On"

  filter "configurations:Release"
    runtime "Release"
    defines "LITTLE_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    runtime "Release"  
    defines "LITTLE_DIST"
    optimize "Full"

project "testGUI"
  location "build"
  kind "ConsoleApp"

  targetdir "build/testGUI/bin"
  objdir "build/testGUI/bin-obj"

  files
  {
    "test/**.cpp",
    "test/**.h"
  }

  links
  {
    "littleGUI"
  }

  includedirs
  {
    "src"
  }

  filter "configurations:Debug"
    runtime "Debug"
    defines "LITTLE_DEBUG"
    symbols "On"

  filter "configurations:Release"
    runtime "Release"
    defines "LITTLE_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    runtime "Release"  
    defines "LITTLE_DIST"
    optimize "Full"
  