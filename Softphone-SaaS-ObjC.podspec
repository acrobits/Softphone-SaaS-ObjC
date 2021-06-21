Pod::Spec.new do |s|
    s.name         = "Softphone-SaaS-ObjC"
    s.version      = "1.0.160612"
    s.summary      = "Acrobits Softphone ObjC Framework"
    s.homepage     = "https://acrobits.net"
    s.author       = { "$(git config user.name)" => "$(git config user.email)" }
    s.license      = 'APACHE 2.0'
    s.source       = { :git => "https://github.com/acrobits/Softphone-SaaS-ObjC.git", :tag => s.version }
    s.platform     = :ios, '13.0'
    s.ios.framework  = 'UIKit'
    s.preserve_paths = 'Frameworks/*'
    s.vendored_frameworks = "Frameworks/Softphone.xcframework"
    s.script_phase = { :name => 'Setup Softphone Framework', :script => 'cp -r ${PODS_ROOT}/Softphone-SaaS-ObjC/Frameworks/${CONFIGURATION}/Softphone.xcframework ${PODS_ROOT}/Softphone-SaaS-ObjC/Frameworks', :execution_position => :before_compile }
    s.requires_arc = true
    s.xcconfig = { 'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17', 'CLANG_CXX_LIBRARY' => 'libc++' }
 end