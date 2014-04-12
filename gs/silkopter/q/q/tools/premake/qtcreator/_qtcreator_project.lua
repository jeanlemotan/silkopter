-- An example project generator; see _example.lua for action description

-- 
-- The project generation function, attached to the action in _example.lua.
-- By now, premake.generate() has created the project file using the name
-- provided in _example.lua, and redirected input to this new file.
--
	local qc = premake.qtcreator
	
  function premake.qtcreator.header()
    _p("<!DOCTYPE QtCreatorProject>")
	  _p("<qtcreator>")
	  
		_p("<data>")
    _p(1,"<variable>ProjectExplorer.Project.EditorSettings</variable>")
    _p(1,"<valuemap type=\"QVariantMap\">")
    _p(2,"<value key=\"EditorConfiguration.UseGlobal\" type=\"bool\">true</value>")
		_p(1,"</valuemap>")
		_p("</data>")
	
  end
  
  function premake.qtcreator.footer()
   
  _p("<data>")
  _p(1, "<variable>ProjectExplorer.Project.Updater.FileVersion</variable>")
  _p(1, "<value type=\"int\">9</value>")
  _p("</data>")
  
  _p("</qtcreator>")
  end
  
  function premake.qtcreator.creator(prj)
    _p("[General]")
  end
  
  function premake.qtcreator.run_configuration(prj, cfg, cfgCounter)
    _p(2, "<valuemap key=\"ProjectExplorer.Target.RunConfiguration.%d\" type=\"QVariantMap\">", cfgCounter)
    _p(3, "<value key=\"ProjectExplorer.CustomExecutableRunConfiguration.Executable\" type=\"QString\">%s</value>", cfg.buildtarget.fullpath)
    _p(3, "<value key=\"ProjectExplorer.CustomExecutableRunConfiguration.UseTerminal\" type=\"bool\">false</value>")
    _p(3, "<value key=\"ProjectExplorer.CustomExecutableRunConfiguration.WorkingDirectory\" type=\"QString\">%%{buildDir}</value>")
    _p(3, "<value key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\" type=\"QString\">execute %s</value>", cfg.buildtarget.fullpath)
    _p(3, "<value key=\"ProjectExplorer.ProjectConfiguration.DisplayName\" type=\"QString\">%s</value>", cfg.longname)
    _p(3, "<value key=\"ProjectExplorer.ProjectConfiguration.Id\" type=\"QString\">ProjectExplorer.CustomExecutableRunConfiguration</value>")
    _p(3, "<value key=\"RunConfiguration.UseCppDebugger\" type=\"bool\">true</value>")
    _p(3, "<value key=\"RunConfiguration.UseQmlDebugger\" type=\"bool\">false</value>")
    _p(2, "</valuemap>");
  end
  
  function premake.qtcreator.build_configuration(prj, cfg, cfgCounter)
    _p(2, "<valuemap key=\"ProjectExplorer.Target.BuildConfiguration.%d\" type=\"QVariantMap\">", cfgCounter)
    _p(3, "<value key=\"GenericProjectManager.GenericBuildConfiguration.BuildDirectory\" type=\"QString\">%s</value>", path.getrelative(cfg.basedir, prj.solution.location))
    _p(3, "<value key=\"ProjectExplorer.BuildCOnfiguration.ToolChain\" type=\"QString\">INVALID</value>")
    
    -- the build steps for "Make"
    _p(3, "<valuemap key=\"ProjectExplorer.BuildConfiguration.BuildStepList.0\" type=\"QVariantMap\">")
    _p(4, "<valuemap key=\"ProjectExplorer.BuildStepList.Step.0\" type=\"QVariantMap\">")
    _p(5, "<value key=\"ProjectExplorer.ProcessStep.Arguments\" type=\"QString\">gmake</value>")
    _p(5, "<value key=\"ProjectExplorer.ProcessStep.Command\" type=\"QString\">premake4</value>")
    _p(5, "<value key=\"ProjectExplorer.ProcessStep.Enabled\" type=\"bool\">true</value>")
    _p(5, "<value key=\"ProjectExplorer.ProcessStep.WorkingDirectory\" type=\"QString\">%%{buildDir}</value>")
    _p(5, "<value key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\" type=\"QString\">Generate Makefiles with premake4</value>")
    _p(5, "<value key=\"ProjectExplorer.ProjectConfiguration.DisplayName\" type=\"QString\">premake4</value>")
    _p(5, "<value key=\"ProjectExplorer.ProjectConfiguration.Id\" type=\"QString\">ProjectExplorer.ProcessStep</value>")
    _p(4, "</valuemap>")
    
    _p(4, "<valuemap key=\"ProjectExplorer.BuildStepList.Step.1\" type=\"QVariantMap\">")
    _p(4, "<valuelist key=\"GenericProjectManager.GenericMakeStep.BuildTargets\" type=\"QVariantList\">")
   _p(5, "<value type=\"QString\">%s</value>", prj.name)
    _p(4, "</valuelist>")
    
    _p(4, "<value key=\"GenericProjectManager.GenericMakeStep.MakeArguments\" type=\"QString\">-R config=%s</value>",  cfg.shortname)
    _p(4, "<value key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\" type=\"QString\">%s</value>", prj.name)
    _p(4, "<value key=\"ProjectExplorer.ProjectConfiguration.DisplayName\" type=\"QString\"></value>")
    
    _p(4, "<value key=\"ProjectExplorer.ProjectConfiguration.Id\" type=\"QString\">GenericProjectManager.GenericMakeStep</value>")
   _p(4, "</valuemap>")
    
    _p(4, "<value key=\"ProjectExplorer.BuildStepList.StepsCount\" type=\"int\">2</value>")
    _p(4, "<value key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\" type=\"QString\"></value>")
    _p(4, "<value key=\"ProjectExplorer.ProjectConfiguration.DisplayName\" type=\"QString\">build</value>")
    _p(4, "<value key=\"ProjectExplorer.ProjectConfiguration.Id\" type=\"QString\">ProjectExplorer.BuildSteps.Build</value>")
    
    _p(3, "</valuemap>")
    
    -- the build steps for "Clean"
    _p(3, "<valuemap key=\"ProjectExplorer.BuildConfiguration.BuildStepList.1\" type=\"QVariantMap\">")
    _p(4, "<valuemap key=\"ProjectExplorer.BuildStepList.Step.0\" type=\"QVariantMap\">")
    _p(5, "<valuelist key=\"GenericProjectManager.GenericMakeStep.BuildTargets\" type=\"QVariantList\">")
    _p(6, "<value type=\"QString\">clean</value>")
    _p(5, "</valuelist>")
    _p(5, "<value key=\"GenericProjectManager.GenericMakeStep.MakeArguments\" type=\"QString\">config=%s</value>", cfg.shortname)
    _p(5, "<value key=\"GenericProjectManager.GenericMakeStep.MakeCommand\" type=\"QString\"></value>")
    _p(5, "<value key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\" type=\"QString\">Make</value>")
    _p(5, "<value key=\"ProjectExplorer.ProjectConfiguration.DisplayName\" type=\"QString\"></value>")
    _p(5, "<value key=\"ProjectExplorer.ProjectConfiguration.Id\" type=\"QString\">GenericProjectManager.GenericMakeStep</value>")
    _p(4, "</valuemap>")
    _p(4, "<value key=\"ProjectExplorer.BuildStepList.StepsCount\" type=\"int\">1</value>")
    _p(4, "<value key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\" type=\"QString\">Clean</value>")
    _p(4, "<value key=\"ProjectExplorer.ProjectConfiguration.DisplayName\" type=\"QString\"></value>")
    _p(4, "<value key=\"ProjectExplorer.ProjectConfiguration.Id\" type=\"QString\">ProjectExplorer.BuildSteps.Clean</value>")
    _p(3, "</valuemap>")
    _p(3, "<value key=\"ProjectExplorer.BuildConfiguration.BuildStepListCount\" type=\"int\">2</value>")
    
    _p(3, "<value key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\" type=\"QString\"></value>")
    _p(3, "<value key=\"ProjectExplorer.ProjectConfiguration.DisplayName\" type=\"QString\">%s</value>", cfg.longname)
    _p(3, "<value key=\"ProjectExplorer.ProjectConfiguration.Id\" type=\"QString\">GenericProjectManager.GenericBuildConfiguration</value>")
    
    _p(2, "</valuemap>") -- end configuration valuemap
  end
  
	function premake.qtcreator.user(prj)
		-- If necessary, set an explicit line ending sequence
		-- io.eol = '\r\n'
		
		local cc = premake[_OPTIONS.cc]
		local platforms = premake.filterplatforms(prj.solution, cc.platforms, "Native")
		
		qc.header()
		
	  _p("<data>")
    _p(1, "<variable>ProjectExplorer.Project.Target.0</variable>")
    _p(1, "<valuemap type=\"QVariantMap\">")
    _p(2, "<value key=\"ProjectExplorer.ProjectConfiguration.DefaultDisplayName\" type=\"QString\">%s</value>", prj.name)
    _p(2, "<value key=\"ProjectExplorer.ProjectConfiguration.DisplayName\" type=\"QString\"></value>")
    _p(2, "<value key=\"ProjectExplorer.Target.ActiveBuildConfiguration\" type=\"int\">0</value>")
    _p(2, "<value key=\"ProjectExplorer.ProjectConfiguration.Id\" type=\"QString\">GenericProjectManager.GenericTarget</value>")
  		
		-- write out configurations
	
		local cfgCounter = 0
		-- build configurations
		for _, platform in ipairs(platforms) do
		  for cfg in premake.eachconfig(prj, platform) do
		    
		    qc.build_configuration(prj, cfg, cfgCounter) 		     
		    cfgCounter = cfgCounter + 1
		  end
		end
		_p(2, "<value key=\"ProjectExplorer.Target.BuildConfigurationCount\" type=\"int\">%d</value>", cfgCounter)
	
	  -- run configurations
    cfgCounter = 0
	  for _, platform in ipairs(platforms) do
	    for cfg in premake.eachconfig(prj, platform) do
        if(cfg.kind == "ConsoleApp" or cfg.kind == "WindowedApp") then	      
	        qc.run_configuration(prj, cfg, cfgCounter) 		     
	        cfgCounter = cfgCounter + 1      
        end -- if kind == *App
	    end
	  end
	  _p(2, "<value key=\"ProjectExplorer.Target.RunConfigurationCount\" type=\"int\">%d</value>", cfgCounter)
		
		_p(1, "</valuemap>")
		_p("</data>")
		
    _p("<data>")
    _p(1,"<variable>ProjectExplorer.Project.Updater.EnvironmentId</variable>")
    _p(1, "<value type=\"QString\">{1cb8bdb9-cc2d-4370-be08-99c433148e91}</value>")
    _p("</data>")
		
	  _p("<data>")
    _p(1, "<variable>ProjectExplorer.Project.TargetCount</variable>")
    _p(1, "<value type=\"int\">1</value>")
    _p("</data>")
		
		qc.footer()
	end 