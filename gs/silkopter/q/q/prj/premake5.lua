-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------

include "../tools/premake/common/"

-------------------------------------------------------------------------------
-- 
-------------------------------------------------------------------------------
solution 'q'
	
	math.randomseed(string.hash(solution().name))

  	startproject "q"
	
	addCommonConfig()

---------------------------------------------------------------------------
-- 
---------------------------------------------------------------------------

external "qbase"

	location  ("../../qbase/prj/" .. GetPathFromPlatform()) 

	kind "StaticLib"

	uuid "0E517723-09BC-CF4A-A56F-5FBA2F07CDDF"
	
external "qdata"

	location  ("../../qdata/prj/" .. GetPathFromPlatform()) 

	kind "StaticLib"

	uuid "DC2E582A-3C90-944C-8031-57D82A54E253"
	


project 'q'
	files 
	{
		 "../src/**",
		 "../include/**"
	}
	includedirs 
	{ 
		'../include/',
		'../../qbase/include/',
		'../../qdata/include/',
		'../../qmath/include/',
		'../../pugixml/src/',
		'../../../../../fw/silkopter/libs/qmath/',
		'../../freetype2/include/',
		'../../../boost/'
	}

	if IsIos() then	
		excludes 
		{ 
			'../src/video/displays/EGL_Display.cpp', 
			'../src/video/displays/WGL_Display.cpp',
		}
	elseif IsVisualStudio(_ACTION) then
		excludes 
		{ 
			'../src/video/displays/EAGL_View.mm', 
			'../src/video/displays/IOS_Display.mm',
			'../src/video/displays/IOS_Display.cpp',
			'../src/video/displays/EGL_Display.cpp'
		}
	end 
	
	kind 'StaticLib'
	uuid '21F5D54C-9378-564C-9C9B-9F30C1B042BF'
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
		pchheader "qstdafx.pch"
		pchsource "../src/QStdAfx.cpp"
	elseif IsVisualStudio(_ACTION) then
		pchheader "QStdAfx.h"
		pchsource "../src/QStdAfx.cpp"
	end

    		
	targetdir ("../lib/" .. GetPathFromPlatform())
	
	configuration "Debug"
		flags( "NoExceptions" )
		targetname( "q_d" )
	
	configuration "Release"
		flags( "NoExceptions" )
		targetname( "q" )
	

