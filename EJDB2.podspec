
Pod::Spec.new do |spec|

  spec.name         = 'EJDB2'
  spec.version      = '1.0.10'
  spec.summary      = 'Embeddable JSON Database engine'
  spec.description  = <<-DESC
  EJDB2 is an embeddable JSON database engine published under MIT license.

  - Single file database
  - Online backups support

  Simple but powerful query language (JQL) as well as support of the following standards:
    - rfc6902 JSON Patch
    - rfc7386 JSON Merge patch
    - rfc6901 JSON Path

  Powered by iowow.io - The persistent key/value storage engine
  Provides HTTP REST/Websockets network endpoints with help of facil.io
  JSON documents are stored in using fast and compact binn binary format
                   DESC

  spec.homepage           = 'https://ejdb.org'
  spec.license            = { :type => 'MIT' }
  spec.author             = { 'Anton Adamansky' => 'adamansky@gmail.com' }
  spec.social_media_url   = 'https://twitter.com/ejdblab'

  spec.swift_version = '5.1'
  spec.static_framework = true
  spec.ios.deployment_target = '8.0'
  # spec.osx.deployment_target = '10.7'
  # spec.watchos.deployment_target = '2.0'
  # spec.tvos.deployment_target = '9.0'


  spec.source       = { :git => 'https://github.com/Softmotions/EJDB2Swift.git', :tag => "v#{spec.version}" }
  spec.source_files = 'Sources/EJDB2/**/*.{c,h,m,swift}'

  spec.xcconfig = { 'HEADER_SEARCH_PATHS' => ['$(PODS_ROOT)/EJDB2/include'],
                    'SWIFT_INCLUDE_PATHS' => ['$(PODS_ROOT)/EJDB2/Sources/CEJDB2'] }

  spec.preserve_paths = ['build-ios.sh', 'Sources/CEJDB/module.modulemap', 'lib/**', 'include/**']
  spec.vendored_libraries = 'lib/IOS/*.a'

  spec.prepare_command = './build-ios.sh'
end