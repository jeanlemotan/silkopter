
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
--
-- define the qt extension
--

premake.extensions.qt = {}
-- path 
--premake.extension.qt.source = "source"

generatedFilesPath = "GeneratedFiles/"

--
-- Adds qt support.
--
-- @param	qtpath The path to your Qt installation. It should contains the
--			include and lib folders. If ommited the directory used will
--			be QTDIR system environment variable.
--
function premake.extensions.qt.add_support(qtpath)

	local qt		= premake.extensions.qt
	local project	= premake.project
	local qmakebin
	local majorVersion
	local defaultQtPath = "../../../../libs/3rdParty/Qt/qtbase"

	if _ACTION == "vs2013" or _ACTION == "vs2012" or _ACTION == "vs2010" or _ACTION=="vs2008" then
		local matchrx = "%$%((.[^)]*)%)"
		local currentMatch
		
		qtpath = qtpath or defaultQtPath
		qmakebin = qtpath .. "/bin/qmake"
		currentMatch = string.match(qmakebin, matchrx)
		while currentMatch do
			local envvar = os.getenv(currentMatch)
			
			qmakebin = string.gsub(qmakebin, "%$%(" .. currentMatch .. "%)", envvar)
			currentMatch = string.match(qmakebin, matchrx)
		end
	elseif _ACTION =="xcode4" then
		qtpath = qtpath or "$QTDIR"
		qmakebin = qtpath .. "/bin/qmake"
	end 
		
	if qtpath == defaultQtPath then
		local dir = path.getabsolute(defaultQtPath)
		dir = path.translate(dir, "/")
		local path = path.getdirectory(dir)		
		if not os.isdir(path) then
			error("$QTDIR doesn't point to a dir " ..dir, 1)
		end	
	end

	local qmake = assert(io.popen(path.getabsolute(qmakebin) .. " -v", 'r'))
	local output = qmake:read('*all')
	qmake:close()

	print("Using "..output)
	
	majorVersion = string.match(output, "Using Qt version (%d)%.%d%.%d")
	
	-- store the Qt version
	qt.version = tonumber(majorVersion)

	-- store the qt path and format it correctly
	qt.path = string.gsub(qtpath, "/", "\\")
	if string.find(qt.path, "\\", -1, true) == nil then
		qt.path = qt.path .. "\\"
	end
	

	if qt.version >= 5 then
		includedirs { qt.path .. "include/QtANGLE" }
		--postbuildcommands { qt.get_copy_command(qt.path .. "bin\\icudt51.dll", "%{cfg.targetdir}") }
		--postbuildcommands { qt.get_copy_command(qt.path .. "bin\\icuin51.dll", "%{cfg.targetdir}") }
		--postbuildcommands { qt.get_copy_command(qt.path .. "bin\\icuuc51.dll", "%{cfg.targetdir}") }
	end

	-- setup our overrides
	premake.override(premake.oven, 'bakeFiles', qt.custombakefiles)
	premake.override(premake.vstudio.vc2010, 'getfilegroup',  qt.customgetfilegroup)
	premake.override(premake.vstudio.vc200x, 'fileConfiguration',  qt.fileConfiguration)

	-- and add the common part
	includedirs { qt.path .. "include" }
	libdirs { qt.path .. "lib" }
end

function premake.extensions.qt.fileConfiguration(base, cfg, node, depth)
	local qt = premake.extensions.qt
	local prj = {}
	local p = node
	
	while(p.parent) do
	    p = p.parent
	end
	
	prj = p.prj
	
	-- check ui files
	if qt.is_ui(node.abspath) then
		qt.add_ui_custombuild(prj, node)

	-- check qrc file
	elseif qt.is_qrc(node.abspath) then
		qt.add_qrc_custombuild(prj, node)

	-- check moc headers
	elseif qt.need_moc(node.abspath) then
		qt.add_moc_custombuild(prj, node)

	end

	return base(cfg, node, depth)
end

--
-- Override the project.bakeFile method to be able to add the Qt generated files
-- to the project.
-- The custom build rules for those files are handled in the getfilegroup override.
--
-- @param	base The original getFiles method.
-- @param	prj The current project.
-- @return	The table of files.
--
function premake.extensions.qt.custombakefiles(base, prj)

	local qt		= premake.extensions.qt
	local project	= premake.project

	-- parse the configurations for the project
	for cfg in project.eachconfig(prj) do

		--print(cfg.name)

		local mocs	= {}
		local qrc	= {}
		local ui    = {}

		-- add the objdir as an include path
		table.insert(cfg.includedirs, generatedFilesPath .. "/".. cfg.name)

		-- check each file in this configuration
		table.foreachi(cfg.files, function(filename)

			if qt.is_qrc(filename) then
				table.insert(qrc, filename)
			elseif qt.need_moc(filename) then
				table.insert(mocs, filename)
			end

		end)

		-- the moc files
		table.foreachi(mocs, function(filename)
			table.insert(cfg.files, generatedFilesPath .."/".. cfg.name .."/" .. "moc_" .. path.getbasename(filename) .. ".cpp")
		end)

		-- the qrc files
		table.foreachi(qrc, function(filename)
			table.insert(cfg.files, generatedFilesPath .."/".. cfg.name .."/" .. "qrc_" .. path.getbasename(filename) .. ".cpp")
		end)

	end

	return base(prj)

end

--
-- override getfilegroup to create custom build rules for special Qt files.
-- @param	base The original getfilegroup function.
-- @param	prj The current project.
-- @param	group The wanted group.
-- @return	prj The list of nodes for the given group.
--
function premake.extensions.qt.customgetfilegroup(base, prj, group)

	local qt = premake.extensions.qt

	-- only filter if the groups are not already created
	local groups = prj.vc2010_file_groups
	if not groups then

		-- parse the whole tree
		local tr = premake.project.getsourcetree(prj)
		premake.tree.traverse(tr, {
			onleaf = function (node)

				-- check ui files
				if qt.is_ui(node.abspath) then
					qt.add_ui_custombuild(prj, node)

				-- check qrc file
				elseif qt.is_qrc(node.abspath) then
					qt.add_qrc_custombuild(prj, node)

				-- check moc headers
				elseif qt.need_moc(node.abspath) then
					qt.add_moc_custombuild(prj, node)

				end
			end
		})
	end

	return base(prj, group)

end

--
-- This function allow adding Qt modules. It configure include paths, library
-- paths and the linked libraries for the current configuration.
--
-- @param	modulename The module name. Available ones are:
--				* core
--				* gui
--				* widgets
--				* opengl
-- @param	suffix A suffix. Used to differenciate debug and release. Usually
--			its empty for release builds and 'd' for debug ones.
--
function premake.extensions.qt.add_module(modulename, suffix)

	local qt = premake.extensions.qt

	-- the modules' definitions
	local modules 

	if qt.version == 5 then
		modules = {
			core = { 
				name = "Qt5Core",
				include = "QtCore"
			},
			gui = {
				name = "Qt5Gui",
				include = "QtGui",
				dependencies = { "libGLESv2", "libEGL" }
			},
			widgets = {
				name = "Qt5Widgets",
				include = "QtWidgets"
			},
			webkit = {
				name = "Qt5WebKit",
				include = "QtWebKit"
			},
			webkitwidgets = {
				name = "Qt5WebKitWidgets",
				include = "QtWebKitWidgets"
			},
			multimediawidgets = {
				name = "Qt5MultimediaWidgets",
				include = "QtMultimediaWidgets"
			},
			multimedia = {
				name = "Qt5Multimedia",
				include = "QtMultimedia"
			},
			network = {
				name = "Qt5Network",
				include = "QtNetwork"
			},
			printsupport = {
				name = "Qt5PrintSupport",
				include = "QtPrintSupport"
			},
			quick = {
				name = "Qt5Quick",
				include = "QtQuick"
			},
			qml = {
				name = "Qt5Qml",
				include = "QtQml"
			},
			declarative = {
				name = "Qt5Declarative",
				include = "QtDeclarative"
			},
			v8 = {
				name = "Qt5V8",
				include = "QtV8"
			},
			sql = {
				name = "Qt5Sql",
				include = "QtSql"
			},
			sensors = {
				name = "Qt5Sensors",
				include = "QtSensors"
			},
			opengl = {
				name = "Qt5OpenGL",
				include = "QtOpenGL",
				links = { "opengl32" },
				dependencies = { "libGLESv2", "libEGL" }
			}
		}
	else	
		modules = {
			core = {
				name = "QtCore",
				include = "QtCore"
			},
			gui = {
				name = "QtGui",
				include = "QtGui"
			},
			webkit = {
				name = "QtWebKit",
				include = "QtWebKit"
			},
			opengl = {
				name = "QtOpenGL",
				include = "QtOpenGL",
				links = { "opengl32" }
			}
		}
		modules.widgtes = modules.gui
		modules.webkitwidgets = modules.webkit
	end

	-- add the module
	if modules[modulename] ~= nil then

		local module	= modules[modulename]
		local name
		local dllname

		if qt.version == 5 then
			name = module.name .. suffix
		else
			name = module.name .. suffix .. "4"
		end
		dllname = name .. ".dll"
		-- configure the module
		includedirs { qt.path .. "include/" .. module.include }
		links { name .. ".lib" }

		-- add additional links
		if module.links ~= nil then
			links { module.links }
		end
		
		--postbuildcommands { qt.get_copy_command(qt.path .. "bin\\" .. dllname, "%{cfg.targetdir}") }

		if module.dependencies then
			table.foreachi(module.dependencies, function(dependency)
				local dllname = dependency .. suffix .. ".dll"
				
				--postbuildcommands { qt.get_copy_command(qt.path .. "bin\\" .. dllname, "%{cfg.targetdir}") }
			end)
		end
	end

end

--
-- Checks if a file node is a ui file.
--
-- @param	filename The file name to check.
-- @return	true if the file needs to be run through the uic tool, false if not.
--
function premake.extensions.qt.is_ui(filename)
	return path.hasextension(filename, { ".ui" })
end

--
-- Adds the custom build for ui files.
--
-- @param	prj The current project.
-- @param	node The ui file node.
--
function premake.extensions.qt.add_ui_custombuild(prj, node)

	local qt = premake.extensions.qt

	-- add the custom build for each project configuration
	for cfg in premake.project.eachconfig(prj) do

		-- get the output file
		local output = generatedFilesPath .. "ui_" .. node.basename .. ".h"

		local prjLocal = path.getrelative(prj.location, path.getabsolute(qt.path));
		prjLocal = path.translate(prjLocal, "\\")

		-- add the custom build rule
		local filecfg = premake.fileconfig.getconfig(node, cfg)
		filecfg.buildmessage	= "Uic'ing " .. filecfg.name
		filecfg.buildcommands	= { prjLocal .. "\\bin\\uic.exe -o \"" .. output .. "\" \"" .. node.relpath .. "\"" }
		filecfg.buildoutputs	= { output }
	end

end

--
-- Checks if a file node is a qrc file.
--
-- @param	filename The file name to check.
-- @return	true if the file needs to be run through the rcc tool, false if not.
--
function premake.extensions.qt.is_qrc(filename)
	return path.hasextension(filename, { ".qrc" })
end

--
-- Adds the custom build for ui files.
--
-- @param	prj The current project.
-- @param	node The ui file node.
--
function premake.extensions.qt.add_qrc_custombuild(prj, node)

	local qt = premake.extensions.qt

	-- add the custom build for each project configuration
	for cfg in premake.project.eachconfig(prj) do

		-- get the output file
		local output =  generatedFilesPath .. "/".. cfg.name .. "/" .. "qrc_" .. node.basename .. ".cpp"

		local commandRelPath = path.getrelative(prj.location, path.getabsolute(qt.path));
		commandRelPath = path.translate(commandRelPath, "\\")
		
		-- add the custom build rule
		local filecfg = premake.fileconfig.getconfig(node, cfg)
		filecfg.buildmessage	= "Rcc'ing " .. filecfg.name
		filecfg.buildcommands	= { commandRelPath .. "\\bin\\rcc.exe -name \"" .. node.basename .. "\" -no-compress \"" .. node.relpath .. "\" -o \"" .. output .. "\"" }
		filecfg.buildoutputs	= { output }
	end

end

--
-- Checks if a file needs moc'ing.
--
-- @param	filename The file name to check.
-- @return	true if the header needs to be run through the moc tool, false if not.
--
function premake.extensions.qt.need_moc(filename)

	local needmoc = false

	-- only handle headers
	if path.iscppheader(filename) then

		-- open the file
		local file = io.open(filename)

		-- scan it to find 'Q_OBJECT'
		for line in file:lines() do
			if line:find("Q_OBJECT") then
				needmoc = true
				break
			end
		end

		io.close(file)
	end

	return needmoc
end

--
-- Adds the custom build for a moc'able file.
--
-- @param	prj The current project
-- @param	node The moc'able header file node.
--
function premake.extensions.qt.add_moc_custombuild (prj, node)

	local qt = premake.extensions.qt

	-- parse configurations
	for cfg in premake.project.eachconfig(prj) do

		-- create the output file name
		local output = generatedFilesPath .. "/".. cfg.name .. "/" .. "moc_" .. node.basename .. ".cpp"

		local commandRelPath = path.getrelative(prj.location, path.getabsolute(qt.path));
		commandRelPath = path.translate(commandRelPath, "\\")

		-- create the moc command
		local command = commandRelPath .. "\\bin\\moc.exe \"" .. node.relpath .. "\" -o \"" .. output .. "\""

		local prjLocal = path.getrelative(prj.location, path.getabsolute(qt.source));

		
		-- if we have a precompiled header, add it, and don't forget to add the original header:
		-- the moc command will override it if we specify a -f include
		if cfg.pchheader then
			command = command .. " \"-f" .. cfg.pchheader .. "\" -f\"" .. path.getrelative(prjLocal ,node.relpath) .. "\""
		end

		-- append the defines to the command
		if #cfg.defines > 0 then
			table.foreachi(cfg.defines, function (define)
				command = command .. " -D" .. define
			end)
		end

		-- append the include directories to the command
		if #cfg.includedirs > 0 then
			table.foreachi(cfg.includedirs, function (include)
				command = command .. " -I\"" .. path.getrelative(prj.location,include) .. "\""
			end)
		end

		-- add the custom build rule
		local filecfg = premake.fileconfig.getconfig(node, cfg)
		filecfg.buildmessage	= "Moc'ing " .. filecfg.name
		filecfg.buildcommands	= { command }
		filecfg.buildoutputs	= { output }

	end

end

--
-- Generate a command line copy
--
-- @param	source The source filename
-- @param	destination The destination filename
-- @return	The command line
--
function premake.extensions.qt.get_copy_command(source, destination)
	local command
	if _ACTION == "vs2012" or _ACTION == "vs2010" or _ACTION=="vs2008" then
		command = "copy /Y"
	elseif _ACTION =="xcode4" then
		command = "cp"
	end 
	return command.." \"" .. source .. "\" \"" .. destination .. "\""
end
