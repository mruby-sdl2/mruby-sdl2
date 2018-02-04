#include "sdl2_surface.h"
#include "sdl2_rect.h"
#include "sdl2_pixels.h"
#include <SDL_endian.h>
#include "mruby/data.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/variable.h"

static struct RClass *class_Surface;

typedef struct mrb_sdl2_video_surface_data_t {
  bool         is_associated;
  SDL_Surface *surface;
} mrb_sdl2_video_surface_data_t;

static void
mrb_sdl2_video_surface_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_surface_data_t* data =
    (mrb_sdl2_video_surface_data_t*)p;
  if (NULL != data) {
    if ((NULL != data->surface) && (false == data->is_associated)) {
      SDL_FreeSurface(data->surface);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_video_surface_data_type = {
  "Surface", mrb_sdl2_video_surface_data_free
};

mrb_value
mrb_sdl2_video_surface(mrb_state *mrb, SDL_Surface *surface, bool is_associated)
{
  mrb_sdl2_video_surface_data_t* data =
    (mrb_sdl2_video_surface_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_surface_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->is_associated = is_associated;
  data->surface = surface;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Surface, &mrb_sdl2_video_surface_data_type, data));
}

SDL_Surface *
mrb_sdl2_video_surface_get_ptr(mrb_state *mrb, mrb_value surface)
{
  mrb_sdl2_video_surface_data_t *data;
  if (mrb_nil_p(surface)) {
    return NULL;
  }
  data =
    (mrb_sdl2_video_surface_data_t*)mrb_data_get_ptr(mrb, surface, &mrb_sdl2_video_surface_data_type);
  return data->surface;
}


static mrb_value
mrb_sdl2_video_surface_initialize(mrb_state *mrb, mrb_value self)
{
  uint32_t flags, rmask, gmask, bmask, amask;
  mrb_int width, height, depth;
  mrb_sdl2_video_surface_data_t *data =
    (mrb_sdl2_video_surface_data_t*)DATA_PTR(self);
  mrb_get_args(mrb, "iiiiiiii", &flags, &width, &height, &depth, &rmask, &gmask, &bmask, &amask);
  if (NULL == data) {
    data = (mrb_sdl2_video_surface_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_surface_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->surface = NULL;
  } else {
    if (NULL != data->surface) {
      SDL_FreeSurface(data->surface);
    }
  }
  data->surface = SDL_CreateRGBSurface(flags, width, height, depth, rmask, gmask, bmask, amask);
  if (NULL == data->surface) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }
  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_video_surface_data_type;
  return self;
}

static mrb_value
mrb_sdl2_video_surface_free(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_surface_data_t *data =
    (mrb_sdl2_video_surface_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_surface_data_type);
  if ((NULL != data->surface)) {
    SDL_FreeSurface(data->surface);
    data->surface = NULL;
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_blit_scaled(mrb_state *mrb, mrb_value self)
{
  int ret;
  SDL_Surface * ds;
  SDL_Rect * sr;
  SDL_Surface * ss;
  SDL_Rect * dr = NULL;
  mrb_value src_rect, dst, dst_rect;
  int argc = mrb_get_args(mrb, "ooo", &src_rect, &dst, &dst_rect);
  ss = mrb_sdl2_video_surface_get_ptr(mrb, self);
  sr = mrb_sdl2_rect_get_ptr(mrb, src_rect);
  ds = mrb_sdl2_video_surface_get_ptr(mrb, dst);
  if (argc == 3)
    dr = mrb_sdl2_rect_get_ptr(mrb, dst_rect);
  ret = SDL_BlitScaled(ss, sr, ds, dr);
  if (0 != ret) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_blit_surface(mrb_state *mrb, mrb_value self)
{
  SDL_Surface * ds;
  SDL_Rect * sr;
  SDL_Surface * ss;
  SDL_Rect * dr;
  SDL_Rect tmp;
  mrb_value src_rect, dst, dst_rect;
  mrb_get_args(mrb, "ooo", &src_rect, &dst, &dst_rect);
  ss = mrb_sdl2_video_surface_get_ptr(mrb, self);
  sr = mrb_sdl2_rect_get_ptr(mrb, src_rect);
  ds = mrb_sdl2_video_surface_get_ptr(mrb, dst);
  dr = mrb_sdl2_rect_get_ptr(mrb, dst_rect);
  if (NULL == dr) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "cannot set 3rd argument nil.");
  }
  tmp = *dr;
  if (0 != SDL_BlitSurface(ss, sr, ds, &tmp)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_convert_format(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s, *new_s;
  mrb_int pixel_format;
  mrb_get_args(mrb, "i", &pixel_format);
  s = mrb_sdl2_video_surface_get_ptr(mrb, self);

  new_s = SDL_ConvertSurfaceFormat(s, (Uint32) pixel_format, 0);
  if (NULL == new_s) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_sdl2_video_surface(mrb, new_s, true);
}

static mrb_value
mrb_sdl2_video_surface_format(mrb_state *mrb, mrb_value self)
{
  mrb_value format = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "__pixel_format__"));

  if (mrb_nil_p(format)){
    format = mrb_sdl2_pixels_pixelformat_new(mrb, mrb_sdl2_video_surface_get_ptr(mrb, self)->format);
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "__pixel_format__"), format);
  }

  return format;
}

static mrb_value
mrb_sdl2_video_surface_fill_rect(mrb_state *mrb, mrb_value self)
{
  int color;
  mrb_value rect;
  SDL_Surface *s;
  SDL_Rect * re = NULL;
  mrb_int r, g, b, a;
  int argc = mrb_get_args(mrb, "iiii|o", &r, &g, &b, &a, &rect);
  s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (argc == 5)
    re = mrb_sdl2_rect_get_ptr(mrb, rect);
  color = SDL_MapRGBA(s->format, r, g, b, a);

  if (0 != SDL_FillRect(s, re, color)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_fill_rects(mrb_state *mrb, mrb_value self)
{
  uint32_t color;
  mrb_value rects;
  mrb_int n;
  SDL_Surface *s;
  SDL_Rect * r;
  mrb_int i;
  mrb_get_args(mrb, "io", &color, &rects);
  if (!mrb_array_p(rects)) {
    mrb_raise(mrb, E_TYPE_ERROR, "given 2nd argument is unexpected type (expected Array).");
  }
  n = RARRAY_LEN(rects);
  s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  r = (SDL_Rect *) SDL_malloc(sizeof(SDL_Rect) * n);
  for (i = 0; i < n; ++i) {
    SDL_Rect const * const ptr = mrb_sdl2_rect_get_ptr(mrb, mrb_ary_ref(mrb, rects, i));
    if (NULL != ptr) {
      r[i] = *ptr;
    } else {
      r[i] = (SDL_Rect){ 0, 0, 0, 0 };
    }
  }
  if (0 != SDL_FillRects(s, r, n, color)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_get_clip_rect(mrb_state *mrb, mrb_value self)
{
  SDL_Rect rect;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  SDL_GetClipRect(s, &rect);
  return mrb_sdl2_rect_direct(mrb, &rect);
}

static mrb_value
mrb_sdl2_video_surface_set_clip_rect(mrb_state *mrb, mrb_value self)
{
  SDL_Rect * rect = NULL;
  mrb_value arg;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  mrb_get_args(mrb, "o", &arg);
  if (!mrb_nil_p(arg))
    rect = mrb_sdl2_rect_get_ptr(mrb, arg);
  return SDL_SetClipRect(s, rect) == SDL_FALSE ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_video_surface_get_color_key(mrb_state *mrb, mrb_value self)
{
  uint32_t key;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_GetColorKey(s, &key)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(key);
}

static mrb_value
mrb_sdl2_video_surface_set_color_key(mrb_state *mrb, mrb_value self)
{
  mrb_int red, green, blue, alpha;
  mrb_value flag;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  mrb_get_args(mrb, "oiiii", &flag, &red, &green, &blue, &alpha);

  if (0 != SDL_SetColorKey(s, mrb_bool(flag) ? SDL_TRUE : SDL_FALSE, SDL_MapRGBA(s->format, red, green, blue, alpha))) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_get_alpha_mod(mrb_state *mrb, mrb_value self)
{
  uint8_t alpha;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_GetSurfaceAlphaMod(s, &alpha)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(alpha);
}

static mrb_value
mrb_sdl2_video_surface_set_alpha_mod(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s;
  mrb_int alpha;
  mrb_get_args(mrb, "i", &alpha);
  s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_SetSurfaceAlphaMod(s, (uint8_t)(alpha & 0xff))) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_get_blend_mode(mrb_state *mrb, mrb_value self)
{
  SDL_BlendMode mode;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_GetSurfaceBlendMode(s, &mode)) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(mode);
}

static mrb_value
mrb_sdl2_video_surface_set_blend_mode(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s;
  mrb_int mode;
  mrb_get_args(mrb, "i", &mode);
  s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_SetSurfaceBlendMode(s, (SDL_BlendMode)mode)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_get_color_mod(mrb_state *mrb, mrb_value self)
{
  mrb_value rgb[3];
  uint8_t r, g, b;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_GetSurfaceColorMod(s, &r, &g, &b)) {
    mruby_sdl2_raise_error(mrb);
  }
  rgb[0] = mrb_fixnum_value(r);
  rgb[1] = mrb_fixnum_value(g);
  rgb[2] = mrb_fixnum_value(b);

  return mrb_obj_new(mrb, mrb_class_get_under(mrb, mod_SDL2, "RGB"), 3, rgb);
}

static mrb_value
mrb_sdl2_video_surface_set_color_mod(mrb_state *mrb, mrb_value self)
{
  uint8_t r, g, b;
  mrb_value color;
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  mrb_get_args(mrb, "o", &color);
  if (!mrb_obj_is_kind_of(mrb, color, mrb_class_get_under(mrb, mod_SDL2, "RGB"))) {
    mrb_raise(mrb, E_TYPE_ERROR, "given 1st argument is unexpected type (expected RGB).");
  }
  r = mrb_fixnum(mrb_iv_get(mrb, color, mrb_intern(mrb, "@r", 2)));
  g = mrb_fixnum(mrb_iv_get(mrb, color, mrb_intern(mrb, "@g", 2)));
  b = mrb_fixnum(mrb_iv_get(mrb, color, mrb_intern(mrb, "@b", 2)));
  if (0 != SDL_SetSurfaceColorMod(s, r, g, b)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_set_palette(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return self;
}

static mrb_value
mrb_sdl2_video_surface_set_rle(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  mrb_bool is_rle_enabled;
  mrb_get_args(mrb, "b", &is_rle_enabled);
  if (0 != SDL_SetSurfaceRLE(s, is_rle_enabled ? 1 : 0)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_lock(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  if (0 != SDL_LockSurface(s)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_video_surface_unlock(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s = mrb_sdl2_video_surface_get_ptr(mrb, self);
  SDL_UnlockSurface(s);
  return self;
}


/*
 * SDL2::Video::Surface::load_bmp
 */
static mrb_value
mrb_sdl2_video_surface_load_bmp(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *surface;
  mrb_value file;
  mrb_get_args(mrb, "S", &file);
  surface = SDL_LoadBMP(RSTRING_PTR(file));
  if (NULL == surface) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_sdl2_video_surface(mrb, surface, false);
}

/*
 * SDL2::Video::Surface::save_bmp
 */
static mrb_value
mrb_sdl2_video_surface_save_bmp(mrb_state *mrb, mrb_value self)
{
  SDL_Surface * s;
  mrb_value surface, file;
  mrb_get_args(mrb, "oo", &surface, &file);
  s = mrb_sdl2_video_surface_get_ptr(mrb, surface);
  if (0 != SDL_SaveBMP(s, RSTRING_PTR(file))) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_video_surface_map_rgba(mrb_state *mrb, mrb_value self)
{
  SDL_Surface * s;
  Uint32 result;
  mrb_value surface;
  mrb_int r,g,b,a;
  mrb_get_args(mrb, "oiiii", &surface, &r, &g, &b, &a);
  s = mrb_sdl2_video_surface_get_ptr(mrb, surface);
  result = SDL_MapRGBA(s->format, r, g, b, a);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_video_surface_map_rgb(mrb_state *mrb, mrb_value self)
{
  SDL_Surface * s;
  Uint32 result;
  mrb_value surface;
  mrb_int r,g,b;
  mrb_get_args(mrb, "oiii", &surface, &r, &g, &b);
  s = mrb_sdl2_video_surface_get_ptr(mrb, surface);
  result = SDL_MapRGB(s->format, r, g, b);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_video_surface_width(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_video_surface_get_ptr(mrb, self)->w);
}

static mrb_value
mrb_sdl2_video_surface_height(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_video_surface_get_ptr(mrb, self)->h);
}

static mrb_value
mrb_sdl2_video_surface_pitch(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mrb_sdl2_video_surface_get_ptr(mrb, self)->pitch);
}

static mrb_value
mrb_sdl2_video_surface_convert(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *s, *new_s;
  SDL_PixelFormat *p;
  mrb_value pixel_format;
  mrb_get_args(mrb, "o", &pixel_format);
  p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, pixel_format);
  s = mrb_sdl2_video_surface_get_ptr(mrb, self);

  new_s = SDL_ConvertSurface(s, p, 0);
  if (NULL == new_s) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_sdl2_video_surface(mrb, new_s, true);
}

static mrb_value
mrb_sdl2_video_surface_return_pixel(mrb_state *mrb, SDL_Surface *surface, int x, int y)
{
  int bpp;
  Uint8 *p;
  bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
      return mrb_fixnum_value(*p);  break;
  case 2:
      return mrb_fixnum_value(*(Uint16 *)p);  break;
  case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
          return mrb_fixnum_value(p[0] << 16 | p[1] << 8 | p[2]);
      else
          return mrb_fixnum_value(p[0] | p[1] << 8 | p[2] << 16);
      break;

  case 4:
      return mrb_fixnum_value(*(Uint32 *)p);  break;

  default:
      return mrb_fixnum_value(0);       /* shouldn't happen, but avoids warnings */
  }

  return mrb_fixnum_value(0);
}

static mrb_value
mrb_sdl2_video_surface_get_pixel(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *surface;
  mrb_int x, y;
  mrb_get_args(mrb, "ii", &x, &y);
  surface = mrb_sdl2_video_surface_get_ptr(mrb, self);
  return mrb_sdl2_video_surface_return_pixel(mrb, surface, x, y);
}

static mrb_value
mrb_sdl2_video_surface_set_pixel(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *surface;
  mrb_int x, y, pixel;
  int bpp;
  Uint8 *p;
  mrb_get_args(mrb, "iii", &x, &y, &pixel);
  surface = mrb_sdl2_video_surface_get_ptr(mrb, self);
  bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
      *p = pixel;
      break;

  case 2:
      *(Uint16 *)p = pixel;
      break;

  case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
          p[0] = (pixel >> 16) & 0xff;
          p[1] = (pixel >> 8) & 0xff;
          p[2] = pixel & 0xff;
      } else {
          p[0] = pixel & 0xff;
          p[1] = (pixel >> 8) & 0xff;
          p[2] = (pixel >> 16) & 0xff;
      }
      break;

  case 4:
      *(Uint32 *)p = pixel;
      break;
  }

  return mrb_true_value();
}

static mrb_value
mrb_sdl2_video_surface_must_lock(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *surface;
  surface = mrb_sdl2_video_surface_get_ptr(mrb, self);

  return SDL_MUSTLOCK(surface) == SDL_FALSE ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_video_surface_locked_num(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *surface;
  surface = mrb_sdl2_video_surface_get_ptr(mrb, self);

  return mrb_fixnum_value(surface->locked);
}

static mrb_value
mrb_sdl2_video_surface_gradient_fill_rect(mrb_state *mrb, mrb_value self)
{
  SDL_Surface *surface;
  mrb_int r1, r2, g1, g2, b1, b2, a1, a2;
  mrb_value rect;
  SDL_Rect *re = NULL;
  mrb_bool vertical;
  float i;
  mrb_get_args(mrb, "iiiiiiiiob", &r1, &g1, &b1, &a1, &r2, &g2, &b2, &a2, &rect, &vertical);
  surface = mrb_sdl2_video_surface_get_ptr(mrb, self);
  re = mrb_sdl2_rect_get_ptr(mrb, rect);

  if (surface == NULL || re == NULL) {
    mruby_sdl2_raise_error(mrb);
  }
  if (SDL_MUSTLOCK(surface)) {
    if (0 != SDL_LockSurface(surface)) {
      mruby_sdl2_raise_error(mrb);
    }
  }

  int difr = r2 - r1;
  int difg = g2 - g1;
  int difb = b2 - b1;
  int difa = a2 - a1;
  if (vertical) {
    for (i = 0; i < re->h; i+=1.0) {
      float mod = i / re->h;
      SDL_Rect rec;
      rec.x = re->x;
      rec.y = re->y + i;
      rec.w = re->w;
      rec.h = 1;
      int r = r1 + difr * mod;
      int g = g1 + difg * mod;
      int b = b1 + difb * mod;
      int a = a1 + difa * mod;
      if (0 != SDL_FillRect(surface,
			    &rec,
			    SDL_MapRGBA(surface->format,
					r,
					g,
					b,
					a))) {
	mruby_sdl2_raise_error(mrb);
      }
    }
  } else {
    for (i = 0; i < re->w; i+=1.0) {
      float mod = i / re->w;
      SDL_Rect rec;
      rec.x = re->x + i;
      rec.y = re->y;
      rec.w = 1;
      rec.h = re->h;
      int r = r1 + difr * mod;
      int g = g1 + difg * mod;
      int b = b1 + difb * mod;
      int a = a1 + difa * mod;
      if (0 != SDL_FillRect(surface, &rec,
			    SDL_MapRGBA(surface->format,
					r, g, b,a))) {
	mruby_sdl2_raise_error(mrb);
      }
    }
  }

  if (SDL_MUSTLOCK(surface)) {
    SDL_UnlockSurface(surface);
  }
  return self;
}


void
mruby_sdl2_video_surface_init(mrb_state *mrb, struct RClass *mod_Video)
{
  int arena_size;
  class_Surface = mrb_define_class_under(mrb, mod_Video, "Surface", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Surface, MRB_TT_DATA);

  mrb_define_method(mrb, class_Surface, "initialize",         mrb_sdl2_video_surface_initialize,         MRB_ARGS_REQ(8));
  mrb_define_method(mrb, class_Surface, "free",               mrb_sdl2_video_surface_free,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "destroy",            mrb_sdl2_video_surface_free,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "blit_scaled",        mrb_sdl2_video_surface_blit_scaled,        MRB_ARGS_REQ(3));
  mrb_define_method(mrb, class_Surface, "blit_surface",       mrb_sdl2_video_surface_blit_surface,       MRB_ARGS_REQ(3));
  mrb_define_method(mrb, class_Surface, "convert_format",     mrb_sdl2_video_surface_convert_format,     MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_Surface, "width",              mrb_sdl2_video_surface_width,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "height",             mrb_sdl2_video_surface_height,             MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "pitch",              mrb_sdl2_video_surface_pitch,              MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "format",             mrb_sdl2_video_surface_format,             MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "fill_rect",          mrb_sdl2_video_surface_fill_rect,          MRB_ARGS_REQ(4) | MRB_ARGS_OPT(5));
  mrb_define_method(mrb, class_Surface, "fill_rects",         mrb_sdl2_video_surface_fill_rects,         MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_Surface, "gradient_fill_rect", mrb_sdl2_video_surface_gradient_fill_rect, MRB_ARGS_REQ(10));
  mrb_define_method(mrb, class_Surface, "get_clip_rect",      mrb_sdl2_video_surface_get_clip_rect,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "set_clip_rect",      mrb_sdl2_video_surface_set_clip_rect,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "color_key_get",      mrb_sdl2_video_surface_get_color_key,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "color_key_set",      mrb_sdl2_video_surface_set_color_key,      MRB_ARGS_REQ(5));
  mrb_define_method(mrb, class_Surface, "alpha_mod",          mrb_sdl2_video_surface_get_alpha_mod,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "alpha_mod=",         mrb_sdl2_video_surface_set_alpha_mod,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "blend_mode",         mrb_sdl2_video_surface_get_blend_mode,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "blend_mode=",        mrb_sdl2_video_surface_set_blend_mode,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "color_mod",          mrb_sdl2_video_surface_get_color_mod,      MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "color_mod=",         mrb_sdl2_video_surface_set_color_mod,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "palette",            mrb_sdl2_video_surface_set_palette,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "rle",                mrb_sdl2_video_surface_set_rle,            MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "must_lock?",         mrb_sdl2_video_surface_must_lock,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "locked_num",         mrb_sdl2_video_surface_locked_num,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "lock",               mrb_sdl2_video_surface_lock,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "unlock",             mrb_sdl2_video_surface_unlock,             MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Surface, "convert",            mrb_sdl2_video_surface_convert,            MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Surface, "get_pixel",          mrb_sdl2_video_surface_get_pixel,          MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class_Surface, "set_pixel",          mrb_sdl2_video_surface_set_pixel,          MRB_ARGS_REQ(3));

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, class_Surface, "SDL_BLENDMODE_NONE",  mrb_fixnum_value(SDL_BLENDMODE_NONE));
  mrb_define_const(mrb, class_Surface, "SDL_BLENDMODE_BLEND", mrb_fixnum_value(SDL_BLENDMODE_BLEND));
  mrb_define_const(mrb, class_Surface, "SDL_BLENDMODE_ADD",   mrb_fixnum_value(SDL_BLENDMODE_ADD));
  mrb_define_const(mrb, class_Surface, "SDL_BLENDMODE_MOD",   mrb_fixnum_value(SDL_BLENDMODE_MOD));
  mrb_gc_arena_restore(mrb, arena_size);

  mrb_define_class_method(mrb, class_Surface, "load_bmp", mrb_sdl2_video_surface_load_bmp, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, class_Surface, "save_bmp", mrb_sdl2_video_surface_save_bmp, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, class_Surface, "map_rgba", mrb_sdl2_video_surface_map_rgba, MRB_ARGS_REQ(5));
  mrb_define_class_method(mrb, class_Surface, "map_rgb",  mrb_sdl2_video_surface_map_rgb,  MRB_ARGS_REQ(4));
}

void
mruby_sdl2_video_surface_final(mrb_state *mrb, struct RClass *mod_Video)
{
}
