math.randomseed(1005)

solution "zlib"

	language "C++"
	location ( _ACTION )

	flags {"Symbols", "NoMinimalRebuild",  "No64BitChecks", "StaticRuntime" , "NoExceptions" , "NoRTTI" }	

	configurations { "Debug", "Release" }
	
	configuration "Release"
		defines { "NDEBUG", "_SECURE_SCL=0", "_SECURE_SCL_THROWS=0", "_SCL_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}		
		flags{ "OptimizeSize"}
		
	configuration "Debug"
		defines { "_SCL_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS" }
	-- Multithreaded compiling
	if _ACTION == "vs2012" then
		buildoptions { "/MP", "/EHsc" }
		defines { "WIN32" }
	end 
	
project "zlib_lib"
	files { 
		"../src/*.c", 
		"../include/*.h",
		"../include/zlib/*.h"
	}
		
	kind "StaticLib"
	uuid "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"
	
	includedirs { 
		"../include/"
	}	
	
	targetdir ( "../lib/" .. _ACTION )
	configuration "Release"
		targetname( "zlib_lib" )
		
	configuration "Debug"
		targetname( "zlib_lib_d" )

