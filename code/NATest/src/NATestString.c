
#include "NATestString.h"
#include <stdarg.h>



NATEST_HIDEF size_t na_Vsnprintf(
  NATestUTF8Char* buffer,
  size_t length,
  const NATestUTF8Char* newstr,
  va_list argumentList)
{
  #if defined _WIN32
    return (size_t)_vsnprintf_s(buffer, (size_t)length, (size_t)length, newstr, argumentList);
  #else
    return (size_t)vsnprintf((char*)buffer, (size_t)length, (const char*)newstr, argumentList);
  #endif
}



// Returns the number of characters needed to transform the given string and
// arguments using sprintf.
NATEST_HIDEF size_t na_VarargStringLength(
  const NATestUTF8Char* string,
  va_list args)
{
  #if defined _WIN32
    return (size_t)_vscprintf(string, args);
  #else
    return na_Vsnprintf(NATEST_NULL, 0, string, args);
  #endif
}



NATEST_HDEF NATestUTF8Char* na_AllocSprintf(const NATestUTF8Char* format, ...){
  va_list argumentList;
  va_list argumentList2;
  va_start(argumentList, format);
  va_copy(argumentList2, argumentList);

  size_t stringLen = na_VarargStringLength(format, argumentList);

  NATestUTF8Char* stringBuf = malloc(stringLen + 1);
  na_Vsnprintf(stringBuf, stringLen + 1, format, argumentList2);
  stringBuf[stringLen] = '\0';

  return stringBuf;
}



NATEST_DEF NATestUTF8Char* naTestPriux8(uint8 value){
  return na_AllocSprintf("%02x", (int)(value & NATEST_MAX_u8));
}
NATEST_DEF NATestUTF8Char* naTestPriix8(int8 value){
  return na_AllocSprintf("%02x", (int)(value & NATEST_MAX_u8));
}
NATEST_DEF NATestUTF8Char* naTestPriux16(uint16 value){
  return na_AllocSprintf("%04x", (int)(value & NATEST_MAX_u16));
}
NATEST_DEF NATestUTF8Char* naTestPriix16(int16 value){
  return na_AllocSprintf("%04x", (int)(value & NATEST_MAX_u16));
}
NATEST_DEF NATestUTF8Char* naTestPriux32(uint32 value){
  return na_AllocSprintf("%08x", (int)value);
}
NATEST_DEF NATestUTF8Char* naTestPriix32(int32 value){
  return na_AllocSprintf("%08x", (int)value);
}

#ifdef UINT64_MAX
  #define naGeti64Hi(i) ((int32)((i) >> 32))
  #define naGeti64Lo(i) ((uint32)i)
  #define naGetu64Hi(u) ((uint32)((u) >> 32))
  #define naGetu64Lo(u) ((uint32)u)

  NATEST_DEF NATestUTF8Char* naTestPriux64(uint64 value){
    return na_AllocSprintf("%08x%08x",
      naGetu64Hi(value),
      naGetu64Lo(value));
  }
  NATEST_DEF NATestUTF8Char* naTestPriix64(int64 value){
    return na_AllocSprintf("%08x%08x",
      naGeti64Hi(value),
      naGeti64Lo(value));
  }
#endif

#ifdef __SIZEOF_INT128__
  #define naGeti128Hi(i) ((int64)((i) >> 64))
  #define naGeti128Lo(i) ((uint64)i)
  #define naGetu128Hi(u) ((uint64)((u) >> 64))
  #define naGetu128Lo(u) ((uint64)u)

  NATEST_DEF NATestUTF8Char* naTestPriux128(uint128 value){
    return na_AllocSprintf("%08x%08x%08x%08x",
      naGetu64Hi(naGetu128Hi(value)),
      naGetu64Lo(naGetu128Hi(value)),
      naGetu64Hi(naGetu128Lo(value)),
      naGetu64Lo(naGetu128Lo(value)));
  }
  NATEST_DEF NATestUTF8Char* naTestPriix128(int128 value){
    return na_AllocSprintf("%08x%08x%08x%08x",
      naGeti64Hi(naGeti128Hi(value)),
      naGeti64Lo(naGeti128Hi(value)),
      naGetu64Hi(naGeti128Lo(value)),
      naGetu64Lo(naGeti128Lo(value)));
  }
#endif

#ifdef __SIZEOF_INT256__
  #define naGeti128Hi(i) ((int128)((i) >> 128))
  #define naGeti128Lo(i) ((uint128)i)
  #define naGetu128Hi(u) ((uint128)((u) >> 128))
  #define naGetu128Lo(u) ((uint128)u)

  NATEST_DEF NATestUTF8Char* naTestPriux256(uint256 value){
    return na_AllocSprintf("%08x%08x%08x%08x%08x%08x%08x%08x",
      naGetu64Hi(naGetu128Hi(naGetu256Hi(value))),
      naGetu64Lo(naGetu128Hi(naGetu256Hi(value))),
      naGetu64Hi(naGetu128Lo(naGetu256Hi(value))),
      naGetu64Lo(naGetu128Lo(naGetu256Hi(value))),
      naGetu64Hi(naGetu128Hi(naGetu256Lo(value))),
      naGetu64Lo(naGetu128Hi(naGetu256Lo(value))),
      naGetu64Hi(naGetu128Lo(naGetu256Lo(value))),
      naGetu64Lo(naGetu128Lo(naGetu256Lo(value))));
  }
  NATEST_DEF NATestUTF8Char* naTestPriix256(int256 value){
    return na_AllocSprintf("%08x%08x%08x%08x%08x%08x%08x%08x",
      naGeti64Hi(naGeti128Hi(naGeti256Hi(value))),
      naGeti64Lo(naGeti128Hi(naGeti256Hi(value))),
      naGetu64Hi(naGeti128Lo(naGeti256Hi(value))),
      naGetu64Lo(naGeti128Lo(naGeti256Hi(value))),
      naGetu64Hi(naGetu128Hi(naGeti256Lo(value))),
      naGetu64Lo(naGetu128Hi(naGeti256Lo(value))),
      naGetu64Hi(naGetu128Lo(naGeti256Lo(value))),
      naGetu64Lo(naGetu128Lo(naGeti256Lo(value))));
  }
#endif


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
