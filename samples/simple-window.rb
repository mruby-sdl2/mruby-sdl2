SDL2::init
SDL2::Video::init
width = 320
height = 240
x = SDL2::Video::Window::SDL_WINDOWPOS_CENTERED
y = SDL2::Video::Window::SDL_WINDOWPOS_CENTERED
flag = 0

#
window = SDL2::Video::Window.new "sample", x,y, width,height, flag
p [window.title,  window.width, window.height]

loop do
  while ev = SDL2::Input::poll()
    if ev.type == SDL2::Input::SDL_KEYDOWN
      window.set_size 160,120
    end
    if ev.type == SDL2::Input::SDL_QUIT
      exit(1)
    end
  end
end
