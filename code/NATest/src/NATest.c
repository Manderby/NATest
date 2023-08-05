
#include "NATest.h"
#include "NATestString.h"
#include "NATestList.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if defined _WIN32
  #include <Tlhelp32.h>
#else
  #include <errno.h>
  #include <libproc.h>
  #include <signal.h>
  #include <sys/fcntl.h>
  #include <sys/sysctl.h>
  #include <sys/time.h>
  #include <unistd.h>
#endif



// How many precomputed random numbers.
#define NATEST_INDEX_COUNT 65521  // prime number smaller than 65535



typedef struct NATestData NATestData;
struct NATestData {
  const char* name;
  size_t lineNum;
  NATestBool success;
  NATestListItem* childs;
  size_t childsCount;
  size_t childSuccessCount;
  size_t leafSuccessCount;
  size_t totalLeafCount;
  NATestData* parent;
};

typedef struct NATesting NATesting;
struct NATesting {
  NATestData* rootTestData;
  NATestData* curTestData;
  double timePerBenchmark;
  NATestBool printAllTests;
  NATestBool printAllTestGroups;
  NATestBool testCaseRunning;
  NATestBool letCrashTestsCrash;
  NATestBool testingStartSuccessful;
  size_t errorCount;
  NATestListItem* untestedStrings;
  size_t untestedStringsCount;
  NATestListItem* testRestriction;
  NATestListItem* restrictionIt;
  size_t curInIndex;
  uint32 in[NATEST_INDEX_COUNT];
  NATestFile logFile;
};

NATesting* na_Testing = NATEST_NULL;



NATEST_HDEF NATestUTF8Char* na_NewTestApplicationPath(void){
  NATestUTF8Char* exePath;
  #if defined _WIN32
    TCHAR modulePath[MAX_PATH];
    GetModuleFileName(NULL, modulePath, MAX_PATH);
    exePath = naAllocTestStringFromSystemString(modulePath);
  #else
    char pathBuf[PROC_PIDPATHINFO_MAXSIZE];
    pid_t pid = (pid_t)getpid();
    proc_pidpath (pid, pathBuf, sizeof(pathBuf));
    exePath = naAllocTestStringWithFormat("%s", pathBuf);
  #endif
  // This is how Linux would do it:
//    char dest[PATH_MAX];
//    memset(dest, 0, sizeof(dest)); // readlink does not null terminate!
//    if (readlink("/proc/self/exe", dest, PATH_MAX) == -1) {
//      perror("readlink");
//    } else {
//      printf("%s\n", dest);
//    }
//    return naAllocTestStringWithFormat("%s", dest);
  return exePath;
}



NATEST_HIDEF void na_InitTestingData(NATestData* testData, const char* name, NATestData* parent, size_t lineNum){
  testData->name = name;
  testData->lineNum = lineNum;
  testData->success = NATEST_TRUE;
  testData->childs = naAllocateTestListItem(NATEST_NULL);
  testData->childsCount = 0;
  testData->childSuccessCount = 0;
  testData->leafSuccessCount = 0;
  testData->totalLeafCount = 0;
  testData->parent = parent;
}



NATEST_HIDEF void na_ClearTestingData(NATestData* testData){
  while(testData->childsCount){
    NATestListItem* lastItem = testData->childs->prev;
    na_ClearTestingData(lastItem->data);
    naDeallocateTestListItem(lastItem);
    testData->childsCount--;
  }
  free(testData->childs);
}



NATEST_HDEF NATestUTF8Char* na_NewTestPath(NATestData* testData, NATestBool escape){
  NATestUTF8Char* str = naAllocTestStringWithFormat("%s", testData->name);
  if(escape)
  {
    NATestUTF8Char* escapeName = naAllocTestStringCEscaped(str);
    free(str);
    str = naAllocTestStringWithFormat("\"%s\"", escapeName);
    free(escapeName);
  }

  if(testData->parent){
    NATestUTF8Char* parentName = na_NewTestPath(testData->parent, escape);
    NATestUTF8Char* combinedName = naAllocTestStringWithFormat("%s %s", parentName, str);
    free(parentName);
    free(str);
    str = combinedName;
  }
  return str;
}



NATEST_HIDEF void na_PrintErrorColumnWithLineNum(NATestUTF8Char code, size_t lineNum){
  printf("%c  Line %zd: ", code, lineNum);
}



NATEST_HIDEF void na_PrintErrorColumn(NATestUTF8Char code){
  printf("%c  ", code);
}



NATEST_HIDEF void na_PrintLineColumn(size_t lineNum){
  printf("Line %zd: ", lineNum);
}



NATEST_HIDEF void na_PrintTestName(NATestData* testData){
  NATestUTF8Char* testPath = na_NewTestPath(testData, NATEST_FALSE);
  printf("%s", testPath);
  free(testPath);
}



NATEST_HIDEF void na_PrintRatio(size_t successCount, size_t totalCount){
  if(totalCount){
    double ratio = (double)successCount / (double)totalCount * 100.;
    printf(" (%.02f%%)", ratio);
  }
}



NATEST_HIDEF void na_PrintTestGroup(NATestData* testData, NATestBool printName){
  if(testData->totalLeafCount == 0){return;}

  printf("Result: ");
  if(printName){
    na_PrintTestName(na_Testing->curTestData);
    printf(" ");
  }
  if(testData->totalLeafCount == testData->childsCount){
    printf("%zd / %zd Tests ok", testData->leafSuccessCount, testData->totalLeafCount);
    na_PrintRatio(testData->leafSuccessCount, testData->totalLeafCount);
    printf(NATEST_NL);
  }else{
    printf("%zd / %zd Groups ok", testData->childSuccessCount, testData->childsCount);
    na_PrintRatio(testData->childSuccessCount, testData->childsCount);
    printf(", %zd / %zd Tests ok", testData->leafSuccessCount, testData->totalLeafCount);
    na_PrintRatio(testData->leafSuccessCount, testData->totalLeafCount);
    printf(NATEST_NL);
  }
}



NATEST_DEF NATestBool naStartTesting(
  const NATestUTF8Char* rootName,
  double timePerBenchmark,
  NATestBool printAllTests,
  NATestBool printAllGroups,
  int argc,
  const char** argv)
{
  if(na_Testing)
    na_TestEmitError("Testing already running.");

//  if(argc > 1){
//    printf("Runnging tests with args:" NATEST_NL);
//    for(int i = 1; i < argc; ++i){
//      printf("%s ", argv[i]);
//    }
//  }

  na_Testing = (NATesting*)malloc(sizeof(NATesting));
  if(!na_Testing){
    na_TestEmitError("Ran out of memory.");
    return NATEST_FALSE;
  }

  na_Testing->rootTestData = (NATestData*)malloc(sizeof(NATestData));
  na_InitTestingData(na_Testing->rootTestData, rootName, NATEST_NULL, 0);

  na_Testing->curTestData = na_Testing->rootTestData;
  na_Testing->timePerBenchmark = timePerBenchmark;
  na_Testing->printAllTests = printAllTests;
  na_Testing->printAllTestGroups = printAllGroups;
  na_Testing->letCrashTestsCrash = NATEST_FALSE;
  na_Testing->testingStartSuccessful = NATEST_FALSE;
  naSetTestCaseRunning(NATEST_FALSE);
  na_ResetErrorCount();

  for(na_Testing->curInIndex = 0; na_Testing->curInIndex < NATEST_INDEX_COUNT; na_Testing->curInIndex++){
    na_Testing->in[na_Testing->curInIndex] = ((uint32)rand() << 20) ^ ((uint32)rand() << 10) ^ ((uint32)rand());
  }

  na_Testing->untestedStrings = naAllocateTestListItem(NATEST_NULL);
  na_Testing->untestedStringsCount = 0;
  na_Testing->testRestriction = naAllocateTestListItem(NATEST_NULL);

  if(argc > 1){
    for(int i = 1; i < argc; ++i)
    {
      if(argv[i][0] == '-'){
        if(argv[i][1] == 'C'){
          na_Testing->letCrashTestsCrash = NATEST_TRUE;
        }else{
          printf("Unrecognized executable argument: %c" NATEST_NL, argv[i][1]);
        }
      }else{
        NATestUTF8Char* argString = naAllocTestStringWithFormat("%s", argv[i]);
        if(argString[0] == '\"'){
          NATestUTF8Char* newArgString = naAllocTestStringDequote(argString);
//          printf("Quotes detected: %s -> %s\n", argString, newArgString);
          free(argString);
          argString = newArgString;
        }
        NATestListItem* newItem = naAllocateTestListItem(argString);
        naAddTestListBefore(na_Testing->testRestriction, newItem);
      }
    }
  }
  
  if(naIsTestListEmpty(na_Testing->testRestriction)){
    NATestUTF8Char* star = malloc(2);
    if(!star){
      na_TestEmitError("Ran out of memory.");
      return NATEST_FALSE;
    }
    star[0] = '*';
    star[1] = '\0';
    NATestListItem* newItem = naAllocateTestListItem(star);
    naAddTestListBefore(na_Testing->testRestriction, newItem);
  }
  na_Testing->restrictionIt = na_Testing->testRestriction->next;

  NATestUTF8Char* modulePath = na_NewTestApplicationPath();
  NATestUTF8Char* runPath = naAllocTestStringWithBasenameOfPath(modulePath);
  NATestUTF8Char* crashLogPath = naAllocTestStringWithFormat("%s_latestCrash.log", runPath);

  #if defined _WIN32
    SECURITY_ATTRIBUTES securityAttributes;
    memset(&securityAttributes, 0, sizeof(SECURITY_ATTRIBUTES));

    securityAttributes.nLength = sizeof(securityAttributes);
    securityAttributes.lpSecurityDescriptor = NULL;
    securityAttributes.bInheritHandle = TRUE;

    TCHAR* systemCrashLogPath = naTestAllocSystemStringWithUTF8String(crashLogPath);
    na_Testing->logFile = CreateFile(
      systemCrashLogPath,
      FILE_APPEND_DATA,
      FILE_SHARE_WRITE | FILE_SHARE_READ,
      &securityAttributes,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL );
    free(systemCrashLogPath);
  #else
    na_Testing->logFile = open(
      crashLogPath,
      NATEST_FILE_OPEN_FLAGS_WRITE,
      NATEST_FILEMODE_DEFAULT);

    if(na_Testing->logFile < 0)
      na_TestEmitError("Could not create file.");
  #endif

  free(crashLogPath);
  free(runPath);
  free(modulePath);

  na_Testing->testingStartSuccessful = na_ShallExecuteGroup(rootName);
  return na_Testing->testingStartSuccessful;
}



NATEST_DEF void naStopTesting(){
  if(!na_Testing)
    na_TestEmitCrash("Testing not running. Use naStartTesting.");

  na_StopTestGroup();
  printf(NATEST_NL);

  if(na_Testing->testingStartSuccessful){
    if(na_Testing->rootTestData->totalLeafCount == 0){
      printf("No tests executed." NATEST_NL);
    }else{
      if(na_Testing->rootTestData->success){
        printf("All test successful." NATEST_NL);
//        if(!na_Testing->printAllTestGroups){
          na_PrintTestGroup(na_Testing->rootTestData, NATEST_FALSE);
//        }
      }
    }
    printf("Testing finished." NATEST_NL NATEST_NL);
  }

  na_ClearTestingData(na_Testing->rootTestData);
  free(na_Testing->rootTestData);
  
  while(na_Testing->untestedStringsCount){
    NATestListItem* lastItem = na_Testing->untestedStrings->prev;
    naDeallocateTestListItem(lastItem);
    na_Testing->untestedStringsCount--;
  }
  free(na_Testing->untestedStrings);

  na_Testing->restrictionIt = NATEST_NULL;
  while(!naIsTestListEmpty(na_Testing->testRestriction)){
    naDeallocateTestListItem(na_Testing->testRestriction->next);
  }
  naDeallocateTestListItem(na_Testing->testRestriction);

  #if defined _WIN32
    CloseHandle(na_Testing->logFile);
  #else
    close(na_Testing->logFile);
  #endif

  free(na_Testing);
  na_Testing = NATEST_NULL;
}



NATEST_DEF void naPrintUntested(void){
  if(!na_Testing->untestedStringsCount){
    printf(NATEST_NL "No untested functionality." NATEST_NL);
  }else{
    printf(NATEST_NL "Untested functionality (%zd):" NATEST_NL, na_Testing->untestedStringsCount);
    NATestListItem* cur = na_Testing->untestedStrings->next;
    while(cur != na_Testing->untestedStrings){
      const NATestUTF8Char* string = (const NATestUTF8Char*)cur->data;
      na_PrintErrorColumn('U');
      printf("%s" NATEST_NL, string);
      cur = cur->next;
    }
  }
  printf(NATEST_NL);
}



NATEST_HDEF void na_UpdateTestParentLeaf(
  NATestData* testData,
  NATestBool leafSuccess)
{
  if(testData->parent){
    na_UpdateTestParentLeaf(testData->parent, leafSuccess);
  }

  testData->totalLeafCount++;
  if(leafSuccess){
    testData->leafSuccessCount++;
  }else{
    if(testData->success && testData->parent){
      testData->parent->childSuccessCount--;
    }
    testData->success = NATEST_FALSE;
  }
}



NATEST_HDEF void na_AddTest(const char* expr, NATestBool success, size_t lineNum){
  if(!na_Testing)
    na_TestEmitCrash("Testing not running. Use naStartTesting.");

  NATestData* testData = (NATestData*)malloc(sizeof(NATestData));
  if(!testData){
    na_TestEmitError("Ran out of memory.");
    return;
  }

  NATestListItem* newItem = naAllocateTestListItem(testData);
  naAddTestListBefore(na_Testing->curTestData->childs, newItem);
  na_Testing->curTestData->childsCount++;
  
  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);
  if(na_GetErrorCount() > 0){
    testData->success = NATEST_FALSE;
    na_UpdateTestParentLeaf(na_Testing->curTestData, NATEST_FALSE);
    na_PrintErrorColumnWithLineNum('E', lineNum);
    printf("%zd errors occured in %s" NATEST_NL, na_GetErrorCount(), expr);
  }else{
    testData->success = (NATestBool)success;
    na_UpdateTestParentLeaf(na_Testing->curTestData, (NATestBool)success);
    if(success && na_Testing->printAllTests){
      na_PrintErrorColumnWithLineNum(' ', lineNum);
    }else if(!success){
      na_PrintErrorColumnWithLineNum('F', lineNum);
    }
    if(!success || na_Testing->printAllTests){
      printf("%s" NATEST_NL, expr);
    }
  }

  na_Testing->restrictionIt = na_Testing->restrictionIt->prev;
}



NATEST_HDEF void na_AddTestError(const char* expr, size_t lineNum){
  #ifndef NDEBUG
  if(!na_Testing)
    na_TestEmitCrash("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = (NATestData*)malloc(sizeof(NATestData));
  if(!testData){
    na_TestEmitError("Ran out of memory.");
    return;
  }

  NATestListItem* newItem = naAllocateTestListItem(testData);
  naAddTestListBefore(na_Testing->curTestData->childs, newItem);
  na_Testing->curTestData->childsCount++;

  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);
  testData->success = na_GetErrorCount() != 0;
  na_UpdateTestParentLeaf(na_Testing->curTestData, (NATestBool)testData->success);
  if(!testData->success){
    na_PrintErrorColumnWithLineNum('N', lineNum);
    printf("No Error raised in %s" NATEST_NL, expr);
  }else{
    na_PrintErrorColumnWithLineNum(' ', lineNum);
    printf("Expected Error in %s" NATEST_NL, expr);
  }

  na_Testing->restrictionIt = na_Testing->restrictionIt->prev;
}



NATEST_HDEF void na_AddTestCrash(const char* expr, size_t lineNum){
  NATEST_UNUSED(expr);
  NATEST_UNUSED(lineNum);

  if(!na_Testing)
    na_TestEmitCrash("Testing not running. Use naStartTesting.");

  // Nothing special to be done here. This method is only back iterating in
  // order to finish the application successfully. If that happens, the
  // process returns a successful return value and the calling process knows
  // that no crash occurred.
  
  na_Testing->restrictionIt = na_Testing->restrictionIt->prev;
}



NATEST_HDEF void na_ExecuteCrashProcess(const char* expr, size_t lineNum){
  if(!na_Testing)
    na_TestEmitCrash("Testing not running. Use naStartTesting.");

  NATestData* testData = (NATestData*)malloc(sizeof(NATestData));
  if(!testData){
    na_TestEmitError("Ran out of memory.");
    return;
  }

  NATestListItem* newItem = naAllocateTestListItem(testData);
  naAddTestListBefore(na_Testing->curTestData->childs, newItem);
  na_Testing->curTestData->childsCount++;

  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);

  // We create a parallel process and run this test application again, but
  // with a path to the current test and the -C argument which causes the
  // test app to execute the test even if it is expected to crash.

  #if defined _WIN32
    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInfo;

    // Set the logFile as the output for stdout and stderr
    memset(&startupInfo, 0, sizeof(STARTUPINFOW));
    startupInfo.cb = sizeof(STARTUPINFOW);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdOutput = na_Testing->logFile;
    startupInfo.hStdError = na_Testing->logFile;

    NATestUTF8Char* modulePath = na_NewTestApplicationPath();
    NATestUTF8Char* testPath = na_NewTestPath(testData, NATEST_TRUE);
    
    // DO NOT TURN -C OPTION OFF!!!
    NATestUTF8Char* commandPath = naAllocTestStringWithFormat("\"%s\" -C %s", modulePath, testPath);
    TCHAR* systemCommandPath = naTestAllocSystemStringWithUTF8String(commandPath);

    BOOL success = CreateProcess(
      NATEST_NULL,
      systemCommandPath,
      NATEST_NULL,
      NATEST_NULL,
      NATEST_TRUE, /* bInheritHandles */
      0,
      NATEST_NULL,
      NATEST_NULL,
      &startupInfo,
      &processInfo
    );

    if(success){
      WaitForSingleObject( processInfo.hProcess, INFINITE );

      DWORD exitCode;
      GetExitCodeProcess(processInfo.hProcess, &exitCode);
      CloseHandle(processInfo.hThread);

      testData->success = exitCode != EXIT_SUCCESS;
      na_UpdateTestParentLeaf(na_Testing->curTestData, (NATestBool)testData->success);

      if(!testData->success){
        na_PrintErrorColumnWithLineNum('C', lineNum);
        printf("No Crash happened in %s" NATEST_NL, expr);
      }

    }else{
      na_PrintErrorColumnWithLineNum('X', lineNum);
      printf("CreateProcess failed in %s (System error code %d)." NATEST_NL, expr, (int)GetLastError());
    }

    free(systemCommandPath);
    free(modulePath);
    free(testPath);
    free(commandPath);
  
  #else

    // Set the logFile as the output for stdout and stderr
    int oldStdOut = dup(1);
    close(1); //Close stdout
    dup(na_Testing->logFile);
    int oldStdErr = dup(2);
    close(2); //Close stderr
    dup(na_Testing->logFile);

    NATestUTF8Char* modulePath = na_NewTestApplicationPath();

    #define NATEST_MAX_INPUT_STRINGS 100
    const char* testPathStrings[NATEST_MAX_INPUT_STRINGS];
    size_t curTestPathStringIndex = 0;

    testPathStrings[curTestPathStringIndex] = expr;
    curTestPathStringIndex++;

    NATestData* curTestData = na_Testing->curTestData;
    while(NATEST_TRUE){
      testPathStrings[curTestPathStringIndex] = curTestData->name;
      curTestPathStringIndex++;
      if(curTestData->parent){
       curTestData = curTestData->parent;
      }else{
        break;
      }
    }

    // Reserve 3 more slots for the args: testAppName -C and a final NULL
    char** const argv = malloc((curTestPathStringIndex + 3) * sizeof(const char*));
    argv[0] = malloc(strlen(modulePath) + 1);
    argv[0][strlen(modulePath)] = '\0';
    memcpy(argv[0], modulePath, strlen(modulePath));
    argv[1] = "-C"; // DO NOT TURN -C OPTION OFF!!!
    int i = 2;

    for(size_t curBackIndex = 0; curBackIndex < curTestPathStringIndex; curBackIndex++){
      const char* curPathItem = testPathStrings[curTestPathStringIndex - curBackIndex - 1];
      NATestUTF8Char* pathItemString = naAllocTestStringWithFormat("%s", curPathItem);
      NATestUTF8Char* escapedPathItemString = naAllocTestStringCEscaped(pathItemString);
      NATestUTF8Char* encapsulatedPathItemString = naAllocTestStringWithFormat("\"%s\"", escapedPathItemString);
      argv[i] = malloc(strlen(encapsulatedPathItemString) + 1);
      argv[i][strlen(encapsulatedPathItemString)] = '\0';
      memcpy(argv[i], encapsulatedPathItemString, strlen(encapsulatedPathItemString));
      free(encapsulatedPathItemString);
      free(escapedPathItemString);
      free(pathItemString);
      ++i;
    }
    argv[curTestPathStringIndex + 2] = NATEST_NULL;

//    for(int i = 0; i < curTestPathStringIndex + 2; i++){
//      printf("Arg %d: %s\n", i, argv[i]);
//    }

    pid_t childPid = fork();
    if(!childPid){

      // Don't use newly constructed structs of NARuntime here!!!
      // It will cause concurrency errors which are hard to track.

      execv(argv[0], argv);

      // If reaching here, something went wrong. Return success so that the
      // test eventually fails.
      printf("Starting crash test was unsuccessful.\n");
      exit(EXIT_SUCCESS);

    }else{
      int exitCode;
      waitpid(childPid, &exitCode, 0);

      free(argv[0]);
      i = 2;
      for(size_t curBackIndex = 0; curBackIndex < curTestPathStringIndex; curBackIndex++){
        free(argv[i]);
        ++i;
      }
      free(argv);

      NATestBool hasExitedNormally = WIFEXITED(exitCode);
//      NATestBool hasBeenSignaled = WIFSIGNALED(exitCode);
//      int sigNum = WTERMSIG(exitCode);
      int exitStatus = WEXITSTATUS(exitCode);
      testData->success = !hasExitedNormally || exitStatus != EXIT_SUCCESS;
      na_UpdateTestParentLeaf(na_Testing->curTestData, (NATestBool)testData->success);

      // Revert the file descriptors
      close(1);
      dup(oldStdOut);
      close(oldStdOut);
      close(2);
      dup(oldStdErr);
      close(oldStdErr);

      if(!testData->success){
        na_PrintErrorColumnWithLineNum('C', lineNum);
        printf("No Crash happened in %s" NATEST_NL, expr);
      }else{
        na_PrintErrorColumnWithLineNum(' ', lineNum);
        printf("Expected Crash happened in %s" NATEST_NL, expr);
      }
    }
    
    free(modulePath);

  #endif
}



NATEST_HDEF void na_RegisterUntested(const char* text){
  NATestUTF8Char* string = naAllocTestStringWithFormat("%s", text);
  NATestListItem* newItem = naAllocateTestListItem(string);
  naAddTestListBefore(na_Testing->untestedStrings, newItem);
  na_Testing->untestedStringsCount++;
}



NATEST_DEF NATestBool naIsTestCaseRunning(){
  return na_Testing->testCaseRunning;
}



NATEST_DEF void naSetTestCaseRunning(NATestBool running){
  na_Testing->testCaseRunning = running;
}



NATEST_HDEF void naIncErrorCount(void){
  na_Testing->errorCount++;
}



NATEST_HDEF void na_ResetErrorCount(void){
  na_Testing->errorCount = 0;
}



NATEST_HDEF size_t na_GetErrorCount(void){
  return na_Testing->errorCount;
}



NATEST_HDEF NATestBool na_LetCrashTestCrash(){
  return na_Testing->letCrashTestsCrash;
}



NATEST_HDEF NATestBool na_ShallExecuteGroup(const char* name){
  const NATestUTF8Char* allowedGroup = (NATestUTF8Char*)na_Testing->restrictionIt->data;
  NATestBool shallExecute =
    (memcmp(allowedGroup, "*", 1) == 0) ||
    (memcmp(allowedGroup, name, strlen(name)) == 0);
  if(shallExecute){
    na_Testing->restrictionIt = na_Testing->restrictionIt->next;
    if(na_Testing->restrictionIt == na_Testing->testRestriction){
      // We arrived at the end of the list. Artificially add an asterix and
      // let the iterator point to this new, last entry.
      NATestUTF8Char* star = malloc(2);
      if(!star){
        na_TestEmitError("Ran out of memory.");
        return NATEST_FALSE;
      }
      star[0] = '*';
      star[1] = '\0';
      NATestListItem* newItem = naAllocateTestListItem(star);
      naAddTestListBefore(na_Testing->testRestriction, newItem);
      na_Testing->restrictionIt = na_Testing->testRestriction->prev;
    }
  }
  return shallExecute;
}



NATEST_HDEF NATestBool na_StartTestGroup(const char* name, size_t lineNum){
  if(!na_Testing)
    na_TestEmitCrash("Testing not running. Use naStartTesting.");

  NATestBool shallExecute = na_ShallExecuteGroup(name);
  if(shallExecute)
  {
    NATestData* testData = (NATestData*)malloc(sizeof(NATestData));
    NATestListItem* newItem = naAllocateTestListItem(testData);
    naAddTestListBefore(na_Testing->curTestData->childs, newItem);
    na_Testing->curTestData->childsCount++;

    na_InitTestingData(testData, name, na_Testing->curTestData, lineNum);
    na_Testing->curTestData->childSuccessCount++;
    na_Testing->curTestData = testData;

    printf("-- ");
    na_PrintTestName(na_Testing->curTestData);
    printf(NATEST_NL);
  }
  return shallExecute;
}



NATEST_HDEF void na_StopTestGroup(){
  if(!na_Testing)
    na_TestEmitCrash("Testing not running. Use naStartTesting.");

//  if(na_Testing->printAllTestGroups || !na_Testing->curTestData->success){
//    na_PrintTestGroup(na_Testing->curTestData, NATEST_TRUE);
//  }
  na_Testing->curTestData = na_Testing->curTestData->parent;
  na_Testing->restrictionIt = na_Testing->restrictionIt->prev;
}



NATEST_HDEF uint32 na_GetBenchmarkIn(){
  na_Testing->curInIndex = (na_Testing->curInIndex + 1) % NATEST_INDEX_COUNT;
  return na_Testing->in[na_Testing->curInIndex];
}



NATEST_HDEF double na_BenchmarkTime(){
  // Note: Reimplemented here because NADateTime uses int64 to compute.
  #if defined _WIN32
    FILETIME fileTime;
    GetSystemTimeAsFileTime(&fileTime);
    fileTime.dwLowDateTime;
    return fileTime.dwLowDateTime / 10000000.; // 10 mil. see dwLowDateTime
  #else
    struct timeval curtime;
    struct timezone curTimeZone;
    gettimeofday(&curtime, &curTimeZone);
    return curtime.tv_sec + curtime.tv_usec / 1000000.;
  #endif
}



NATEST_HDEF double na_GetBenchmarkLimit(){
  return na_Testing->timePerBenchmark;
}



NATEST_HDEF size_t na_GetBenchmarkTestSizeLimit(){
  return 30;  // in bits. Not more than 32
}



NATEST_HDEF void na_PrintBenchmark(double timeDiff, size_t testSize, const char* exprString, size_t lineNum){
  na_PrintLineColumn(lineNum);
  if(timeDiff < na_GetBenchmarkLimit() || testSize >= ((size_t)1 << na_GetBenchmarkTestSizeLimit())){
    printf("Immeasurable   : %s" NATEST_NL, exprString);
  }else{
    double execsPerSec = testSize / timeDiff;
    if(execsPerSec > 1000000000.)
      printf("%6.2f G : %s" NATEST_NL, execsPerSec * .000000001, exprString);
    else if(execsPerSec > 1000000.)
      printf("%6.2f M : %s" NATEST_NL, execsPerSec * .000001, exprString);
    else if(execsPerSec > 1000.)
      printf("%6.2f k : %s" NATEST_NL, execsPerSec * .001, exprString);
    else
      printf("%6.2f   : %s" NATEST_NL, execsPerSec, exprString);
  }
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
