# Copyright (c) 2009, Object Computing, Inc.
# All rights reserved.
# See the file license.txt for licensing information.
#
# Optional CMake build for QuickFAST.
#
# This is an *addition* to the existing MPC build, not a replacement: no MPC
# file, setup script or source file is modified by this build system, and both
# builds can be used side by side from the same working copy.
#
# Quick start:
#   cmake -S . -B build -DQUICKFAST_BUILD_TESTS=ON
#   cmake --build build
#   ctest --test-dir build --output-on-failure
#
# Boost and Xerces-C are located with find_package, so they can come from the
# platform packages, from a local build or from a package manager.

cmake_minimum_required(VERSION 3.16)

project(QuickFAST
  VERSION 1.5.0
  DESCRIPTION "An implementation of the FAST protocol for native C++"
  HOMEPAGE_URL "https://github.com/objectcomputing/quickfast"
  LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(QUICKFAST_BUILD_TESTS "Build the QuickFAST unit tests" OFF)
option(QUICKFAST_BUILD_EXAMPLES "Build the QuickFAST example programs" OFF)
option(QUICKFAST_USE_PCH "Use a precompiled header (mirrors the MPC build)" ON)

# QuickFAST predates C++11.  C++11 is the lowest standard that current Boost
# releases support, so it is used as the floor rather than the compiler default.
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# CMake 3.30 removed its own FindBoost in favour of the config file Boost
# itself installs.  Ask for that behaviour where the policy exists.
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost REQUIRED COMPONENTS system thread date_time)
find_package(XercesC REQUIRED)

# ---------------------------------------------------------------------------
# The QuickFAST library
#
# The MPC build collects whole directories (see src/QuickFAST.mpc), so globbing
# is used here to keep the two builds from drifting apart when files are added.
# ---------------------------------------------------------------------------
file(GLOB_RECURSE QUICKFAST_SOURCES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Application/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Codecs/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Communication/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Messages/*.cpp)

file(GLOB_RECURSE QUICKFAST_HEADERS CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Application/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Codecs/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Communication/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Messages/*.h)

add_library(QuickFAST ${QUICKFAST_SOURCES} ${QUICKFAST_HEADERS})
add_library(QuickFAST::QuickFAST ALIAS QuickFAST)

target_include_directories(QuickFAST PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/QuickFAST>)

target_link_libraries(QuickFAST PUBLIC
  Boost::system
  Boost::thread
  Boost::date_time
  XercesC::XercesC)

set_target_properties(QuickFAST PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  OUTPUT_NAME QuickFAST)

# src/Common/QuickFAST_Export.h decides between dllexport, dllimport and
# nothing.  These definitions give it the same answers the MPC build does.
if(BUILD_SHARED_LIBS)
  target_compile_definitions(QuickFAST PRIVATE QUICKFAST_BUILD_DLL)
else()
  target_compile_definitions(QuickFAST PUBLIC QUICKFAST_HAS_DLL=0)
endif()

if(MSVC)
  target_compile_definitions(QuickFAST PUBLIC
    BOOST_DATE_TIME_NO_LIB
    BOOST_REGEX_NO_LIB
    _WIN32_WINNT=0x0601)
  # QuickFAST predates several of the warnings current MSVC emits; the
  # precompiled header already disables the ones the authors reviewed.
  target_compile_options(QuickFAST PRIVATE /wd4267 /wd4244)
else()
  # FieldInstructionInteger generates spurious warnings based on
  # signed/unsigned template arguments (see QuickFASTApplication.mpb).
  target_compile_options(QuickFAST PRIVATE -Wtype-limits)
endif()

if(QUICKFAST_USE_PCH)
  target_precompile_headers(QuickFAST PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/QuickFASTPch.h)
endif()

# ---------------------------------------------------------------------------
# Installation and the QuickFASTConfig.cmake package
# ---------------------------------------------------------------------------
install(TARGETS QuickFAST
  EXPORT QuickFASTTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Headers are installed under a QuickFAST/ prefix so that the include style used
# throughout the sources -- #include <Codecs/Decoder.h> -- keeps working without
# putting directories named Common or Messages on a consumer's include path.
foreach(module Application Codecs Common Communication Messages)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${module}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/QuickFAST
    FILES_MATCHING PATTERN "*.h")
endforeach()

install(EXPORT QuickFASTTargets
  FILE QuickFASTTargets.cmake
  NAMESPACE QuickFAST::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QuickFAST)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/QuickFASTConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/QuickFASTConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QuickFAST)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/QuickFASTConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/QuickFASTConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/QuickFASTConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QuickFAST)

# ---------------------------------------------------------------------------
# Optional components
# ---------------------------------------------------------------------------
if(QUICKFAST_BUILD_TESTS)
  enable_testing()
  add_subdirectory(src/Tests)
endif()

if(QUICKFAST_BUILD_EXAMPLES)
  add_subdirectory(src/Examples)
endif()
