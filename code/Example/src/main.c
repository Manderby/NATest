
#include "NATest.h"

void untestedThings(void){
  naUntested("I need to test P = NP");
  naUntested("Test buffer overflows");
}

void simpleThings(void){
  // naTest tests whether the given expression evaluates to true.
  naTest(1 == 1);
  naTest(1 == 0);
  naTest(7 > 3);
  naTest(4 < 6);
  naTest(4 > 6);
  naTest(NATEST_TRUE);
}

void functionWithoutError(void){
  // do nothing
}

void functionWithError(void){
  // This simulates an error.
  naIncErrorCount();
}

void buggyThings(void){
  // naTestVoid simply executes the given expression and reacts upon any error
  // which might occur. In order for something to count as an error, the
  // naIncErrorCount function must be called.
  naTestVoid(functionWithoutError());
  naTestVoid(functionWithError());
  
  // naTestError also executes the given expression but expects there to be an
  // error. If non happends, this is equivalent to a failed test.
  naTestError(functionWithoutError());
  naTestError(functionWithError());
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

  naTestFunction(simpleThings);
  naTestFunction(buggyThings);

//  naTestFunction(untestedThings);
//  naPrintUntested();

  naStopTesting();

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
