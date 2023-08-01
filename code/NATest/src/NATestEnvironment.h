
#ifndef NA_TEST_ENVIRONMENT_INCLUDED
#define NA_TEST_ENVIRONMENT_INCLUDED

// Do not include this file directly!
// This is the base include file of NATest which covers all needed datatypes
// definitions and such.



#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#ifndef __cplusplus
  // This is a C implementation
  #if defined __STDC__
    #if defined __STDC_VERSION__
      #if __STDC_VERSION__ >= 201112L
        #define NATEST_C11
      #endif
    #endif
  #elif defined _MSC_VER
    #if _MSC_VER >= 1910  // VS2017 does C11, but not completely.
      #define NATEST_C11
    #endif
  #endif
#else
  // This is a C++ implementation
  #if __cplusplus >= 201103L
    #define NATEST_CPP11
  #endif
#endif



// The newlines used on different systems as utf8 strings.
#if defined NATEST_C11 || defined NATEST_CPP11
  #define NATEST_NL_UNIX  u8"\n"
  #define NATEST_NL_WIN   u8"\r\n"
#else
  #define NATEST_NL_UNIX  "\n"
  #define NATEST_NL_WIN   "\r\n"
#endif



#if defined _WIN32
  #define NATEST_INLINE             _inline
  #define NATEST_LINKER_NO_EXPORT
  #define NATEST_LINKER_EXPORT      __declspec(dllexport)
  #define NATEST_NL NATEST_NL_WIN
  typedef HANDLE NATestFile;
  #define NATEST_FILEMODE_DEFAULT (_S_IREAD | _S_IWRITE)
  #define NATEST_FILE_OPEN_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC | O_BINARY)
#else
  #define NATEST_INLINE             inline
  #define NATEST_LINKER_NO_EXPORT   __attribute__ ((visibility("hidden")))
  #define NATEST_LINKER_EXPORT      __attribute__ ((visibility("default")))
  #define NATEST_NL NATEST_NL_UNIX
  typedef int NATestFile;
  #define NATEST_FILEMODE_DEFAULT 0644
  #define NATEST_FILE_OPEN_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC)
#endif



#if (defined NATEST_CPP11) && defined __cplusplus
  #define NATEST_NULL nullptr
#else
  #define NATEST_NULL NULL
#endif



#define NATEST_API     NATEST_LINKER_NO_EXPORT
#define NATEST_DEF     NATEST_LINKER_NO_EXPORT
#define NATEST_IAPI    static NATEST_INLINE
#define NATEST_IDEF    static NATEST_INLINE
#define NATEST_HAPI    NATEST_LINKER_NO_EXPORT
#define NATEST_HDEF    NATEST_LINKER_NO_EXPORT
#define NATEST_HIAPI   static NATEST_INLINE
#define NATEST_HIDEF   static NATEST_INLINE
#define NATEST_FALSE   0
#define NATEST_TRUE    1
#define NATEST_MAX_u8  0xff
#define NATEST_MAX_u16 0xffff


typedef void     (*NATestMutator)(void*);
typedef int      NATestBool;
typedef char     NATestUTF8Char;
typedef int8_t   int8;
typedef uint8_t  uint8;
typedef int16_t  int16;
typedef uint16_t uint16;
typedef int32_t  int32;
typedef uint32_t uint32;
#ifdef UINT64_MAX
  typedef int64_t  int64;
  typedef uint64_t uint64;
#endif
#ifdef __SIZEOF_INT128__
  typedef signed   __int128 int128;
  typedef unsigned __int128 uint128;
#endif
#ifdef __SIZEOF_INT256__
  typedef signed   __int256 int256;
  typedef unsigned __int256 uint256;
#endif



#endif // NA_TEST_ENVIRONMENT_INCLUDED



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
