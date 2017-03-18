# mruby-sdl2

_mruby-sdl2_ wrapped 'SDL2.0'.

# How to build

1. edit your 'build_config.rb'.
2. run 'make' command.

build_config.rb:

    conf.gem :github => 'ecirmoa/mruby-sdl2', :branch => 'master'

# License

MIT License

this library forked from https://github.com/Moon4u/mruby-sdl2 and https://github.com/crimsonwoods/mruby-sdl2

# Class and API

(incomplete)

- SDL2::Video
  - SDL2::Video::GL
  - SDL2::Video::Window
  - SDL2::Video::DisplayMode
  - SDL2::Video::GLContext
  - SDL2::Video::Surface
  - SDL2::Video::Renderer
  - SDL2::Video::Texture
  - SDL2::Video::RendererInfo  
- SDL2::Input
  - SDL2::Input::Mouse
    - SDL2::Input::Mouse::Cursor
  - SDL2::Input::Keybord
    - SDL2::Input::Keybord::Keysym
  - SDL2::Input::Event
  - SDL2::Input::ControllerAxisEvent
  - SDL2::Input::ControllerButtonEvent
  - SDL2::Input::ControllerDeviceEvent
  - SDL2::Input::DollarGestureEvent
  - SDL2::Input::DropEvent
  - SDL2::Input::JoyAxisEvent
  - SDL2::Input::JoyBallEvent
  - SDL2::Input::JoyButtonEvent
  - SDL2::Input::JoyHatEvent
  - SDL2::Input::JoyDeviceEvent
  - SDL2::Input::KeyboardEvent
  - SDL2::Input::MouseButtonEvent
  - SDL2::Input::MouseMotionEvent
  - SDL2::Input::MouseWheelEvent
  - SDL2::Input::MultiGestureEvent
  - SDL2::Input::OsEvent
  - SDL2::Input::QuitEvent
  - SDL2::Input::SysWMEvent
  - SDL2::Input::TextEditingEvent
  - SDL2::Input::TextInputEvent
  - SDL2::Input::TouchFingerEvent
  - SDL2::Input::UserEvent
  - SDL2::Input::WindowEvent
- SDL2::Rect
- SDL2::Point
- SDL2::Size
- SDL2::Timer
- SDL2::GameControllers
  - SDL2::GameController
