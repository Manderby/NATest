

#include "NATestEnvironment.h"
#include "NATestString.h"



const char* na_YesNoStrings[] = {
  "No",
  "Yes",
};



#define NATEST_MAX_MACRO_LENGTH 40

NATEST_HDEF void na_PrintMacroHead(const NATestUTF8Char* macroString) {
  printf("%s", macroString);
  printf(" ");  // Write at least one space
  size_t stringLen = strlen(macroString);
  if(stringLen > NATEST_MAX_MACRO_LENGTH - 1) { stringLen = NATEST_MAX_MACRO_LENGTH - 1;}
  for(size_t c = 0; c < NATEST_MAX_MACRO_LENGTH - stringLen - 1; c++) {
    printf(" ");
  }
}



NATEST_HDEF void na_PrintMacroEnumBase(int macroValue, const NATestUTF8Char** strings, int enumCount) {
  if(macroValue >= enumCount) {
    printf("%d Invalid value", macroValue);
  }else{
    printf("%d (%s)", macroValue, strings[macroValue]);
  }
}



NATEST_HDEF void na_PrintMacroPlain(const NATestUTF8Char* macroString, const NATestUTF8Char* macroStringified, NATestBool printContent) {
  na_PrintMacroHead(macroString);
  if(strncmp(macroString, macroStringified, strlen(macroString)) != 0) {
    if(strlen(macroStringified) == 0) {
      printf("(Defined)");
    }else{
      if(printContent)
        printf("%s", macroStringified);
      else
        printf("(Defined with content)");
    }
  }else{
    printf("(Undefined)");
  }
  printf(NATEST_NL);
}



NATEST_HDEF void na_PrintMacroInt(const NATestUTF8Char* macroString, int value, NATestBool hex) {
  na_PrintMacroHead(macroString);
  if(hex) {
    printf("0x%x", value);
  }else{
    printf("%d", value);
  }
  printf(NATEST_NL);
}



NATEST_HDEF void na_PrintMacroIntSpecial(const NATestUTF8Char* macroString, int value, int specialValue, const NATestUTF8Char* specialString, NATestBool hex) {
  na_PrintMacroHead(macroString);
  if(hex) {
    if(value == specialValue) {
      printf("0x%x (%s)", value, specialString);
    }else{
      printf("0x%x", value);
    }
  }else{
    if(value == specialValue) {
      printf("%d (%s)", value, specialString);
    }else{
      printf("%d", value);
    }
  }
  printf(NATEST_NL);
}



NATEST_HDEF void na_PrintMacroIntYesNo(const NATestUTF8Char* macroString, int value) {
  na_PrintMacroHead(macroString);
  na_PrintMacroEnumBase(value, na_YesNoStrings, 2);
  printf(NATEST_NL);
}



NATEST_HDEF void na_PrintMacroEnum(const NATestUTF8Char* macroString, int value, const NATestUTF8Char** strings, int enumCount) {
  na_PrintMacroHead(macroString);
  na_PrintMacroEnumBase(value, strings, enumCount);
  printf(NATEST_NL);
}


NATEST_HDEF void na_PrintAndFreeHexString(NATestUTF8Char* hexString) {
  if(hexString) {
    printf("0x%s", hexString);
    free(hexString);
  }
  printf(NATEST_NL);
}



NATEST_DEF NATestUTF8Char* na_TestPriux8(uint16 value) {
  return na_AllocSprintf("%02x", (int)(value & NATEST_MAX_u8));
}
NATEST_DEF NATestUTF8Char* na_TestPriix8(int16 value) {
  return na_AllocSprintf("%02x", (int)(value & NATEST_MAX_u8));
}
NATEST_HDEF void na_PrintMacroux8(const NATestUTF8Char* macroString, uint8 value) {
  na_PrintMacroHead(macroString);
  na_PrintAndFreeHexString(na_TestPriux8(value));
}
NATEST_HDEF void na_PrintMacroix8(const NATestUTF8Char* macroString, int8 value) {
  na_PrintMacroHead(macroString);
  na_PrintAndFreeHexString(na_TestPriix8(value));
}



NATEST_DEF NATestUTF8Char* na_TestPriux16(uint16 value) {
  return na_AllocSprintf("%04x", (int)(value & NATEST_MAX_u16));
}
NATEST_DEF NATestUTF8Char* na_TestPriix16(int16 value) {
  return na_AllocSprintf("%04x", (int)(value & NATEST_MAX_u16));
}
NATEST_HDEF void na_PrintMacroux16(const NATestUTF8Char* macroString, uint16 value) {
  na_PrintMacroHead(macroString);
  na_PrintAndFreeHexString(na_TestPriux16(value));
}
NATEST_HDEF void na_PrintMacroix16(const NATestUTF8Char* macroString, int16 value) {
  na_PrintMacroHead(macroString);
  na_PrintAndFreeHexString(na_TestPriix16(value));
}



NATEST_DEF NATestUTF8Char* na_TestPriux32(uint32 value) {
  return na_AllocSprintf("%08x", (int)(value & NATEST_MAX_u32));
}
NATEST_DEF NATestUTF8Char* na_TestPriix32(int32 value) {
  return na_AllocSprintf("%08x", (int)(value & NATEST_MAX_u32));
}
NATEST_HDEF void na_PrintMacroux32(const NATestUTF8Char* macroString, uint32 value) {
  na_PrintMacroHead(macroString);
  na_PrintAndFreeHexString(na_TestPriux32(value));
}
NATEST_HDEF void na_PrintMacroix32(const NATestUTF8Char* macroString, int32 value) {
  na_PrintMacroHead(macroString);
  na_PrintAndFreeHexString(na_TestPriix32(value));
}



#ifdef NATEST_i64_NATIVE
  #define naGeti64Hi(i) ((int32)((i) >> 32))
  #define naGeti64Lo(i) ((uint32)i)
  #define naGetu64Hi(u) ((uint32)((u) >> 32))
  #define naGetu64Lo(u) ((uint32)u)

  NATEST_DEF NATestUTF8Char* na_TestPriux64(uint64 value) {
    return na_AllocSprintf("%08x%08x",
      naGetu64Hi(value),
      naGetu64Lo(value));
  }
  NATEST_DEF NATestUTF8Char* na_TestPriix64(int64 value) {
    return na_AllocSprintf("%08x%08x",
      naGeti64Hi(value),
      naGeti64Lo(value));
  }
  NATEST_HDEF void na_PrintMacroux64(const NATestUTF8Char* macroString, uint64 value) {
    na_PrintMacroHead(macroString);
    na_PrintAndFreeHexString(na_TestPriux64(value));
  }
  NATEST_HDEF void na_PrintMacroix64(const NATestUTF8Char* macroString, int64 value) {
    na_PrintMacroHead(macroString);
    na_PrintAndFreeHexString(na_TestPriix64(value));
  }
#else
  NATEST_HDEF void na_PrintMacroux64(const NATestUTF8Char* macroString) {
    na_PrintMacroHead(macroString);
    printf("64 Bit integers not supported");
    printf(NATEST_NL);
  }
  NATEST_HDEF void na_PrintMacroix64(const NATestUTF8Char* macroString) {
    na_PrintMacroHead(macroString);
    printf("64 Bit integers not supported");
    printf(NATEST_NL);
  }
#endif

#ifdef NATEST_i128_NATIVE
  #define naGeti128Hi(i) ((int64)((i) >> 64))
  #define naGeti128Lo(i) ((uint64)i)
  #define naGetu128Hi(u) ((uint64)((u) >> 64))
  #define naGetu128Lo(u) ((uint64)u)

  NATEST_DEF NATestUTF8Char* na_TestPriux128(uint128 value) {
    return na_AllocSprintf("%08x%08x%08x%08x",
      naGetu64Hi(naGetu128Hi(value)),
      naGetu64Lo(naGetu128Hi(value)),
      naGetu64Hi(naGetu128Lo(value)),
      naGetu64Lo(naGetu128Lo(value)));
  }
  NATEST_DEF NATestUTF8Char* na_TestPriix128(int128 value) {
    return na_AllocSprintf("%08x%08x%08x%08x",
      naGeti64Hi(naGeti128Hi(value)),
      naGeti64Lo(naGeti128Hi(value)),
      naGetu64Hi(naGeti128Lo(value)),
      naGetu64Lo(naGeti128Lo(value)));
  }
  NATEST_HDEF void na_PrintMacroux128(const NATestUTF8Char* macroString, uint128 value) {
    na_PrintMacroHead(macroString);
    na_PrintAndFreeHexString(na_TestPriux128(value));
  }
  NATEST_HDEF void na_PrintMacroix128(const NATestUTF8Char* macroString, int128 value) {
    na_PrintMacroHead(macroString);
    na_PrintAndFreeHexString(na_TestPriix128(value));
  }
#else
  NATEST_HDEF void na_PrintMacroux128(const NATestUTF8Char* macroString) {
    na_PrintMacroHead(macroString);
    printf("128 Bit integers not supported");
    printf(NATEST_NL);
  }
  NATEST_HDEF void na_PrintMacroix128(const NATestUTF8Char* macroString) {
    na_PrintMacroHead(macroString);
    printf("128 Bit integers not supported");
    printf(NATEST_NL);
  }
#endif

#ifdef NATEST_i256_NATIVE
  #define naGeti128Hi(i) ((int128)((i) >> 128))
  #define naGeti128Lo(i) ((uint128)i)
  #define naGetu128Hi(u) ((uint128)((u) >> 128))
  #define naGetu128Lo(u) ((uint128)u)

  NATEST_DEF NATestUTF8Char* na_TestPriux256(uint256 value) {
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
  NATEST_DEF NATestUTF8Char* na_TestPriix256(int256 value) {
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
  NATEST_HDEF void na_PrintMacroux256(const NATestUTF8Char* macroString, uint256 value) {
    na_PrintMacroHead(macroString);
    na_PrintAndFreeHexString(na_TestPriux256(value));
  }
  NATEST_HDEF void na_PrintMacroix256(const NATestUTF8Char* macroString, int256 value) {
    na_PrintMacroHead(macroString);
    na_PrintAndFreeHexString(na_TestPriix256(value));
  }
#else
  NATEST_HDEF void na_PrintMacroux256(const NATestUTF8Char* macroString) {
    na_PrintMacroHead(macroString);
    printf("256 Bit integers not supported");
    printf(NATEST_NL);
  }
  NATEST_HDEF void na_PrintMacroix256(const NATestUTF8Char* macroString) {
    na_PrintMacroHead(macroString);
    printf("256 Bit integers not supported");
    printf(NATEST_NL);
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
