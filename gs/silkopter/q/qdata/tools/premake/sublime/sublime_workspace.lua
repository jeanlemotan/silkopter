--
-- sublime_workspace.lua
-- Generate a Sublime Text 2 workspace.
--

	function premake.sublime.addpath(path, lastpath)
		_p(2, '{')
		_p(3, '"path": "%s"', path)
		if lastpath then
			_p(2, '}')
		else
			_p(2, '},')
		end
	end

	function premake.sublime.solution(sln)
		_p('{')
		_p(1, '"folders":')
		_p(1, '[')

		-- collect the list of folders to be added by iterating through the projects
		local folders = {}
		for prj in premake.solution.eachproject(sln) do
				premake.sublime.project(prj, folders)
		end

		-- write out the list of collected folders
		local numfolders = # folders
		local folderindex = 1
		for _,v in ipairs(folders) do
				if folderindex == numfolders then
					premake.sublime.addpath(v, true)
				else
					premake.sublime.addpath(v, false)
				end
				folderindex = folderindex + 1
		end

		_p(1, ']')
		_p('}')

	end