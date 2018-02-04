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


## SDL2 < Module
 - delay
 - init
 - init?
 - init_subsystem
 - log
 - platform
 - quit
 - quit_subsystem

## SDL2::Audio < Module
 - close
 - current_driver
 - devices
 - drivers
 - init
 - lock
 - mix_audio
 - open
 - open_device
 - pause
 - quit
 - status
 - unlock

## SDL2::Audio::AudioCVT < Object
 - convert

## SDL2::Audio::AudioData < Object
 - buffer
 - destroy
 - length
 - spec

## SDL2::Audio::AudioDevice < Object
 - close
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
 - format=
 - freq
 - freq=
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
 - to_i

## SDL2::BGRA8888 < SDL2::BGRA

## SDL2::Buffer < Object
 - address
 - cptr
 - size

## SDL2::ByteBuffer < SDL2::Buffer
 - []
 - []=

## SDL2::CPUInfo < Module
 - cache_line_size
 - get_cpu_count
 - get_ram
 - has_3DNow?
 - has_AltiVec?
 - has_RDTSC?
 - has_SSE2?
 - has_SSE3?
 - has_SSE41?
 - has_SSE42?
 - has_SSE?
 - has_mmx?

## SDL2::Clipboard < Module
 - has_text?
 - text
 - text=

## SDL2::Cond < Object
 - broadcast
 - destroy
 - signal
 - wait
 - wait_timeout

## SDL2::FloatBuffer < SDL2::Buffer
 - []
 - []=
 - size

## SDL2::GameControllers < Module
 - add_mapping
 - get_axis_from_string
 - get_button_from_string
 - get_mapping_from_guid
 - get_name_from_index
 - get_string_from_axis
 - get_string_from_button
 - is_gamecontroller_supported?
 - update

## SDL2::GameControllers::GameController < Object
 - destroy
 - free
 - get_axis
 - get_button_state
 - get_event_state
 - get_joystick
 - get_mapping
 - get_name
 - is_attached?

## SDL2::Gesture < Module
 - load_dollar_template
 - record_gesture
 - save_all_dollar_templates
 - save_dollar_template

## SDL2::Haptics < Module
 - device_open?
 - get_name
 - num

## SDL2::Haptics::Haptic < Object
 - destroy
 - destroy_effect
 - free
 - get_index
 - init_rumble
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

## SDL2::Hints < Module
 - clear
 - get
 - set
 - set_with_priority

## SDL2::Input < Module
 - event_state
 - flush
 - has_events?
 - poll
 - push
 - quit_requested?
 - register
 - wait
 - wait_timeout

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

## SDL2::Input::Keyboard < Module
 - key_from_name
 - key_from_scancode
 - key_name
 - keyboard_focus
 - mod_state
 - mod_state=
 - scancode_from_key
 - scancode_from_name
 - scancode_name
 - text_input_rect
 - text_input_start
 - text_input_stop

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

## SDL2::Input::Mouse < Module
 - cursor
 - cursor=
 - cursor_hide
 - cursor_show
 - is_haptic?
 - location
 - mouse_focus
 - mouse_state
 - open_haptic
 - relative=
 - relative?
 - warp
 - x
 - y

## SDL2::Input::Mouse::Cursor < Object
 - destroy
 - free

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

## SDL2::Joysticks < Module
 - get_guid_as_string
 - get_guid_from_index
 - get_guid_from_string
 - name_from_index
 - num

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
 - is_attached?
 - is_haptic?
 - open_haptic
 - update

## SDL2::MessageBox < Object
 - destroy
 - set_buttons
 - set_color_scheme
 - set_flags
 - set_message
 - set_title
 - set_window
 - show

## SDL2::Mutex < Object
 - destroy
 - lock
 - try_lock
 - unlock

## SDL2::Pixels < Module
 - calculate_gamma_ramp
 - format_to_masks
 - get_format_name
 - masks_to_format

## SDL2::Pixels::Palette < Object
 - destroy
 - free
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
 - free
 - get_rgb
 - get_rgba
 - mapRGB
 - mapRGBA
 - next
 - padding
 - palette
 - refcount
 - set_palette

## SDL2::Platform < Module
 - get_platform

## SDL2::Point < Object
 - x
 - x=
 - y
 - y=

## SDL2::Power < Module
 - get_info

## SDL2::RGB < Object
 - b
 - b=
 - g
 - g=
 - r
 - r=
 - to_i

## SDL2::RGB888 < SDL2::RGB

## SDL2::RGBA < SDL2::RGB
 - a
 - a=
 - to_i

## SDL2::RGBA8888 < SDL2::RGBA

## SDL2::RWops < Object
 - close
 - destroy
 - file_exists?
 - free
 - read
 - seek
 - size
 - tell
 - write

## SDL2::Rect < Object
 - empty?
 - h
 - h=
 - has_intersection?
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
 - post
 - try_wait
 - value
 - wait
 - wait_timeout

## SDL2::Thread < Object
 - id
 - join
 - wait

## SDL2::Timer < Module
 - add
 - clock
 - delay
 - perf_counter
 - perf_freq
 - remove
 - ticks

## SDL2::Touch < Module
 - get_num_touch_fingers
 - get_touch_device
 - get_touch_finger
 - num_touch_devices

## SDL2::Version < Object
 - major
 - minor
 - patch
 - revision
 - revision_str

## SDL2::Video < Module
 - current_driver
 - display_modes
 - displays
 - init
 - quit
 - screen_saver=
 - screen_saver?
 - video_drivers

## SDL2::Video::DisplayMode < Object

## SDL2::Video::GL < Module
 - attribute_get
 - attribute_set
 - extension_supported?
 - swap_interval
 - swap_interval=
 - swap_window

## SDL2::Video::GLContext < Object
 - create
 - delete
 - destroy

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
 - free
 - get_clip_rect
 - get_pixel
 - gradient_fill_rect
 - height
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
 - free
 - get_color_mod
 - height
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
 - make_current
 - maximize
 - maximum_size
 - maximum_size=
 - minimize
 - minimum_size
 - minimum_size=
 - position
 - position=
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
