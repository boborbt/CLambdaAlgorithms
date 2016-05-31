#pragma once

#ifndef __unused
  #define UNUSED(a) a __attribute__((unused))
#else
  #define UNUSED(a) a __unused
#endif
