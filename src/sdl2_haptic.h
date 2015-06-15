#ifndef MRUBY_SDL2_HAPTIC_H
#define MRUBY_SDL2_HAPTIC_H

#include "sdl2.h"
#include <SDL2/SDL_haptic.h>


#ifdef __cplusplus
extern "C" {
#endif

extern void mruby_sdl2_haptic_init(mrb_state *mrb);
extern void mruby_sdl2_haptic_final(mrb_state *mrb);


#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_HAPTIC_H */
