MRuby::Gem::Specification.new('mruby-sdl2') do |spec|
  spec.license = 'MIT'
  spec.authors = 'crimsonwoods'

  spec.cc.flags << '`sdl2-config --cflags`'
  spec.linker.flags_before_libraries << '-L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL2 -lSDL2_image -lpthread'
end
