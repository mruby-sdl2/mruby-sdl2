#include "sdl2_messagebox.h"
#include "sdl2_video.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "mruby/data.h"

struct RClass *class_MessageBoxData = NULL;

typedef struct mrb_sdl2_message_boxdata_data_t {
  SDL_MessageBoxData *boxdata;
} mrb_sdl2_message_boxdata_data_t;

static void
mrb_sdl2_message_boxdata_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_message_boxdata_data_t *data =
    (mrb_sdl2_message_boxdata_data_t*)p;
  if (NULL != data) {
    if (NULL != data->boxdata) {
      SDL_free(data->boxdata);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_message_boxdata_data_type = {
  "MessageBox", mrb_sdl2_message_boxdata_data_free
};

SDL_MessageBoxData *
mrb_sdl2_message_boxdata_get_ptr(mrb_state *mrb, mrb_value boxdata)
{
  mrb_sdl2_message_boxdata_data_t *data;
  if (mrb_nil_p(boxdata)) {
    return NULL;
  }

  data =
    (mrb_sdl2_message_boxdata_data_t*)mrb_data_get_ptr(mrb, boxdata, &mrb_sdl2_message_boxdata_data_type);
  return data->boxdata;
}

static mrb_value
mrb_sdl2_message_boxdata_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value window;
  mrb_sdl2_message_boxdata_data_t *data =
    (mrb_sdl2_message_boxdata_data_t*)DATA_PTR(self);

  int const argc = mrb_get_args(mrb, "|o", &window);
  if ((0 != argc) && (1 != argc)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == data) {
    data = (mrb_sdl2_message_boxdata_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_message_boxdata_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
  }
  data->boxdata = (SDL_MessageBoxData *) SDL_malloc(sizeof(SDL_MessageBoxData));
  if (NULL == data->boxdata) {
    mruby_sdl2_raise_error(mrb);
  }
  if (1 == argc) {
    data->boxdata->window = mrb_sdl2_video_window_get_ptr(mrb, window);
  } else if (0 != argc) {
    data->boxdata = NULL;
  }
  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_message_boxdata_data_type;
  return self;
}

static mrb_value
mrb_sdl2_message_boxdata_destroy(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_message_boxdata_data_t *data =
    (mrb_sdl2_message_boxdata_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_message_boxdata_data_type);
  if (NULL != data->boxdata) {
    SDL_free(data->boxdata);
    data->boxdata = NULL;
  }
  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_window_show_simple_message_box(mrb_state *mrb, mrb_value self)
{
  SDL_Window * window;
  mrb_int flags;
  mrb_value title, message;
  mrb_get_args(mrb, "iSS", &flags, &title, &message);

  window = mrb_sdl2_video_window_get_ptr(mrb, self);
  SDL_ShowSimpleMessageBox((Uint32) flags, RSTRING_PTR(title), RSTRING_PTR(message), window);
  return self;
}

static mrb_value
mrb_sdl2_message_boxdata_set_fags(mrb_state *mrb, mrb_value self)
{
  mrb_int flags;
  mrb_get_args(mrb, "i", &flags);

  ((mrb_sdl2_message_boxdata_data_t*)DATA_PTR(self))->boxdata->flags = (Uint32) flags;
  return self;
}

static mrb_value
mrb_sdl2_message_boxdata_set_window(mrb_state *mrb, mrb_value self)
{
  mrb_value window;
  mrb_get_args(mrb, "o", &window);

  ((mrb_sdl2_message_boxdata_data_t*)DATA_PTR(self))->boxdata->window = mrb_sdl2_video_window_get_ptr(mrb, window);
  return self;
}

static mrb_value
mrb_sdl2_message_boxdata_set_title(mrb_state *mrb, mrb_value self)
{
  mrb_value title;
  mrb_get_args(mrb, "S", &title);
  ((mrb_sdl2_message_boxdata_data_t*)DATA_PTR(self))->boxdata->title = RSTRING_PTR(title);
  return self;
}

static mrb_value
mrb_sdl2_message_boxdata_set_message(mrb_state *mrb, mrb_value self)
{
  mrb_value message;
  mrb_get_args(mrb, "S", &message);

  ((mrb_sdl2_message_boxdata_data_t*)DATA_PTR(self))->boxdata->message = RSTRING_PTR(message);
  return self;
}

static mrb_value
mrb_sdl2_message_boxdata_set_buttons(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_message_boxdata_data_t *data_ptr;
  SDL_MessageBoxButtonData * data;
  struct RArray * rary;
  mrb_value buttons;
  int i;
  mrb_get_args(mrb, "A", &buttons);

  data_ptr =
    (mrb_sdl2_message_boxdata_data_t*)DATA_PTR(self);

  rary = mrb_ary_ptr(buttons);
  data_ptr->boxdata->numbuttons = (int) rary->len;
  data = (SDL_MessageBoxButtonData *) SDL_malloc(sizeof(SDL_MessageBoxButtonData) * rary->len);
  for (i = 0; i<rary->len; ++i) {
    struct RArray * temp_rary = mrb_ary_ptr(rary->ptr[i]);
    data[i].flags = (Uint32) temp_rary->ptr[0].value.i;
    data[i].buttonid = (int) temp_rary->ptr[1].value.i;
    data[i].text = RSTRING_PTR(temp_rary->ptr[2]);
  }

  data_ptr->boxdata->buttons = data;
  return self;
}

static mrb_value
mrb_sdl2_message_boxdata_set_color_scheme(mrb_state *mrb, mrb_value self)
{
  // TODO
  return self;
}

static mrb_value
mrb_sdl2_message_boxdata_show(mrb_state *mrb, mrb_value self)
{
  int result;
  mrb_sdl2_message_boxdata_data_t *data_ptr =
    (mrb_sdl2_message_boxdata_data_t*)DATA_PTR(self);

  SDL_ShowMessageBox(data_ptr->boxdata, &result);
  return mrb_fixnum_value(result);
}

void
mruby_sdl2_messagebox_init(mrb_state *mrb)
{
  struct RClass * mod_Video;
  struct RClass * class_Window;
  class_MessageBoxData = mrb_define_class_under(mrb, mod_SDL2, "MessageBox", mrb->object_class);
  mod_Video = mrb_module_get_under(mrb, mod_SDL2, "Video");
  class_Window = mrb_class_get_under(mrb, mod_Video, "Window");

  mrb_define_module_function(mrb, class_Window, "show_simple_message_box", mrb_sdl2_window_show_simple_message_box,  ARGS_REQ(3));

  mrb_define_method(mrb, class_MessageBoxData, "initialize",       mrb_sdl2_message_boxdata_initialize,       ARGS_REQ(1) | ARGS_NONE());
  mrb_define_method(mrb, class_MessageBoxData, "destroy",          mrb_sdl2_message_boxdata_destroy,          ARGS_NONE());
  mrb_define_method(mrb, class_MessageBoxData, "set_flags",        mrb_sdl2_message_boxdata_set_fags,         ARGS_REQ(1));
  mrb_define_method(mrb, class_MessageBoxData, "set_window",       mrb_sdl2_message_boxdata_set_window,       ARGS_REQ(1));
  mrb_define_method(mrb, class_MessageBoxData, "set_title",        mrb_sdl2_message_boxdata_set_title,        ARGS_REQ(1));
  mrb_define_method(mrb, class_MessageBoxData, "set_message",      mrb_sdl2_message_boxdata_set_message,      ARGS_REQ(1));
  mrb_define_method(mrb, class_MessageBoxData, "set_buttons",      mrb_sdl2_message_boxdata_set_buttons,      ARGS_REQ(1));
  mrb_define_method(mrb, class_MessageBoxData, "set_color_scheme", mrb_sdl2_message_boxdata_set_color_scheme, ARGS_REQ(1));
  mrb_define_method(mrb, class_MessageBoxData, "show",             mrb_sdl2_message_boxdata_show,             ARGS_NONE());
}

void
mruby_sdl2_messagebox_final(mrb_state *mrb)
{
}
