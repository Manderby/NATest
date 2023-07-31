
// This is the base include file of NATest. Do not include this file directly!

#include <stdlib.h>
#include <inttypes.h>

#if NA_OS == NA_OS_WINDOWS
  #define NATEST_LINKER_NO_EXPORT
  #define NATEST_LINKER_EXPORT      __declspec(dllexport)
#else
  #define NATEST_LINKER_NO_EXPORT   __attribute__ ((visibility("hidden")))
  #define NATEST_LINKER_EXPORT      __attribute__ ((visibility("default")))
#endif

#define NATEST_API    NATEST_LINKER_NO_EXPORT
#define NATEST_DEF    NATEST_LINKER_NO_EXPORT
#define NATEST_HAPI   NATEST_LINKER_NO_EXPORT
#define NATEST_HDEF   NATEST_LINKER_NO_EXPORT
typedef int      NATestBool;
typedef char     NATestUTF8Char;
typedef int32_t  int32;
typedef uint32_t uint32;
