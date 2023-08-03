
#ifndef NATEST_II_INCLUDED
#define NATEST_II_INCLUDED



// This file contains inline implementations of testing methods.

// First, undefine all macros defined in the .h file
#undef naTest
#undef naTestVoid
#undef naTestError
#undef naTestCrash
#undef naTestGroup
#undef naTestFunction
#undef naUntested
#undef naBenchmark
#undef naTestIn



NATEST_HAPI void       na_AddTest(const char* expr, NATestBool success, size_t lineNum);
NATEST_HAPI void       na_AddTestError(const char* expr, size_t lineNum);
NATEST_HAPI void       na_AddTestCrash(const char* expr, size_t lineNum);
NATEST_HAPI void       na_ExecuteCrashProcess(const char* expr, size_t lineNum);
NATEST_HAPI NATestBool na_StartTestGroup(const char* name, size_t lineNum);
NATEST_HAPI void       na_StopTestGroup(void);
NATEST_HAPI void       na_RegisterUntested(const char* text);
NATEST_HAPI NATestBool na_GetTestCaseRunning(void);
NATEST_HAPI void       na_SetTestCaseRunning(NATestBool running);
NATEST_HAPI void       na_ResetErrorCount(void);
NATEST_HAPI size_t     na_GetErrorCount(void);
NATEST_HDEF NATestBool na_LetCrashTestCrash(void);
NATEST_HAPI NATestBool na_ShallExecuteGroup(const char* name);

NATEST_HAPI uint32 na_GetBenchmarkIn(void);
NATEST_HAPI double na_BenchmarkTime(void);
NATEST_HAPI double na_GetBenchmarkLimit(void);
NATEST_HAPI size_t na_GetBenchmarkTestSizeLimit(void);
NATEST_HAPI void   na_PrintBenchmark(double timeDiff, size_t testSize, const char* exprString, size_t lineNum);



// Starting and stopping tests
#define NATEST_START_TEST_CASE\
  if(na_GetTestCaseRunning())\
    na_TestEmitError("A test case is already running. This might lead to bad test results.");\
  na_SetTestCaseRunning(NATEST_TRUE);\
  na_ResetErrorCount();

#define NATEST_STOP_TEST_CASE\
  na_SetTestCaseRunning(NATEST_FALSE);



// Testing expressions
#define naTest(expr)\
  if(na_ShallExecuteGroup(#expr)){\
    NATEST_START_TEST_CASE\
    NATestBool success = expr;\
    NATEST_STOP_TEST_CASE\
    na_AddTest(#expr, success, (size_t)__LINE__);\
  }

#define naTestVoid(expr)\
  if(na_ShallExecuteGroup(#expr)){\
    NATEST_START_TEST_CASE\
    expr;\
    NATEST_STOP_TEST_CASE\
    na_AddTest(#expr, NATEST_TRUE, (size_t)__LINE__);\
  }
  
  

#define naTestError(expr)\
  if(na_ShallExecuteGroup(#expr)){\
    NATEST_START_TEST_CASE\
    { expr; }\
    NATEST_STOP_TEST_CASE\
    na_AddTestError(#expr, (size_t)__LINE__);\
  }

#define naTestCrash(expr)\
  if(na_ShallExecuteGroup(#expr)){\
    if(na_LetCrashTestCrash()){\
      NATEST_START_TEST_CASE\
      { expr; }\
      NATEST_STOP_TEST_CASE\
      na_AddTestCrash(#expr, (size_t)__LINE__);\
    }else{\
      na_ExecuteCrashProcess(#expr, (size_t)__LINE__);\
    }\
  }



// Grouping tests
#define naTestGroup(string)\
  for(int g = (int)na_StartTestGroup(string, (size_t)__LINE__); g > 0 ; g--, na_StopTestGroup())

#define naTestFunction(function)\
  {\
  if(na_StartTestGroup(#function, (size_t)__LINE__)){\
    function();\
    na_StopTestGroup();\
  }\
  }



// Untested functionality
#define naUntested(text)\
  na_RegisterUntested(#text);



// Benchmarking
#define naBenchmark(expr)\
{\
  size_t testSize = 1;\
  double timeDiff = 0;\
  size_t pow;\
  double startT = na_BenchmarkTime();\
  double endT;\
  /* The number of tested expressions doubles every loop. */\
  for(pow = 0; pow < na_GetBenchmarkTestSizeLimit(); pow++){\
    for(size_t testRun = 0; testRun < testSize; testRun++){\
      {\
        (void)expr; (void)0;\
      }\
    }\
    endT = na_BenchmarkTime();\
    timeDiff = endT - startT;\
    if(timeDiff < 0.){timeDiff = 0.; break;}\
    if(timeDiff > na_GetBenchmarkLimit()){break;}\
    testSize <<= 1;\
  }\
  na_PrintBenchmark(timeDiff, testSize * 2, #expr, (size_t)__LINE__);\
}

#define naTestIn\
  na_GetBenchmarkIn()



#endif // NATEST_II_INCLUDED



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
