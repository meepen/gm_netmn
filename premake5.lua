workspace "netmn"

	configurations { "Debug32", "Release32", "Debug64", "Release64" }

	language "C++"
	location "./proj"

	characterset "MBCS"

	filter "configurations:*64"
		libdirs "lib64"
		architecture "x86_64"

	filter "configurations:*32"
		libdirs "lib"
		architecture "x86"



	project "netmn"
		flags { "NoPCH", "NoImportLib" }

		symbols "On"
		vectorextensions "SSE"

		staticruntime "on"
		kind "SharedLib"

		targetdir "bin"

		links {
			"tier0",
			"vstdlib",
		}

		includedirs {
			"gmod-module-base/include",
			"src",
			"mysql",
			"luajit",
			"source-engine-2013",
			"source-engine-2013/tier0",
			"source-engine-2013/tier1",
			"source-engine-2013/vstdlib",
		}

		files {
			"src/**.cpp",
			"src/**.hpp",
			"src/**.h",

			"source-engine-2013/**.cpp",
			"source-engine-2013/**.c",
			"source-engine-2013/**.hpp",
			"source-engine-2013/**.h",

			"source-engine-2013/tier0/**.cpp",
			"source-engine-2013/tier0/**.c",
			"source-engine-2013/tier0/**.hpp",
			"source-engine-2013/tier0/**.h",

			"source-engine-2013/tier1/**.cpp",
			"source-engine-2013/tier1/**.c",
			"source-engine-2013/tier1/**.hpp",
			"source-engine-2013/tier1/**.h",

			"source-engine-2013/vstdlib/**.cpp",
			"source-engine-2013/vstdlib/**.c",
			"source-engine-2013/vstdlib/**.hpp",
			"source-engine-2013/vstdlib/**.h",

			"luajit/**.hpp",
			"luajit/**.h",
		}

		defines {
			"GMMODULE",
			"GMOD_USE_SOURCESDK",
		}

		targetprefix "gmsv_"

		filter "system:linux"
			linkoptions { "-Wl,--as-needed" }

		filter "system:windows"
			links "legacy_stdio_definitions"

		filter "configurations:Debug*"
			optimize "Debug"

		filter "configurations:Release*"
			optimize "On"


		filter { "system:windows", "configurations:*32" }
			targetextension "_win32.dll"
		
		filter { "system:windows", "configurations:*64" }
			targetextension "_win64.dll"
		
		filter { "system:linux", "configurations:*32" }
			targetextension "_linux32.dll"
		
		filter { "system:linux", "configurations:*64" }
			targetextension "_linux64.dll"