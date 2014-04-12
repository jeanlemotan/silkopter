-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------

include "../tools/premake/common/"

-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------
solution 'qbase'
	
	math.randomseed(string.hash(solution().name))

  	startproject "qbase"
	
	addCommonConfig()

---------------------------------------------------------------------------
-- 
---------------------------------------------------------------------------

project 'qbase'
	files 
	{
		 "../src/**",
		 "../include/**"
	}
	includedirs 
	{ 
		'../include/',
		'../../pugixml/src/',
		'../../../boost/'
	}

	kind 'StaticLib'
	uuid '0E517723-09BC-CF4A-A56F-5FBA2F07CDDF'

	if IsXCode() then
		addCommonXcodeBuildSettings()
 
		xcodebuildsettings 
		{
		}
	end
		
	if IsVisualStudio(_ACTION) then
		includedirs { "../../../libs/zlib/include/zlib" }
	end
	
	if _ACTION == "xcode4" then
		pchheader "qbasestdafx.pch"
		pchsource "../src/QBaseStdAfx.cpp"
	elseif IsVisualStudio(_ACTION) then
		pchheader "QBaseStdAfx.h"
		pchsource "../src/QBaseStdAfx.cpp"
	end

    		
	targetdir ("../lib/" .. GetPathFromPlatform())
	
	configuration "Debug"
		flags( "NoExceptions" )
		targetname( "qbase_d" )
	
	configuration "Release"
		flags( "NoExceptions" )
		targetname( "qbase" )
	

