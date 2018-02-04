def scancode_to_s(code)
  SDL2::Input::Keyboard::scancode_name(code)
end

def windowevent_to_s(event)
  case event
  when SDL2::Input::SDL_WINDOWEVENT_SHOWN then "shown"
  when SDL2::Input::SDL_WINDOWEVENT_HIDDEN then "hidden"
  when SDL2::Input::SDL_WINDOWEVENT_EXPOSED then "exposed"
  when SDL2::Input::SDL_WINDOWEVENT_MOVED then "moved"
  when SDL2::Input::SDL_WINDOWEVENT_RESIZED then "resized"
  when SDL2::Input::SDL_WINDOWEVENT_SIZE_CHANGED then "size_changed"
  when SDL2::Input::SDL_WINDOWEVENT_MINIMIZED then "minimized"
  when SDL2::Input::SDL_WINDOWEVENT_MAXIMIZED then "maximized"
  when SDL2::Input::SDL_WINDOWEVENT_RESTORED then "restored"
  when SDL2::Input::SDL_WINDOWEVENT_ENTER then "enter"
  when SDL2::Input::SDL_WINDOWEVENT_LEAVE then "leave"
  when SDL2::Input::SDL_WINDOWEVENT_FOCUS_GAINED then "focus_gained"
  when SDL2::Input::SDL_WINDOWEVENT_FOCUS_LOST then "focus_lost"
  when SDL2::Input::SDL_WINDOWEVENT_CLOSE then "close"
  else "unknown"
  end
end

SDL2::init

begin
  SDL2::Video::init
  begin
    w = SDL2::Video::Window.new "sample", 100, 100, 640, 480, SDL2::Video::Window::SDL_WINDOW_SHOWN

    renderer = SDL2::Video::Renderer.new(w)
    renderer.set_draw_color(0, 0, 0, 0)
    renderer.clear
    renderer.set_draw_color(0xff, 0xff, 0xff, 0xFF)
    old_p = nil
    render_on = false

    running = true
    while running
      while ev = SDL2::Input::poll()
        puts case ev.type
        when SDL2::Input::SDL_KEYDOWN then
          "down: #{scancode_to_s(ev.keysym.scancode)}"
        when SDL2::Input::SDL_KEYUP then
          "up: #{scancode_to_s(ev.keysym.scancode)}"
        when SDL2::Input::SDL_WINDOWEVENT then
          "window: #{windowevent_to_s(ev.event)}"
        when SDL2::Input::SDL_MOUSEMOTION then
          if render_on then
            if old_p.nil? then
              old_p = SDL2::Point.new(ev.x, ev.y)
            else
              renderer.draw_line(old_p, SDL2::Point.new(ev.x, ev.y))
              old_p = SDL2::Point.new(ev.x, ev.y)
            end
          end
          "mouse motion: #{ev.x}, #{ev.y}"
        when SDL2::Input::SDL_MOUSEBUTTONDOWN then
          render_on = true
          old_p = nil
          "mouse button down: #{ev.x}, #{ev.y}"
        when SDL2::Input::SDL_MOUSEBUTTONUP then
          render_on = false
          old_p = nil
          "mouse button up: #{ev.x}, #{ev.y}"
        when SDL2::Input::SDL_QUIT then
          "quit"
        else
          "?: #{ev.to_s}"
        end
        if ev.type == SDL2::Input::SDL_QUIT then
          running = false
        end
      end
      SDL2::delay 20
      renderer.present
    end
    renderer.destroy
    w.destroy
  ensure
    SDL2::Video::quit
  end
ensure
  SDL2::quit
end
