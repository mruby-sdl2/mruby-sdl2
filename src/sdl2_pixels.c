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
  mrb_sdl2_pixels_palette_data_t *data;
  if (mrb_nil_p(palette))
    return NULL;

   data =
    (mrb_sdl2_pixels_palette_data_t*)mrb_data_get_ptr(mrb, palette, &mrb_sdl2_pixels_palette_data_type);
  return data->palette;
}

mrb_value
mrb_sdl2_pixels_palette(mrb_state *mrb, SDL_Palette *palette)
{
  mrb_sdl2_pixels_palette_data_t *data;
  if (NULL == palette) {
    return mrb_nil_value();
  }

  data =
    (mrb_sdl2_pixels_palette_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_pixels_palette_data_t));

  data->is_associated = false;
  data->palette = palette;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Palette, &mrb_sdl2_pixels_palette_data_type, data));
}

mrb_value
mrb_sdl2_pixels_associated_palette(mrb_state *mrb, SDL_Palette *palette)
{
  mrb_sdl2_pixels_palette_data_t *data;
  if (NULL == palette) {
    return mrb_nil_value();
  }

  data =
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
  mrb_sdl2_pixels_pixelformat_data_t *data;
  if (mrb_nil_p(pixelformat)) {
    return NULL;
  }

  data =
    (mrb_sdl2_pixels_pixelformat_data_t*)mrb_data_get_ptr(mrb, pixelformat, &mrb_sdl2_pixels_pixelformat_data_type);
  return data->pixelformat;
}

mrb_value
mrb_sdl2_pixels_pixelformat(mrb_state *mrb, SDL_PixelFormat *pixelformat)
{
  mrb_sdl2_pixels_pixelformat_data_t *data;
  if (NULL == pixelformat) {
    return mrb_nil_value();
  }

  data =
    (mrb_sdl2_pixels_pixelformat_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_pixels_pixelformat_data_t));
  data->is_associated = false;
  data->pixelformat = pixelformat;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_PixelFormat, &mrb_sdl2_pixels_pixelformat_data_type, data));
}

mrb_value
mrb_sdl2_pixels_associated_pixelformat(mrb_state *mrb, SDL_PixelFormat *pixelformat)
{
  mrb_sdl2_pixels_pixelformat_data_t *data;
  if (NULL == pixelformat) {
    return mrb_nil_value();
  }

  data =
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
  mrb_value result;
  mrb_get_args(mrb, "i", &format);

  result = mrb_str_new_cstr(mrb, SDL_GetPixelFormatName((int) format));
  return result;
}

static mrb_value
mrb_sdl2_pixels_format_to_masks(mrb_state *mrb, mrb_value mod)
{
  mrb_int format;
  int bpp;
  Uint32 Rmask;
  Uint32 Gmask;
  Uint32 Bmask;
  Uint32 Amask;
  mrb_value array;
  mrb_get_args(mrb, "i", &format);

  if (SDL_PixelFormatEnumToMasks((Uint32) format, &bpp, &Rmask, &Gmask, &Bmask, &Amask) != SDL_FALSE) {
    array = mrb_ary_new_capa(mrb, 4);
    mrb_ary_push(mrb, array, mrb_fixnum_value(bpp));
    mrb_ary_push(mrb, array, mrb_fixnum_value(Rmask));
    mrb_ary_push(mrb, array, mrb_fixnum_value(Gmask));
    mrb_ary_push(mrb, array, mrb_fixnum_value(Amask));
    return array;
  }
  return mrb_false_value();
}

static mrb_value
mrb_sdl2_pixels_masks_to_format(mrb_state *mrb, mrb_value mod) {
  mrb_int bpp, Rmask, Gmask, Bmask, Amask;
  Uint32 result;
  mrb_get_args(mrb, "iiiii", &bpp, &Rmask, &Gmask, &Bmask, &Amask);

  result = SDL_MasksToPixelFormatEnum((int) bpp, (Uint32) Rmask, (Uint32) Gmask, (Uint32) Bmask, (Uint32) Amask);

  if (result == SDL_PIXELFORMAT_UNKNOWN)
    return mrb_false_value();

  return mrb_fixnum_value((mrb_int) result);
}


static mrb_value
mrb_sdl2_pixels_pixelformat_set_palette(mrb_state *mrb, mrb_value self)
{
  mrb_value palette;
  SDL_PixelFormat * pixelformat_p;
  SDL_Palette * palette_p;
  int result;
  mrb_get_args(mrb, "o", &palette);
  pixelformat_p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self);
  palette_p = mrb_sdl2_pixels_palette_get_ptr(mrb, palette);
  result = SDL_SetPixelFormatPalette(pixelformat_p, palette_p);
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
  SDL_Palette *palette_p;
  int result;
  SDL_Color color;
  mrb_get_args(mrb, "iiiiii", &r, &g, &b, &a, &firstcolor, &ncolors);
  color.r = (Uint8) r;
  color.g = (Uint8) g;
  color.b = (Uint8) b;
  color.a = (Uint8) a;
  palette_p = mrb_sdl2_pixels_palette_get_ptr(mrb, self);
  result = SDL_SetPaletteColors(palette_p, &color, (int) firstcolor, (int) ncolors);
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
  Uint32 result;
  SDL_PixelFormat * pixelformat_p;
  mrb_get_args(mrb, "iii", &r, &g, &b);
  pixelformat_p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self);
  result = SDL_MapRGB(pixelformat_p, (Uint8) r, (Uint8) g, (Uint8) b);

  return mrb_fixnum_value((mrb_int) result);
}

static mrb_value
mrb_sdl2_pixels_pixelformat_map_rgba(mrb_state *mrb, mrb_value self)
{
  mrb_int r, g, b, a;
  Uint32 result;
  SDL_PixelFormat * pixelformat_p;
  mrb_get_args(mrb, "iiii", &r, &g, &b, &a);
  pixelformat_p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self);
  result = SDL_MapRGBA(pixelformat_p, (Uint8) r, (Uint8) g, (Uint8) b, (Uint8) a);

  return mrb_fixnum_value((mrb_int) result);
}

static mrb_value
mrb_sdl2_pixels_pixelformat_get_rgb(mrb_state *mrb, mrb_value self)
{
  mrb_int pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  SDL_PixelFormat * pixelformat_p;
  mrb_value array;
  mrb_get_args(mrb, "i", &pixel);

  pixelformat_p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self);

  SDL_GetRGB((Uint32) pixel, pixelformat_p, &r, &g ,&b);

  array = mrb_ary_new_capa(mrb, 3);
  mrb_ary_push(mrb, array, mrb_fixnum_value(r));
  mrb_ary_push(mrb, array, mrb_fixnum_value(g));
  mrb_ary_push(mrb, array, mrb_fixnum_value(b));

  return array;
}

static mrb_value
mrb_sdl2_pixels_pixelformat_get_rgba(mrb_state *mrb, mrb_value self)
{
  mrb_int pixel;
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
  SDL_PixelFormat * pixelformat_p;
  mrb_value array;
  mrb_get_args(mrb, "i", &pixel);

  pixelformat_p = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self);

  SDL_GetRGBA((Uint32) pixel, pixelformat_p, &r, &g , &b, &a);

  array = mrb_ary_new_capa(mrb, 4);
  mrb_ary_push(mrb, array, mrb_fixnum_value(r));
  mrb_ary_push(mrb, array, mrb_fixnum_value(g));
  mrb_ary_push(mrb, array, mrb_fixnum_value(b));
  mrb_ary_push(mrb, array, mrb_fixnum_value(a));

  return array;
}


static mrb_value
mrb_sdl2_pixels_calculate_gamma_ramp(mrb_state *mrb, mrb_value self)
{
  mrb_float gamma;
  Uint16 ramp;
  mrb_get_args(mrb, "f", &gamma);
  SDL_CalculateGammaRamp((float) gamma, &ramp);

  return mrb_fixnum_value(ramp);
}
/***************************************************************************
*
* module SDL2::Pixels::PixelFormat
*
***************************************************************************/

mrb_value
mrb_sdl2_pixels_pixelformat_new(mrb_state *mrb, SDL_PixelFormat *format)
{
  //@todo how do better?
  mrb_sdl2_pixels_pixelformat_data_t *data;
  mrb_value args[] = {mrb_fixnum_value(format->format)};
  mrb_value oformat = mrb_obj_new(mrb, class_PixelFormat, 1, args);
  data = (mrb_sdl2_pixels_pixelformat_data_t*)DATA_PTR(oformat);
  data->pixelformat = format;
  DATA_PTR(oformat) = data;
  return oformat;
}

static mrb_value
mrb_sdl2_pixels_pixelformat_initialize(mrb_state *mrb, mrb_value self)
{
	SDL_PixelFormat *pixelformat;
  mrb_sdl2_pixels_pixelformat_data_t *data =
    (mrb_sdl2_pixels_pixelformat_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_pixels_pixelformat_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_pixels_pixelformat_data_t));
    data->is_associated = false;
    data->pixelformat = NULL;
  }

  pixelformat = NULL;
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
	SDL_Palette *palette;
  mrb_sdl2_pixels_palette_data_t *data =
    (mrb_sdl2_pixels_palette_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_pixels_palette_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_pixels_palette_data_t));
    data->is_associated = false;
    data->palette = NULL;
  }

  palette = NULL;
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

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_format(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->format); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_palette(mrb_state *mrb, mrb_value self) { 
  return mrb_sdl2_pixels_palette(mrb, (mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->palette)); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_BitsPerPixel(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->BitsPerPixel); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_BytesPerPixel(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->BytesPerPixel); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_padding(mrb_state *mrb, mrb_value self) { 

  Uint8 *r = mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->padding;
  mrb_value array = mrb_ary_new_capa(mrb, 2);
  mrb_ary_push(mrb, array, mrb_fixnum_value(r[0]));
  mrb_ary_push(mrb, array, mrb_fixnum_value(r[1]));

  return array; 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Rmask(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Rmask); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Gmask(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Gmask); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Bmask(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Bmask); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Amask(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Amask); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Rloss(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Rloss); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Gloss(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Gloss); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Bloss(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Bloss); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Aloss(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Aloss); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Rshift(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Rshift); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Gshift(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Gshift); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Bshift(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Bshift); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_Ashift(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->Ashift); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_refcount(mrb_state *mrb, mrb_value self) { 
  return mrb_fixnum_value(mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->refcount); 
}

static mrb_value 
mrb_sdl2_pixels_pixelformat_get_next(mrb_state *mrb, mrb_value self) { 
  return mrb_sdl2_pixels_associated_pixelformat(mrb, (mrb_sdl2_pixels_pixelformat_get_ptr(mrb, self)->next)); 
}


void
mruby_sdl2_pixels_init(mrb_state *mrb)
{
	int arena_size;
  struct RClass *mod_Pixels = mrb_define_module_under(mrb, mod_SDL2, "Pixels");

  class_PixelFormat = mrb_define_class_under(mrb, mod_Pixels, "PixelFormat", mrb->object_class);
  class_Palette     = mrb_define_class_under(mrb, mod_Pixels, "Palette",     mrb->object_class);

  MRB_SET_INSTANCE_TT(class_PixelFormat, MRB_TT_DATA);
  MRB_SET_INSTANCE_TT(class_Palette,     MRB_TT_DATA);

  mrb_define_module_function(mrb, mod_Pixels, "get_format_name",      mrb_sdl2_pixels_get_pixel_format_name, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Pixels, "format_to_masks",      mrb_sdl2_pixels_format_to_masks,       MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Pixels, "masks_to_format",      mrb_sdl2_pixels_masks_to_format,       MRB_ARGS_REQ(5));
  mrb_define_module_function(mrb, mod_Pixels, "calculate_gamma_ramp", mrb_sdl2_pixels_calculate_gamma_ramp,  MRB_ARGS_REQ(1));

  mrb_define_method(mrb, class_PixelFormat, "initialize",     mrb_sdl2_pixels_pixelformat_initialize,         MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_PixelFormat, "destroy",        mrb_sdl2_pixels_pixelformat_free,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "free",           mrb_sdl2_pixels_pixelformat_free,               MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "format",         mrb_sdl2_pixels_pixelformat_get_format,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "palette",        mrb_sdl2_pixels_pixelformat_get_palette,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "BitsPerPixel",   mrb_sdl2_pixels_pixelformat_get_BitsPerPixel,   MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "BytesPerPixel",  mrb_sdl2_pixels_pixelformat_get_BytesPerPixel,  MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "padding",        mrb_sdl2_pixels_pixelformat_get_padding,        MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Rmask",          mrb_sdl2_pixels_pixelformat_get_Rmask,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Gmask",          mrb_sdl2_pixels_pixelformat_get_Gmask,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Bmask",          mrb_sdl2_pixels_pixelformat_get_Bmask,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Amask",          mrb_sdl2_pixels_pixelformat_get_Amask,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Rloss",          mrb_sdl2_pixels_pixelformat_get_Rloss,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Gloss",          mrb_sdl2_pixels_pixelformat_get_Gloss,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Bloss",          mrb_sdl2_pixels_pixelformat_get_Bloss,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Aloss",          mrb_sdl2_pixels_pixelformat_get_Aloss,          MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Rshift",         mrb_sdl2_pixels_pixelformat_get_Rshift,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Gshift",         mrb_sdl2_pixels_pixelformat_get_Gshift,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Bshift",         mrb_sdl2_pixels_pixelformat_get_Bshift,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "Ashift",         mrb_sdl2_pixels_pixelformat_get_Ashift,         MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "refcount",       mrb_sdl2_pixels_pixelformat_get_refcount,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "next",           mrb_sdl2_pixels_pixelformat_get_next,           MRB_ARGS_NONE());
  mrb_define_method(mrb, class_PixelFormat, "set_palette",    mrb_sdl2_pixels_pixelformat_set_palette,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_PixelFormat, "mapRGB",         mrb_sdl2_pixels_pixelformat_map_rgb,            MRB_ARGS_REQ(3));
  mrb_define_method(mrb, class_PixelFormat, "mapRGBA",        mrb_sdl2_pixels_pixelformat_map_rgba,           MRB_ARGS_REQ(4));
  mrb_define_method(mrb, class_PixelFormat, "get_rgb",        mrb_sdl2_pixels_pixelformat_get_rgb,            MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_PixelFormat, "get_rgba",       mrb_sdl2_pixels_pixelformat_get_rgba,           MRB_ARGS_REQ(1));
  // SDL_MapRGB

  mrb_define_method(mrb, class_Palette, "initialize", mrb_sdl2_pixels_palette_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_Palette, "set_color",  mrb_sdl2_pixels_palette_set_color,  MRB_ARGS_REQ(6) /*| MRB_ARGS_REQ(3)*/);
  mrb_define_method(mrb, class_Palette, "destroy",    mrb_sdl2_pixels_palette_free,       MRB_ARGS_NONE());
  mrb_define_method(mrb, class_Palette, "free",       mrb_sdl2_pixels_palette_free,       MRB_ARGS_NONE());

  /* PixelFormats start */
  arena_size = mrb_gc_arena_save(mrb);
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
