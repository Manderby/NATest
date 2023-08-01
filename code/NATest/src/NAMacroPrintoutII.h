
// First, undefine the macros from the .h file.
#undef naPrintMacro
#undef naPrintMacroDefined
#undef naPrintMacroInt
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



// Prototypes of helper functions which should be invisible to the user.
NATEST_HAPI void na_PrintMacroPlain(const NATestUTF8Char* macroString, const NATestUTF8Char* macroStringified, NATestBool printContent);
NATEST_HAPI void na_PrintMacroInt(const NATestUTF8Char* macroString, int value);
NATEST_HAPI void na_PrintMacroIntSpecial(const NATestUTF8Char* macroString, int value, int specialValue, const NATestUTF8Char* specialString);
NATEST_HAPI void na_PrintMacroIntSpecialHex(const NATestUTF8Char* macroString, int value, int specialValue, const NATestUTF8Char* specialString);
NATEST_HAPI void na_PrintMacroIntYesNo(const NATestUTF8Char* macroString, int value);
NATEST_HAPI void na_PrintMacroEnum(const NATestUTF8Char* macroString, int value, const NATestUTF8Char** strings, int enumCount);

#define naPrintMacro(macro)\
  na_PrintMacroPlain(#macro, NA_STRINGIFY(macro), NA_TRUE)
#define naPrintMacroDefined(macro)\
  na_PrintMacroPlain(#macro, NA_STRINGIFY(macro), NA_FALSE)
#define naPrintMacroInt(macro)\
  na_PrintMacroInt(#macro, (int)macro)
#define naPrintMacroIntSpecial(macro, specialValue, specialString)\
  na_PrintMacroIntSpecial(#macro, (int)macro, (int)specialValue, specialString)
#define naPrintMacroIntSpecialHex(macro, specialValue, specialString)\
  na_PrintMacroIntSpecialHex(#macro, (int)macro, (int)specialValue, specialString)
#define naPrintMacroIntYesNo(macro)\
  na_PrintMacroIntYesNo(#macro, (int)macro)
#define naPrintMacroEnum(macro, strings, maxValue)\
  na_PrintMacroEnum(#macro, (int)macro, strings, maxValue)

NATEST_HAPI void na_PrintMacroux8(const NATestUTF8Char* macroString, uint8 value);
NATEST_HAPI void na_PrintMacroix8(const NATestUTF8Char* macroString, int8 value);
#define naPrintMacroux8(macro)\
  na_PrintMacroux8(#macro, macro)
#define naPrintMacroix8(macro)\
  na_PrintMacroix8(#macro, macro)

NATEST_HAPI void na_PrintMacroux16(const NATestUTF8Char* macroString, uint16 value);
NATEST_HAPI void na_PrintMacroix16(const NATestUTF8Char* macroString, int16 value);
#define naPrintMacroux16(macro)\
  na_PrintMacroux16(#macro, macro)
#define naPrintMacroix16(macro)\
  na_PrintMacroix16(#macro, macro)

NATEST_HAPI void na_PrintMacroux32(const NATestUTF8Char* macroString, uint32 value);
NATEST_HAPI void na_PrintMacroix32(const NATestUTF8Char* macroString, int32 value);
#define naPrintMacroux32(macro)\
  na_PrintMacroux32(#macro, macro)
#define naPrintMacroix32(macro)\
  na_PrintMacroix32(#macro, macro)

#ifdef UINT64_MAX
  NATEST_HAPI void na_PrintMacroux64(const NATestUTF8Char* macroString, uint64 value);
  NATEST_HAPI void na_PrintMacroix64(const NATestUTF8Char* macroString, int64 value);
  #define naPrintMacroux64(macro)\
    na_PrintMacroux64(#macro, macro)
  #define naPrintMacroix64(macro)\
    na_PrintMacroix64(#macro, macro)
#else
  NATEST_HAPI void na_PrintMacroux64(const NATestUTF8Char* macroString);
  NATEST_HAPI void na_PrintMacroix64(const NATestUTF8Char* macroString);
  #define naPrintMacroux64(macro)\
    na_PrintMacroux64(#macro)
  #define naPrintMacroix64(macro)\
    na_PrintMacroix64(#macro)
#endif

#ifdef __SIZEOF_INT128__
  NATEST_HAPI void na_PrintMacroux128(const NATestUTF8Char* macroString, uint128 value);
  NATEST_HAPI void na_PrintMacroix128(const NATestUTF8Char* macroString, int128 value);
  #define naPrintMacroux128(macro)\
    na_PrintMacroux128(#macro, macro)
  #define naPrintMacroix128(macro)\
    na_PrintMacroix128(#macro, macro)
#else
  NATEST_HAPI void na_PrintMacroux128(const NATestUTF8Char* macroString);
  NATEST_HAPI void na_PrintMacroix128(const NATestUTF8Char* macroString);
  #define naPrintMacroux128(macro)\
    na_PrintMacroux128(#macro)
  #define naPrintMacroix128(macro)\
    na_PrintMacroix128(#macro)
#endif

#ifdef __SIZEOF_INT256__
  NATEST_HAPI void na_PrintMacroux256(const NATestUTF8Char* macroString, uint256 value);
  NATEST_HAPI void na_PrintMacroix256(const NATestUTF8Char* macroString, int256 value);
  #define naPrintMacroux256(macro)\
    na_PrintMacroux256(#macro, macro)
  #define naPrintMacroix256(macro)\
    na_PrintMacroix256(#macro, macro)
#else
  NATEST_HAPI void na_PrintMacroux256(const NATestUTF8Char* macroString);
  NATEST_HAPI void na_PrintMacroix256(const NATestUTF8Char* macroString);
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
