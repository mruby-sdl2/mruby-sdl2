assert('SDL2::Audio.devices') do
  SDL2::init
  assert_true SDL2::Audio.devices.is_a?(Array)
  SDL2::quit
end