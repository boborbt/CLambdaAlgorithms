#pragma once

// This header file gather into one place the definitions needed to support
// conditional compilation based on the OS capabilities. In line of principle
// the creation of this file should be automatized using a tool like autoconf.
// Presently, the easier (but less general) approach is taken and this file
// is being maintained by hand and the user needs to comment/decomment the
// relevant variables in Makefile.vars

#ifdef LINUX
  #define OS_HAVE_MALLOC_H
#endif

#ifdef MACOS
  #define OS_HAVE_MALLOC_SIZE
#endif
