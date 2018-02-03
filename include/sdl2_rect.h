#ifndef MRUBY_SDL2_RECT_H
#define MRUBY_SDL2_RECT_H

#include "sdl2.h"
#include <SDL_rect.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void mruby_sdl2_rect_init(mrb_state *mrb);
extern void mruby_sdl2_rect_final(mrb_state *mrb);

extern mrb_value mrb_sdl2_rect_direct(mrb_state *mrb, SDL_Rect const *rect);
extern mrb_value mrb_sdl2_rect(mrb_state *mrb, int x, int y, int w, int h);
extern mrb_value mrb_sdl2_point(mrb_state *mrb, int x, int y);
extern mrb_value mrb_sdl2_size(mrb_state *mrb, int w, int h);

extern SDL_Rect    *mrb_sdl2_rect_get_ptr(mrb_state *mrb, mrb_value rect);
extern SDL_Point   *mrb_sdl2_point_get_ptr(mrb_state *mrb, mrb_value point);

#ifdef __cplusplus
}
#endif

#endif /* end of MRUBY_SDL2_RECT_H */


