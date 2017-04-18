#ifndef MRUBY_SDL2_GESTURE_H
#define MRUBY_SDL2_GESTURE_H

#include "sdl2.h"
#include <SDL_gesture.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void mruby_sdl2_gesture_init(mrb_state *mrb);
extern void mruby_sdl2_gesture_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_GESTURE_H */
