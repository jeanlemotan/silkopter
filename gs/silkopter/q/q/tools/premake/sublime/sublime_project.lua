--
-- sublime_project.lua
-- Generate a Sublime Text 2 workspace.
--

	function premake.sublime.project(prj, folders)

		-- Go through the build configurations and builda  list of unique include directories
		for cfg in premake.eachconfig(prj) do
			for _,v in ipairs(cfg.includedirs) do
				if not table.contains(folders, v) then
					table.insert(folders, v)
				end
			end
		end

		-- List out the folders and files that make up the build
		local tr = premake.project.buildsourcetree(prj)
		premake.tree.sort(tr)
		premake.tree.traverse(tr, {
			onbranch = function(node, depth)
				local abspath = path.getrelative(prj.solution.location, node.name)
				if not table.contains(folders, abspath) then
					table.insert(folders, abspath)
				end
			end,

			onleaf = function(node, depth)
				--_p(depth, path.getname(node.name))
			end
		})

	end