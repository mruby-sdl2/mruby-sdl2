#include "sdl2_joystick.h"
#include "sdl2_gamecontroller.h"
#include "mruby/value.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"

static struct RClass *class_GameController = NULL;

typedef struct mrb_sdl2_gamecontrollers_gamecontroller_data_t {
  SDL_GameController *gamecontroller;
} mrb_sdl2_gamecontrollers_gamecontroller_data_t;

static void
mrb_sdl2_gamecontrollers_gamecontroller_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_gamecontrollers_gamecontroller_data_t *data =
    (mrb_sdl2_gamecontrollers_gamecontroller_data_t*)p;
  if (NULL != data) {
    if (NULL != data->gamecontroller) {
      SDL_GameControllerClose(data->gamecontroller);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_gamecontrollers_gamecontroller_data_type = {
  "GameController", &mrb_sdl2_gamecontrollers_gamecontroller_data_free
};

SDL_GameController *
mrb_sdl2_gamecontrollers_gamecontroller_get_ptr(mrb_state *mrb, mrb_value gamecontroller)
{
  if (mrb_nil_p(gamecontroller)) {
    return NULL;
  }

  mrb_sdl2_gamecontrollers_gamecontroller_data_t *data =
    (mrb_sdl2_gamecontrollers_gamecontroller_data_t*)mrb_data_get_ptr(mrb, gamecontroller, &mrb_sdl2_gamecontrollers_gamecontroller_data_type);
  return data->gamecontroller;
}

mrb_value
mrb_sdl2_gamecontrollers_gamecontroller(mrb_state *mrb, SDL_GameController *gamecontroller)
{
  if (NULL == gamecontroller) {
    return mrb_nil_value();
  }

  mrb_sdl2_gamecontrollers_gamecontroller_data_t *data =
    (mrb_sdl2_gamecontrollers_gamecontroller_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_gamecontrollers_gamecontroller_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->gamecontroller = gamecontroller;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_GameController, &mrb_sdl2_gamecontrollers_gamecontroller_data_type, data));
}

mrb_value
mrb_sdl2_gamecontrollers_associated_gamecontroller(mrb_state *mrb, SDL_GameController *gamecontroller)
{
  if (NULL == gamecontroller) {
    return mrb_nil_value();
  }

  mrb_sdl2_gamecontrollers_gamecontroller_data_t *data =
    (mrb_sdl2_gamecontrollers_gamecontroller_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_gamecontrollers_gamecontroller_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->gamecontroller = gamecontroller;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_GameController, &mrb_sdl2_gamecontrollers_gamecontroller_data_type, data));
}

static mrb_value
mrb_sdl2_gamecontrollers_gamecontroller_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_gamecontrollers_gamecontroller_data_t *data =
    (mrb_sdl2_gamecontrollers_gamecontroller_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_gamecontrollers_gamecontroller_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_gamecontrollers_gamecontroller_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->gamecontroller = NULL;
  }

  SDL_GameController *gamecontroller = NULL;
  if (1 == mrb->c->ci->argc) {
    mrb_int index;
    mrb_get_args(mrb, "i", &index);
    gamecontroller = SDL_GameControllerOpen(index);
  } else {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == gamecontroller) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }

  data->gamecontroller = gamecontroller;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_gamecontrollers_gamecontroller_data_type;
  return self;
}

static mrb_value
mrb_sdl2_gamecontrollers_gamecontroller_free(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_gamecontrollers_gamecontroller_data_t *data =
    (mrb_sdl2_gamecontrollers_gamecontroller_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_gamecontrollers_gamecontroller_data_type);
  if (NULL != data->gamecontroller) {
    SDL_GameControllerClose(data->gamecontroller);
    data->gamecontroller = NULL;
  }
  return self;
}

static mrb_value
mrb_sdl2_gamecontrollers_add_mapping(mrb_state *mrb, mrb_value self)
{
  mrb_value mappingString;
  mrb_get_args(mrb, "S", &mappingString);
  int result = SDL_GameControllerAddMapping(RSTRING_PTR(mappingString));
}

static mrb_value
mrb_sdl2_gamecontrollers_get_mapping_from_guid(mrb_state *mrb, mrb_value self)
{
  // TBD
  return mrb_false_value();
}

static mrb_value
mrb_sdl2_gamecontrollers_gamecontroler_get_mapping(mrb_state *mrb, mrb_value self)
{
  SDL_GameController * gamecontroller_p = mrb_sdl2_gamecontrollers_gamecontroller_get_ptr(mrb, self);
  char * result = SDL_GameControllerMapping(gamecontroller_p);
  if (result == NULL) {
    return mrb_false_value();
  }

  return mrb_str_new_cstr(mrb, result);
}

static mrb_value
mrb_sdl2_gamecontrollers_is_gamecontroller_supported(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  SDL_bool result = SDL_IsGameController(index);

  return (result == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_gamecontrollers_get_name_from_index(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  char * result = SDL_GameControllerNameForIndex(index);
  if (result == NULL)
    return mrb_false_value();

  return mrb_str_new_cstr(mrb, result);
}

static mrb_value
mrb_sdl2_gamecontrollers_gamecontroler_get_name(mrb_state *mrb, mrb_value self)
{
  SDL_GameController * gamecontroller_p = mrb_sdl2_gamecontrollers_gamecontroller_get_ptr(mrb, self);
  char * result = SDL_GameControllerName(gamecontroller_p);
  if (result == NULL)
    return mrb_false_value();

  return mrb_str_new_cstr(mrb, result);
}

static mrb_value
mrb_sdl2_gamecontrollers_gamecontroler_is_attached(mrb_state *mrb, mrb_value self)
{
  SDL_GameController * gamecontroller_p = mrb_sdl2_gamecontrollers_gamecontroller_get_ptr(mrb, self);
  SDL_bool result = SDL_GameControllerGetAttached(gamecontroller_p);

  return (result == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_gamecontrollers_gamecontroler_get_joystick(mrb_state *mrb, mrb_value self)
{
  SDL_GameController * gamecontroller_p = mrb_sdl2_gamecontrollers_gamecontroller_get_ptr(mrb, self);
  SDL_Joystick * result = SDL_GameControllerGetJoystick(gamecontroller_p);

  return mrb_sdl2_joystick_joystick(mrb, result);
}

static mrb_value
mrb_sdl2_gamecontrollers_gamecontroler_get_event_state(mrb_state *mrb, mrb_value self)
{
  mrb_int state;
  mrb_get_args(mrb, "i", &state);
  int result = SDL_GameControllerEventState(state);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gamecontrollers_get_axis_from_string(mrb_state *mrb, mrb_value self)
{
  mrb_value pchString;
  mrb_get_args(mrb, "S", &pchString);

  return mrb_fixnum_value(SDL_GameControllerGetAxisFromString(RSTRING_PTR(pchString)));
}

static mrb_value
mrb_sdl2_gamecontrollers_get_string_from_axis(mrb_state *mrb, mrb_value self)
{
  mrb_int axis;
  mrb_get_args(mrb, "i", &axis);
  char * result = SDL_GameControllerGetStringForAxis(axis);
  return mrb_str_new_cstr(mrb, result);
}

static mrb_value
mrb_sdl2_gamecontrollers_gamecontroler_get_axis(mrb_state *mrb, mrb_value self)
{
  mrb_int axis;
  mrb_get_args(mrb, "i", &axis);
  SDL_GameController * gamecontroller_p = mrb_sdl2_gamecontrollers_gamecontroller_get_ptr(mrb, self);

  Sint16 result = SDL_GameControllerGetAxis(gamecontroller_p, axis);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gamecontrollers_get_button_from_string(mrb_state *mrb, mrb_value self)
{
  mrb_value pchString;
  mrb_get_args(mrb, "S", &pchString);

  return mrb_fixnum_value(SDL_GameControllerGetButtonFromString(RSTRING_PTR(pchString)));
}

static mrb_value
mrb_sdl2_gamecontrollers_get_string_from_button(mrb_state *mrb, mrb_value self)
{
  mrb_int button;
  mrb_get_args(mrb, "i", &button);
  char * result = SDL_GameControllerGetStringForButton(button);
  return mrb_str_new_cstr(mrb, result);
}

static mrb_value
mrb_sdl2_gamecontrollers_gamecontroler_get_button_state(mrb_state *mrb, mrb_value self)
{
  mrb_int button;
  mrb_get_args(mrb, "i", &button);
  SDL_GameController * gamecontroller_p = mrb_sdl2_gamecontrollers_gamecontroller_get_ptr(mrb, self);

  Uint8 result = SDL_GameControllerGetButton(gamecontroller_p, button);
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_gamecontrollers_update(mrb_state *mrb, mrb_value self)
{
  SDL_GameControllerUpdate();
}

void
mruby_sdl2_gamecontroller_init(mrb_state *mrb)
{
  struct RClass * mod_GameController = mrb_define_module_under(mrb, mod_SDL2, "GameControllers");
  class_GameController = mrb_define_class_under(mrb, mod_GameController, "GameController", mrb->object_class);
  MRB_SET_INSTANCE_TT(class_GameController, MRB_TT_DATA);

  mrb_define_module_function(mrb, mod_GameController, "add_mapping", mrb_sdl2_gamecontrollers_add_mapping,  ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GameController, "get_mapping_from_guid", mrb_sdl2_gamecontrollers_get_mapping_from_guid,  ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GameController, "is_gamecontroller_supported?", mrb_sdl2_gamecontrollers_is_gamecontroller_supported,  ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GameController, "get_name_from_index", mrb_sdl2_gamecontrollers_get_name_from_index,  ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GameController, "get_axis_from_string", mrb_sdl2_gamecontrollers_get_axis_from_string,  ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GameController, "get_string_from_axis", mrb_sdl2_gamecontrollers_get_string_from_axis,  ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GameController, "get_button_from_string", mrb_sdl2_gamecontrollers_get_button_from_string,  ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GameController, "get_string_from_button", mrb_sdl2_gamecontrollers_get_string_from_button,  ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_GameController, "update", mrb_sdl2_gamecontrollers_update,  ARGS_NONE());


  mrb_define_method(mrb, class_GameController, "initialize", mrb_sdl2_gamecontrollers_gamecontroller_initialize, ARGS_NONE());
  mrb_define_method(mrb, class_GameController, "destroy", mrb_sdl2_gamecontrollers_gamecontroller_free, ARGS_NONE());
  mrb_define_method(mrb, class_GameController, "free", mrb_sdl2_gamecontrollers_gamecontroller_free, ARGS_NONE());
  mrb_define_method(mrb, class_GameController, "get_mapping", mrb_sdl2_gamecontrollers_gamecontroler_get_mapping, ARGS_NONE());
  mrb_define_method(mrb, class_GameController, "get_name", mrb_sdl2_gamecontrollers_gamecontroler_get_name, ARGS_NONE());
  mrb_define_method(mrb, class_GameController, "is_attached?", mrb_sdl2_gamecontrollers_gamecontroler_is_attached, ARGS_NONE());
  mrb_define_method(mrb, class_GameController, "get_joystick", mrb_sdl2_gamecontrollers_gamecontroler_get_joystick, ARGS_NONE());
  mrb_define_method(mrb, class_GameController, "get_event_state", mrb_sdl2_gamecontrollers_gamecontroler_get_event_state, ARGS_REQ(1));
  mrb_define_method(mrb, class_GameController, "get_axis", mrb_sdl2_gamecontrollers_gamecontroler_get_axis, ARGS_REQ(1));
  mrb_define_method(mrb, class_GameController, "get_button_state", mrb_sdl2_gamecontrollers_gamecontroler_get_button_state, ARGS_REQ(1));


  int arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_AXIS_INVALID",      mrb_fixnum_value(SDL_CONTROLLER_AXIS_INVALID));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_AXIS_LEFTX",        mrb_fixnum_value(SDL_CONTROLLER_AXIS_LEFTX));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_AXIS_LEFTY",        mrb_fixnum_value(SDL_CONTROLLER_AXIS_LEFTY));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_AXIS_RIGHTX",       mrb_fixnum_value(SDL_CONTROLLER_AXIS_RIGHTX));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_AXIS_RIGHTY",       mrb_fixnum_value(SDL_CONTROLLER_AXIS_RIGHTY));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_AXIS_TRIGGERLEFT",  mrb_fixnum_value(SDL_CONTROLLER_AXIS_TRIGGERLEFT));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_AXIS_TRIGGERRIGHT", mrb_fixnum_value(SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_AXIS_MAX",          mrb_fixnum_value(SDL_CONTROLLER_AXIS_MAX));
  mrb_gc_arena_restore(mrb, arena_size);

  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_INVALID",       mrb_fixnum_value(SDL_CONTROLLER_BUTTON_INVALID));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_A",             mrb_fixnum_value(SDL_CONTROLLER_BUTTON_A));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_B",             mrb_fixnum_value(SDL_CONTROLLER_BUTTON_B));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_X",             mrb_fixnum_value(SDL_CONTROLLER_BUTTON_X));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_Y",             mrb_fixnum_value(SDL_CONTROLLER_BUTTON_Y));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_BACK",          mrb_fixnum_value(SDL_CONTROLLER_BUTTON_BACK));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_GUIDE",         mrb_fixnum_value(SDL_CONTROLLER_BUTTON_GUIDE));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_START",         mrb_fixnum_value(SDL_CONTROLLER_BUTTON_START));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_LEFTSTICK",     mrb_fixnum_value(SDL_CONTROLLER_BUTTON_LEFTSTICK));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_RIGHTSTICK",    mrb_fixnum_value(SDL_CONTROLLER_BUTTON_RIGHTSTICK));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_LEFTSHOULDER",  mrb_fixnum_value(SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_RIGHTSHOULDER", mrb_fixnum_value(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_DPAD_UP",       mrb_fixnum_value(SDL_CONTROLLER_BUTTON_DPAD_UP));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_DPAD_DOWN",     mrb_fixnum_value(SDL_CONTROLLER_BUTTON_DPAD_DOWN));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_DPAD_LEFT",     mrb_fixnum_value(SDL_CONTROLLER_BUTTON_DPAD_LEFT));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_DPAD_RIGHT",    mrb_fixnum_value(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
  mrb_define_const(mrb, mod_GameController, "SDL_CONTROLLER_BUTTON_MAX",           mrb_fixnum_value(SDL_CONTROLLER_BUTTON_MAX));
  mrb_gc_arena_restore(mrb, arena_size);
}

void
mruby_sdl2_gamecontroller_final(mrb_state *mrb)
{

}
