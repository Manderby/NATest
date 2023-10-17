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
  src/NAMacroPrintout.c
  src/NAMacroPrintout.h
  src/NAMacroPrintoutII.h
  src/NATest.c
  src/NATest.h
  src/NATestEnvironment.h
  src/NATestII.h
  src/NATestList.h
  src/NATestString.c
  src/NATestString.h
  src/NATestStringII.h
)

source_group("" FILES ${sourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${sourceFiles})

