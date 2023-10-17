cmake_minimum_required(VERSION 3.13)
project(NATestExample)
set(TARGET_NAME NATestExample)
add_executable(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
  # multi processor compilation
  target_compile_options(${TARGET_NAME} PRIVATE /MP)
  # unicode. Theoretically, needs _MBCS to be undefined, but cmake does that.
  add_definitions(-D_UNICODE)
else()
endif()



# ######### Sources ################

set(sourceFiles
  src/main.c
)

source_group("" FILES ${sourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${sourceFiles})



# ######### Linking to libraries ################

set(NATEST_DIR "../NATest")
target_include_directories(${TARGET_NAME} PRIVATE ${NATEST_DIR}/src)
add_subdirectory(${NATEST_DIR} NATest)
target_link_libraries(${TARGET_NAME} PRIVATE NATest)

