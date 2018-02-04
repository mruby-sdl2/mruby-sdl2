assert('SDL2::Video::Window.new') do
  SDL2::init
  SDL2::Video::init
  width = 320
  height = 240
  x = SDL2::Video::Window::SDL_WINDOWPOS_UNDEFINED
  y = SDL2::Video::Window::SDL_WINDOWPOS_UNDEFINED
  flag = 0
  window = SDL2::Video::Window.new "sample", x,y, width,height, flag
  # SDL2::Input::poll()

  assert_equal width, window.width
  assert_equal height, window.height

  SDL2::Video::quit
  SDL2::quit
end
