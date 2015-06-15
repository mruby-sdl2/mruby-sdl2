#include "sdl2_power.h"
#include "mruby/value.h"
#include "mruby/array.h"



static mrb_value
mruby_sdl2_power_info(mrb_state *mrb, mrb_value self)
{
  int *secs;
  int *pcs;
  SDL_PowerState result = SDL_GetPowerInfo(secs, pcs);

  mrb_value array = mrb_ary_new_capa(mrb, 3);
  mrb_ary_push(mrb, array, mrb_fixnum_value(result));
  mrb_ary_push(mrb, array, mrb_fixnum_value(*secs));
  mrb_ary_push(mrb, array, mrb_fixnum_value(*pcs));

  return array;
}

void
mruby_sdl2_power_init(mrb_state *mrb)
{

  struct RClass * mod_Power = mrb_define_module_under(mrb, mod_SDL2, "Power");
  mrb_define_module_function(mrb, mod_Power, "get_info", mruby_sdl2_power_info,  ARGS_NONE());

  int arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_Power, "SDL_POWERSTATE_UNKNOWN",    mrb_fixnum_value(SDL_POWERSTATE_UNKNOWN));
  mrb_define_const(mrb, mod_Power, "SDL_POWERSTATE_ON_BATTERY", mrb_fixnum_value(SDL_POWERSTATE_ON_BATTERY));
  mrb_define_const(mrb, mod_Power, "SDL_POWERSTATE_NO_BATTERY", mrb_fixnum_value(SDL_POWERSTATE_NO_BATTERY));
  mrb_define_const(mrb, mod_Power, "SDL_POWERSTATE_CHARGING",   mrb_fixnum_value(SDL_POWERSTATE_CHARGING));
  mrb_define_const(mrb, mod_Power, "SDL_POWERSTATE_CHARGED",    mrb_fixnum_value(SDL_POWERSTATE_CHARGED));
  mrb_gc_arena_restore(mrb, arena_size);
}

void
mruby_sdl2_power_final(mrb_state *mrb)
{
}
