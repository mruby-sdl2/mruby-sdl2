classes = []
ObjectSpace.each_object(Class){|klass|
  classes << klass if klass.name.to_s.start_with? "SDL2"
}
classes.sort{|a,b| a.name <=> b.name }.each{|klass|
  puts ""
  puts "## #{klass.name} < #{klass.ancestors.at(1).name}"
  if klass.instance_methods(false).size > 0
    klass.instance_methods(false).sort{|a,b| a.to_s <=> b.to_s }.each{|m|
      puts " - #{m}"
    }
  end
}