#ifndef MRUBY_SDL2_FILESYSTEM_H
#define MRUBY_SDL2_FILESYSTEM_H

#include "sdl2.h"
#include <SDL_filesystem.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void mruby_sdl2_filesystem_init(mrb_state *mrb);
extern void mruby_sdl2_filesystem_final(mrb_state *mrb);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_FILESYSTEM_H */
