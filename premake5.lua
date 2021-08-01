workspace "lilGUI"
  location "build"
  architecture "x86_64"
  language "C++"
  cppdialect "C++17"
  staticruntime "On"
  startproject "lilTest"

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

project "lilGUI"
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

project "lilTest"
  location "build"
  kind "ConsoleApp"

  targetdir "build/lilTest/bin"
  objdir "build/lilTest/bin-obj"

  files
  {
    "test/**.cpp",
    "test/**.h"
  }

  links
  {
    "lilGUI"
  }

  includedirs
  {
    "src",
    "includes"
  }

  filter "configurations:Debug"
    runtime "Debug"
    defines "LIL_DEBUG"
    symbols "On"

  filter "configurations:Release"
    runtime "Release"
    defines "LIL_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    runtime "Release"  
    defines "LIL_DIST"
    optimize "Full"
    
  filter "system:macosx"
    defines "LIL_MACOS"
    
    libdirs "thirdparty/macos"
    links
    {
      "Cocoa.framework",
      "IOKit.framework",
      "glfw3"
    }
