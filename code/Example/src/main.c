
#include "NATest.h"

void simpleThings(void){
  // naTest tests whether the given expression evaluates to true.
  
  naTestGroup("Arithmetical tests"){
    naTest(1 == 1);
    naTest(1 == 0);
    naTest(7 > 3);
    naTest(4 < 6);
    naTest(4 > 6);
  }
  
  naTestGroup("Logical tests"){
    naTest(NATEST_TRUE);
    naTest(NATEST_FALSE);
  }
}

void functionWithoutError(void){
  // do nothing
}

void functionWithError(void){
  // This simulates an error.
  naIncErrorCount();
}

void functionNotCrashing(void){
  // do nothing
}

void functionCrashing(void){
  int* badPointer = NATEST_NULL;
  *badPointer = 1234;
}

void buggyThings(void){
//  // naTestVoid simply executes the given expression and reacts upon any error
//  // which might occur. In order for something to count as an error, the
//  // naIncErrorCount function must be called.
//  naTestVoid(functionWithoutError());
//  naTestVoid(functionWithError());
//  
//  // naTestError also executes the given expression but expects there to be an
//  // error. If non happends, this is equivalent to a failed test.
//  naTestError(functionWithoutError());
//  naTestError(functionWithError());

  // naTestCrash tries to execute the expression but expects it to crash. If
  // it does not crash, this is equivalent to a failed test.
  naTestCrash(functionNotCrashing());
  naTestCrash(functionCrashing());
}

void functionWhichNeedsTime(void){
  for(int i = 0; i < 10000; ++i){
    static int myCounter = 0;
    myCounter++;
  }
}

void functionWhichNeedsEvenMoreTime(void){
  for(int i = 0; i < 1000000; ++i){
    static int myCounter = 0;
    myCounter++;
  }
}

void functionWhichComputesSomething(void){
  for(int i = 0; i < 10000; ++i){
    static int myCounter = 0;
    // The expression naTestIn always evaluates to a different uint32 number
    // each time the programm executes it.
    myCounter += naTestIn;
  }
}

void benchmarks(void){
  naBenchmark(functionWhichNeedsTime());
  naBenchmark(functionWhichNeedsEvenMoreTime());
  naBenchmark(functionWhichComputesSomething());
}

int main(int argc, const char **argv){

  // Start the testing environment.
  NATestBool success = naStartTesting(
    "Example",
    0.01,
    NATEST_TRUE, 
    NATEST_TRUE, 
    argc,
    argv);
    
  if(!success) {
    printf("Could not start Testing.");
    exit(1);
  }

  // Start executing tests
  naTestFunction(simpleThings);
  naTestFunction(buggyThings);

  // Use naUntested anywhere in the test application to remember things which
  // are not tested yet. A call to naPrintUntested will list all registered
  // Strings.
  naUntested("I need to test P = NP");
  naUntested("Test buffer overflows");
  naPrintUntested();

  // You can run benchmars anywhere in your test application and they will be
  // executed on the spot with approximately the number of seconds each which
  // you defined when using naStartTesting.
  printf("Benchmarks:" NATEST_NL);
  benchmarks();

  // When naStopTesting is called, a final printout is made an all testing
  // memory is released.
  naStopTesting();

  naPrintMacro(__LINE__);
  naPrintMacroDefined(UNDEFINED_MACRO);
  #define DEFINED_MACRO
  naPrintMacroDefined(DEFINED_MACRO);
  #define DEFINED_MACRO_WITH_VALUE 1234
  naPrintMacroDefined(DEFINED_MACRO_WITH_VALUE);

  naPrintMacroInt(DEFINED_MACRO_WITH_VALUE);
  naPrintMacroIntHex(DEFINED_MACRO_WITH_VALUE);
  naPrintMacroIntSpecial(DEFINED_MACRO_WITH_VALUE, 1234, "My favourite value!");
  naPrintMacroIntSpecialHex(DEFINED_MACRO_WITH_VALUE, 1234, "My favourite value!");
  
  #define THIS_IS_TRUE 1
  naPrintMacroIntYesNo(THIS_IS_TRUE);
  
  enum{
    MY_VALUE_A,
    MY_VALUE_B,
    MY_VALUE_C,
    COUNT
  };
  const char* enumStrings[COUNT] = {
    "Value A",
    "Value B",
    "Value C"
  };
  naPrintMacroEnum(MY_VALUE_A, enumStrings, COUNT);
  
  #define MY_NUMBER 0xfdecba98
  naPrintMacroux8(MY_NUMBER);
  naPrintMacroix8(MY_NUMBER);
  naPrintMacroux16(MY_NUMBER);
  naPrintMacroix16(MY_NUMBER);
  naPrintMacroux32(MY_NUMBER);
  naPrintMacroix32(MY_NUMBER);
  naPrintMacroux64(MY_NUMBER);
  naPrintMacroix64(MY_NUMBER);
  naPrintMacroux128(MY_NUMBER);
  naPrintMacroix128(MY_NUMBER);
  naPrintMacroux256(MY_NUMBER);
  naPrintMacroix256(MY_NUMBER);
  
  return 0;
}

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
