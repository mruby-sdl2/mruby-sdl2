#ifndef MRUBY_SDL2_GAMECONTROLLER_H
#define MRUBY_SDL2_GAMECONTROLLER_H

#include "sdl2.h"
#include <SDL2/SDL_gamecontroller.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void mruby_sdl2_gamecontroller_init(mrb_state *mrb);
extern void mruby_sdl2_gamecontroller_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_GAMECONTROLLER_H */
