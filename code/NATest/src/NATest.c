
#include "NATest.h"
#include "NATestString.h"

#if NA_TESTING_ENABLED == 1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if defined _WIN32
  #include <Tlhelp32.h>
  #include <windows.h>
#else
  #include <errno.h>
  #include <libproc.h>
  #include <signal.h>
  #include <sys/fcntl.h>
  #include <sys/sysctl.h>
  #include <sys/time.h>
  #include <unistd.h>
#endif

#include "../../../../../code/NALib/src/NAStruct/NAStack.h"
#include "../../../../../code/NALib/src/NAStruct/NAList.h"



// How many precomputed random numbers.
#define NA_TEST_INDEX_COUNT 0x10000
#define NA_TEST_INDEX_MASK (NA_TEST_INDEX_COUNT - 1)



typedef struct NATestData NATestData;
struct NATestData {
  const char* name;
  int lineNum;
  NATestBool success;
  NAStack childs;
  int childSuccessCount;
  int leafSuccessCount;
  int totalLeafCount;
  NATestData* parent;
};

typedef struct NATesting NATesting;
struct NATesting {
  NATestData* rootTestData;
  NATestData* curTestData;
  double timePerBenchmark;
  NATestBool printAllTestGroups;
  NATestBool testCaseRunning;
  NATestBool letCrashTestsCrash;
  NATestBool testingStartSuccessful;
  int errorCount;
  NAStack untestedStrings;
  NAList testRestriction;
  NAListIterator restrictionIt;
  int curInIndex;
  uint32 in[NA_TEST_INDEX_COUNT];
  NATestFile logFile;
};

NATesting* na_Testing = NATEST_NULL;



void na_TestError(NATestUTF8Char* message){
  fprintf(stderr, "Testing Error: %s\n", message);
}

NATEST_HDEF NATestUTF8Char* na_NewTestApplicationPath(void){
  NATestUTF8Char* exePath;
  #if defined _WIN32
    TCHAR modulePath[MAX_PATH];
    GetModuleFileName(NULL, modulePath, MAX_PATH);
    exePath = naNewStringFromSystemString(modulePath);
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
//    return naNewStringWithFormat("%s", dest);
  return exePath;
}



NATEST_HIDEF void na_InitTestingData(NATestData* testData, const char* name, NATestData* parent, int lineNum){
  testData->name = name;
  testData->lineNum = lineNum;
  testData->success = NATEST_TRUE;
  naInitStack(&(testData->childs), sizeof(NATestData), 0, 0);
  testData->childSuccessCount = 0;
  testData->leafSuccessCount = 0;
  testData->totalLeafCount = 0;
  testData->parent = parent;
}



NATEST_HIDEF void na_ClearTestingData(NATestData* testData){
  naForeachStackMutable(&(testData->childs), (NATestMutator)na_ClearTestingData);
  naClearStack(&(testData->childs));
}



NATEST_HDEF NATestUTF8Char* na_NewTestPath(NATestData* testData, NATestBool escape){
  NATestUTF8Char* str = naAllocTestStringWithFormat("%s", testData->name);
  if(escape)
  {
    NATestUTF8Char* escapeName = naAllocTestStringCEscaped(str);
    naDelete(str);
    str = naAllocTestStringWithFormat("\"%s\"", escapeName);
    naDelete(escapeName);
  }

  if(testData->parent){
    NATestUTF8Char* parentName = na_NewTestPath(testData->parent, escape);
    NATestUTF8Char* combinedName = naAllocTestStringWithFormat("%s %s", parentName, str);
    naDelete(parentName);
    naDelete(str);
    str = combinedName;
  }
  return str;
}



NATEST_HIDEF void na_PrintTestName(NATestData* testData){
  NATestUTF8Char* testPath = na_NewTestPath(testData, NATEST_FALSE);
  printf("%s", testPath);
  naDelete(testPath);
}



NATEST_HIDEF void na_PrintRatio(int successCount, int totalCount){
  if(totalCount){
    double ratio = (double)successCount / (double)totalCount * 100.;
    printf (" (%.02f%%)", ratio);
  }
}



NATEST_HIDEF void na_PrintTestGroup(NATestData* testData){
  int childTotalCount = (int)naGetStackCount(&(testData->childs));
  if(testData->totalLeafCount == 0){return;}

  printf("G ");
  if(testData->parent){na_PrintTestName(testData);}
  if(testData->totalLeafCount == childTotalCount){
    printf(": %d / %d Tests ok", testData->leafSuccessCount, testData->totalLeafCount);
    na_PrintRatio(testData->leafSuccessCount, testData->totalLeafCount);
    printf(NA_NL);
  }else{
    printf(": %d / %d Groups ok", testData->childSuccessCount, childTotalCount);
    na_PrintRatio(testData->childSuccessCount, childTotalCount);
    printf(", %d / %d Tests ok", testData->leafSuccessCount, testData->totalLeafCount);
    na_PrintRatio(testData->leafSuccessCount, testData->totalLeafCount);
    printf(NA_NL);
  }
}



NATEST_DEF NATestBool naStartTesting(
  const NATestUTF8Char* rootName,
  double timePerBenchmark,
  NATestBool printAllGroups,
  int argc,
  const char** argv)
{
  #if NA_DEBUG
    if(na_Testing)
      na_TestError("Testing already running.");
  #endif

//  if(argc > 1){
//    printf("Runnging tests with args:" NA_NL);
//    for(int i = 1; i < argc; ++i){
//      printf("%s ", argv[i]);
//    }
//  }

  na_Testing = naAlloc(NATesting);

  na_Testing->rootTestData = naAlloc(NATestData);
  na_InitTestingData(na_Testing->rootTestData, rootName, NATEST_NULL, 0);

  na_Testing->curTestData = na_Testing->rootTestData;
  na_Testing->timePerBenchmark = timePerBenchmark;
  na_Testing->printAllTestGroups = printAllGroups;
  na_Testing->letCrashTestsCrash = NATEST_FALSE;
  na_Testing->testingStartSuccessful = NATEST_FALSE;
  na_SetTestCaseRunning(NATEST_FALSE);
  na_ResetErrorCount();

  for(na_Testing->curInIndex = 0; na_Testing->curInIndex < NA_TEST_INDEX_COUNT; na_Testing->curInIndex++){
    na_Testing->in[na_Testing->curInIndex] = ((uint32)rand() << 20) ^ ((uint32)rand() << 10) ^ ((uint32)rand());
  }

  naInitStack(&(na_Testing->untestedStrings), sizeof(NATestUTF8Char*), 0, 0);
  naInitList(&(na_Testing->testRestriction));

  if(argc > 1){
    for(int i = 1; i < argc; ++i)
    {
      if(argv[i][0] == '-'){
        if(argv[i][1] == 'C'){
          na_Testing->letCrashTestsCrash = NATEST_TRUE;
        }else{
          printf("Unrecognized executable argument: %c" NA_NL, argv[i][1]);
        }
      }else{
        NATestUTF8Char* argString = naAllocTestStringWithFormat("%s", argv[i]);
        if(argString[0] == '\"'){
          NATestUTF8Char* newArgString = naAllocTestStringDequote(argString);
//          printf("Quotes detected: %s -> %s\n", naGetStringUTF8Pointer(argString), naGetStringUTF8Pointer(newArgString));
          naDelete(argString);
          argString = newArgString;
        }
        naAddListLastMutable(&(na_Testing->testRestriction), argString);
      }
    }
  }
  
  if(naIsListEmpty(&(na_Testing->testRestriction))){
    naAddListLastMutable(&(na_Testing->testRestriction), naAllocTestStringWithFormat("*"));
  }
  na_Testing->restrictionIt = naMakeListAccessor(&(na_Testing->testRestriction));
  naIterateList(&(na_Testing->restrictionIt));

  NATestUTF8Char* modulePath = na_NewTestApplicationPath();
  NATestUTF8Char* runPath = naAllocTestStringWithBasenameOfPath(modulePath);
  NATestUTF8Char* crashLogPath = naAllocTestStringWithFormat("%s_latestCrash.log", runPath);

  #if defined _WIN32
    SECURITY_ATTRIBUTES securityAttributes;
      securityAttributes.nLength = sizeof(securityAttributes);
      securityAttributes.lpSecurityDescriptor = NULL;
      securityAttributes.bInheritHandle = TRUE;
    TCHAR* systemCrashLogPath = naAllocSystemStringWithUTF8String(naGetStringUTF8Pointer(crashLogPath));
    na_Testing->logFile = CreateFile(
      systemCrashLogPath,
      FILE_APPEND_DATA,
      FILE_SHARE_WRITE | FILE_SHARE_READ,
      &securityAttributes,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL );
    naFree(systemCrashLogPath);
  #else
    na_Testing->logFile = open(
      crashLogPath,
      NATEST_FILE_OPEN_FLAGS_WRITE,
      NATEST_FILEMODE_DEFAULT);
    #if NA_DEBUG
      if(na_Testing->logFile < 0)
        na_TestError("Could not create file.");
    #endif
  #endif

  free(crashLogPath);
  free(runPath);
  free(modulePath);

  na_Testing->testingStartSuccessful = na_ShallExecuteGroup(rootName);
  return na_Testing->testingStartSuccessful;
}



NATEST_DEF void naStopTesting(){
  #if NA_DEBUG
  if(!na_Testing)
    naCrash("Testing not running. Use naStartTesting.");
  #endif

  na_StopTestGroup();

  if(na_Testing->testingStartSuccessful){
    if(na_Testing->rootTestData->totalLeafCount == 0){
      printf("No tests executed." NA_NL);
    }else{
      if(na_Testing->rootTestData->success){
        printf("All test successful." NA_NL);
        if(!na_Testing->printAllTestGroups){
          na_PrintTestGroup(na_Testing->rootTestData);
        }
      }
    }
    printf("Testing finished." NA_NL NA_NL);
  }

  na_ClearTestingData(na_Testing->rootTestData);
  naFree(na_Testing->rootTestData);
  naForeachStackpMutable(&(na_Testing->untestedStrings), (NATestMutator)free);
  naClearStack(&(na_Testing->untestedStrings));

  naClearListIterator(&(na_Testing->restrictionIt));
  naForeachListMutable(&(na_Testing->testRestriction), (NATestMutator)free);
  naClearList(&(na_Testing->testRestriction));

  #if defined _WIN32
    CloseHandle(na_Testing->logFile);
  #else
    close(na_Testing->logFile);
  #endif

  naFree(na_Testing);
  na_Testing = NATEST_NULL;
}



NATEST_DEF void naPrintUntested(void){
  int stackCount = (int)naGetStackCount(&(na_Testing->untestedStrings));
  if(!stackCount){
    printf(NA_NL "No untested functionality." NA_NL);
  }else{
    printf(NA_NL "Untested functionality (%d):" NA_NL, stackCount);
    NAStackIterator iter = naMakeStackAccessor(&(na_Testing->untestedStrings));
    while(naIterateStack(&iter)){
      const NATestUTF8Char* string = naGetStackCurpConst(&iter);
      printf("U %s" NA_NL, string);
    }
    naClearStackIterator(&iter);
  }
}



NA_HDEF void na_UpdateTestParentLeaf(
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



NA_HDEF void na_AddTest(const char* expr, int success, int lineNum){
  #if NA_DEBUG
    if(!na_Testing)
      naCrash("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);
  if(na_GetErrorCount() > 0){
    testData->success = NATEST_FALSE;
    na_UpdateTestParentLeaf(na_Testing->curTestData, NATEST_FALSE);
    printf("  ");
    if(testData->parent){na_PrintTestName(testData->parent);}
    printf(" Line %d: %d errors occured in %s" NA_NL, lineNum, na_GetErrorCount(), expr);
  }else{
    testData->success = (NATestBool)success;
    na_UpdateTestParentLeaf(na_Testing->curTestData, (NATestBool)success);
    if(!success){
      printf("  ");
      if(testData->parent){na_PrintTestName(testData->parent);}
      printf(" Line %d: %s" NA_NL, lineNum, expr);
    }
  }

  naIterateListBack(&(na_Testing->restrictionIt));
}



NA_HDEF void na_AddTestError(const char* expr, int lineNum){
  #if NA_DEBUG
  if(!na_Testing)
    naCrash("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);
  testData->success = na_GetErrorCount() != 0;
  na_UpdateTestParentLeaf(na_Testing->curTestData, (NATestBool)testData->success);
  if(!testData->success){
    printf("  ");
    if(testData->parent){na_PrintTestName(testData->parent);}
    printf(" Line %d: No Error raised in %s" NA_NL, lineNum, expr);
  }

  naIterateListBack(&(na_Testing->restrictionIt));
}



NA_HDEF void na_AddTestCrash(const char* expr, int lineNum){
  NA_UNUSED(expr);
  NA_UNUSED(lineNum);
  #if NA_DEBUG
  if(!na_Testing)
    naCrash("Testing not running. Use naStartTesting.");
  #endif

  // Nothing special to be done here. This method is only back iterating in
  // order to finish the application successfully. If that happens, the
  // process returns a successful return value and the calling process knows
  // that no crash occurred.
  
  naIterateListBack(&(na_Testing->restrictionIt));
}



NA_HDEF void na_ExecuteCrashProcess(const char* expr, int lineNum){
  #if NA_DEBUG
  if(!na_Testing)
    naCrash("Testing not running. Use naStartTesting.");
  #endif

  NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
  na_InitTestingData(testData, expr, na_Testing->curTestData, lineNum);

  // We create a parallel process and run this test application again, but
  // with a path to the current test and the -C argument which causes the
  // test app to execute the test even if it is expected to crash.

  #if defined _WIN32
    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInfo;

    // Set the logFile as the output for stdout and stderr
    naZeron(&startupInfo, sizeof(STARTUPINFOW));
    startupInfo.cb = sizeof(STARTUPINFOW);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdOutput = na_Testing->logFile;
    startupInfo.hStdError = na_Testing->logFile;

    NATestUTF8Char* modulePath = na_NewTestApplicationPath();
    NATestUTF8Char* testPath = na_NewTestPath(testData, NATEST_TRUE);
    
    // DO NOT TURN -C OPTION OFF!!!
    NATestUTF8Char* commandPath = naNewStringWithFormat("\"%s\" -C %s", naGetStringUTF8Pointer(modulePath), naGetStringUTF8Pointer(testPath));
    TCHAR* systemCommandPath = naAllocSystemStringWithUTF8String(naGetStringUTF8Pointer(commandPath));

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
        printf("  ");
        if(testData->parent){na_PrintTestName(testData->parent);}
        printf(" Line %d: No Crash happened in %s" NA_NL, lineNum, expr);
      }

    }else{
      printf("  ");
      if(testData->parent){na_PrintTestName(testData->parent);}
      printf(" Line %d: CreateProcess failed in %s (System error code %d)." NA_NL, lineNum, expr, (int)GetLastError());
    }

    naFree(systemCommandPath);
    naDelete(modulePath);
    naDelete(testPath);
    naDelete(commandPath);
  
  #else

    // Set the logFile as the output for stdout and stderr
    int oldStdOut = dup(1);
    close(1); //Close stdout
    dup(na_Testing->logFile);
    int oldStdErr = dup(2);
    close(2); //Close stderr
    dup(na_Testing->logFile);

    NATestUTF8Char* modulePath = na_NewTestApplicationPath();

    #define NA_MAX_TEST_INPUT_STRINGS 100
    const char* testPathStrings[NA_MAX_TEST_INPUT_STRINGS];
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
    char** const argv = naMalloc((curTestPathStringIndex + 3) * sizeof(const char*));
    argv[0] = naMalloc(strlen(modulePath) + 1);
    argv[0][strlen(modulePath)] = '\0';
    memcpy(argv[0], modulePath, strlen(modulePath));
    argv[1] = "-C"; // DO NOT TURN -C OPTION OFF!!!
    int i = 2;

    for(size_t curBackIndex = 0; curBackIndex < curTestPathStringIndex; curBackIndex++)
    {
      const char* curPathItem = testPathStrings[curTestPathStringIndex - curBackIndex - 1];
      NATestUTF8Char* pathItemString = naAllocTestStringWithFormat("%s", curPathItem);
      NATestUTF8Char* escapedPathItemString = naAllocTestStringCEscaped(pathItemString);
      NATestUTF8Char* encapsulatedPathItemString = naAllocTestStringWithFormat("\"%s\"", escapedPathItemString);
      argv[i] = naMalloc(strlen(encapsulatedPathItemString) + 1);
      argv[i][strlen(encapsulatedPathItemString)] = '\0';
      memcpy(argv[i], encapsulatedPathItemString, strlen(encapsulatedPathItemString));
      naDelete(encapsulatedPathItemString);
      naDelete(escapedPathItemString);
      naDelete(pathItemString);
      ++i;
    }
    argv[curTestPathStringIndex + 2] = NATEST_NULL;

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

      naFree(argv[0]);
      i = 2;
      for(size_t curBackIndex = 0; curBackIndex < curTestPathStringIndex; curBackIndex++){
        naFree(argv[i]);
        ++i;
      }
      naFree(argv);

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
        printf("  ");
        if(testData->parent){na_PrintTestName(testData->parent);}
        printf(" Line %d: No Crash happened in %s" NA_NL, lineNum, expr);
      }
    }
    
    naDelete(modulePath);

  #endif
}



NA_HDEF void na_RegisterUntested(const char* text){
  NATestUTF8Char** string = naPushStack(&(na_Testing->untestedStrings));
  *string = naAllocTestStringWithFormat("%s", text);
}



NA_HDEF NATestBool na_GetTestCaseRunning(){
  return na_Testing->testCaseRunning;
}



NA_HDEF void na_SetTestCaseRunning(NATestBool running){
  na_Testing->testCaseRunning = running;
}



NA_HDEF void na_IncErrorCount(void){
  na_Testing->errorCount++;
}



NA_HDEF void na_ResetErrorCount(void){
  na_Testing->errorCount = 0;
}



NA_HDEF int na_GetErrorCount(void){
  return na_Testing->errorCount;
}



NA_HDEF NATestBool na_LetCrashTestCrash(){
  return na_Testing->letCrashTestsCrash;
}



NA_HDEF NATestBool na_ShallExecuteGroup(const char* name){
  const NATestUTF8Char* allowedGroup = naGetListCurConst(&(na_Testing->restrictionIt));
  NATestBool shallExecute =
    (memcmp(allowedGroup, "*", 1) == 0) ||
    (memcmp(allowedGroup, name, strlen(name)) == 0);
  if(shallExecute){
    naIterateList(&(na_Testing->restrictionIt));
    if(naIsListAtInitial(&(na_Testing->restrictionIt))){
      // We arrived at the end of the list. Artificially add an asterix and
      // let the iterator point to this new, last entry.
      naAddListLastConst(&(na_Testing->testRestriction), naAllocTestStringWithFormat("*"));
      naIterateListBack(&(na_Testing->restrictionIt));
    }
  }
  return shallExecute;
}



NA_HDEF NATestBool na_StartTestGroup(const char* name, int lineNum){
  #if NA_DEBUG
  if(!na_Testing)
    naCrash("Testing not running. Use naStartTesting.");
  #endif

  NATestBool shallExecute = na_ShallExecuteGroup(name);
  if(shallExecute)
  {
    NATestData* testData = naPushStack(&(na_Testing->curTestData->childs));
    na_InitTestingData(testData, name, na_Testing->curTestData, lineNum);
    na_Testing->curTestData->childSuccessCount++;
    na_Testing->curTestData = testData;
  }
  return shallExecute;
}



NA_HDEF void na_StopTestGroup(){
  #if NA_DEBUG
  if(!na_Testing)
    naCrash("Testing not running. Use naStartTesting.");
  #endif

  if(na_Testing->printAllTestGroups || !na_Testing->curTestData->success){
    na_PrintTestGroup(na_Testing->curTestData);
  }
  na_Testing->curTestData = na_Testing->curTestData->parent;
  naIterateListBack(&(na_Testing->restrictionIt));
}



NA_HDEF uint32 na_GetBenchmarkIn(){
  na_Testing->curInIndex = (na_Testing->curInIndex + 1) & NA_TEST_INDEX_MASK;
  return na_Testing->in[na_Testing->curInIndex];
}



NA_HDEF double na_BenchmarkTime(){
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



NA_HDEF double na_GetBenchmarkLimit(){
  return na_Testing->timePerBenchmark;
}



NA_HDEF size_t na_GetBenchmarkTestSizeLimit(){
  return 30;  // in bits
}



NA_HDEF void na_PrintBenchmark(double timeDiff, size_t testSize, const char* exprString, int lineNum){
  if(timeDiff < na_GetBenchmarkLimit() || testSize >= naPowerOf2s(na_GetBenchmarkTestSizeLimit())){
      printf("Line %4d: Immeasurable   : %s" NA_NL, lineNum, exprString);
  }else{
    double execsPerSec = testSize / timeDiff;
    if(execsPerSec > 1000000000.)
      printf("Line %4d: %6.2f G : %s" NA_NL, lineNum, execsPerSec * .000000001, exprString);
    else if(execsPerSec > 1000000.)
      printf("Line %4d: %6.2f M : %s" NA_NL, lineNum, execsPerSec * .000001, exprString);
    else if(execsPerSec > 1000.)
      printf("Line %4d: %6.2f k : %s" NA_NL, lineNum, execsPerSec * .001, exprString);
    else
      printf("Line %4d: %6.2f   : %s" NA_NL, lineNum, execsPerSec, exprString);
  }
}



#else // NA_TESTING_ENABLED == 1fCould

NATEST_DEF NATestBool naStartTesting(
  const NATestUTF8Char* rootName,
  double timePerBenchmark,
  NATestBool printAllGroups,
  int argc,
  const char** argv)
{
  NA_UNUSED(rootName);
  NA_UNUSED(timePerBenchmark);
  NA_UNUSED(printAllGroups);
  NA_UNUSED(argc);
  NA_UNUSED(argv);
  #if NA_DEBUG
    na_TestError("Testing is not enabled. Go look for NA_TESTING_ENABLED");
  #endif
  return NATEST_FALSE;
}



NATEST_DEF void naStopTesting(){
  #if NA_DEBUG
    na_TestError("Testing is not enabled. Go look for NA_TESTING_ENABLED");
  #endif
}

#endif // NA_TESTING_ENABLED == 1



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
