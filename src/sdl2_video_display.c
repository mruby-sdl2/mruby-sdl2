#include "sdl2.h"
#include "sdl2_video.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/array.h"
#include <SDL_video.h>

static struct RClass *class_DisplayMode = NULL;

typedef struct mrb_sdl2_video_displaymode_data_t {
  SDL_DisplayMode mode;
} mrb_sdl2_video_displaymode_data_t;

static void
mrb_sdl2_video_displaymode_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_displaymode_data_t *data =
    (mrb_sdl2_video_displaymode_data_t*)p;
  if (NULL != data) {
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_video_displaymode_data_type = {
  "DisplayMode", mrb_sdl2_video_displaymode_data_free
};

SDL_DisplayMode *
mrb_sdl2_video_displaymode_get_ptr(mrb_state *mrb, mrb_value displaymode)
{
  mrb_sdl2_video_displaymode_data_t *data;
  if (mrb_nil_p(displaymode)) {
    return NULL;
  }
  data =
    (mrb_sdl2_video_displaymode_data_t*)
      mrb_data_get_ptr(mrb,
                       displaymode,
                       &mrb_sdl2_video_displaymode_data_type);
  return &data->mode;
}

/* * * * */


/*
 * SDL2::Video::display_modes
 */
static mrb_value
mrb_sdl2_video_get_display_modes(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  int i;
  mrb_value array;
  int n;
  mrb_get_args(mrb, "i", &index);
  n = SDL_GetNumDisplayModes(index);
  if (0 > n) {
    mruby_sdl2_raise_error(mrb);
  }
  array = mrb_ary_new_capa(mrb, n);
  for (i = 0; i < n; ++i) {
    mrb_sdl2_video_displaymode_data_t *data =
      (mrb_sdl2_video_displaymode_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_displaymode_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    if (0 == SDL_GetDisplayMode(index, i, &data->mode)) {
      mrb_value const item = mrb_obj_value(Data_Wrap_Struct(mrb, class_DisplayMode, &mrb_sdl2_video_displaymode_data_type, data));
      mrb_ary_push(mrb, array, item);
    } else {
      mrb_free(mrb, data);
    }
  }
  return array;
}

/*
 * SDL2::Video::Window::display_mode
 */
static mrb_value
mrb_sdl2_video_window_get_display_mode(mrb_state *mrb, mrb_value self)
{
  SDL_DisplayMode dm;
  mrb_value item;
  mrb_sdl2_video_displaymode_data_t *data = (mrb_sdl2_video_displaymode_data_t*)
      mrb_malloc(mrb, sizeof(mrb_sdl2_video_displaymode_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }

  SDL_GetWindowDisplayMode(mrb_sdl2_video_window_get_ptr(mrb, self), &dm);
  data->mode = dm;
  item = mrb_obj_value(
      Data_Wrap_Struct(mrb,
                       class_DisplayMode,
                       &mrb_sdl2_video_displaymode_data_type,
                       data));
  return item;
}

/*
 * SDL2::Video::Window::display_mode=
 */
static mrb_value
mrb_sdl2_video_window_set_display_mode(mrb_state *mrb, mrb_value self)
{
  mrb_value displaymode;
  mrb_get_args(mrb, "o", &displaymode);
  if (0 != SDL_SetWindowDisplayMode(
          mrb_sdl2_video_window_get_ptr(mrb, self),
          mrb_sdl2_video_displaymode_get_ptr(mrb, displaymode))) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

/*
 * SDL2::Video::Window::brightness
 */
static mrb_value
mrb_sdl2_video_window_get_brightness(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb,
                         SDL_GetWindowBrightness(
                             mrb_sdl2_video_window_get_ptr(mrb, self)));
}

/*
 * SDL2::Video::Window::brightness=
 */
static mrb_value
mrb_sdl2_video_window_set_brightness(mrb_state *mrb, mrb_value self)
{
  mrb_float brightness;
  mrb_get_args(mrb, "f", &brightness);
  if (0 != SDL_SetWindowBrightness(
          mrb_sdl2_video_window_get_ptr(mrb, self),
          brightness)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

/*
 * SDL2::Video::Window::gamma_ramp
 */
static mrb_value
mrb_sdl2_video_window_get_gamma_ramp(mrb_state *mrb, mrb_value self)
{
  mrb_value ary;
  Uint16 red, green, blue;
  ary = mrb_ary_new_capa(mrb, 3);
  SDL_GetWindowGammaRamp(mrb_sdl2_video_window_get_ptr(mrb, self),
                         &red,
                         &green,
                         &blue);
  mrb_ary_push(mrb, ary, mrb_fixnum_value(red));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(green));
  mrb_ary_push(mrb, ary, mrb_fixnum_value(blue));
  return ary;
}

/*
 * SDL2::Video::Window::gamma_ramp=
 */
static mrb_value
mrb_sdl2_video_window_set_gamma_ramp(mrb_state *mrb, mrb_value self)
{
  mrb_int red, green, blue;
  Uint16 r,g,b;
  mrb_get_args(mrb, "iii", &red, &green, &blue);
  r = red;
  g = green;
  b = blue;
  if (0 != SDL_SetWindowGammaRamp(mrb_sdl2_video_window_get_ptr(mrb, self),&r,&g,&b)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

/* * * * */

void
mruby_sdl2_video_display_init(mrb_state *mrb)
{
  class_DisplayMode = mrb_define_class_under(mrb, mod_Video, "DisplayMode", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_DisplayMode, MRB_TT_DATA);

  mrb_define_module_function(mrb, mod_Video, "display_modes", mrb_sdl2_video_get_display_modes,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "brightness",           mrb_sdl2_video_window_get_brightness,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "brightness=",          mrb_sdl2_video_window_set_brightness,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "display_mode",         mrb_sdl2_video_window_get_display_mode,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "display_mode=",        mrb_sdl2_video_window_set_display_mode,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Window, "gamma_ramp",           mrb_sdl2_video_window_get_gamma_ramp,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Window, "gamma_ramp=",          mrb_sdl2_video_window_set_gamma_ramp,       MRB_ARGS_REQ(1));
}

void
mruby_sdl2_video_display_final(mrb_state *mrb)
{
}
