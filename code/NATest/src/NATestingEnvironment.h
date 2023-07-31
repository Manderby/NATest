
#ifndef NA_TESTING_ENVIRONMENT_INCLUDED
#define NA_TESTING_ENVIRONMENT_INCLUDED

// This is the base include file of NATest. Do not include this file directly!

#include <stdlib.h>
#include <inttypes.h>

#if defined _WIN32
  #define NATEST_INLINE                 _inline
  #define NATEST_LINKER_NO_EXPORT
  #define NATEST_LINKER_EXPORT      __declspec(dllexport)
#else
  #define NATEST_INLINE                 inline
  #define NATEST_LINKER_NO_EXPORT   __attribute__ ((visibility("hidden")))
  #define NATEST_LINKER_EXPORT      __attribute__ ((visibility("default")))
#endif



#define NATEST_API    NATEST_LINKER_NO_EXPORT
#define NATEST_DEF    NATEST_LINKER_NO_EXPORT
#define NATEST_HAPI   NATEST_LINKER_NO_EXPORT
#define NATEST_HDEF   NATEST_LINKER_NO_EXPORT
#define NATEST_HIAPI  static NATEST_INLINE
#define NATEST_HIDEF  static NATEST_INLINE
#define NATEST_NULL   NULL
#define NATEST_FALSE  0
#define NATEST_TRUE   1
typedef void     (*NATestMutator)(void*);
typedef int      NATestBool;
typedef char     NATestUTF8Char;
typedef int32_t  int32;
typedef uint32_t uint32;

#endif // NA_TESTING_ENVIRONMENT_INCLUDED
