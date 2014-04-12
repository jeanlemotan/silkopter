--
-- _sublime.lua
-- Define action for creating a Sublime Text 2 project
--

	premake.sublime = { }
	local sublime = premake.sublime


	-- Register the "sublime" action

	newaction
	{
		trigger = "sublime",
		shortname = "Sublime Text 3",
		description = "Generates .sublime-project files",
		valid_kinds = { "ConsoleApp", "WindowedApp", "SharedLib", "StaticLib" },
		valid_languages = { "C", "C++", "C#" },
		valid_tools     = {
			cc     = { "msc", "gcc", "ow" },
			dotnet = { "mono", "msnet", "pnet" },
		},


		onsolution = function(sln)
			premake.generate(sln, "%%.sublime-project", premake.sublime.solution)
		end,

		oncleansolution = function(sln)
			premake.clean.file(sln, "%%.sublime-project")
		end,
	}
