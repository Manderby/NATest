
#undef naTestPriux64
#undef naTestPriix64
#undef naTestPriux128
#undef naTestPriix128
#undef naTestPriux256
#undef naTestPriix256



#ifdef UINT64_MAX
  NATEST_API NATestUTF8Char* naTestPriux64 (uint64 value);
  NATEST_API NATestUTF8Char* naTestPriix64 (int64  value);
  #define naTestPriux64(value) naTestPriux64(value)
  #define naTestPriix64(value) naTestPriix64(value)
#else
  NATEST_API NATestUTF8Char* naTestPriux64 (void);
  NATEST_API NATestUTF8Char* naTestPriix64 (void);
  #define naTestPriux64(value) naTestPriux64()
  #define naTestPriix64(value) naTestPriix64()
#endif

#ifdef __SIZEOF_INT128__
  NATEST_API NATestUTF8Char* naTestPriux128 (uint128 value);
  NATEST_API NATestUTF8Char* naTestPriix128 (int128  value);
  #define naTestPriux128(value) naTestPriux128(value)
  #define naTestPriix128(value) naTestPriix128(value)
#else
  NATEST_API NATestUTF8Char* naTestPriux128 (void);
  NATEST_API NATestUTF8Char* naTestPriix128 (void);
  #define naTestPriux128(value) naTestPriux128()
  #define naTestPriix128(value) naTestPriix128()
#endif

#ifdef __SIZEOF_INT256__
  NATEST_API NATestUTF8Char* naTestPriux256 (uint256 value);
  NATEST_API NATestUTF8Char* naTestPriix256 (int256  value);
  #define naTestPriux256(value) naTestPriux256(value)
  #define naTestPriix256(value) naTestPriix256(value)
#else
  NATEST_API NATestUTF8Char* naTestPriux256 (void);
  NATEST_API NATestUTF8Char* naTestPriix256 (void);
  #define naTestPriux256(value) naTestPriux256()
  #define naTestPriix256(value) naTestPriix256()
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
