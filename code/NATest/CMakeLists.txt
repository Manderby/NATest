set(TARGET_NAME NATest)
add_library(NATest STATIC)

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE C)
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
  # multi processor compilation
  target_compile_options(${TARGET_NAME} PRIVATE /MP)
else()
endif()

set(sourceFiles
  src/NAMacroPrintout.h
  src/NATest.h
)

set(coreSourceFiles
  src/Core/NATest.c
  src/Core/NAMacroPrintout.c
  src/Core/NAMacroPrintoutII.h
  src/Core/NATestEnvironment.h
  src/Core/NATestII.h
  src/Core/NATestList.h
  src/Core/NATestString.c
  src/Core/NATestString.h
)

source_group("" FILES ${sourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${sourceFiles})

source_group("Core" FILES ${coreSourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${coreSourceFiles})

