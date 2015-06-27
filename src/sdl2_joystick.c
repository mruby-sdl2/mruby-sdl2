#include "sdl2_joystick.h"
#include "mruby/value.h"
#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"


struct RClass *class_Joystick = NULL;

typedef struct mrb_sdl2_joystick_joystick_data_t {
  bool        is_associated;
  SDL_Joystick *joystick;
} mrb_sdl2_joystick_joystick_data_t;

static void
mrb_sdl2_joystick_joystick_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_joystick_joystick_data_t *data =
    (mrb_sdl2_joystick_joystick_data_t*)p;
  if (NULL != data) {
    if ((!data->is_associated) && (NULL != data->joystick)) {
      SDL_JoystickClose(data->joystick);
    }
    mrb_free(mrb, data);
  }
}

static struct mrb_data_type const mrb_sdl2_joystick_joystick_data_type = {
  "Joystick", &mrb_sdl2_joystick_joystick_data_free
};

SDL_Joystick *
mrb_sdl2_joystick_joystick_get_ptr(mrb_state *mrb, mrb_value joystick)
{
  mrb_sdl2_joystick_joystick_data_t *data;
  if (mrb_nil_p(joystick)) {
    return NULL;
  }

  data =
    (mrb_sdl2_joystick_joystick_data_t*)mrb_data_get_ptr(mrb, joystick, &mrb_sdl2_joystick_joystick_data_type);
  return data->joystick;
}

mrb_value
mrb_sdl2_joystick_joystick(mrb_state *mrb, SDL_Joystick *joystick)
{
  mrb_sdl2_joystick_joystick_data_t *data;
  if (NULL == joystick) {
    return mrb_nil_value();
  }

  data =
    (mrb_sdl2_joystick_joystick_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_joystick_joystick_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->is_associated = false;
  data->joystick = joystick;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Joystick, &mrb_sdl2_joystick_joystick_data_type, data));
}

mrb_value
mrb_sdl2_joystick_associated_joystick(mrb_state *mrb, SDL_Joystick *joystick)
{
  mrb_sdl2_joystick_joystick_data_t *data;
  if (NULL == joystick) {
    return mrb_nil_value();
  }

  data =
    (mrb_sdl2_joystick_joystick_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_joystick_joystick_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->is_associated = true;
  data->joystick = joystick;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Joystick, &mrb_sdl2_joystick_joystick_data_type, data));
}



/***************************************************************************
*
* module SDL2::Joystick::Joystick
*
***************************************************************************/


static mrb_value
mrb_sdl2_joystick_joystick_initialize(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick *joystick = NULL;
  mrb_sdl2_joystick_joystick_data_t *data =
    (mrb_sdl2_joystick_joystick_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_joystick_joystick_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_joystick_joystick_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->is_associated = false;
    data->joystick = NULL;
  }

  if (1 == mrb->c->ci->argc) {
    mrb_int device_index;
    mrb_get_args(mrb, "i", &device_index);
    joystick = SDL_JoystickOpen((int) device_index);
  } else {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == joystick) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }

  data->joystick = joystick;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_joystick_joystick_data_type;
  return self;
}

static mrb_value
mrb_sdl2_joystick_joystick_free(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_joystick_joystick_data_t *data =
    (mrb_sdl2_joystick_joystick_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_joystick_joystick_data_type);
  if ((!data->is_associated) && (NULL != data->joystick)) {
    SDL_JoystickClose(data->joystick);
    data->joystick = NULL;
  }
  return self;
}


static mrb_value
mrb_sdl2_joystick_num(mrb_state *mrb, mrb_value self)
{
  int result = SDL_NumJoysticks();
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_joystick_get_name(mrb_state *mrb, mrb_value self)
{
  const char * result;
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  result = SDL_JoystickNameForIndex((int) index);

  return mrb_str_new_cstr(mrb, result);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_name(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  const char * result = SDL_JoystickName(joystick_p);
  return mrb_str_new_cstr(mrb, result);
}

static mrb_value
mrb_sdl2_joystick_get_guid(mrb_state *mrb, mrb_value self)
{
  SDL_JoystickGUID result;
  mrb_int index;
  mrb_get_args(mrb, "i", &index);
  result = SDL_JoystickGetDeviceGUID(index);
  return mrb_fixnum_value(*result.data);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_guid(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  SDL_JoystickGUID result = SDL_JoystickGetGUID(joystick_p);
  return mrb_fixnum_value(*result.data);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_guid_as_string(mrb_state *mrb, mrb_value self)
{
  char result[33];
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  SDL_JoystickGUID joystick_guid = SDL_JoystickGetGUID(joystick_p);
  SDL_JoystickGetGUIDString(joystick_guid, result, 33);

  return mrb_str_new_cstr(mrb, result);
}

static mrb_value
mrb_sdl2_joystick_get_guid_as_string(mrb_state *mrb, mrb_value self)
{
  // TBD
  // get the Uint8 data[16] that was send to with get_guid_from_index
  // use it to call SDL_JoystickGetGUIDString
  // return the string
  return mrb_false_value();
}

static mrb_value
mrb_sdl2_joystick_get_guid_from_string(mrb_state *mrb, mrb_value self)
{
  mrb_value name;
  SDL_JoystickGUID result;
  mrb_get_args(mrb, "S", &name);
  result = SDL_JoystickGetGUIDFromString(RSTRING_PTR(name));

  return mrb_fixnum_value(*result.data);
}

static mrb_value
mrb_sdl2_joystick_joystick_is_attached(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  SDL_bool result = SDL_JoystickGetAttached(joystick_p);

  return (result == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_joystick_joystick_get_id(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  SDL_JoystickID result = SDL_JoystickInstanceID(joystick_p);
  if (result < 1) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_num_axes(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  int result = SDL_JoystickNumAxes(joystick_p);
  if (result < 1) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_num_balls(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  int result = SDL_JoystickNumBalls(joystick_p);
  if (result < 1) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_num_hats(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  int result = SDL_JoystickNumHats(joystick_p);
  if (result < 1) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_num_buttons(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  int result = SDL_JoystickNumBalls(joystick_p);
  if (result < 1) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_joystick_joystick_update(mrb_state *mrb, mrb_value self)
{
  SDL_JoystickUpdate();
  return self;
}

static mrb_value
mrb_sdl2_joystick_joystick_event_state(mrb_state *mrb, mrb_value self)
{
  int result;
  mrb_int state;
  mrb_get_args(mrb, "i", &state);
  result = SDL_JoystickEventState((int) state);

  if (0 > result) {
    mruby_sdl2_raise_error(mrb);
  }
  return (0 == result) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_joystick_joystick_get_axis(mrb_state *mrb, mrb_value self)
{
  Sint16 result;
  mrb_int axis;
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  mrb_get_args(mrb, "i", &axis);
  result = SDL_JoystickGetAxis(joystick_p, (int) axis);
  if (result == 0) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_axis_x(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  Sint16 result = SDL_JoystickGetAxis(joystick_p, 0);
  if (result == 0) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_axis_y(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  Sint16 result = SDL_JoystickGetAxis(joystick_p, 1);
  if (result == 0) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_hat(mrb_state *mrb, mrb_value self)
{
  Uint8 result;
  mrb_int hat;
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  mrb_get_args(mrb, "i", &hat);
  result = SDL_JoystickGetHat(joystick_p, hat);

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_joystick_joystick_get_ball(mrb_state *mrb, mrb_value self)
{
  mrb_value array;
  int dx;
  int dy;
  int result;
  mrb_int ball;
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  mrb_get_args(mrb, "i", &ball);
  result = SDL_JoystickGetBall(joystick_p, ball, &dx, &dy);
  if (0 > result) {
    mruby_sdl2_raise_error(mrb);
  }
  array = mrb_ary_new_capa(mrb, 2);
  mrb_ary_push(mrb, array, mrb_fixnum_value(dx));
  mrb_ary_push(mrb, array, mrb_fixnum_value(dy));
  return array;
}

static mrb_value
mrb_sdl2_joystick_joystick_get_button(mrb_state *mrb, mrb_value self)
{
  Uint8 result;
  mrb_int button;
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  mrb_get_args(mrb, "i", &button);
  result = SDL_JoystickGetButton(joystick_p, button);

  return (0 == result) ? mrb_false_value() : mrb_true_value();
}


void
mruby_sdl2_joystick_init(mrb_state *mrb)
{
  int arena_size;
  struct RClass *mod_Joystick = mrb_define_module_under(mrb, mod_SDL2, "Joysticks");
  class_Joystick = mrb_define_class_under(mrb, mod_Joystick, "Joystick", mrb->object_class);

  MRB_SET_INSTANCE_TT(class_Joystick, MRB_TT_DATA);

  mrb_define_module_function(mrb, mod_Joystick, "num",      mrb_sdl2_joystick_num, ARGS_NONE());
  mrb_define_module_function(mrb, mod_Joystick, "name_from_index", mrb_sdl2_joystick_get_name, ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Joystick, "get_guid_from_index",       mrb_sdl2_joystick_get_guid, ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Joystick, "get_guid_as_string",        mrb_sdl2_joystick_get_guid_as_string, ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Joystick, "get_guid_from_string",        mrb_sdl2_joystick_get_guid_from_string, ARGS_REQ(1));

  mrb_define_method(mrb, class_Joystick, "initialize",  mrb_sdl2_joystick_joystick_initialize,  ARGS_REQ(1));
  mrb_define_method(mrb, class_Joystick, "destroy",     mrb_sdl2_joystick_joystick_free,        ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "free",        mrb_sdl2_joystick_joystick_free,        ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "get_name",        mrb_sdl2_joystick_joystick_get_name, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "get_guid",        mrb_sdl2_joystick_joystick_get_guid, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "get_guid_as_string",        mrb_sdl2_joystick_joystick_get_guid_as_string, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "is_attached?",        mrb_sdl2_joystick_joystick_is_attached, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "get_id",         mrb_sdl2_joystick_joystick_get_id, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "get_general_num_axes",         mrb_sdl2_joystick_joystick_get_num_axes, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "get_general_num_balls",         mrb_sdl2_joystick_joystick_get_num_balls, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "get_general_num_hats",         mrb_sdl2_joystick_joystick_get_num_hats, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "get_general_num_buttons",         mrb_sdl2_joystick_joystick_get_num_buttons, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "update",         mrb_sdl2_joystick_joystick_update, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "event_state",         mrb_sdl2_joystick_joystick_event_state, ARGS_REQ(1));
  mrb_define_method(mrb, class_Joystick, "get_axis",         mrb_sdl2_joystick_joystick_get_axis, ARGS_REQ(1));
  mrb_define_method(mrb, class_Joystick, "get_axis_x", mrb_sdl2_joystick_joystick_get_axis_x, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "get_axis_y", mrb_sdl2_joystick_joystick_get_axis_y, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "get_hat",         mrb_sdl2_joystick_joystick_get_hat, ARGS_REQ(1));
  mrb_define_method(mrb, class_Joystick, "get_ball",         mrb_sdl2_joystick_joystick_get_ball, ARGS_REQ(1));
  mrb_define_method(mrb, class_Joystick, "get_button",         mrb_sdl2_joystick_joystick_get_button, ARGS_REQ(1));


  arena_size = mrb_gc_arena_save(mrb);
  mrb_define_const(mrb, mod_Joystick, "SDL_HAT_CENTERED",  mrb_fixnum_value(SDL_HAT_CENTERED));
  mrb_define_const(mrb, mod_Joystick, "SDL_HAT_UP",        mrb_fixnum_value(SDL_HAT_UP));
  mrb_define_const(mrb, mod_Joystick, "SDL_HAT_RIGHT",     mrb_fixnum_value(SDL_HAT_RIGHT));
  mrb_define_const(mrb, mod_Joystick, "SDL_HAT_DOWN",      mrb_fixnum_value(SDL_HAT_DOWN));
  mrb_define_const(mrb, mod_Joystick, "SDL_HAT_LEFT",      mrb_fixnum_value(SDL_HAT_LEFT));
  mrb_define_const(mrb, mod_Joystick, "SDL_HAT_RIGHTUP",   mrb_fixnum_value(SDL_HAT_RIGHTUP));
  mrb_define_const(mrb, mod_Joystick, "SDL_HAT_RIGHTDOWN", mrb_fixnum_value(SDL_HAT_RIGHTDOWN));
  mrb_define_const(mrb, mod_Joystick, "SDL_HAT_LEFTUP",    mrb_fixnum_value(SDL_HAT_LEFTUP));
  mrb_define_const(mrb, mod_Joystick, "SDL_HAT_LEFTDOWN",  mrb_fixnum_value(SDL_HAT_LEFTDOWN));
  mrb_gc_arena_restore(mrb, arena_size);

}


void
mruby_sdl2_joystick_final(mrb_state *mrb)
{
}
