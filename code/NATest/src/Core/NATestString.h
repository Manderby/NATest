
#include "NATestEnvironment.h"

NATEST_HAPI NATestUTF8Char* na_AllocSprintf(const NATestUTF8Char* format, ...);

NATEST_API NATestUTF8Char* naAllocTestStringEmpty(void);
NATEST_API NATestUTF8Char* naAllocTestStringWithFormat(const NATestUTF8Char* format, ...);
NATEST_API NATestUTF8Char* naAllocTestStringCEscaped(const NATestUTF8Char* inputString);
NATEST_API NATestUTF8Char* naAllocTestStringDequote(const NATestUTF8Char* inputString);
NATEST_API NATestUTF8Char* naAllocTestStringWithBasenameOfPath(const NATestUTF8Char* filePath);

#if defined _WIN32
  // Returns a newly allocated memory block containing the system-encoded
  // string. COPIES ALWAYS!
  NATEST_API TCHAR* naTestAllocSystemStringWithUTF8String(const NATestUTF8Char* utf8String);

  // Returns a newly allocated memory block containing the utf8-encoded
  // string. COPIES ALWAYS!
  NATEST_API NATestUTF8Char* naAllocTestStringFromSystemString(const TCHAR* systemString);
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
