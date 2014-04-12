-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------

include "../tools/premake/common/"

-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------
solution 'qdata'
	
	math.randomseed(string.hash(solution().name))

  	startproject "qdata"
	
	addCommonConfig()

---------------------------------------------------------------------------
-- 
---------------------------------------------------------------------------

external "qbase"

	location  ("../../qbase/prj/" .. GetPathFromPlatform()) 
	kind "StaticLib"
	uuid "0E517723-09BC-CF4A-A56F-5FBA2F07CDDF" 


project 'qdata'
	files 
	{
		 "../src/**",
		 "../include/**"
	}
	includedirs 
	{ 
		'../include/',
		'../../qbase/include', 
		'../../pugixml/src/',
		'../../../boost/'
	}

	if IsVisualStudio(_ACTION) then
		includedirs { "../../zlib/include" }
	end 

	kind 'StaticLib'
	uuid 'DC2E582A-3C90-944C-8031-57D82A54E253'

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
		pchheader "qdatastdafx.pch"
		pchsource "../src/QDataStdAfx.cpp"
	elseif IsVisualStudio(_ACTION) then
		pchheader "QDataStdAfx.h"
		pchsource "../src/QDataStdAfx.cpp"
	end

    		
	targetdir ("../lib/" .. GetPathFromPlatform())
	
	configuration "Debug"
		flags( "NoExceptions" )
		targetname( "qdata_d" )
	
	configuration "Release"
		flags( "NoExceptions" )
		targetname( "qdata" )
	

