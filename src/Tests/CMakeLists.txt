# Copyright (c) 2009, Object Computing, Inc.
# All rights reserved.
# See the file license.txt for licensing information.
#
# The QuickFAST unit tests (see the *test project in src/QuickFAST.mpc).

find_package(Boost REQUIRED COMPONENTS unit_test_framework filesystem system thread date_time)

file(GLOB QUICKFAST_TEST_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(QuickFASTTest ${QUICKFAST_TEST_SOURCES})

target_link_libraries(QuickFASTTest PRIVATE
  QuickFAST
  Boost::unit_test_framework
  Boost::filesystem)

# Boost.Test wants BOOST_TEST_DYN_LINK when it is a shared library, and must
# not have it when it is static: the definition decides whether the test
# runner's main() comes from the library or from the test binary.
get_target_property(quickfast_boost_test_type Boost::unit_test_framework TYPE)
if(quickfast_boost_test_type STREQUAL "SHARED_LIBRARY")
  target_compile_definitions(QuickFASTTest PRIVATE BOOST_TEST_DYN_LINK)
endif()

if(QUICKFAST_USE_PCH)
  target_precompile_headers(QuickFASTTest PRIVATE
    ${CMAKE_SOURCE_DIR}/src/Common/QuickFASTPch.h)
endif()

add_test(NAME QuickFASTTest COMMAND QuickFASTTest)

# Several tests locate their XML templates through $QUICKFAST_ROOT rather than
# through the working directory, so the variable is supplied to ctest here.
set_tests_properties(QuickFASTTest PROPERTIES
  ENVIRONMENT QUICKFAST_ROOT=${CMAKE_SOURCE_DIR})
