#include "sdl2_cpuinfo.h"
#include "mruby/value.h"


static mrb_value
mrb_sdl2_cpuinfo_get_cpu_count(mrb_state *mrb, mrb_value self)
{
  int result = SDL_GetCPUCount();
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_cpuinfo_cache_line_size(mrb_state *mrb, mrb_value self)
{
  int result = SDL_GetCPUCacheLineSize();
  return mrb_fixnum_value(result);
}

static mrb_value
mrb_sdl2_cpuinfo_has_rdtsc(mrb_state *mrb, mrb_value self)
{
  return (SDL_HasRDTSC() == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_cpuinfo_has_altivec(mrb_state *mrb, mrb_value self)
{
  return (SDL_HasAltiVec() == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_cpuinfo_has_mmx(mrb_state *mrb, mrb_value self)
{
  return (SDL_HasMMX() == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_cpuinfo_has_3dnow(mrb_state *mrb, mrb_value self)
{
  return (SDL_Has3DNow() == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_cpuinfo_has_sse(mrb_state *mrb, mrb_value self)
{
  return (SDL_HasSSE() == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_cpuinfo_has_sse2(mrb_state *mrb, mrb_value self)
{
  return (SDL_HasSSE2() == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_cpuinfo_has_sse3(mrb_state *mrb, mrb_value self)
{
  return (SDL_HasSSE3() == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_cpuinfo_has_sse41(mrb_state *mrb, mrb_value self)
{
  return (SDL_HasSSE41() == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_cpuinfo_has_sse42(mrb_state *mrb, mrb_value self)
{
  return (SDL_HasSSE42() == SDL_FALSE) ? mrb_false_value() : mrb_true_value();
}

static mrb_value
mrb_sdl2_cpuinfo_get_ram(mrb_state *mrb, mrb_value self)
{
  int result = SDL_GetSystemRAM();
  return mrb_fixnum_value(result);
}

void mruby_sdl2_cpuinfo_init(mrb_state *mrb)
{

  struct RClass * mod_CPUInfo = mrb_define_module_under(mrb, mod_SDL2, "CPUInfo");
  mrb_define_module_function(mrb, mod_CPUInfo, "get_cpu_count",   mrb_sdl2_cpuinfo_get_cpu_count,    MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "cache_line_size", mrb_sdl2_cpuinfo_cache_line_size,  MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "has_RDTSC?",      mrb_sdl2_cpuinfo_has_rdtsc,        MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "has_mmx?",        mrb_sdl2_cpuinfo_has_mmx,          MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "has_AltiVec?",    mrb_sdl2_cpuinfo_has_altivec,      MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "has_3DNow?",      mrb_sdl2_cpuinfo_has_3dnow,        MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "has_SSE?",        mrb_sdl2_cpuinfo_has_sse,          MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "has_SSE2?",       mrb_sdl2_cpuinfo_has_sse2,         MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "has_SSE3?",       mrb_sdl2_cpuinfo_has_sse3,         MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "has_SSE41?",      mrb_sdl2_cpuinfo_has_sse41,        MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "has_SSE42?",      mrb_sdl2_cpuinfo_has_sse42,        MRB_ARGS_NONE());
  mrb_define_module_function(mrb, mod_CPUInfo, "get_ram",         mrb_sdl2_cpuinfo_get_ram,          MRB_ARGS_NONE());
}


extern void mruby_sdl2_cpuinfo_final(mrb_state *mrb)
{

}
