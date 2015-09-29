#include "sdl2_touch.h"
#include "sdl2_rwops.h"
#include "sdl2_gesture.h"


static mrb_value
mrb_sdl2_gesture_record(mrb_state *mrb, mrb_value self)
{
  int result;
  mrb_int touchid;
  mrb_get_args(mrb, "i", &touchid);
  result = SDL_RecordGesture(touchid);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gesture_save_dollar_templates(mrb_state *mrb, mrb_value self)
{
  int result;
  SDL_RWops * rwops_src;
  mrb_value src;
  mrb_get_args(mrb, "o", &src);
  rwops_src = mrb_sdl2_rwops_get_ptr(mrb, src);
  result = SDL_SaveAllDollarTemplates(rwops_src);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gesture_save_dollar_template(mrb_state *mrb, mrb_value self)
{
  mrb_int gestureid;
  mrb_value src;
  int result;
  SDL_RWops * rwops_src;
  mrb_get_args(mrb, "io", &gestureid, &src);
  rwops_src = mrb_sdl2_rwops_get_ptr(mrb, src);
  result = SDL_SaveDollarTemplate((SDL_GestureID) gestureid, rwops_src);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gesture_load_dollar_template(mrb_state *mrb, mrb_value self)
{
  mrb_int gestureid;
  int result;
  mrb_value src;
  SDL_RWops * rwops_src;
  mrb_get_args(mrb, "io", &gestureid, &src);
  rwops_src = mrb_sdl2_rwops_get_ptr(mrb, src);
  result = SDL_LoadDollarTemplates((SDL_GestureID) gestureid, rwops_src);
  return mrb_fixnum_value(result);
}

void
mruby_sdl2_gesture_init(mrb_state *mrb)
{
  struct RClass * mod_Gesture = mrb_define_module_under(mrb, mod_SDL2, "Gesture");

  mrb_define_module_function(mrb, mod_Gesture, "record_gesture",            mrb_sdl2_gesture_record,                MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Gesture, "save_all_dollar_templates", mrb_sdl2_gesture_save_dollar_templates, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Gesture, "save_dollar_template",      mrb_sdl2_gesture_save_dollar_template,  MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, mod_Gesture, "load_dollar_template",      mrb_sdl2_gesture_load_dollar_template,  MRB_ARGS_REQ(1));
}


void
mruby_sdl2_gesture_final(mrb_state *mrb)
{
}
