-- namespace
	premake.qtcreator = { }
	local qc = premake.qtcreator

	
---- 
-- action for creating new projects from a solution
----
	newaction 
	{
		trigger = "qtc",
		shortname = "Qt Creator",		
		description = "Generates Qt Creator projects files (wraps GNU make)",

		valid_kinds = { "ConsoleApp", "WindowedApp", "SharedLib", "StaticLib" },
		valid_languages = { "C", "C++" },

		valid_tools     = {
			cc     = { "gcc"},
			dotnet = { },
		},

		onproject = function(prj)
		  premake.generate(prj, "%%.creator", premake.qtcreator.creator)
      premake.generate(prj, "%%.creator.user", premake.qtcreator.user)
      premake.generate(prj, "%%.files", premake.qtcreator.files)
      premake.generate(prj, "%%.includes", premake.qtcreator.includes)
      premake.generate(prj, "%%.config", premake.qtcreator.defines)
		end,

		oncleanproject  = function(prj)
    	premake.clean.file(prj, "%%.creator.user")
    	premake.clean.file(prj, "%%.creator")
    	premake.clean.file(prj, "%%.files")
    	premake.clean.file(prj, "%%.includes")
    	premake.clean.file(prj, "%%.config")
		end,
		
		oncleantarget   = function(trg)
		  local files = os.matchfiles(trg .."*")
	  for _, fname in ipairs(files) do
			  os.remove(fname)
		  end
		end
	}
	
  ----
  -- action to only update the .include file
  ----
	newaction 
	{
		trigger = "qtc-includes",
		shortname = "Qt Creator \".includes\"",		
		description = "Updates the \".includes\" file of the Qt Creator projects",

		valid_kinds = { "ConsoleApp", "WindowedApp", "SharedLib", "StaticLib" },
		valid_languages = { "C", "C++" },

		valid_tools     = {
			cc     = { "gcc"},
			dotnet = { },
		},

		onproject = function(prj)
      premake.generate(prj, "%%.includes", premake.qtcreator.includes)
		end,
	
		oncleanproject  = function(prj)
    	premake.clean.file(prj, "%%.includes")
		end,
		
	}
	
	----
  -- action to only update the .files
 ----
	newaction 
	{
		trigger = "qtc-files",
		shortname = "Qt Creator \".files\"",		
		description = "Updates the \".files\" file of the Qt Creator projects",

		valid_kinds = { "ConsoleApp", "WindowedApp", "SharedLib", "StaticLib" },
		valid_languages = { "C", "C++" },

		valid_tools     = {
			cc     = { "gcc"},
			dotnet = { },
		},

		onproject = function(prj)
      premake.generate(prj, "%%.files", premake.qtcreator.files)
		end,
	
		oncleanproject  = function(prj)
    	premake.clean.file(prj, "%%.files")
		end,
		
	}
