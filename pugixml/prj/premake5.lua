-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------

include "../tools/premake/common/"

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------
solution "pugixml"

	math.randomseed(string.hash(solution().name))
	
  	startproject "test"
		
	addCommonConfig()

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------
		
project "pugixml"
	
	files 
	{ 
		"../src/*.cpp", 		
		"../include/*.h"
	}
		
	includedirs 
	{ 
		"../include/", 
	}
	kind "StaticLib"
	
	uuid "89A1E353-E2DC-495C-B403-742BE206ACED"
	
	if IsXCode() then 

	addCommonXcodeBuildSettings()

	xcodebuildsettings 
	{
		["ONLY_ACTIVE_ARCH"] = "NO",  	
	}

		
	end
		
	targetdir ("../lib/" .. GetPathFromPlatform())
	
	configuration "Release"
		targetname( "pugixml" )
		
	configuration "Debug"
		targetname( "pugixml_d" )

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------
		
project "test"		
	files 
	{ 
		"../test/*.cpp",
		 "../test/*.h"
	}
	
	includedirs 
	{ 
		"../include",
		"../../boost/",
	}
	
	--removeflags "NoRTTI"
	
	if IsMac() or IsIos() then
		kind "WindowedApp"
		files { "xcode4/".._OPTIONS['arch'].."/Info.plist" }
	else
		kind "ConsoleApp"		
	end


	uuid "E9BF689B-7C0E-0A4C-84BE-D6B65FFD475E"

	if IsXCode() then 

	addCommonXcodeBuildSettings()
	
	xcodebuildsettings 
	{
		["CODE_SIGN_IDENTITY"] = "iPhone Developer: Valeri Vuchov (WDTMDP2J2J)",
	}
	
	end
		
	links 
	{ 		
		"pugixml",
	}

	targetdir ("../build/" .. GetPathFromPlatform())
		
	debugdir ("../data/")

	libdirs 
	{ 	
		"../lib/" .. GetPathFromPlatform(),
		'../../boost/stage/lib/' .. GetPathFromPlatform()
	}

	configuration "Release"
		targetname( "test" )
		if IsMac() or IsIos() then
		links { "boost_unit_test_framework"}
		end 
							
	configuration "Debug"
		targetname( "test_d" )
		if IsMac() or IsIos() then
		links { "boost_unit_test_framework_d"}
		end 
-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------
