#include "sdl2_filesystem.h"
#include "mruby/string.h"


static mrb_value
mrb_sdl2_filesystem_get_base_path(mrb_state *mrb, mrb_value self)
{
  char * result = SDL_GetBasePath();
  return mrb_str_new_cstr(mrb, result);
}

static mrb_value
mrb_sdl2_filesystem_get_pref_path(mrb_state *mrb, mrb_value self)
{
  char * result;
  mrb_value org, path;
  mrb_get_args(mrb, "SS", &org, &path);
  result = SDL_GetPrefPath(RSTRING_PTR(org), RSTRING_PTR(path));
  return mrb_str_new_cstr(mrb, result);
}

void
mruby_sdl2_filesystem_init(mrb_state *mrb)
{
  struct RClass *mod_FileSystem = mrb_define_module_under(mrb, mod_SDL2, "FileSystem");

  mrb_define_method(mrb, mod_FileSystem, "get_base_path", mrb_sdl2_filesystem_get_base_path, MRB_ARGS_NONE());
  mrb_define_method(mrb, mod_FileSystem, "get_pref_path", mrb_sdl2_filesystem_get_pref_path, MRB_ARGS_REQ(2));
}

void
mruby_sdl2_filesystem_final(mrb_state *mrb)
{
}
