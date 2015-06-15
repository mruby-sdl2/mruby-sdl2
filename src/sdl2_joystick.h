#ifndef MRUBY_SDL2_JOYSTICK_H
#define MRUBY_SDL2_JOYSTICK_H

#include "sdl2.h"
#include <SDL2/SDL_joystick.h>

#ifdef __cplusplus
extern "C" {
#endif
extern struct RClass *class_Joystick;
extern SDL_Joystick * mrb_sdl2_joystick_joystick_get_ptr(mrb_state *mrb, mrb_value joystick);
extern void mruby_sdl2_joystick_init(mrb_state *mrb);
extern void mruby_sdl2_joystick_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_JOYSTICK_H */
