#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint libdeno_plugin.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'libdeno_plugin'
  s.version          = '0.0.1'
  s.summary          = 'A new flutter plugin project.'
  s.description      = <<-DESC
A new flutter plugin project.
                       DESC
  s.homepage         = 'http://example.com'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Your Company' => 'email@example.com' }
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES', 'HEADER_SEARCH_PATHS' => '$(PODS_ROOT)/Classes' }
  s.dependency 'FlutterMacOS'
  s.preserve_path = 'Classes/libdeno_plugin_extern.h'
  # s.module_map = 'module/module.modulemap'

  # s.header_mappings_dir = "Classes/**"
  s.vendored_libraries = 'liblibdeno.dylib'
  s.platform = :osx, '10.11'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.swift_version = '5.0'
  s.xcconfig = { 
    'OTHER_LDFLAGS' => '-lc++',
    'USER_HEADER_SEARCH_PATHS' => '"${PROJECT_DIR}"/../Flutter/ephemeral/.symlinks/plugins/libdeno_plugin/macos/Classes/'#, '"${PROJECT_DIR}"/../Flutter/ephemeral/.symlinks/plugins/libdeno_plugin/macos/deno_c_library/include/',
    # 'LIBRARY_SEARCH_PATHS' => ''
    # "LIBRARY_SEARCH_PATHS" => '"${PROJECT_DIR}"/../Flutter/ephemeral/.symlinks/plugins/libdeno_plugin/macos/',
  }
end
