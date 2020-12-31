workspace "MNISTIO"
   configurations { "Debug", "Release" }
   platforms { "Win32", "Win64", "Linux32", "Linux64" }

project "MNISTIO"
	kind "StaticLib"
	language "C"
	targetdir "bin/lib/%{cfg.buildcfg}%{cfg.architecture}"
	targetname "mnistio"

	includedirs {
			"include",
			"thirdparty/libbmp"
			}

	files { "include/**.h",
			"src/**.c",
			"thirdparty/libbmp/libbmp.c"
		}

	filter "configurations:Debug"
	  defines { "DEBUG" }
	  symbols "On"

	filter "configurations:Release"
	  defines { "NDEBUG" }
	  optimize "On"
	  
	filter "platforms:Win32"
	  architecture "x86"
	  system "windows"
	  defines {"WINDOWS"}

	filter "platforms:Win64"
	  architecture "x86_64"
	  system "windows"
	  defines {"WINDOWS"}
	  
	filter "platforms:Linux32"
	  architecture "x86"
	  system "linux"
	  defines {"LINUX"}

	filter "platforms:Linux64"
	  architecture "x86_64"
	  system "linux"
	  defines {"LINUX"}

project "MNISTIO_Demo"
	kind "ConsoleApp"
	language "C"
	targetdir "bin/demos/%{cfg.buildcfg}%{cfg.architecture}"
	targetname "demo"

	includedirs {
		"include"
	}
	
	libdirs {
	
	}
	
	links {
		"MNISTIO"
	}

	files { 
		"src/**.h",
		"demo/**.c"
	}
	
	debugargs{
		"mnist_data/train-images.idx3-ubyte",
		"mnist_data/train-labels.idx1-ubyte"
	}

	filter "configurations:Debug"
	  defines { "DEBUG" }
	  symbols "On"

	filter "configurations:Release"
	  defines { "NDEBUG" }
	  optimize "On"
	  
	filter "platforms:Win32"
	  architecture "x86"
	  system "windows"

	filter "platforms:Win64"
	  architecture "x86_64"
	  system "windows"
	  
	filter "platforms:Linux32"
	  architecture "x86"
	  system "linux"

	filter "platforms:Linux64"
	  architecture "x86_64"
	  system "linux"


project "test_bmp"
	kind "ConsoleApp"
	language "C"
	targetdir "bin/tests/%{cfg.buildcfg}%{cfg.architecture}"

	includedirs {
		"include"
	}
	
	libdirs {
		"include"
	}
	
	links {
		"MNISTIO"
	}

	files {
		"tests/test_bmp.c"
	}
	
	debugargs{
		"mnist_data/train-images.idx3-ubyte",
		"mnist_data/train-labels.idx1-ubyte"
	}

	filter "configurations:Debug"
	  defines { "DEBUG" }
	  symbols "On"

	filter "configurations:Release"
	  defines { "NDEBUG" }
	  optimize "On"
	  
	filter "platforms:Win32"
	  architecture "x86"
	  system "windows"

	filter "platforms:Win64"
	  architecture "x86_64"
	  system "windows"
	  
	filter "platforms:Linux32"
	  architecture "x86"
	  system "linux"

	filter "platforms:Linux64"
	  architecture "x86_64"
	  system "linux"

project "test_save_add"
	kind "ConsoleApp"
	language "C"
	targetdir "bin/tests/%{cfg.buildcfg}%{cfg.architecture}"

	includedirs {
		"include"
	}
	
	libdirs {
		"include"
	}
	
	links {
		"MNISTIO"
	}

	files {
		"tests/test_save_add.c"
	}
	
	debugargs{
		"mnist_data/train-images.idx3-ubyte",
		"mnist_data/train-labels.idx1-ubyte"
	}

	filter "configurations:Debug"
	  defines { "DEBUG" }
	  symbols "On"

	filter "configurations:Release"
	  defines { "NDEBUG" }
	  optimize "On"
	  
	filter "platforms:Win32"
	  architecture "x86"
	  system "windows"

	filter "platforms:Win64"
	  architecture "x86_64"
	  system "windows"
	  
	filter "platforms:Linux32"
	  architecture "x86"
	  system "linux"

	filter "platforms:Linux64"
	  architecture "x86_64"
	  system "linux"