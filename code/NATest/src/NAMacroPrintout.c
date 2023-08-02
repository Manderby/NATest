

#include "NATestEnvironment.h"
#include "NATestString.h"



const char* na_YesNoStrings[] = {
  "No",
  "Yes",
};



#define NATEST_MAX_MACRO_LENGTH 40

NATEST_HDEF void na_PrintMacroHead(const NATestUTF8Char* macroString){
  printf("%s", macroString);
  printf(" ");  // Write at least one space
  for(size_t c = 0; c < NATEST_MAX_MACRO_LENGTH - strlen(macroString) - 1; c++){
    printf(" ");
  }
}



NATEST_HDEF void na_PrintMacroEnumBase(int macroValue, const NATestUTF8Char** strings, int enumCount){
  if(macroValue >= enumCount){
    printf("%d Invalid value", macroValue);
  }else{
    printf("%d (%s)", macroValue, strings[macroValue]);
  }
}



NATEST_HDEF void na_PrintMacroPlain(const NATestUTF8Char* macroString, const NATestUTF8Char* macroStringified, NATestBool printContent){
  na_PrintMacroHead(macroString);
  if(strncmp(macroString, macroStringified, strlen(macroString)) != 0){
    if(strlen(macroStringified) == 0){
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



NATEST_HDEF void na_PrintMacroInt(const NATestUTF8Char* macroString, int value){
  na_PrintMacroHead(macroString);
  printf("%d", value);
  printf(NATEST_NL);
}



NATEST_HDEF void na_PrintMacroIntHex(const NATestUTF8Char* macroString, int value){
  na_PrintMacroHead(macroString);
  printf("0x%x", value);
  printf(NATEST_NL);
}



NATEST_HDEF void na_PrintMacroIntSpecial(const NATestUTF8Char* macroString, int value, int specialValue, const NATestUTF8Char* specialString){
  na_PrintMacroHead(macroString);
  if(value == specialValue){
    printf("%d (%s)", value, specialString);
  }else{
    printf("%d", value);
  }
  printf(NATEST_NL);
}



NATEST_HDEF void na_PrintMacroIntSpecialHex(const NATestUTF8Char* macroString, int value, int specialValue, const NATestUTF8Char* specialString){
  na_PrintMacroHead(macroString);
  if(value == specialValue){
    printf("0x%x (%s)", value, specialString);
  }else{
    printf("0x%x", value);
  }
  printf(NATEST_NL);
}



NATEST_HDEF void na_PrintMacroIntYesNo(const NATestUTF8Char* macroString, int value){
  na_PrintMacroHead(macroString);
  na_PrintMacroEnumBase(value, na_YesNoStrings, 2);
  printf(NATEST_NL);
}



NATEST_HDEF void na_PrintMacroEnum(const NATestUTF8Char* macroString, int value, const NATestUTF8Char** strings, int enumCount){
  na_PrintMacroHead(macroString);
  na_PrintMacroEnumBase(value, strings, enumCount);
  printf(NATEST_NL);
}



NATEST_HDEF void na_PrintMacroux8(const NATestUTF8Char* macroString, uint8 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naTestPriux8(value));
  printf(NATEST_NL);
}
NATEST_HDEF void na_PrintMacroix8(const NATestUTF8Char* macroString, int8 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naTestPriix8(value));
  printf(NATEST_NL);
}

NATEST_HDEF void na_PrintMacroux16(const NATestUTF8Char* macroString, uint16 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naTestPriux16(value));
  printf(NATEST_NL);
}
NATEST_HDEF void na_PrintMacroix16(const NATestUTF8Char* macroString, int16 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naTestPriix16(value));
  printf(NATEST_NL);
}

NATEST_HDEF void na_PrintMacroux32(const NATestUTF8Char* macroString, uint32 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naTestPriux32(value));
  printf(NATEST_NL);
}
NATEST_HDEF void na_PrintMacroix32(const NATestUTF8Char* macroString, int32 value){
  na_PrintMacroHead(macroString);
  printf("0x%s", naTestPriix32(value));
  printf(NATEST_NL);
}

#ifdef UINT64_MAX
  NATEST_HDEF void na_PrintMacroux64(const NATestUTF8Char* macroString, uint64 value){
    na_PrintMacroHead(macroString);
    NATestUTF8Char* valueString = naTestPriux64(value);
    printf("0x%s", valueString);
    free(valueString);
    printf(NATEST_NL);
  }
  NATEST_HDEF void na_PrintMacroix64(const NATestUTF8Char* macroString, int64 value){
    na_PrintMacroHead(macroString);
    NATestUTF8Char* valueString = naTestPriix64(value);
    printf("0x%s", valueString);
    free(valueString);
    printf(NATEST_NL);
  }
#else
  NATEST_HDEF void na_PrintMacroux64(const NATestUTF8Char* macroString){
    na_PrintMacroHead(macroString);
    printf("64 Bit integers not supported");
    printf(NATEST_NL);
  }
  NATEST_HDEF void na_PrintMacroix64(const NATestUTF8Char* macroString){
    na_PrintMacroHead(macroString);
    printf("64 Bit integers not supported");
    printf(NATEST_NL);
  }
#endif

#ifdef __SIZEOF_INT128__
  NATEST_HDEF void na_PrintMacroux128(const NATestUTF8Char* macroString, uint128 value){
    na_PrintMacroHead(macroString);
    NATestUTF8Char* valueString = naTestPriux128(value);
    printf("0x%s", valueString);
    free(valueString);
    printf(NATEST_NL);
  }
  NATEST_HDEF void na_PrintMacroix128(const NATestUTF8Char* macroString, int128 value){
    na_PrintMacroHead(macroString);
    NATestUTF8Char* valueString = naTestPriix128(value);
    printf("0x%s", valueString);
    free(valueString);
    printf(NATEST_NL);
  }
#else
  NATEST_HDEF void na_PrintMacroux128(const NATestUTF8Char* macroString){
    na_PrintMacroHead(macroString);
    printf("128 Bit integers not supported");
    printf(NATEST_NL);
  }
  NATEST_HDEF void na_PrintMacroix128(const NATestUTF8Char* macroString){
    na_PrintMacroHead(macroString);
    printf("128 Bit integers not supported");
    printf(NATEST_NL);
  }
#endif

#ifdef __SIZEOF_INT256__
  NATEST_HDEF void na_PrintMacroux256(const NATestUTF8Char* macroString, uint256 value){
    na_PrintMacroHead(macroString);
    NATestUTF8Char* valueString = naTestPriux256(value);
    printf("0x%s", valueString);
    free(valueString);
    printf(NATEST_NL);
  }
  NATEST_HDEF void na_PrintMacroix256(const NATestUTF8Char* macroString, int256 value){
    na_PrintMacroHead(macroString);
    NATestUTF8Char* valueString = naTestPriix256(value);
    printf("0x%s", valueString);
    free(valueString);
    printf(NATEST_NL);
  }
#else
NATEST_HDEF void na_PrintMacroux256(const NATestUTF8Char* macroString){
    na_PrintMacroHead(macroString);
    printf("256 Bit integers not supported");
    printf(NATEST_NL);
  }
  NATEST_HDEF void na_PrintMacroix256(const NATestUTF8Char* macroString){
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
