#include "sdl2_haptic.h"
#include "sdl2_mouse.h"
#include "sdl2_joystick.h"
#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/string.h"


static struct RClass *mod_Haptic = NULL;
static struct RClass *class_Haptic    = NULL;


typedef struct mrb_sdl2_haptic_haptic_data_t {
  SDL_Haptic * haptic;
} mrb_sdl2_haptic_haptic_data_t;

static void
mrb_sdl2_haptic_haptic_data_free(mrb_state *mrb, void *p)
{
  mrb_sdl2_haptic_haptic_data_t *data =
    (mrb_sdl2_haptic_haptic_data_t*)p;
  if (NULL != data) {
    if (NULL != data->haptic) {
      mrb_free(mrb, data->haptic);
    }
    mrb_free(mrb, p);
  }
}


struct mrb_data_type const mrb_sdl2_haptic_haptic_data_type = {
  "Haptic", mrb_sdl2_haptic_haptic_data_free
};

SDL_Haptic *
mrb_sdl2_haptic_get_ptr(mrb_state *mrb, mrb_value value)
{
  mrb_sdl2_haptic_haptic_data_t *data;
  if (mrb_nil_p(value)) {
    return NULL;
  }

  data =
    (mrb_sdl2_haptic_haptic_data_t*)mrb_data_get_ptr(mrb, value, &mrb_sdl2_haptic_haptic_data_type);
  return data->haptic;
}

mrb_value
mrb_sdl2_haptic(mrb_state *mrb, SDL_Haptic *value)
{
  mrb_sdl2_haptic_haptic_data_t *data =
    (mrb_sdl2_haptic_haptic_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_haptic_haptic_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  data->haptic = value;
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Haptic, &mrb_sdl2_haptic_haptic_data_type, data));
}

static mrb_value
mrb_sdl2_haptic_num(mrb_state *mrb, mrb_value mod)
{
  int result = SDL_NumHaptics();
  return mrb_fixnum_value(result);
}




static mrb_value
mrb_sdl2_haptic_haptic_initialize(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic *haptic = NULL;
  mrb_sdl2_haptic_haptic_data_t *data =
    (mrb_sdl2_haptic_haptic_data_t*)DATA_PTR(self);

  if (NULL == data) {
    data = (mrb_sdl2_haptic_haptic_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_haptic_haptic_data_t));
    if (NULL == data) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
    }
    data->haptic = NULL;
  }

  if (1 == mrb->c->ci->argc) {
    mrb_int device_index;
    mrb_get_args(mrb, "i", &device_index);
    haptic = SDL_HapticOpen((int) device_index);
  } else {
    mrb_free(mrb, data);
    mrb_raise(mrb, E_ARGUMENT_ERROR, "wrong number of arguments.");
  }
  if (NULL == haptic) {
    mrb_free(mrb, data);
    mruby_sdl2_raise_error(mrb);
  }

  data->haptic = haptic;

  DATA_PTR(self) = data;
  DATA_TYPE(self) = &mrb_sdl2_haptic_haptic_data_type;
  return self;
}

static mrb_value
mrb_sdl2_haptic_haptic_free(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_haptic_haptic_data_t *data =
    (mrb_sdl2_haptic_haptic_data_t*)mrb_data_get_ptr(mrb, self, &mrb_sdl2_haptic_haptic_data_type);
  if (NULL != data->haptic) {
    SDL_HapticClose(data->haptic);
    data->haptic = NULL;
  }
  return self;
}


static mrb_value
mrb_sdl2_haptic_name(mrb_state *mrb, mrb_value self)
{

  mrb_int device_index;
  const char * result;
  mrb_get_args(mrb, "i", &device_index);
  result = SDL_HapticName(device_index);
  return mrb_str_new_cstr(mrb, result);
}

static mrb_value
mrb_sdl2_haptic_devic_open(mrb_state *mrb, mrb_value self)
{
  mrb_int device_index;
  int result;
  mrb_get_args(mrb, "i", &device_index);
  result = SDL_HapticOpened(device_index);
  return (result == 0) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_get_index(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  int result = SDL_HapticIndex(haptic_p);
  if (0 > result) {
      mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(result);

}

static mrb_value
mrb_sdl2_haptic_mouse_haptic(mrb_state *mrb, mrb_value self)
{
  return (SDL_MouseIsHaptic() == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_mouse_open(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_haptic_haptic_data_t *data;
  SDL_Haptic * haptic_p = SDL_HapticOpenFromMouse();
  if (haptic_p == NULL) {
      mruby_sdl2_raise_error(mrb);
  }

  data =
    (mrb_sdl2_haptic_haptic_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_haptic_haptic_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Haptic, &mrb_sdl2_haptic_haptic_data_type, data));
}

static mrb_value
mrb_sdl2_haptic_joystick_haptic(mrb_state *mrb, mrb_value self)
{
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  return (SDL_JoystickIsHaptic(joystick_p) == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_joystick_open(mrb_state *mrb, mrb_value self)
{
  mrb_sdl2_haptic_haptic_data_t *data;
  SDL_Joystick * joystick_p = mrb_sdl2_joystick_joystick_get_ptr(mrb, self);
  SDL_Haptic * haptic_p = SDL_HapticOpenFromJoystick(joystick_p);
  if (haptic_p == NULL) {
      mruby_sdl2_raise_error(mrb);
  }

  data =
    (mrb_sdl2_haptic_haptic_data_t*)mrb_malloc(mrb, sizeof(mrb_sdl2_haptic_haptic_data_t));
  if (NULL == data) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "insufficient memory.");
  }
  return mrb_obj_value(Data_Wrap_Struct(mrb, class_Haptic, &mrb_sdl2_haptic_haptic_data_type, data));
}


static mrb_value
mrb_sdl2_haptic_haptic_num_effects(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  int result = SDL_HapticNumEffects(haptic_p);
  if (0 > result) {
      mruby_sdl2_raise_error(mrb);
  }
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_haptic_haptic_num_effects_playing(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  int result = SDL_HapticNumEffectsPlaying(haptic_p);
  if (0 > result) {
      mruby_sdl2_raise_error(mrb);
  }

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_haptic_haptic_query(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  unsigned int result = SDL_HapticQuery(haptic_p);

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_haptic_haptic_num_axes(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  int result = SDL_HapticNumAxes(haptic_p);

  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_haptic_haptic_run_effect(mrb_state *mrb, mrb_value self)
{
  mrb_int effect, iterations;
  int result;
  SDL_Haptic * haptic_p;
  mrb_get_args(mrb, "ii", &effect, &iterations);
  haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  result = SDL_HapticRunEffect(haptic_p, effect, (Uint32) iterations);
  if (-1 == result) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_stop_effect(mrb_state *mrb, mrb_value self)
{
  mrb_int effect;
  SDL_Haptic * haptic_p;
  int result;
  mrb_get_args(mrb, "i", &effect);
  haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  result = SDL_HapticStopEffect(haptic_p, effect);
  if (-1 == result) {
    mruby_sdl2_raise_error(mrb);
  }
  return mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_destroy_effect(mrb_state *mrb, mrb_value self)
{
  mrb_int effect;
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  mrb_get_args(mrb, "i", &effect);
  SDL_HapticDestroyEffect(haptic_p, effect);

  return mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_effect_playing(mrb_state *mrb, mrb_value self)
{
  mrb_int effect;
  SDL_Haptic * haptic_p;
  int result;
  mrb_get_args(mrb, "i", &effect);
  haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  result = SDL_HapticGetEffectStatus(haptic_p, effect);
  if (-1 == result) {
    mruby_sdl2_raise_error(mrb);
  }
  return (result == 0) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_set_gain(mrb_state *mrb, mrb_value self)
{
  mrb_int gain;
  SDL_Haptic * haptic_p;
  int result;
  mrb_get_args(mrb, "i", &gain);
  if (gain < 0 || gain > 100) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "gain must be between 0 and 100.");
    return self;
  }
  haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  result = SDL_HapticSetGain(haptic_p, gain);
  if (-1 == result) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_set_autocenter(mrb_state *mrb, mrb_value self)
{
  mrb_int autocenter;
  SDL_Haptic * haptic_p;
  int result;
  mrb_get_args(mrb, "i", &autocenter);
  if (autocenter < 0 || autocenter > 100) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "autocenter must be between 0 and 100.");
    return self;
  }
  haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  result = SDL_HapticSetAutocenter(haptic_p, autocenter);
  if (-1 == result) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_pause(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  int result = SDL_HapticPause(haptic_p);
  if (-1 == result) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_unpause(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  int result = SDL_HapticUnpause(haptic_p);
  if (-1 == result) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_stop_all(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  int result = SDL_HapticStopAll(haptic_p);
  if (-1 == result) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_rumble_support(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  int result = SDL_HapticRumbleSupported(haptic_p);

  return (result == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_rumble_init(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  int result = SDL_HapticRumbleInit(haptic_p);
  if (result == -1) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_rumble_play(mrb_state *mrb, mrb_value self)
{
  mrb_float strength;
  mrb_int length;
  SDL_Haptic * haptic_p;
  int result;
  mrb_get_args(mrb, "fi", &strength, &length);

  haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  result = SDL_HapticRumblePlay(haptic_p, strength, (Uint32) length);
  if (result == -1) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_true_value();
}

static mrb_value
mrb_sdl2_haptic_haptic_rumble_stop(mrb_state *mrb, mrb_value self)
{
  SDL_Haptic * haptic_p = mrb_sdl2_haptic_get_ptr(mrb, self);
  int result = SDL_HapticRumbleStop(haptic_p);
  if (result == -1) {
    mruby_sdl2_raise_error(mrb);
  }

  return mrb_true_value();
}

void
mruby_sdl2_haptic_init(mrb_state *mrb)
{
  mod_Haptic = mrb_define_module_under(mrb, mod_SDL2, "Haptics");
  class_Haptic = mrb_define_class_under(mrb, mod_Haptic, "Haptic", mrb->object_class);

  mrb_define_module_function(mrb, mod_Haptic, "num",             mrb_sdl2_haptic_num,             ARGS_NONE());
  mrb_define_module_function(mrb, mod_Haptic, "get_name",             mrb_sdl2_haptic_name,             ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Haptic, "device_open?", mrb_sdl2_haptic_devic_open, ARGS_REQ(1));
  mrb_define_module_function(mrb, mod_Mouse, "is_haptic?", mrb_sdl2_haptic_mouse_haptic, ARGS_NONE());
  mrb_define_module_function(mrb, mod_Mouse, "open_haptic", mrb_sdl2_haptic_mouse_open, ARGS_NONE());

  mrb_define_method(mrb, class_Joystick, "is_haptic?", mrb_sdl2_haptic_joystick_haptic, ARGS_NONE());
  mrb_define_method(mrb, class_Joystick, "open_haptic", mrb_sdl2_haptic_joystick_open, ARGS_NONE());
  //mrb_define_module_function(mrb, mod_Haptic, "open_Mouse")

  mrb_define_method(mrb, class_Haptic, "initialize",  mrb_sdl2_haptic_haptic_initialize,  ARGS_REQ(1));
  mrb_define_method(mrb, class_Haptic, "destroy", mrb_sdl2_haptic_haptic_free, ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "free", mrb_sdl2_haptic_haptic_free, ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "get_index",  mrb_sdl2_haptic_haptic_get_index,  ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "num_effects",  mrb_sdl2_haptic_haptic_num_effects,  ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "num_effects_playing",  mrb_sdl2_haptic_haptic_num_effects_playing,  ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "query",  mrb_sdl2_haptic_haptic_query,  ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "num_axes",  mrb_sdl2_haptic_haptic_num_axes,  ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "run_effect",  mrb_sdl2_haptic_haptic_run_effect,  ARGS_REQ(2));
  mrb_define_method(mrb, class_Haptic, "stop_effect",  mrb_sdl2_haptic_haptic_stop_effect,  ARGS_REQ(1));
  mrb_define_method(mrb, class_Haptic, "destroy_effect",  mrb_sdl2_haptic_haptic_destroy_effect,  ARGS_REQ(1));
  mrb_define_method(mrb, class_Haptic, "is_effect_playing?",  mrb_sdl2_haptic_haptic_effect_playing,  ARGS_REQ(1));
  mrb_define_method(mrb, class_Haptic, "set_gain",  mrb_sdl2_haptic_haptic_set_gain,  ARGS_REQ(1));
  mrb_define_method(mrb, class_Haptic, "set_autocenter",  mrb_sdl2_haptic_haptic_set_autocenter,  ARGS_REQ(1));
  mrb_define_method(mrb, class_Haptic, "pause",  mrb_sdl2_haptic_haptic_pause,  ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "unpause",  mrb_sdl2_haptic_haptic_unpause,  ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "stop_all",  mrb_sdl2_haptic_haptic_stop_all,  ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "is_rumble_supported?",  mrb_sdl2_haptic_haptic_rumble_support,  ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "init_rumble",  mrb_sdl2_haptic_haptic_rumble_init,  ARGS_NONE());
  mrb_define_method(mrb, class_Haptic, "play_rumble",  mrb_sdl2_haptic_haptic_rumble_play,  ARGS_REQ(2));
  mrb_define_method(mrb, class_Haptic, "stop_rumble",  mrb_sdl2_haptic_haptic_rumble_stop,  ARGS_NONE());


  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_CONSTANT",  mrb_fixnum_value(SDL_HAPTIC_CONSTANT));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_SINE",  mrb_fixnum_value(SDL_HAPTIC_SINE));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_LEFTRIGHT",  mrb_fixnum_value(SDL_HAPTIC_LEFTRIGHT));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_TRIANGLE",  mrb_fixnum_value(SDL_HAPTIC_TRIANGLE));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_SAWTOOTHUP",  mrb_fixnum_value(SDL_HAPTIC_SAWTOOTHUP));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_SAWTOOTHDOWN",  mrb_fixnum_value(SDL_HAPTIC_SAWTOOTHDOWN));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_RAMP",  mrb_fixnum_value(SDL_HAPTIC_RAMP));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_SPRING",  mrb_fixnum_value(SDL_HAPTIC_SPRING));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_DAMPER",  mrb_fixnum_value(SDL_HAPTIC_DAMPER));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_INERTIA",  mrb_fixnum_value(SDL_HAPTIC_INERTIA));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_FRICTION",  mrb_fixnum_value(SDL_HAPTIC_FRICTION));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_CUSTOM",  mrb_fixnum_value(SDL_HAPTIC_CUSTOM));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_GAIN",  mrb_fixnum_value(SDL_HAPTIC_GAIN));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_AUTOCENTER",  mrb_fixnum_value(SDL_HAPTIC_AUTOCENTER));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_STATUS",  mrb_fixnum_value(SDL_HAPTIC_STATUS));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_PAUSE",  mrb_fixnum_value(SDL_HAPTIC_PAUSE));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_POLAR",  mrb_fixnum_value(SDL_HAPTIC_POLAR));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_CARTESIAN",  mrb_fixnum_value(SDL_HAPTIC_CARTESIAN));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_SPHERICAL",  mrb_fixnum_value(SDL_HAPTIC_SPHERICAL));
  mrb_define_const(mrb, mod_Haptic, "SDL_HAPTIC_INFINITY",  mrb_fixnum_value(SDL_HAPTIC_INFINITY));
}

void
mruby_sdl2_haptic_final(mrb_state *mrb)
{
}
