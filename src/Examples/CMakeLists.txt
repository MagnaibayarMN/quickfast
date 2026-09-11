# Copyright (c) 2009, 2010, 2011, Object Computing, Inc.
# All rights reserved.
# See the file license.txt for licensing information.
#
# The QuickFAST example programs (see src/Examples/Examples.mpc).

# Code shared by all of the examples; QuickFASTExample.mpb folds these sources
# into every example program, so a static helper library is used here instead.
file(GLOB QUICKFAST_EXAMPLES_SUPPORT CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/Examples/*.cpp)

add_library(QuickFASTExamplesSupport STATIC ${QUICKFAST_EXAMPLES_SUPPORT})
target_include_directories(QuickFASTExamplesSupport PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(QuickFASTExamplesSupport PUBLIC QuickFAST)

if(QUICKFAST_USE_PCH)
  target_precompile_headers(QuickFASTExamplesSupport PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Examples/ExamplesPch.h)
endif()

foreach(example
    InterpretApplication
    TutorialApplication
    PerformanceTest
    FileToTCP
    FileToMulticast
    PCapToMulticast)
  file(GLOB ${example}_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/${example}/*.cpp)
  add_executable(${example} ${${example}_SOURCES})
  target_link_libraries(${example} PRIVATE QuickFASTExamplesSupport)
  if(QUICKFAST_USE_PCH)
    target_precompile_headers(${example} REUSE_FROM QuickFASTExamplesSupport)
  endif()
endforeach()
