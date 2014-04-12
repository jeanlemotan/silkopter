
    local android = premake.android

	-- Various helper function
	function path_without_drive_letter( path )
		local final_path

		final_path = path

		if final_path:sub( 2,2 ) == ":" then -- windows style file path
			return final_path:sub( 4 ) -- return what is after C:\
		else
			return path
		end
	end
	
	function base_path( path )
        local drive
        
        if os.is( "windows" ) then
		    drive = path:sub( 1,2 )
        else
            drive = "/"
        end

		return drive
	end
    
    function get_config_suffix( config_name )
    
        -- debug config will be default jni folder to make sure ndk-build will not fail to locate the files
        if string.find( string.lower( config_name ), "debug" ) then
            return ""
        end
        
        return string.gsub( config_name, "%s+", "" )
    end

	-- Android.mk file helper functions

	function android.ouput_included_shared_libs( included_shared_libs )
		_p( 'LOCAL_SHARED_LIBRARIES := \\' )

		for i, lib in ipairs( included_shared_libs ) do
			_p( 1, '%s \\', lib )
		end

		_p( 1, "gnustl_shared \\" );
		_p( '' )
	end

	function android.output_ld_libs( cfg, included_shared_libs )
		_p( 'LOCAL_LDLIBS := \\' )

		for i, libdir in ipairs( cfg.libdirs ) do
			_p( 1, '-L%s/$(TARGET_ARCH_ABI) \\', libdir )
		end

		-- specific android links :
		_p( 1, '-llog \\' )
		_p( 1, '-lm \\' )
		_p( 1, '-landroid \\' )

		local libs = premake.getlinks( cfg, "all", "fullpath" )

		for i, shared_lib in ipairs( included_shared_libs ) do
			for j, lib in ipairs( libs ) do
				if lib == shared_lib then
					table.remove( libs, j )
					break
				end
			end
		end

		for i, lib in ipairs( libs ) do
			_p( 1, '-l%s \\', lib )
		end

		_p( '' )
	end

	function android.output_shared_libs( cfg )
		-- prebuild-shared-libs :

		local included_shared_libs = {}

		for i, lib in ipairs( premake.getlinks( cfg, 'all', 'fullpath' ) ) do

			-- if lib is found in .so then include it here
			for i, libdir in ipairs( cfg.libdirs ) do
				local files

				lib_files = os.matchfiles( libdir .. "/**".. lib .. ".so" )
				for i, lib_file in ipairs( lib_files ) do
					_p( "include $(CLEAR_VARS)" )

					_p( 1, 'LOCAL_PATH 					:= %s%s', base_path( libdir ) .. "/", path_without_drive_letter( libdir ) )
					_p( 1, 'LOCAL_MODULE				:= %s', lib )
					_p( 1, 'LOCAL_SRC_FILES				:= $(TARGET_ARCH_ABI)/%s', path.getname( lib_file ) )
					--_p( 1, 'LOCAL_EXPORT_C_INCLUDES 	:= INCLUDE' ) -- useless

					_p( 'include $(PREBUILT_SHARED_LIBRARY)' )
					_p( '' )

					table.insert( included_shared_libs, i, lib )

					break
				end
			end
		end

		return included_shared_libs
	end

	function android.output_src_files( prj )
		-- List out the folders and files that make up the build

        local tr = premake5.project.getsourcetree( prj )
        local it_has_printed_base_path = false

		premake.tree.traverse( tr, {
			onleaf = function( node, depth )

                if not it_has_printed_base_path then
                    _p( 'LOCAL_PATH := %s', base_path( node.abspath ) )
		            _p( '' )
		            _p( 'LOCAL_SRC_FILES := \\' )

                    it_has_printed_base_path = true
                end

				_p( 1, path_without_drive_letter( node.abspath ) .. ' \\' )
			end
		})

		_p( '' )
	end

	function android.output_compiler_flags( cfg )
		_p( 'LOCAL_CPPFLAGS    := \\' )

		local local_c_flags = premake.gcc.getcflags( cfg )

		for i, flag in ipairs( local_c_flags ) do
			_p( 1, '%s \\', flag )
		end

		local local_cxx_flags = premake.gcc.getcxxflags( cfg )

		for i, flag in ipairs( local_cxx_flags ) do
			_p( 1, '%s \\', flag )
		end

		-- compiler options

		for i, option in ipairs( cfg.buildoptions ) do
			_p( 1, '%s \\', option )
		end

		-- preprocessor
        
        if premake.config.isdebugbuild( cfg ) then
            _p( 1, '-D_DEBUG \\' )
        end
        
		for i, define in ipairs( cfg.defines ) do
			_p( 1, '-D%s \\', define )
		end

		-- user includes
		for i, dir in ipairs( cfg.userincludedirs ) do
			_p( 1, '-iquote %s \\', dir )
		end

		-- include directories
		for i, dir in ipairs( cfg.includedirs ) do
			_p( 1, '-I %s \\', dir )
		end

		_p( '' )
	end

	-- file generation

	function premake.android.buildxml( prj )
		_p( '<?xml version="1.0" encoding="UTF-8"?><project name="%s" default="help">', prj.name )
		_p( '<property file="local.properties" />' )
		_p( '<property file="ant.properties" />' )
		_p( '<property environment="env" /><condition property="sdk.dir" value="${env.ANDROID_HOME}"><isset property="env.ANDROID_HOME" /></condition>' )
		_p( '<loadproperties srcFile="project.properties" />' )
		_p( '<fail message="sdk.dir is missing. Make sure to generate local.properties using \'android update project\' or to inject it through the ANDROID_HOME environment variable." unless="sdk.dir"/>' )
		_p( '<import file="custom_rules.xml" optional="true" />' )
		_p( '<import file="${sdk.dir}/tools/ant/build.xml" />' )
		_p( '</project>' )
	end

	function premake.android.sln( prj )
		printf( "On solution ; unused" )
	end

	function premake.android.antprop( prj )
		_p( 'key.alias=%s', prj.name )
		_p( 'target=%s', prj.androidsdk )
		_p( 'jar.libs.dir=libs' )
		_p( 'resource.dir=./res' )
        _p( 'sdk.dir=${ANDROID_SDK}' )
	end

    function premake.android.projprop( prj )
		_p( 'target=android-10' )
	end

	--function premake.android.manifest(prj)
	--	_p( '<?xml version="1.0" encoding="utf-8"?>' )
	--	_p( '<manifest xmlns:android="http://schemas.android.com/apk/res/android" ' );
	--	_p( 'package="com.fishingcactus.%s"', prj.name )
	--	_p( 'android:versionCode="1" ' )
	--	_p( 'android:versionName="1.0">' )
	--	
	--	_p( '<application android:label="@string/app_name" android:hasCode="true" android:debuggable="true" android:icon="@drawable/icon" android:theme="@android:style/Theme.NoTitleBar.Fullscreen" >' )
	--	_p( '<activity android:name=".AfterBurnerClimax" android:label="@string/app_name" android:configChanges="orientation|keyboardHidden" android:screenOrientation="landscape" android:installLocation="preferExternal" >' )
	--	_p( '<intent-filter><action android:name="android.intent.action.MAIN" /><category android:name="android.intent.category.LAUNCHER" /></intent-filter>' )
	--	_p( '</activity>' )
	--	_p( '</application>' )
	--	_p( '<uses-feature android:glEsVersion="0x00020000"/>' )
	--	_p( '<uses-sdk android:minSdkVersion="8" />' )
	--	_p( '</manifest>' )
	--end

	function premake.android.applicationmk( prj, cfg, jni_folder )
        local it_has_rtti = true
        local it_has_exceptions = true
        local cpp_features = ""
            
        local local_c_flags = premake.gcc.getcxxflags( cfg )

        for i, flag in ipairs( local_c_flags ) do
            if ( flag == "-fno-rtti" ) then
                it_has_rtti = false
            end

            if flag == "-fno-exceptions" then
                it_has_exceptions = false
            end
        end

        if ( it_has_rtti ) then
            cpp_features = cpp_features .. "rtti "
        end

        if ( it_has_exceptions ) then
            cpp_features = cpp_features .. " exceptions"
        end

        if premake.config.isdebugbuild( cfg ) then
            _p( 'APP_OPTIM 						:= debug' )
        end
        
		_p( 'APP_MODULES      				:= %s', prj.name )
        
        if #prj.androidappabi > 0 then
            -- TODO : check if the setup sdk is valid on this file system
            _p( 'APP_ABI     					:= %s', table.concat(prj.androidappabi, " ") )
        else
            error( "must define at least one androidappabi (%s) ", table.concat( prj.androidappabi.allowed ) )
        end
        
		_p( 'APP_PLATFORM 					:= %s', prj.androidsdk )
		_p( 'APP_STL 						:= gnustl_shared' )
		_p( 'APP_GNUSTL_FORCE_CPP_FEATURES 	:= %s', cpp_features )
		_p( 'APP_BUILD_SCRIPT 				:= %s/Android.mk', jni_folder )
		
		
	end

	function premake.android.androidmk( prj, cfg )
		printf( "generating ANDROID project file" )
		io.eol = '\r\n'

		_p( 'LOCAL_PATH := $(call my-dir)' )
		_p( '' )

        local included_shared_libs = android.output_shared_libs( cfg )

        _p( '' )
        _p( 'include $(CLEAR_VARS)' )
        _p( '' )
        _p( 'LOCAL_MODULE := %s', prj.name )
        _p( '' )

        _p( 0, 'LOCAL_LDFLAGS := -Wl,-Map,xxx.map' ) -- no custom ld flags for now
        _p( '' )

        _p( '' )
        android.output_compiler_flags( cfg )
        _p( '' )
        android.output_src_files( prj )

        android.output_ld_libs( cfg, included_shared_libs )

        android.ouput_included_shared_libs( included_shared_libs )

        -- for native apps
        _p( '' )
        _p( 'LOCAL_STATIC_LIBRARIES := \\' )
        _p( 1, 'android_native_app_glue' )

        --
        _p( '' )
        _p( 'include $(BUILD_SHARED_LIBRARY)' )
        _p( '' )
        _p( '' )
        _p( '$(call import-module,android/native_app_glue)' )
	end
    
    -- HACK : android will
    function premake.android.project( prj )
    
        -- make_dirrule does print 
        for cfg in premake.eachconfig( prj, "android" ) do
            local makefiles_directory = "jni" .. get_config_suffix( cfg.name )
            
            premake.make_mkdirrule( makefiles_directory )
        end
                
		-- List the build configurations, and the settings for each
		
        for cfg in premake.eachconfig( prj, "android" ) do
            local makefiles_directory = "jni" .. get_config_suffix( cfg.name )
            
            -- generate android.mk for this config
            filename = premake.project.getfilename( prj, makefiles_directory .. "/Android.mk")

            local f, err = io.open( filename, "wb" )
            if ( not f ) then
                error( err, 0 )
            end

            io.output( f )
                premake.android.androidmk( prj, cfg )
            f:close()
            
            -- generate application.mk for this config
            filename = premake.project.getfilename( prj, makefiles_directory .. "/Application.mk" )

            local f, err = io.open( filename, "wb" )
            if ( not f ) then
                error( err, 0 )
            end

            io.output( f )
                premake.android.applicationmk( prj, cfg, makefiles_directory )
            f:close()
        end
    end
