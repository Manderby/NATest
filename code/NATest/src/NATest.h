#ifndef NATEST_INCLUDED
#define NATEST_INCLUDED

#ifdef __cplusplus
  extern "C"{
#endif

// All necessary includes and definitions.
#include "NATestEnvironment.h"
// Also include the macro printouts for convenience.
#include "NAMacroPrintout.h"



// Starts and stops the testing. Provide the top name of the code you
// want to test. The timePerBenchmark denotes the time spent per benchmark
// case. Usual values are around .01 seconds. Lower values are quicker but
// less sound. If printAllGroups is false, only the groups which have
// errors will be printed. A final printout will be made upon stopping.
//
// Returns true if the testing did start sucessfully, false otherwise.
// A common reason for an unsuccessful start is to forget the rootName in
// the command line arguments.
NATEST_API NATestBool naStartTesting(
  const NATestUTF8Char* rootName,
  double timePerBenchmark,
  NATestBool printAllTests,
  NATestBool printAllGroups,
  int argc,
  const char** argv);

// Stops the testing and prints some final results.
NATEST_API void naStopTesting(void);

// Prints a list of all strings added by calling naUntested.
NATEST_API void naPrintUntested(void);

// Manually interact with the test cases from outside the testing framework.
// You can enquiry the status whether a test case is currently running or
// set the status manually. Additionally, you can increment the internal error
// count as you like, which is needed for naTestError for example.
NATEST_API NATestBool naIsTestCaseRunning(void);
NATEST_API void naSetTestCaseRunning(NATestBool running);
NATEST_API void naIncErrorCount(void);

// Tests successfully if the given expression evaluates to true.
#define naTest(expr)

// Simply executes the given expression but keeps track of any errors. See
// naIncErrorCount.
#define naTestVoid(expr)

// Tests successfully if one or more naIncErrorCount calls happen.
#define naTestError(expr)

// Tests successfully if the expression would crash the application.
#define naTestCrash(expr)

// Groups together tests. Use it like naTestGroup("Multiple Tests"){ ... }
#define naTestGroup(string)

// Groups together tests by calling the given function.
#define naTestFunction(function)

// Stores an arbitrary text which denotes something to be untested. Use it
// together with naPrintUntested to get a complete list of all untested
// functionality before calling naStopTesting.
// Use this to mark things untested but not forgotten.
#define naUntested(text)

// Runs a benchmark of expr as long until the time defined in naStartTesting
// is reached. Outputs the number of executions per second.
#define naBenchmark(expr)

// Evaluates to a uint32 pseudo random number. Use this for test inputs to
// your benchmark expression.
#define naTestIn



// Inline implementations are in a separate file:
#include "NATestII.h"



#ifdef __cplusplus
  }
#endif
#endif // NATEST_INCLUDED



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
