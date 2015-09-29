#include "sdl2_touch.h"
#include "mruby/value.h"
#include "mruby/array.h"
#include "mruby/data.h"



static mrb_value
mrb_sdl2_touch_num_devices(mrb_state *mrb, mrb_value self)
{
  int result = SDL_GetNumTouchDevices();
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_touch_get_device(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  SDL_TouchID result;
  mrb_get_args(mrb, "i", &index);
  result = SDL_GetTouchDevice(index);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_touch_num_fingers(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  int result;
  mrb_get_args(mrb, "i", &index);
  result = SDL_GetNumTouchFingers((Sint64) index);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_touch_get_touch_finger(mrb_state *mrb, mrb_value self)
{
  mrb_int touchid, index;
  SDL_Finger * result;
  mrb_value array;
  mrb_get_args(mrb, "ii", &touchid, &index);
  result = SDL_GetTouchFinger((SDL_TouchID) touchid, index);

  array = mrb_ary_new_capa(mrb, 4);
  mrb_ary_push(mrb, array, mrb_fixnum_value(result->id));
  mrb_ary_push(mrb, array, mrb_float_value(mrb, result->x));
  mrb_ary_push(mrb, array, mrb_float_value(mrb, result->y));
  mrb_ary_push(mrb, array, mrb_float_value(mrb, result->pressure));
  return array;
}

void
mruby_sdl2_touch_init(mrb_state *mrb)
{
  struct RClass * mod_Touch = mrb_define_module_under(mrb, mod_SDL2, "Touch");

  mrb_define_module_function(mrb, mod_Touch, "num_touch_devices", mrb_sdl2_touch_num_devices,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_Touch, "get_touch_device", mrb_sdl2_touch_get_device,  MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Touch, "get_num_touch_fingers", mrb_sdl2_touch_num_fingers,  MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Touch, "get_touch_finger", mrb_sdl2_touch_get_touch_finger,  MRB_ARGS_REQ(2));


}


void
mruby_sdl2_touch_final(mrb_state *mrb)
{
}
