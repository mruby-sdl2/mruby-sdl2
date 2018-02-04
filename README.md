[![Build Status](https://travis-ci.org/ecirmoa/mruby-sdl2.svg?branch=master)](https://travis-ci.org/ecirmoa/mruby-sdl2)

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


## SDL2::Audio::AudioCVT < Object
 - convert
 - initialize

## SDL2::Audio::AudioData < Object
 - buffer
 - destroy
 - initialize
 - length
 - spec

## SDL2::Audio::AudioDevice < Object
 - close
 - initialize
 - lock
 - pause
 - spec
 - status
 - unlock

## SDL2::Audio::AudioSpec < Object
 - callback
 - callback=
 - channels
 - channels=
 - format
 - format=
 - freq
 - freq=
 - initialize
 - samples
 - samples=
 - silence
 - size
 - userdata
 - userdata=

## SDL2::BGR < SDL2::RGB
 - to_i

## SDL2::BGR888 < SDL2::BGR

## SDL2::BGRA < SDL2::BGR
 - a
 - a=
 - initialize
 - to_i

## SDL2::BGRA8888 < SDL2::BGRA

## SDL2::Buffer < Object
 - address
 - cptr
 - initialize
 - size

## SDL2::ByteBuffer < SDL2::Buffer
 - []
 - []=
 - initialize

## SDL2::Cond < Object
 - broadcast
 - destroy
 - initialize
 - signal
 - wait
 - wait_timeout

## SDL2::FloatBuffer < SDL2::Buffer
 - []
 - []=
 - initialize
 - size

## SDL2::GameControllers::GameController < Object
 - destroy
 - free
 - get_axis
 - get_button_state
 - get_event_state
 - get_joystick
 - get_mapping
 - get_name
 - initialize
 - is_attached?

## SDL2::Haptics::Haptic < Object
 - destroy
 - destroy_effect
 - free
 - get_index
 - init_rumble
 - initialize
 - is_effect_playing?
 - is_rumble_supported?
 - num_axes
 - num_effects
 - num_effects_playing
 - pause
 - play_rumble
 - query
 - run_effect
 - set_autocenter
 - set_gain
 - stop_all
 - stop_effect
 - stop_rumble
 - unpause

## SDL2::Input::ControllerAxisEvent < SDL2::Input::Event
 - axis
 - timestamp
 - value
 - which

## SDL2::Input::ControllerButtonEvent < SDL2::Input::Event
 - button
 - state
 - timestamp
 - which

## SDL2::Input::ControllerDeviceEvent < SDL2::Input::Event

## SDL2::Input::DollarGestureEvent < SDL2::Input::Event

## SDL2::Input::DropEvent < SDL2::Input::Event

## SDL2::Input::Event < Object
 - type

## SDL2::Input::JoyAxisEvent < SDL2::Input::Event

## SDL2::Input::JoyBallEvent < SDL2::Input::Event

## SDL2::Input::JoyButtonEvent < SDL2::Input::Event

## SDL2::Input::JoyDeviceEvent < SDL2::Input::Event

## SDL2::Input::JoyHatEvent < SDL2::Input::Event

## SDL2::Input::Keyboard::Keysym < Object
 - modifier
 - scancode
 - symbol

## SDL2::Input::KeyboardEvent < SDL2::Input::Event
 - keysym
 - repeat
 - state
 - timestamp
 - window_id

## SDL2::Input::Mouse::Cursor < Object
 - destroy
 - free
 - initialize

## SDL2::Input::MouseButtonEvent < SDL2::Input::Event
 - button
 - state
 - timestamp
 - which
 - window_id
 - x
 - y

## SDL2::Input::MouseMotionEvent < SDL2::Input::Event
 - state
 - timestamp
 - which
 - window_id
 - x
 - xrel
 - y
 - yrel

## SDL2::Input::MouseWheelEvent < SDL2::Input::Event
 - timestamp
 - which
 - window_id
 - x
 - y

## SDL2::Input::MultiGestureEvent < SDL2::Input::Event

## SDL2::Input::OsEvent < SDL2::Input::Event

## SDL2::Input::QuitEvent < SDL2::Input::Event
 - timestamp

## SDL2::Input::SysWMEvent < SDL2::Input::Event

## SDL2::Input::TextEditingEvent < SDL2::Input::Event

## SDL2::Input::TextInputEvent < SDL2::Input::Event

## SDL2::Input::TouchFingerEvent < SDL2::Input::Event

## SDL2::Input::UserEvent < SDL2::Input::Event
 - code
 - code=
 - data1
 - data1=
 - data2
 - data2=
 - initialize
 - timestamp
 - timestamp=
 - type=
 - window_id
 - window_id=

## SDL2::Input::WindowEvent < SDL2::Input::Event
 - data1
 - data2
 - event
 - timestamp
 - window_id

## SDL2::Joysticks::Joystick < Object
 - destroy
 - event_state
 - free
 - get_axis
 - get_axis_x
 - get_axis_y
 - get_ball
 - get_button
 - get_general_num_axes
 - get_general_num_balls
 - get_general_num_buttons
 - get_general_num_hats
 - get_guid
 - get_guid_as_string
 - get_hat
 - get_id
 - get_name
 - initialize
 - is_attached?
 - is_haptic?
 - open_haptic
 - update

## SDL2::MessageBox < Object
 - destroy
 - initialize
 - set_buttons
 - set_color_scheme
 - set_flags
 - set_message
 - set_title
 - set_window
 - show

## SDL2::Mutex < Object
 - destroy
 - initialize
 - lock
 - try_lock
 - unlock

## SDL2::Pixels::Palette < Object
 - destroy
 - free
 - initialize
 - set_color

## SDL2::Pixels::PixelFormat < Object
 - Aloss
 - Amask
 - Ashift
 - BitsPerPixel
 - Bloss
 - Bmask
 - Bshift
 - BytesPerPixel
 - Gloss
 - Gmask
 - Gshift
 - Rloss
 - Rmask
 - Rshift
 - depth
 - destroy
 - format
 - free
 - get_rgb
 - get_rgba
 - initialize
 - mapRGB
 - mapRGBA
 - next
 - padding
 - palette
 - refcount
 - set_palette

## SDL2::Point < Object
 - initialize
 - x
 - x=
 - y
 - y=

## SDL2::RGB < Object
 - b
 - b=
 - g
 - g=
 - initialize
 - r
 - r=
 - to_i

## SDL2::RGB888 < SDL2::RGB

## SDL2::RGBA < SDL2::RGB
 - a
 - a=
 - initialize
 - to_i

## SDL2::RGBA8888 < SDL2::RGBA

## SDL2::RWops < Object
 - close
 - destroy
 - file_exists?
 - free
 - initialize
 - read
 - seek
 - size
 - tell
 - write

## SDL2::Rect < Object
 - !=
 - ==
 - empty?
 - h
 - h=
 - has_intersection?
 - initialize
 - intersection
 - intersection_line
 - position
 - position=
 - union
 - w
 - w=
 - x
 - x=
 - y
 - y=

## SDL2::SDL2Error < StandardError

## SDL2::Semaphore < Object
 - destroy
 - initialize
 - post
 - try_wait
 - value
 - wait
 - wait_timeout

## SDL2::Thread < Object
 - id
 - initialize
 - join
 - wait

## SDL2::Version < Object
 - major
 - minor
 - patch
 - revision
 - revision_str
 - to_s

## SDL2::Video::DisplayMode < Object

## SDL2::Video::GLContext < Object
 - create
 - delete
 - destroy
 - initialize

## SDL2::Video::PixelBuffer < Object
 - pitch
 - rect

## SDL2::Video::Renderer < Object
 - clear
 - clip_rect
 - clip_rect=
 - copy
 - copy_ex
 - destroy
 - draw_blend_mode
 - draw_blend_mode=
 - draw_line
 - draw_lines
 - draw_point
 - draw_points
 - draw_rect
 - draw_rects
 - fill_rect
 - fill_rects
 - get_draw_color
 - info
 - initialize
 - name
 - present
 - read_pixels
 - save_bmp
 - set_draw_color
 - target=
 - view_port
 - view_port=

## SDL2::Video::RendererInfo < Object
 - flags
 - max_texture_height
 - max_texture_width
 - name
 - texture_formats

## SDL2::Video::Surface < Object
 - alpha_mod
 - alpha_mod=
 - blend_mode
 - blend_mode=
 - blit_scaled
 - blit_surface
 - color_key_get
 - color_key_set
 - color_mod
 - color_mod=
 - convert
 - convert_format
 - destroy
 - fill_rect
 - fill_rects
 - format
 - free
 - get_clip_rect
 - get_pixel
 - gradient_fill_rect
 - height
 - initialize
 - lock
 - locked_num
 - must_lock?
 - palette
 - pitch
 - rle
 - set_clip_rect
 - set_pixel
 - unlock
 - width

## SDL2::Video::Texture < Object
 - access
 - alpha_mod
 - alpha_mod=
 - blend_mode
 - blend_mode=
 - destroy
 - format
 - free
 - get_color_mod
 - height
 - initialize
 - lock
 - set_color_mod
 - unlock
 - update
 - update_locked
 - width

## SDL2::Video::Window < Object
 - brightness
 - brightness=
 - create
 - create_with_renderer
 - destroy
 - diplay_index
 - display_mode
 - display_mode=
 - flags
 - gamma_ramp
 - gamma_ramp=
 - grab
 - grab=
 - height
 - hide
 - icon=
 - id
 - initialize
 - make_current
 - maximize
 - maximum_size
 - maximum_size=
 - minimize
 - minimum_size
 - minimum_size=
 - position
 - position=
 - raise
 - renderer
 - restore
 - set_opacity
 - set_size
 - show
 - show_simple_message_box
 - surface
 - swap
 - title
 - title=
 - update_surface
 - update_surface_rects
 - width
