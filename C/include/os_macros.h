#pragma once

#ifdef LINUX
  #define OS_HAVE_MALLOC_H
#endif

#ifdef MACOS
  #define OS_HAVE_MALLOC_SIZE
#endif
