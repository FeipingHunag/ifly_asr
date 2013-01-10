Gem::Specification.new do |s|
  s.name        = 'ifly_asr'
  s.version     = '0.0.0'
  s.date        = '2013-01-09'
  s.summary     = "Automatic Speech Recognition"
  s.description = "Automatic Speech Recognition with iFLY"
  s.authors     = ["feipinghuang, vvdpzz"]
  s.email       = 'feipinghuang@gmail.com'
  s.default_executable = "ifly_asr"
  s.require_paths = ["lib"]
  s.files = Dir.glob('lib/**/*.rb') +
            Dir.glob('ext/**/*.{c,h,rb}')
  s.extensions = ['ext/ifly_asr/extconf.rb']
  s.executables = ['ifly_asr']

  s.homepage    = 'http://rubygems.org/gems/ifly_asr'
end
