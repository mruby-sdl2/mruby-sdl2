
def klass_name(klass)
  name = klass.inspect
  if name.start_with? "#<Class:"
    name.slice! "#<Class:"
    name.slice! ">"
  end
  name
end

classes = {}
ObjectSpace.each_object(Class){|klass|
  name = klass_name klass
  if name.start_with? "SDL2"
    classes[name] = klass
  end
}
classes.keys.sort.each{|klass_name|
  klass = classes[klass_name]
  puts ""
  puts "## #{klass_name} < #{klass_name klass.ancestors.at(1)}"
  methods = klass.instance_methods(false) - Object.instance_methods(true) - Module.instance_methods(true)
  methods.sort{|a,b| a.to_s <=> b.to_s }.each{|m|
    puts " - #{m}"
  }
}