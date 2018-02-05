#include "sdl2.h"
#include "sdl2_video.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include <SDL_video.h>

static struct RClass *mod_GL            = NULL;
static struct RClass *class_GLContext   = NULL;

typedef struct mrb_sdl2_video_glcontext_data_t {
  SDL_GLContext context;
} mrb_sdl2_video_glcontext_data_t;

static void
mrb_sdl2_video_glcontext_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_video_glcontext_data_t *data =
    (mrb_sdl2_video_glcontext_data_t*)p;
  if (NULL != data) {
    if (NULL != data->context) {
      SDL_GL_DeleteContext(data->context);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_video_glcontext_data_type = {
  "GLContext", mrb_sdl2_video_glcontext_data_free
};

/***************************************************************************
*
* module SDL2::Video::GL
*
***************************************************************************/

/*
 * SDL2::Video::GL::attribute_get
 */
static mrb_value
mrb_sdl2_video_gl_get_attribute(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/*
 * SDL2::Video::GL::attribute_set
 */
static mrb_value
mrb_sdl2_video_gl_set_attribute(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/*
 * SDL2::Video::GL::exntension_supported?
 */
static mrb_value
mrb_sdl2_video_gl_is_extension_supported(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/*
 * SDL2::Video::GL::swap_interval
 */
static mrb_value
mrb_sdl2_video_gl_get_swap_interval(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/*
 * SDL2::Video::GL::swap_interval=
 */
static mrb_value
mrb_sdl2_video_gl_set_swap_interval(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/*
 * SDL2::Video::GL::swap_window
 */
static mrb_value
mrb_sdl2_video_gl_swap_window(mrb_state *mrb, mrb_value self)
{
  mrb_raise(mrb, E_NOTIMP_ERROR, "not implemented.");
  return mrb_nil_value();
}

/***************************************************************************
*
* class SDL2::Video::GLContext
*
***************************************************************************/

/*
 * SDL2::Video::GLContext.initialize
 */
static mrb_value
mrb_sdl2_video_glcontext_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value window;
  SDL_Window *sdl_window;
  SDL_GLContext context = NULL;
  mrb_sdl2_video_glcontext_data_t *data =
    (mrb_sdl2_video_glcontext_data_t*)DATA_PTR(self);

  int const argc = mrb_get_args(mrb, "|o", &window);

  if (NULL == data) {
    data = (mrb_sdl2_video_glcontext_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_video_glcontext_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
  }
  if (0 != argc) {
    sdl_window = mrb_sdl2_video_window_get_ptr(mrb,window);
    context = SDL_GL_CreateContext(sdl_window);
    if (NULL == context) {
      mrb_free(mrb, data);
      mruby_sdl2_raise_error(mrb);
    }
  }
  data->context = context;
  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_video_glcontext_data_type;
  return self;
}

/*
 * SDL2::Video::GLContext.create
 */
static mrb_value
mrb_sdl2_video_glcontext_create(mrb_state *mrb, mrb_value self)
{
  SDL_Window *sdl_window;
  SDL_GLContext context = NULL;
  mrb_value window;
  mrb_sdl2_video_glcontext_data_t *data =
    (mrb_sdl2_video_glcontext_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_glcontext_data_type);
  mrb_get_args(mrb, "o", &window);
  if (mrb_nil_p(window)) {
    context = SDL_GL_CreateContext(NULL);
  } else {
    sdl_window = mrb_sdl2_video_window_get_ptr(mrb,window);
    context = SDL_GL_CreateContext(sdl_window);
  }
  if (NULL == context) {
    mruby_sdl2_raise_error(mrb);
  }
  data->context = context;
  return self;
}

/*
 * SDL2::Video::GLContext.delete
 */
static mrb_value
mrb_sdl2_video_glcontext_delete(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_video_glcontext_data_t *data =
    (mrb_sdl2_video_glcontext_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_video_glcontext_data_type);
  SDL_GL_DeleteContext(data->context);
  data->context = NULL;
  return self;
}

/* SDL2::Video::Window */

static mrb_value
mrb_sdl2_video_window_make_current(mrb_state *mrb, mrb_value self)
{
  SDL_Window *window;
  SDL_GLContext *context = NULL;
  mrb_value arg;

  window = mrb_sdl2_video_window_get_ptr(mrb,self);
  if (NULL == window) {
    return mrb_nil_value();
  }
  mrb_get_args(mrb, "o", &arg);
  if (!mrb_nil_p(arg)) {
    context =
      ((mrb_sdl2_video_glcontext_data_t*)mrb_data_get_ptr(mrb, arg, &mrb_sdl2_video_glcontext_data_type))->context;
  }
  if (0 != SDL_GL_MakeCurrent(window, context)) {
    mruby_sdl2_raise_error(mrb);
  }
  return self;
}

/* * * * */

void
mruby_sdl2_video_gl_init(mrb_state *mrb)
{
  mod_GL    = mrb_define_module_under(mrb, mod_Video, "GL");
  class_GLContext   = mrb_define_class_under(mrb, mod_Video, "GLContext",   mrb->object_class);

  MRB_SET_INSTANCE_TT(class_GLContext,   MRB_TT_DATA);

  mrb_define_method(mrb, class_Window, "make_current",         mrb_sdl2_video_window_make_current,         MRB_ARGS_REQ(1));

  mrb_define_module_function(mrb, mod_GL, "attribute_get",        mrb_sdl2_video_gl_get_attribute,          MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GL, "attribute_set",        mrb_sdl2_video_gl_set_attribute,          MRB_ARGS_REQ(2));
  mrb_define_module_function(mrb, mod_GL, "extension_supported?", mrb_sdl2_video_gl_is_extension_supported, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GL, "swap_interval",        mrb_sdl2_video_gl_get_swap_interval,      MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_GL, "swap_interval=",       mrb_sdl2_video_gl_set_swap_interval,      MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GL, "swap_window",          mrb_sdl2_video_gl_swap_window,            MRB_ARGS_REQ(1));

  mrb_define_method(mrb, class_GLContext, "initialize", mrb_sdl2_video_glcontext_initialize, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, class_GLContext, "create",     mrb_sdl2_video_glcontext_create,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_GLContext, "delete",     mrb_sdl2_video_glcontext_delete,     MRB_ARGS_NONE());
  mrb_define_method(mrb, class_GLContext, "destroy",    mrb_sdl2_video_glcontext_delete,     MRB_ARGS_NONE());

  /* SDL_GLattr */
  mrb_define_const(mrb, mod_GL, "SDL_GL_RED_SIZE",                   mrb_fixnum_value(SDL_GL_RED_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_GREEN_SIZE",                 mrb_fixnum_value(SDL_GL_GREEN_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_BLUE_SIZE",                  mrb_fixnum_value(SDL_GL_BLUE_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ALPHA_SIZE",                 mrb_fixnum_value(SDL_GL_ALPHA_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_BUFFER_SIZE",                mrb_fixnum_value(SDL_GL_BUFFER_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_DOUBLEBUFFER",               mrb_fixnum_value(SDL_GL_DOUBLEBUFFER));
  mrb_define_const(mrb, mod_GL, "SDL_GL_DEPTH_SIZE",                 mrb_fixnum_value(SDL_GL_DEPTH_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_STENCIL_SIZE",               mrb_fixnum_value(SDL_GL_STENCIL_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ACCUM_RED_SIZE",             mrb_fixnum_value(SDL_GL_ACCUM_RED_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ACCUM_GREEN_SIZE",           mrb_fixnum_value(SDL_GL_ACCUM_GREEN_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ACCUM_BLUE_SIZE",            mrb_fixnum_value(SDL_GL_ACCUM_BLUE_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ACCUM_ALPHA_SIZE",           mrb_fixnum_value(SDL_GL_ACCUM_ALPHA_SIZE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_STEREO",                     mrb_fixnum_value(SDL_GL_STEREO));
  mrb_define_const(mrb, mod_GL, "SDL_GL_MULTISAMPLEBUFFERS",         mrb_fixnum_value(SDL_GL_MULTISAMPLEBUFFERS));
  mrb_define_const(mrb, mod_GL, "SDL_GL_MULTISAMPLESAMPLES",         mrb_fixnum_value(SDL_GL_MULTISAMPLESAMPLES));
  mrb_define_const(mrb, mod_GL, "SDL_GL_ACCELERATED_VISUAL",         mrb_fixnum_value(SDL_GL_ACCELERATED_VISUAL));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_MAJOR_VERSION",      mrb_fixnum_value(SDL_GL_CONTEXT_MAJOR_VERSION));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_MINOR_VERSION",      mrb_fixnum_value(SDL_GL_CONTEXT_MINOR_VERSION));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_EGL",                mrb_fixnum_value(SDL_GL_CONTEXT_EGL));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_FLAGS",              mrb_fixnum_value(SDL_GL_CONTEXT_FLAGS));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_PROFILE_MASK",       mrb_fixnum_value(SDL_GL_CONTEXT_PROFILE_MASK));
  mrb_define_const(mrb, mod_GL, "SDL_GL_SHARE_WITH_CURRENT_CONTEXT", mrb_fixnum_value(SDL_GL_SHARE_WITH_CURRENT_CONTEXT));

  /* SDL_GLcontextFlag */
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_DEBUG_FLAG",              mrb_fixnum_value(SDL_GL_CONTEXT_DEBUG_FLAG));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG", mrb_fixnum_value(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG",      mrb_fixnum_value(SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_RESET_ISOLATION_FLAG",    mrb_fixnum_value(SDL_GL_CONTEXT_RESET_ISOLATION_FLAG));

  /* SDL_GLprofile */
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_PROFILE_CORE",          mrb_fixnum_value(SDL_GL_CONTEXT_PROFILE_CORE));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_PROFILE_COMPATIBILITY", mrb_fixnum_value(SDL_GL_CONTEXT_PROFILE_COMPATIBILITY));
  mrb_define_const(mrb, mod_GL, "SDL_GL_CONTEXT_PROFILE_ES",            mrb_fixnum_value(SDL_GL_CONTEXT_PROFILE_ES));
}

void
mruby_sdl2_video_gl_final(mrb_state *mrb)
{
}
