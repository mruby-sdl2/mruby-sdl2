#include "sdl2_platform.h"



static mrb_value
mrb_sdl2_platform_get_platform(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, SDL_GetPlatform());
}

void
mruby_sdl2_platform_init(mrb_state *mrb)
{
  struct RClass * mod_Platform = mrb_define_module_under(mrb, mod_SDL2, "Platform");
  mrb_define_module_function(mrb, mod_Platform, "get_platform", mrb_sdl2_platform_get_platform,  MRB_ARGS_NONE());
}



void
mruby_sdl2_platform_final(mrb_state *mrb)
{
}
