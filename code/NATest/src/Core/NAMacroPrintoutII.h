
// First, undefine the macros from the .h file.
#undef naPrintMacro
#undef naPrintMacroDefined
#undef naPrintMacroInt
#undef naPrintMacroIntHex
#undef naPrintMacroIntSpecial
#undef naPrintMacroIntSpecialHex
#undef naPrintMacroIntYesNo
#undef naPrintMacroEnum

#undef naPrintMacroux8
#undef naPrintMacroix8
#undef naPrintMacroux16
#undef naPrintMacroix16
#undef naPrintMacroux32
#undef naPrintMacroix32
#undef naPrintMacroux64
#undef naPrintMacroix64
#undef naPrintMacroux128
#undef naPrintMacroix128
#undef naPrintMacroux256
#undef naPrintMacroix256



// The authors proudest achievement at 4 o'clock in the morning.
// Converts any macro, even an empty one, into a string literal.
#define NATEST_STRINGIFY(...) #__VA_ARGS__ ""



// All macros are translated into calls to internal functions (marked by na_).
// Here, the prototypes of these functions are declared. They are implemented
// in the NAMacroPrintout.c file.

NATEST_HAPI void na_PrintMacroPlain(
  const NATestUTF8Char* macroString,
  const NATestUTF8Char* macroStringified,
  NATestBool printContent);
#define naPrintMacro(macro)\
  na_PrintMacroPlain(\
    #macro,\
    NATEST_STRINGIFY(macro),\
    NATEST_TRUE)
#define naPrintMacroDefined(macro)\
  na_PrintMacroPlain(\
    #macro,\
    NATEST_STRINGIFY(macro),\
    NATEST_FALSE)
  
NATEST_HAPI void na_PrintMacroInt(
  const NATestUTF8Char* macroString,
  int value,
  NATestBool hex);
#define naPrintMacroInt(macro)\
  na_PrintMacroInt(\
    #macro,\
    (int)(macro),\
    NATEST_FALSE)
#define naPrintMacroIntHex(macro)\
  na_PrintMacroInt(\
    #macro,\
    (int)(macro),\
    NATEST_TRUE)

NATEST_HAPI void na_PrintMacroIntSpecial(
  const NATestUTF8Char* macroString,
  int value,
  int specialValue,
  const NATestUTF8Char* specialString,
  NATestBool hex);
#define naPrintMacroIntSpecial(macro, specialValue, specialString)\
  na_PrintMacroIntSpecial(\
    #macro,\
    (int)(macro),\
    (int)(specialValue),\
    specialString,\
    NATEST_FALSE)
#define naPrintMacroIntSpecialHex(macro, specialValue, specialString)\
  na_PrintMacroIntSpecial(\
    #macro,\
    (int)(macro),\
    (int)(specialValue),\
    specialString,\
    NATEST_TRUE)
  
NATEST_HAPI void na_PrintMacroIntYesNo(
  const NATestUTF8Char* macroString,
  int value);
#define naPrintMacroIntYesNo(macro)\
  na_PrintMacroIntYesNo(\
    #macro,\
    (int)(macro))

NATEST_HAPI void na_PrintMacroEnum(
  const NATestUTF8Char* macroString,
  int value,
  const NATestUTF8Char** strings,
  int enumCount);
#define naPrintMacroEnum(macro, strings, maxValue)\
  na_PrintMacroEnum(\
    #macro,\
    (int)(macro),\
    strings,\
    (int)(maxValue))

NATEST_HAPI void na_PrintMacroux8(
  const NATestUTF8Char* macroString,
  uint8 value);
NATEST_HAPI void na_PrintMacroix8(
  const NATestUTF8Char* macroString,
  int8 value);
#define naPrintMacroux8(macro)\
  na_PrintMacroux8(\
    #macro,\
    (uint8)(macro))
#define naPrintMacroix8(macro)\
  na_PrintMacroix8(\
    #macro,\
    (int8)(macro))

NATEST_HAPI void na_PrintMacroux16(
  const NATestUTF8Char* macroString,
  uint16 value);
NATEST_HAPI void na_PrintMacroix16(
  const NATestUTF8Char* macroString,
  int16 value);
#define naPrintMacroux16(macro)\
  na_PrintMacroux16(\
    #macro,\
    (uint16)(macro))
#define naPrintMacroix16(macro)\
  na_PrintMacroix16(\
    #macro,\
    (int16)(macro))

NATEST_HAPI void na_PrintMacroux32(
  const NATestUTF8Char* macroString,
  uint32 value);
NATEST_HAPI void na_PrintMacroix32(
  const NATestUTF8Char* macroString,
  int32 value);
#define naPrintMacroux32(macro)\
  na_PrintMacroux32(\
    #macro,\
    (uint32)(macro))
#define naPrintMacroix32(macro)\
  na_PrintMacroix32(\
    #macro,\
    (int32)(macro))

#ifdef NATEST_i64_NATIVE
  NATEST_HAPI void na_PrintMacroux64(
    const NATestUTF8Char* macroString,
    uint64 value);
  NATEST_HAPI void na_PrintMacroix64(
    const NATestUTF8Char* macroString,
    int64 value);
  #define naPrintMacroux64(macro)\
    na_PrintMacroux64(\
      #macro,\
      (uint64)(macro))
  #define naPrintMacroix64(macro)\
    na_PrintMacroix64(\
      #macro,\
      (int64)(macro))
#else
  NATEST_HAPI void na_PrintMacroux64(
    const NATestUTF8Char* macroString);
  NATEST_HAPI void na_PrintMacroix64(
    const NATestUTF8Char* macroString);
  #define naPrintMacroux64(macro)\
    na_PrintMacroux64(#macro)
  #define naPrintMacroix64(macro)\
    na_PrintMacroix64(#macro)
#endif

#ifdef NATEST_i128_NATIVE
  NATEST_HAPI void na_PrintMacroux128(
    const NATestUTF8Char* macroString,
    uint128 value);
  NATEST_HAPI void na_PrintMacroix128(
    const NATestUTF8Char* macroString,
    int128 value);
  #define naPrintMacroux128(macro)\
    na_PrintMacroux128(\
      #macro,\
      (uint128)(macro))
  #define naPrintMacroix128(macro)\
    na_PrintMacroix128(\
      #macro,\
      (int128)(macro))
#else
  NATEST_HAPI void na_PrintMacroux128(
    const NATestUTF8Char* macroString);
  NATEST_HAPI void na_PrintMacroix128(
    const NATestUTF8Char* macroString);
  #define naPrintMacroux128(macro)\
    na_PrintMacroux128(#macro)
  #define naPrintMacroix128(macro)\
    na_PrintMacroix128(#macro)
#endif

#ifdef NATEST_i256_NATIVE
  NATEST_HAPI void na_PrintMacroux256(
    const NATestUTF8Char* macroString,
    uint256 value);
  NATEST_HAPI void na_PrintMacroix256(
    const NATestUTF8Char* macroString,
    int256 value);
  #define naPrintMacroux256(macro)\
    na_PrintMacroux256(\
      #macro,\
      (uint256)(macro))
  #define naPrintMacroix256(macro)\
    na_PrintMacroix256(\
      #macro,\
      (int256)(macro))
#else
  NATEST_HAPI void na_PrintMacroux256(
    const NATestUTF8Char* macroString);
  NATEST_HAPI void na_PrintMacroix256(
    const NATestUTF8Char* macroString);
  #define naPrintMacroux256(macro)\
    na_PrintMacroux256(#macro)
  #define naPrintMacroix256(macro)\
    na_PrintMacroix256(#macro)
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
