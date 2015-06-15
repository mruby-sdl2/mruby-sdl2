#ifndef MRUBY_SDL2_RWOPS_H
#define MRUBY_SDL2_RWOPS_H

#include "sdl2.h"
#include <SDL2/SDL_rwops.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void mruby_sdl2_rwops_init(mrb_state *mrb);
extern void mruby_sdl2_rwops_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_log_H */
