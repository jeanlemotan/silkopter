-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------

newoption 
{
	trigger = "to",
	value   = "path",
	description = "Set the output location for the generated files"
}
  
newoption 
{
	trigger = "arch",
	value   = "String",
	description = "type of platform"
}

-- add a new flag in order to support the v110_xp toolset to support running apps on windows xp
premake.api.addAllowed("flags", "XpCompat") 
premake.api.addAllowed("flags", "LinkerWarningAsError")

premake.extensions.gl = {}

function premake.extensions.gl.customPlatformToolset(base, cfg)
	local p = cfg.flags.XpCompat and "v110_xp" or "v110"
	if _ACTION > "vs2012" then
		p = cfg.flags.XpCompat and "v120_xp" or "v120"
	end	
	if _ACTION > "vs2010" then
		_p(2,'<PlatformToolset>%s</PlatformToolset>', p)
	end
end

function premake.extensions.gl.customProgramDataBaseFileName(base, cfg)
	if cfg.flags.Symbols and cfg.debugformat ~= "c7" then
		if cfg.kind == premake.CONSOLEAPP then
			local filename = cfg.buildtarget.basename
			_p(3,'<ProgramDataBaseFileName>$(IntDir)vc$(PlatformToolsetVersion).pdb</ProgramDataBaseFileName>')
		else
			base(cfg)
		end	
	end
end


function premake.extensions.gl.customTreatLinkerWarningAsErrors(base, cfg)
	if cfg.flags.FatalWarnings and cfg.flags.LinkerWarningAsError then
		local el = iif(cfg.kind == premake.STATICLIB, "Lib", "Linker")
		_p(3,'<Treat%sWarningAsErrors>true</Treat%sWarningAsErrors>', el, el)
	end
end 

--todo add flag for custom database ???

premake.override(premake.vstudio.vc2010, 'platformToolset',  premake.extensions.gl.customPlatformToolset)
premake.override(premake.vstudio.vc2010, 'programDataBaseFileName',  premake.extensions.gl.customProgramDataBaseFileName)
premake.override(premake.vstudio.vc2010, 'treatLinkerWarningAsErrors',  premake.extensions.gl.customTreatLinkerWarningAsErrors)

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------

function IsVisualStudio(action)
	local act = action or _ACTION
	return act == "vs2013" or act == "vs2012" or act == "vs2010" or act == "vs2008"
end

function IsWindows()
	return os.is("windows")
end

function IsLinux()
	return os.is("linux")
end

function IsXCode(action)
	local act = action or _ACTION
	return act == "xcode4"
end

function IsMacSystem()
	return os.is("macosx")
end

function IsMac()
	return _OPTIONS['arch'] == 'macosx'
end

function IsIos()
	return _OPTIONS['arch'] == 'ios'
end

function GetPathFromPlatform(action)
	local act = action or _ACTION
	return act .."/" .._OPTIONS['arch']
end


if _ACTION=="android" then
include "../android/_android.lua"
end	

if _ACTION=="xcode4" then
include "../xcode/xcode.lua"
end

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------

debugFlags = {"Symbols", "StaticRuntime"}
releaseFlags = {"Symbols", "StaticRuntime"}

commonDefines = { "_SCL_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
 
debugDefines = {"_DEBUG"}
releaseDefines = {"NDEBUG", "_SECURE_SCL=0", "_SECURE_SCL_THROWS=0"}


-- for all release configurations including with exceptions
configuration "Release*"
	defines { releaseDefines, commonDefines }
	flags{ releaseFlags }
	optimize "Speed"

-- for all debug configurations including with exceptions
configuration "Debug*"
	defines { debugDefines, commonDefines }
	flags { debugFlags }	

configuration "Release"
	flags { "NoExceptions"}
	
configuration "Debug"
	flags { "NoExceptions"}
	
-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------

function addCommonXcodeBuildSettings()

	if (_OPTIONS['arch'] == "ios") then
	xcodebuildsettings
	{
		["GCC_VERSION"] = "com.apple.compilers.llvm.clang.1_0";
		["IPHONEOS_DEPLOYMENT_TARGET"] = "6.0";
		["TARGETED_DEVICE_FAMILY"]	= "1,2";
		["GCC_SYMBOLS_PRIVATE_EXTERN"] = "YES";
		["GCC_DYNAMIC_NO_PIC"] = "NO";			
		["GCC_C_LANGUAGE_STANDARD"] = "c11";			
		["CLANG_CXX_LANGUAGE_STANDARD"] = "c++11";
		["CLANG_CXX_LIBRARY"]  = "libc++";
		["ARCHS"] = "armv7 armv7s";
		["VALID_ARCHS"] = "armv7 armv7s";	
		["SDKROOT"] = "iphoneos";	
	}
	elseif (_OPTIONS['arch'] == "macosx") then
	xcodebuildsettings
	{
		["GCC_VERSION"] = "com.apple.compilers.llvm.clang.1_0";
		["GCC_SYMBOLS_PRIVATE_EXTERN"] = "YES";
		["GCC_DYNAMIC_NO_PIC"] = "NO";			
		["GCC_C_LANGUAGE_STANDARD"] = "c11";			
		["CLANG_CXX_LANGUAGE_STANDARD"] = "c++11";
		["CLANG_CXX_LIBRARY"]  = "libc++";		
		["SDKROOT"] = "macosx";	
	}
	end
end	

function addCommonConfig()

	language "C++"

	location ( _OPTIONS["to"] )
	
	configurations { "Debug", "Release" , "Debug Exceptions" , "Release Exceptions" }

	flags {"FatalWarnings", "NoMinimalRebuild" } 
		
	if IsVisualStudio(_ACTION) then
		flags { "MultiProcessorCompile" }
		defines { "_WIN32_WINNT=0x0501"} -- needed for boost 
		--buildoptions { "/EHsc"} -- neeed for c++ exception handler used but unwind semantics not enabled !!!
		buildoptions { "/wd4530"} -- silence warning about unwind semantics not enabled !!!
		--linkoptions { "/ignore:4221" } -- ignore empty obj file
	end 

	if (_OPTIONS['arch'] == "x64") then
		architecture "x64"
	end	

	if IsLinux() then
		buildoptions { "-std=c++11" }
		buildoptions { "-stdlib=libc++" }	
		linkoptions { "-stdlib=libc++" }	
	end	
end

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------
