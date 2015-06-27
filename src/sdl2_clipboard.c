#include "sdl2_clipboard.h"
#include "mruby/string.h"

static mrb_value
mrb_sdl2_clipboard_text(mrb_state *mrb, mrb_value self)
{
  if (SDL_HasClipboardText()) {
    mrb_value s;
    char * text = SDL_GetClipboardText();

    if (!text)
      return mrb_nil_value();

    s = mrb_str_new_cstr(mrb, text);
    free(text);
    return s;
  }

  return mrb_nil_value();
}

static mrb_value
mrb_sdl2_clipboard_set_text(mrb_state *mrb, mrb_value self)
{
  mrb_value text;
  mrb_get_args(mrb, "S", &text);
  SDL_SetClipboardText(RSTRING_PTR(text));
  return text;
}

static mrb_value
mrb_sdl2_clipboard_has_text(mrb_state *mrb, mrb_value self)
{
  SDL_bool r = SDL_HasClipboardText();
  return (SDL_FALSE == r) ? mrb_false_value() : mrb_true_value();
}

void
mruby_sdl2_clipboard_init(mrb_state *mrb)
{
  struct RClass *mod_Clipboard = mrb_define_module_under(mrb, mod_SDL2, "Clipboard");

  mrb_define_module_function(mrb, mod_Clipboard, "text",      mrb_sdl2_clipboard_text,     ARGS_NONE()); // SDL_GetClipboardText
  mrb_define_module_function(mrb, mod_Clipboard, "text=",     mrb_sdl2_clipboard_set_text, ARGS_REQ(1)); // SDL_SetClipboardText
  mrb_define_module_function(mrb, mod_Clipboard, "has_text?", mrb_sdl2_clipboard_has_text, ARGS_NONE()); // SDL_HasClipboardText
}

void
mruby_sdl2_clipboard_final(mrb_state *mrb)
{
}
