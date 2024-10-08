
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



NATEST_HDEF NATestUTF8Char* na_AllocSprintf(const NATestUTF8Char* format, ...) {
  va_list argumentList;
  va_list argumentList2;
  va_start(argumentList, format);
  va_copy(argumentList2, argumentList);

  size_t stringLen = na_VarargStringLength(format, argumentList);

  NATestUTF8Char* stringBuf = malloc(stringLen + 1);
  if(!stringBuf) {
    na_TestEmitError("Ran out of memory.");
    return NATEST_NULL;
  }

  na_Vsnprintf(stringBuf, stringLen + 1, format, argumentList2);
  stringBuf[stringLen] = '\0';

  return stringBuf;
}



NATEST_DEF NATestUTF8Char* naAllocTestStringEmpty() {
  NATestUTF8Char* string = malloc(1);
  if(!string) {
    na_TestEmitError("Ran out of memory.");
    return NATEST_NULL;
  }

  string[0] = '\0';
  return string;
}



NATEST_HDEF NATestUTF8Char* na_AllocTestStringWithArguments(const NATestUTF8Char* format, va_list argumentList) {
  NATestUTF8Char* string;
  va_list argumentList2;
  va_list argumentList3;
  va_copy(argumentList2, argumentList);
  va_copy(argumentList3, argumentList);
  size_t stringLen = na_VarargStringLength(format, argumentList2);
  if(stringLen) {
    NATestUTF8Char* stringBuf = malloc(stringLen + 1);
    if(!stringBuf) {
      na_TestEmitError("Ran out of memory.");
      return NATEST_NULL;
    }

    na_Vsnprintf(stringBuf, stringLen + 1, format, argumentList3);
    stringBuf[stringLen] = '\0';
    string = stringBuf;
  }else{
    string = naAllocTestStringEmpty();
  }
  va_end(argumentList2);
  va_end(argumentList3);
  return string;
}



NATEST_DEF NATestUTF8Char* naAllocTestStringWithFormat(const NATestUTF8Char* format, ...) {
  NATestUTF8Char* string;
  va_list argumentList;
  va_start(argumentList, format);
  string = na_AllocTestStringWithArguments(format, argumentList);
  va_end(argumentList);
  return string;
}



// Returns \0 if the character does not need to be escaped. Otherwise returns
// the character excape sequence (Withoutt the \ character).
NATEST_HDEF NATestUTF8Char* na_GetCEscapedChar(NATestUTF8Char inputChar, size_t* charCount) {
  switch(inputChar) {
  case '\a': *charCount = 1; return "a";
  case '\b': *charCount = 1; return "b";
  case '\f': *charCount = 1; return "f";
  case '\n': *charCount = 1; return "n";
  case '\r': *charCount = 1; return "r";
  case '\t': *charCount = 1; return "t";
  case '\v': *charCount = 1; return "v";
  case '\\': *charCount = 1; return "\\";
  case '\'': *charCount = 1; return "\'";
  case '\"': *charCount = 1; return "\"";
  case '\?': *charCount = 1; return "?";
  // todo: Add more escapes
  default: *charCount = 0; return "";
  }
}


NATEST_DEF NATestUTF8Char* naAllocTestStringCEscaped(const NATestUTF8Char* inputString) {
  size_t len = strlen(inputString);
  if(!len) {
    return naAllocTestStringEmpty();
  }
  
  // First, count how many characters are needed.
  size_t escapeCount = 0;
  for(size_t i = 0; i < len; ++i) {
    size_t charCount = 0;
    na_GetCEscapedChar(inputString[i], &charCount);
    escapeCount += charCount;
  }
  
  NATestUTF8Char* outString = malloc(len + escapeCount + 1);
    outString[len + escapeCount] = '\0';

  if(!outString) {
    na_TestEmitError("Ran out of memory.");
    return NATEST_NULL;
  }

  // Fill the new buffer up with the correct bytes.
  size_t outIndex = 0;
  for(size_t i = 0; i < len; ++i) {
    size_t charCount = 0;
    NATestUTF8Char* escape = na_GetCEscapedChar(inputString[i], &charCount);
    if(charCount) {
      outString[outIndex] = '\\';
      memcpy(&outString[outIndex + 1], escape, charCount);
    }else{
      outString[outIndex] = inputString[i];
    }
    outIndex += 1 + charCount;
  }

  return outString;
}



NATEST_API NATestUTF8Char* naAllocTestStringDequote(const NATestUTF8Char* inputString) {
  size_t len = strlen(inputString);
  NATestUTF8Char* outString = malloc(len - 2 + 1);
    outString[len - 2] = '\0';

  if(!outString) {
    na_TestEmitError("Ran out of memory.");
    return NATEST_NULL;
  }

  memcpy(outString, &inputString[1], len - 2);
  return outString;
}



NATEST_DEF NATestUTF8Char* naAllocTestStringWithBasenameOfPath(const NATestUTF8Char* filePath) {
  size_t originalLen = strlen(filePath);
  size_t len = originalLen;
  
  while(len) {
    if(filePath[len - 1] == '.') { break; }
    len = len - 1;
  }

  if(!len) {len = originalLen;}
  if(!len) {
    return naAllocTestStringEmpty();
  }
  
  NATestUTF8Char* outString = malloc(len + 1);
  outString[len] = '\0';
  memcpy(outString, filePath, len);

  return outString;
}



#if defined _WIN32
  NATEST_DEF wchar_t* naTestAllocWideCharStringWithUTF8String(const NATestUTF8Char* utf8String) {
    size_t length = strlen(utf8String);
    size_t wideLength = (size_t)MultiByteToWideChar(CP_UTF8, 0, utf8String, (int)length, NATEST_NULL, 0);
    wchar_t* outStr = (wchar_t*)malloc(((wideLength + 1) * sizeof(wchar_t)));
    MultiByteToWideChar(CP_UTF8, 0, utf8String, (int)length, outStr, (int)wideLength);
    outStr[wideLength] = 0;
    return outStr;
  }

  NATEST_DEF char* naTestAllocAnsiStringWithUTF8String(const NATestUTF8Char* utf8String) {
    size_t length = strlen(utf8String);

    // We have to convert UTF8 first to WideChar, then back to 8 bit Ansi.
    size_t wideLength = (size_t)MultiByteToWideChar(CP_UTF8, 0, utf8String, (int)length, NATEST_NULL, 0);
    wchar_t* wstr = malloc(((wideLength + 1) * sizeof(wchar_t)));
    MultiByteToWideChar(CP_UTF8, 0, utf8String, (int)length, wstr, (int)wideLength);
    wstr[wideLength] = 0;
    size_t ansiLength = (size_t)WideCharToMultiByte(CP_ACP, 0, wstr, (int)wideLength, NATEST_NULL, 0, NATEST_NULL, NATEST_NULL);
    char* outStr = (char*)malloc(((ansiLength + 1) * sizeof(char)));
    WideCharToMultiByte(CP_ACP, 0, wstr, (int)wideLength, outStr, (int)ansiLength, NATEST_NULL, NATEST_NULL);
    free(wstr);
    outStr[ansiLength] = 0;
    return outStr;
  }

  NATEST_DEF TCHAR* naTestAllocSystemStringWithUTF8String(const NATestUTF8Char* utf8String) {
    #ifdef UNICODE
      return naTestAllocWideCharStringWithUTF8String(utf8String);
    #else
      return naTestAllocAnsiStringWithUTF8String(utf8String);
    #endif
  }

  NATEST_DEF NATestUTF8Char* naTestAllocStringFromWideCharString(const wchar_t* wcharString) {
    size_t length = wcslen(wcharString);
    int utf8Length = WideCharToMultiByte(CP_UTF8, 0, wcharString, (int)length, NATEST_NULL, 0, NATEST_NULL, NATEST_NULL);
    NATestUTF8Char* stringBuf = malloc((size_t)(utf8Length + 1) * sizeof(NATestUTF8Char));
    WideCharToMultiByte(CP_UTF8, 0, wcharString, (int)length, stringBuf, (int)utf8Length, NATEST_NULL, NATEST_NULL);
    stringBuf[utf8Length] = '\0';
    return stringBuf;
  }

  NATEST_DEF NATestUTF8Char* naTestAllocStringFromAnsiString(const char* ansiString) {
    size_t length = strlen(ansiString);
    size_t wideLength = (size_t)MultiByteToWideChar(CP_ACP, 0, ansiString, (int)length, NATEST_NULL, 0);
    wchar_t* wstr = malloc(((wideLength + 1) * sizeof(wchar_t)));
    MultiByteToWideChar(CP_ACP, 0, ansiString, (int)length, wstr, (int)wideLength);
    wstr[wideLength] = 0;
    int utf8Length = WideCharToMultiByte(CP_UTF8, 0, wstr, (int)wideLength, NATEST_NULL, 0, NATEST_NULL, NATEST_NULL);
    NATestUTF8Char* stringBuf = (NATestUTF8Char*)malloc(((size_t)(utf8Length + 1) * sizeof(NATestUTF8Char)));
    WideCharToMultiByte(CP_UTF8, 0, wstr, (int)wideLength, stringBuf, (int)utf8Length, NATEST_NULL, NATEST_NULL);
    free(wstr);
    stringBuf[utf8Length] = '\0';
    return stringBuf;
  }

  // Creates a new NAString from a system-encoded string. COPIES ALWAYS!
  NATEST_DEF NATestUTF8Char* naAllocTestStringFromSystemString(const TCHAR* systemString) {
    #ifdef UNICODE
      return naTestAllocStringFromWideCharString(systemString);
    #else
      return naTestAllocStringFromAnsiString(systemString);
    #endif
  }

#endif // _WIN32

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
