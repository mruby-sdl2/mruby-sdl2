#include "sdl2_pixels.h"
#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/data.h"



static struct RClass *class_PixelFormat = NULL;
static struct RClass *class_Palette = NULL;

typedef struct mrb_sdl2_pixels_palette_data_t {
  bool        is_associated;
  SDL_Palette *palette;
} mrb_sdl2_pixels_palette_data_t;

static void
mrb_sdl2_pixels_palette_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_pixels_palette_data_t *data =
    (mrb_sdl2_pixels_palette_data_t*)p;
  if (NULL != data) {
    if ((!data->is_associated) && (NULL != data->palette)) {
      SDL_FreePalette(data->palette);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_pixels_palette_data_type = {
  "palette", &mrb_sdl2_pixels_palette_data_free
};

SDL_Palette *
mrb_sdl2_pixels_palette_get_ptr(mrb_state *mrb, mrb_value palette)
{
  if (mrb_nil_p(palette)) {
    return NULL;
  }

  mrb_sdl2_pixels_palette_data_t *data =
    (mrb_sdl2_pixels_palette_data_t*)mrb_data_get_ptr(mrb, palette, &mrb_sdl2_pixels_palette_data_type);
  return data->palette;
}

mrb_value
mrb_sdl2_pixels_palette(mrb_state *mrb, SDL_PixelFormat *palette)
{
  if (NULL == palette) {
    return mrb_nil_value();
  }

  mrb_sdl2_pixels_palette_data_t *data =
    (mrb_sdl2_pixels_palette_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_pixels_palette_data_t));

  data->is_associated = false;
  data->palette = palette;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Palette, &mrb_sdl2_pixels_palette_data_type, data));
}

mrb_value
mrb_sdl2_pixels_associated_palette(mrb_state *mrb, SDL_Palette *palette)
{
  if (NULL == palette) {
    return mrb_nil_value();
  }

  mrb_sdl2_pixels_palette_data_t *data =
    (mrb_sdl2_pixels_palette_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_pixels_palette_data_t));

  data->is_associated = true;
  data->palette = palette;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Palette, &mrb_sdl2_pixels_palette_data_type, data));
}



typedef struct mrb_sdl2_pixels_pixelformat_data_t {
  bool        is_associated;
  SDL_PixelFormat *pixelformat;
} mrb_sdl2_pixels_pixelformat_data_t;

static void
mrb_sdl2_pixels_pixelformat_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_pixels_pixelformat_data_t *data =
    (mrb_sdl2_pixels_pixelformat_data_t*)p;
  if (NULL != data) {
    if ((!data->is_associated) && (NULL != data->pixelformat)) {
      SDL_FreeFormat(data->pixelformat);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_pixels_pixelformat_data_type = {
  "PixelFormat", &mrb_sdl2_pixels_pixelformat_data_free
};

SDL_PixelFormat *
mrb_sdl2_pixels_pixelformat_get_ptr(mrb_state *mrb, mrb_value pixelformat)
{
  if (mrb_nil_p(pixelformat)) {
    return NULL;
  }

  mrb_sdl2_pixels_pixelformat_data_t *data =
    (mrb_sdl2_pixels_pixelformat_data_t*)mrb_data_get_ptr(mrb, pixelformat, &mrb_sdl2_pixels_pixelformat_data_type);
  return data->pixelformat;
}

mrb_value
mrb_sdl2_pixels_pixelformat(mrb_state *mrb, SDL_PixelFormat *pixelformat)
{
  if (NULL == pixelformat) {
    return mrb_nil_value();
  }

  mrb_sdl2_pixels_pixelformat_data_t *data =
    (mrb_sdl2_pixels_pixelformat_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_pixels_pixelformat_data_t));
  data->is_associated = false;
  data->pixelformat = pixelformat;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_PixelFormat, &mrb_sdl2_pixels_pixelformat_data_type, data));
}

mrb_value
mrb_sdl2_pixels_associated_pixelformat(mrb_state *mrb, SDL_PixelFormat *pixelformat)
{
  if (NULL == pixelformat) {
    return mrb_nil_value();
  }

  mrb_sdl2_pixels_pixelformat_data_t *data =
    (mrb_sdl2_pixels_pixelformat_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_pixels_pixelformat_data_t));

  data->is_associated = true;
  data->pixelformat = pixelformat;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_PixelFormat, &mrb_sdl2_pixels_pixelformat_data_type, data));
}


/***************************************************************************
*
* module SDL2::Pixels
*
***************************************************************************/

static mrb_value
mrb_sdl2_pixels_get_pixel_format_name(mrb_state *mrb, mrb_value mod)
{
  mrb_int format;
  mrb_get_args(mrb, "i", &format);

  mrb_value result = mrb_str_new_cstr(mrb, SDL_GetPixelFormatName((int) format));
  return result;
}

static mrb_value
mrb_sdl2_pixels_format_to_masks(mrb_state *mrb, mrb_value mod)
{
  mrb_int format;
  mrb_get_args(mrb, "i", &format);

  int * bpp;
  Uint32 * Rmask;
  Uint32 * Gmask;
  Uint32 * Bmask;
  Uint32 * Amask;
  if (SDL_PixelFormatEnumToMasks((Uint32) format, bpp, Rmask, Gmask, Bmask, Amask) != SDL_FALSE) {
    mrb_value array = mrb_ary_new_capa(mrb, 4);
    mrb_ary_push(mrb, array, mrb_fixnum_value(format));
    mrb_ary_push(mrb, array, mrb_fixnum_value(format));
    /* TBD
    mrb_ary_push(mrb, array, mrb_fixnum_value(bpp));
    mrb_ary_push(mrb, array, mrb_fixnum_value(Rmask));
    mrb_ary_push(mrb, array, mrb_fixnum_value(Gmask));
    mrb_ary_push(mrb, array, mrb_fixnum_value(Amask));
    */
    return array;
  }
  return mrb_false_value();
}

static mrb_value
mrb_sdl2_pixels_masks_to_format(mrb_state *mrb, mrb_value mod) {
  mrb_int bpp, Rmask, Gmask, Bmask, Amask;
  mrb_get_args(mrb, "iiiii", &bpp, &Rmask, &Gmask, &Bmask, &Amask);

  Uint32 result = SDL_MasksToPixelFormatEnum((int) bpp, (Uint32) Rmask, (Uint32) Gmask, (Uint32) Bmask, (Uint32) Amask);

  if (result == SDL_PIXELFORMAT_UNKNOWN)
    return mrb_false_value();

  return mrb_fixnum_value((mrb_int) result);
}


static mrb_value
mrb_sdl2_pixels_pixelformat_set_palette(mrb_state *mrb, mrb_value self)
{
  mrb_value palette;
  mrb_get_args(mrb, "o", &palette);
  SDL_PixelFormat * pixelformat_p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self);
  SDL_Palette * palette_p = mrb_sdl2_pixels_palette_get_ptr(mrb, palette);
  int result = SDL_SetPixelFormatPalette(pixelformat_p, palette_p);
  if (0 > result) {
    // error occurred
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}


static mrb_value
mrb_sdl2_pixels_palette_set_color(mrb_state *mrb, mrb_value self)
{
  mrb_int r, g, b, a, firstcolor, ncolors;
  mrb_get_args(mrb, "iiiiii", &r, &g, &b, &a, &firstcolor, &ncolors);
  SDL_Color color;
  color.r = (Uint8) r;
  color.g = (Uint8) g;
  color.b = (Uint8) b;
  color.a = (Uint8) a;
  SDL_Palette *palette_p = mrb_sdl2_pixels_palette_get_ptr(mrb, self);
  int result = SDL_SetPaletteColors(palette_p, &color, (int) firstcolor, (int) ncolors);
  if (result == 1) {
    // error occurred
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

static mrb_value
mrb_sdl2_pixels_pixelformat_map_rgb(mrb_state *mrb, mrb_value self)
{
  mrb_int r, g, b;
  mrb_get_args(mrb, "iii", &r, &g, &b);
  SDL_PixelFormat * pixelformat_p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self);
  Uint32 result = SDL_MapRGB(pixelformat_p, (Uint8) r, (Uint8) g, (Uint8) b);

  return mrb_fixnum_value((mrb_int) result);
}

static mrb_value
mrb_sdl2_pixels_pixelformat_map_rgba(mrb_state *mrb, mrb_value self)
{
  mrb_int r, g, b, a;
  mrb_get_args(mrb, "iiii", &r, &g, &b, &a);
  SDL_PixelFormat * pixelformat_p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self);
  Uint32 result = SDL_MapRGBA(pixelformat_p, (Uint8) r, (Uint8) g, (Uint8) b, (Uint8) a);

  return mrb_fixnum_value((mrb_int) result);
}

static mrb_value
mrb_sdl2_pixels_pixelformat_get_rgb(mrb_state *mrb, mrb_value self)
{
  mrb_int pixel;
  mrb_get_args(mrb, "i", &pixel);

  SDL_PixelFormat * pixelformat_p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self);

  Uint8 * r;
  Uint8 * g;
  Uint8 * b;
  SDL_GetRGB((Uint32) pixel, pixelformat_p, r, g ,b);

  mrb_value array = mrb_ary_new_capa(mrb, 3);
  mrb_ary_push(mrb, array, mrb_fixnum_value((int) *r));
  mrb_ary_push(mrb, array, mrb_fixnum_value((int) *g));
  mrb_ary_push(mrb, array, mrb_fixnum_value((int) *b));

  return array;
}

static mrb_value
mrb_sdl2_pixels_pixelformat_get_rgba(mrb_state *mrb, mrb_value self)
{
  mrb_int pixel;
  mrb_get_args(mrb, "i", &pixel);

  SDL_PixelFormat * pixelformat_p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self);

  Uint8 * r;
  Uint8 * g;
  Uint8 * b;
  Uint8 * a;
  SDL_GetRGBA((Uint32) pixel, pixelformat_p, r, g ,b, a);

  mrb_value array = mrb_ary_new_capa(mrb, 4);
  mrb_ary_push(mrb, array, mrb_fixnum_value((int) *r));
  mrb_ary_push(mrb, array, mrb_fixnum_value((int) *g));
  mrb_ary_push(mrb, array, mrb_fixnum_value((int) *b));
  mrb_ary_push(mrb, array, mrb_fixnum_value((int) *a));

  return array;
}


static mrb_value
mrb_sdl2_pixels_calculate_gamma_ramp(mrb_state *mrb, mrb_value self)
{
  mrb_float gamma;
  mrb_get_args(mrb, "f", &gamma);
  Uint16 * ramp;
  SDL_CalculateGammaRamp((float) gamma, ramp);

  return mrb_fixnum_value(ramp);
}
/***************************************************************************
*
* module SDL2::Pixels::PixelFormat
*
***************************************************************************/


static mrb_value
mrb_sdl2_pixels_pixelformat_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_pixels_pixelformat_data_t *data =
    (mrb_sdl2_pixels_pixelformat_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_pixels_pixelformat_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_pixels_pixelformat_data_t));
    data->is_associated = false;
    data->pixelformat = NULL;
  }

  SDL_PixelFormat *pixelformat = NULL;
  if (1 == mrb->c->ci->argc) {
    mrb_int pixel_format;
    mrb_get_args(mrb, "i", &pixel_format);
    pixelformat = SDL_AllocFormat((Uint32) pixel_format);
  } else {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == pixelformat) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }

  data->pixelformat = pixelformat;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_pixels_pixelformat_data_type;
  return self;
}

static mrb_value
mrb_sdl2_pixels_pixelformat_free(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_pixels_pixelformat_data_t *data =
    (mrb_sdl2_pixels_pixelformat_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_pixels_pixelformat_data_type);
  if ((!data->is_associated) && (NULL != data->pixelformat)) {
    SDL_FreeFormat(data->pixelformat);
    data->pixelformat = NULL;
  }
  return self;
}

/***************************************************************************
*
* module SDL2::Pixels::palette
*
***************************************************************************/


static mrb_value
mrb_sdl2_pixels_palette_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_pixels_palette_data_t *data =
    (mrb_sdl2_pixels_palette_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_pixels_palette_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_pixels_palette_data_t));
    data->is_associated = false;
    data->palette = NULL;
  }

  SDL_Palette *palette = NULL;
  if (1 == mrb->c->ci->argc) {
    mrb_int ncolors;
    mrb_get_args(mrb, "i", &ncolors);
    palette = SDL_AllocPalette((int) ncolors);
  } else {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == palette) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }

  data->palette = palette;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_pixels_palette_data_type;
  return self;
}

static mrb_value
mrb_sdl2_pixels_palette_free(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_pixels_palette_data_t *data =
    (mrb_sdl2_pixels_palette_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_pixels_palette_data_type);
  if ((!data->is_associated) && (NULL != data->palette)) {
    SDL_FreePalette(data->palette);
    data->palette = NULL;
  }
  return self;
}


void
mruby_sdl2_pixels_init(mrb_state *mrb)
{
  struct RClass *mod_Pixels = mrb_define_module_under(mrb, mod_SDL2, "Pixels");

  class_PixelFormat = mrb_define_class_under(mrb, mod_Pixels, "PixelFormat", mrb->object_class);
  class_Palette     = mrb_define_class_under(mrb, mod_Pixels, "Palette",     mrb->object_class);

  MRB_SET_INSTANCE_TT(class_PixelFormat, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Palette,     MRB_TT_DATA);

  mrb_define_module_function(mrb, mod_Pixels, "get_format_name",      mrb_sdl2_pixels_get_pixel_format_name, ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Pixels, "format_to_masks",      mrb_sdl2_pixels_format_to_masks,       ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Pixels, "masks_to_format",      mrb_sdl2_pixels_masks_to_format,       ARGS_REQ(5));
  mrb_define_module_function(mrb, mod_Pixels, "calculate_gamma_ramp", mrb_sdl2_pixels_calculate_gamma_ramp,  ARGS_REQ(1));

  mrb_define_method(mrb, class_PixelFormat, "initialize",  mrb_sdl2_pixels_pixelformat_initialize,  ARGS_REQ(1));
  mrb_define_method(mrb, class_PixelFormat, "set_palette", mrb_sdl2_pixels_pixelformat_set_palette, ARGS_REQ(1));
  mrb_define_method(mrb, class_PixelFormat, "destroy",     mrb_sdl2_pixels_pixelformat_free,        ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "free",        mrb_sdl2_pixels_pixelformat_free,        ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "mapRGB",      mrb_sdl2_pixels_pixelformat_map_rgb,     ARGS_REQ(3));
  mrb_define_method(mrb, class_PixelFormat, "mapRGBA",     mrb_sdl2_pixels_pixelformat_map_rgba,    ARGS_REQ(4));
  mrb_define_method(mrb, class_PixelFormat, "get_rgb",     mrb_sdl2_pixels_pixelformat_get_rgb,     ARGS_REQ(1));
  mrb_define_method(mrb, class_PixelFormat, "get_rgba",    mrb_sdl2_pixels_pixelformat_get_rgba,    ARGS_REQ(1));
  // SDL_MapRGB

  mrb_define_method(mrb, class_Palette, "initialize", mrb_sdl2_pixels_palette_initialize, ARGS_REQ(1));
  mrb_define_method(mrb, class_Palette, "set_color",  mrb_sdl2_pixels_palette_set_color,  ARGS_REQ(6) /*| ARGS_REQ(3)*/);
  mrb_define_method(mrb, class_Palette, "destroy",    mrb_sdl2_pixels_palette_free,       ARGS_NONE());
  mrb_define_method(mrb, class_Palette, "free",       mrb_sdl2_pixels_palette_free,       ARGS_NONE());

  /* PixelFormats start */
  int arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_UNKNOWN",       mrb_fixnum_value(SDL_PIXELFORMAT_UNKNOWN));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_INDEX1LSB",     mrb_fixnum_value(SDL_PIXELFORMAT_INDEX1LSB));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_INDEX1MSB",     mrb_fixnum_value(SDL_PIXELFORMAT_INDEX1MSB));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_INDEX4LSB",     mrb_fixnum_value(SDL_PIXELFORMAT_INDEX4LSB));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_INDEX4MSB",     mrb_fixnum_value(SDL_PIXELFORMAT_INDEX4MSB));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_INDEX8",        mrb_fixnum_value(SDL_PIXELFORMAT_INDEX8));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_RGB332",        mrb_fixnum_value(SDL_PIXELFORMAT_RGB332));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_RGB444",        mrb_fixnum_value(SDL_PIXELFORMAT_RGB444));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_RGB555",        mrb_fixnum_value(SDL_PIXELFORMAT_RGB555));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_BGR555",        mrb_fixnum_value(SDL_PIXELFORMAT_BGR555));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_ARGB4444",      mrb_fixnum_value(SDL_PIXELFORMAT_ARGB4444));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_RGBA4444",      mrb_fixnum_value(SDL_PIXELFORMAT_RGBA4444));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_ABGR4444",      mrb_fixnum_value(SDL_PIXELFORMAT_ABGR4444));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_BGRA4444",      mrb_fixnum_value(SDL_PIXELFORMAT_BGRA4444));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_ARGB1555",      mrb_fixnum_value(SDL_PIXELFORMAT_ARGB1555));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_RGBA5551",      mrb_fixnum_value(SDL_PIXELFORMAT_RGBA5551));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_ABGR1555",      mrb_fixnum_value(SDL_PIXELFORMAT_ABGR1555));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_BGRA5551",      mrb_fixnum_value(SDL_PIXELFORMAT_BGRA5551));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_RGB565",        mrb_fixnum_value(SDL_PIXELFORMAT_RGB565));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_BGR565",        mrb_fixnum_value(SDL_PIXELFORMAT_BGR565));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_RGB24",         mrb_fixnum_value(SDL_PIXELFORMAT_RGB24));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_BGR24",         mrb_fixnum_value(SDL_PIXELFORMAT_BGR24));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_RGB888",        mrb_fixnum_value(SDL_PIXELFORMAT_RGB888));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_RGBX8888",      mrb_fixnum_value(SDL_PIXELFORMAT_RGBX8888));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_BGR888",        mrb_fixnum_value(SDL_PIXELFORMAT_BGR888));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_BGRX8888",      mrb_fixnum_value(SDL_PIXELFORMAT_BGRX8888));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_ARGB8888",      mrb_fixnum_value(SDL_PIXELFORMAT_ARGB8888));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_RGBA8888",      mrb_fixnum_value(SDL_PIXELFORMAT_RGBA8888));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_ABGR8888",      mrb_fixnum_value(SDL_PIXELFORMAT_ABGR8888));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_BGRA8888",      mrb_fixnum_value(SDL_PIXELFORMAT_BGRA8888));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_ARGB2101010",   mrb_fixnum_value(SDL_PIXELFORMAT_ARGB2101010));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_YV12",          mrb_fixnum_value(SDL_PIXELFORMAT_YV12));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_IYUV",          mrb_fixnum_value(SDL_PIXELFORMAT_IYUV));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_YUY2",          mrb_fixnum_value(SDL_PIXELFORMAT_YUY2));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_UYVY",          mrb_fixnum_value(SDL_PIXELFORMAT_UYVY));
  mrb_define_const(mrb, mod_Pixels, "SDL_PIXELFORMAT_YVYU",          mrb_fixnum_value(SDL_PIXELFORMAT_YVYU));
  mrb_gc_arena_restore(mrb, arena_size);
  /* PixelFormats end */

}


void
mruby_sdl2_pixels_final(mrb_state *mrb)
{
}
