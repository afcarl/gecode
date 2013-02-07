# CMake build script for Gecode.
# Author: Victor Zverovich

cmake_minimum_required(VERSION 2.6)

project(GECODE)

include(CheckCXXCompilerFlag)
if (GECODE_DISABLE_WARNINGS)
  if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wno-overloaded-virtual)
    add_definitions(-Wno-switch)
    check_cxx_compiler_flag(
      -Wno-unused-but-set-variable HAVE_WNO_UNUSED_BUT_SET_VARIABLE_FLAG)
    if (HAVE_WNO_UNUSED_BUT_SET_VARIABLE_FLAG)
      add_definitions(-Wno-unused-but-set-variable)
    endif ()
  elseif (MSVC)
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      string(REPLACE "/W3" "/w" ${flag_var} "${${flag_var}}")
    endforeach()
  endif ()
endif ()

# The following part of config.h is hard to derive from configure.ac.
string(REGEX MATCHALL "[^\n]*\n" CONFIG
"/* Whether to include audit code */
#undef GECODE_AUDIT

/* User-defined prefix of dll names */
#undef GECODE_DLL_USERPREFIX

/* User-defined suffix of dll names */
#undef GECODE_DLL_USERSUFFIX

/* Supported version of FlatZinc */
#undef GECODE_FLATZINC_VERSION

/* Whether gcc understands visibility attributes */
#undef GECODE_GCC_HAS_CLASS_VISIBILITY

/* Whether to compile boost dependent parts */
#undef GECODE_HAS_BOOST

/* whether __builtin_ffsl is available */
#undef GECODE_HAS_BUILTIN_FFSL

/* Whether Gist is available */
#undef GECODE_HAS_GIST

/* Whether GNU hash_map is available */
#undef GECODE_HAS_GNU_HASH_MAP

/* Whether to build INT variables */
#undef GECODE_HAS_INT_VARS

/* Whether we have mtrace for memory leak debugging */
#undef GECODE_HAS_MTRACE

/* Whether Qt is available */
#undef GECODE_HAS_QT

/* Whether to build SET variables */
#undef GECODE_HAS_SET_VARS

/* Whether unistd.h is available */
#undef GECODE_HAS_UNISTD_H

/* Gecode version */
#undef GECODE_LIBRARY_VERSION

/* Heap memory alignment */
#undef GECODE_MEMORY_ALIGNMENT

/* Whether we are compiling static libraries */
#undef GECODE_STATIC_LIBS

/* Whether we have posix threads */
#undef GECODE_THREADS_PTHREADS

/* Whether we have windows threads */
#undef GECODE_THREADS_WINDOWS

/* Use clock() for time-measurement */
#undef GECODE_USE_CLOCK

/* Use gettimeofday for time-measurement */
#undef GECODE_USE_GETTIMEOFDAY

/* Gecode version */
#undef GECODE_VERSION

/* Gecode version */
#undef GECODE_VERSION_NUMBER

/* Define to 1 if you have the `getpagesize' function. */
#undef HAVE_GETPAGESIZE

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define to 1 if you have the <memory.h> header file. */
#undef HAVE_MEMORY_H

/* Define to 1 if you have a working `mmap' system call. */
#undef HAVE_MMAP

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#undef HAVE_STDLIB_H

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#undef HAVE_STRING_H

/* Define to 1 if you have the <sys/param.h> header file. */
#undef HAVE_SYS_PARAM_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/types.h> header file. */
#undef HAVE_SYS_TYPES_H

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the home page for this package. */
#undef PACKAGE_URL

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* The size of `int', as computed by sizeof. */
#undef SIZEOF_INT

/* Define to 1 if you have the ANSI C header files. */
#undef STDC_HEADERS

/* How to tell the compiler to really, really inline */
#undef forceinline
")

# Get version numbers and parts of config.h from configure.ac.
file(READ configure.ac LINES)
# Replace semicolons with "<semi>" to avoid CMake messing with them.
string(REPLACE ";" "<semi>" LINES "${LINES}")
# Split into lines keeping newlines to avoid foreach skipping empty ones.
string(REGEX MATCHALL "[^\n]*\n" LINES "${LINES}")
set(ah_command FALSE)
foreach (line "${EXTRA_CONFIG}" ${LINES})
  string(REPLACE ";" "" line "${line}")
  if (ah_command)
    # Do nothing.
  elseif (line MATCHES "AC_INIT\\(([^,]*), *([^,]*), *([^)]*)\\)")
    set(PACKAGE ${CMAKE_MATCH_1})
    set(VERSION ${CMAKE_MATCH_2})
    set(PACKAGE_BUGREPORT ${CMAKE_MATCH_3})
    message(STATUS "Got VERSION=${VERSION} from configure.ac")
  elseif (line MATCHES "ac_gecode_flatzincversion=(.*)\n")
    set(GECODE_FLATZINC_VERSION "${CMAKE_MATCH_1}")
  elseif (line MATCHES "AH_BOTTOM\\(\\[(.*)")
    set(ah_command bottom)
    set(line "${CMAKE_MATCH_1}")
  elseif (line MATCHES "AH_VERBATIM[^,]+,(.*)")
    set(ah_command verbatim)
    set(line "${CMAKE_MATCH_1}")
  endif ()
  if (ah_command)
    set(saved_ah_command ${ah_command})
    if (line MATCHES "^\\[(.*)")
      set(line "${CMAKE_MATCH_1}")
    endif ()
    if (line MATCHES "\\]\\)")
      set(ah_command FALSE)
      string(REPLACE "])" "" line "${line}")
    endif ()
    # For some reason CMake may bundle several lines together. Split them too.
    string(REGEX MATCHALL "[^\n]*\n" sublines "${line}")
    set(config_add "")
    foreach (subline ${sublines})
      set(config_add ${config_add} "${subline}")
    endforeach ()
    if (saved_ah_command STREQUAL "verbatim")
      set(CONFIG ${config_add} ${CONFIG})
    else ()
      set(CONFIG ${CONFIG} "\n" ${config_add})
    endif ()
  endif ()
endforeach ()
set(PACKAGE_NAME ${PACKAGE})
string(TOLOWER ${PACKAGE} PACKAGE_TARNAME)
set(PACKAGE_URL "")
set(PACKAGE_VERSION ${VERSION})
set(${PACKAGE}_VERSION ${VERSION})
string(REPLACE "." "-" GECODE_LIBRARY_VERSION "${VERSION}")
set(PACKAGE_STRING "${PACKAGE} ${VERSION}")
if (VERSION MATCHES "(.*)\\.(.*)\\.(.*)")
  math(EXPR GECODE_VERSION_NUMBER
    "${CMAKE_MATCH_1} * 100000 + ${CMAKE_MATCH_2} * 100 + ${CMAKE_MATCH_3}")
endif ()

set(GECODE_DLL_USERPREFIX "")
set(GECODE_DLL_USERSUFFIX "")
set(GECODE_HAS_INT_VARS "/**/")
set(GECODE_HAS_SET_VARS "/**/")
set(GECODE_STATIC_LIBS 1)

check_cxx_compiler_flag(-fvisibility=hidden HAVE_VISIBILITY_HIDDEN_FLAG)
if (HAVE_VISIBILITY_HIDDEN_FLAG)
  set(GECODE_GCC_HAS_CLASS_VISIBILITY "/**/")
endif ()

if (WIN32)
  set(GECODE_THREADS_WINDOWS 1)
else ()
  set(GECODE_THREADS_PTHREADS 1)
endif ()

find_package(Qt4)
if (QT4_FOUND)
  set(GECODE_HAS_QT "/**/")
  set(GECODE_HAS_GIST "/**/")
endif ()

include(CheckSymbolExists)
check_symbol_exists(getpagesize unistd.h HAVE_GETPAGESIZE)
check_symbol_exists(mmap sys/mman.h HAVE_MMAP)

# Checks for header files.
include(CheckIncludeFiles)
foreach (header inttypes.h memory.h stdint.h stdlib.h strings.h string.h
                sys/param.h sys/stat.h sys/time.h sys/types.h unistd.h)
  string(TOUPPER HAVE_${header} var)
  string(REGEX REPLACE "\\.|/" "_" var ${var})
  check_include_files(${header} ${var})
endforeach ()
check_include_files(stdio.h STDC_HEADERS)
if (HAVE_SYS_TIME_H)
  set(GECODE_USE_GETTIMEOFDAY 1)
else ()
  set(GECODE_USE_CLOCK 1)
endif ()
if (HAVE_UNISTD_H)
  set(GECODE_HAS_UNISTD_H 1)
endif ()

include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
   #include <ext/hash_map>
   int main() {}" HAVE_EXT_HASH_MAP)
if (HAVE_EXT_HASH_MAP)
  set(GECODE_HAS_GNU_HASH_MAP "/**/")
endif ()

include(CheckTypeSize)
check_type_size(int SIZEOF_INT)

# Check for inline.
include(CheckCSourceCompiles)
check_c_source_compiles("
  inline __attribute__ ((__always_inline__)) void foo(void) {}
  int main() {}" HAVE_ALWAYS_INLINE)
set(forceinline inline)
if (HAVE_ALWAYS_INLINE)
  set(forceinline "inline __attribute__ ((__always_inline__))")
endif ()

check_c_source_compiles("
  int main() { return __builtin_ffsl(0); }" HAVE_BUILTIN_FFSL)
if (HAVE_BUILTIN_FFSL)
  set(GECODE_HAS_BUILTIN_FFSL "/**/")
endif ()

# Process config.hpp using autoconf rules.
list(LENGTH CONFIG length)
math(EXPR length "${length} - 1")
foreach (i RANGE ${length})
  list(GET CONFIG ${i} line)
  if (line MATCHES "^#( *)undef (.*)\n")
    set(space "${CMAKE_MATCH_1}")
    set(var ${CMAKE_MATCH_2})
    if (NOT DEFINED ${var} OR (var MATCHES "HAVE_.*_H" AND NOT ${var}))
      set(line "/* #${space}undef ${var} */\n")
    else ()
      if ("${${var}}" STREQUAL "/**/" OR "${var}" STREQUAL "GECODE_VERSION_NUMBER" OR
          "${var}" STREQUAL "forceinline" OR var MATCHES "SIZEOF_.*")
        set(value ${${var}})
      elseif (NOT (var MATCHES ^HAVE OR ${var} EQUAL 0 OR ${var} EQUAL 1))
        set(value \"${${var}}\")
      elseif (${var})
        set(value 1)
      else ()
        set(value 0)
      endif ()
      set(line "#${space}define ${var} ${value}\n")
    endif ()
  endif ()
  string(REPLACE "<semi>" ";" line "${line}")
  set(CONFIG_OUT "${CONFIG_OUT}${line}")
endforeach ()
file(WRITE ${GECODE_BINARY_DIR}/gecode/support/config.hpp
"/* gecode/support/config.hpp.  Generated from config.hpp.in by configure.  */
/* gecode/support/config.hpp.in.  Generated from configure.ac by autoheader.  */

/* Disable autolink because all the dependencies are handled by CMake. */
#define GECODE_BUILD_SUPPORT
#define GECODE_BUILD_KERNEL
#define GECODE_BUILD_SEARCH
#define GECODE_BUILD_INT
#define GECODE_BUILD_SET
#define GECODE_BUILD_MINIMODEL

${CONFIG_OUT}")

# Expands a value substituting variables and appends the result to ${var}.
function (expand var value)
  if (value MATCHES "\\$\\(([^:]+)(.*)\\)")
    # Perform substitution.
    set(pattern ${CMAKE_MATCH_2})
    set(items ${${CMAKE_MATCH_1}})
    if (pattern MATCHES ":%=([^%]*)%([^%]*)")
      set(values )
      foreach (item ${items})
        set(values ${values} ${CMAKE_MATCH_1}${item}${CMAKE_MATCH_2})
      endforeach ()
    else ()
      set(values ${items})
    endif ()
  else ()
    set(values ${value})
  endif ()
  set(${var} ${${var}} ${values} PARENT_SCOPE)
endfunction ()

# Parse Makefile.in extracting variables.
file(READ Makefile.in text)
string(REPLACE "\\\n" "" text "${text}")
string(REGEX REPLACE "#[^\n]*\n" "" text "${text}")
string(REGEX MATCHALL "[^\n]+" lines "${text}")
foreach (line ${lines})
  if (line MATCHES "([^ \t]+)[ \t]*=[ \t]*(.*)")
    set(var ${CMAKE_MATCH_1})
    set(${var} )
    string(REGEX MATCHALL "[^ \t]+" items "${CMAKE_MATCH_2}")
    foreach (item ${items})
      expand(${var} ${item})
    endforeach ()
  endif ()
endforeach ()

foreach (lib support kernel search int set minimodel)
  if (lib STREQUAL "minimodel")
    set(libupper MM)
  else ()
    string(TOUPPER ${lib} libupper)
  endif ()
  add_library(gecode${lib} ${${libupper}SRC} ${${libupper}HDR})
endforeach ()

find_package(Threads)
target_link_libraries(gecodesupport ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(gecodekernel gecodesupport)
target_link_libraries(gecodesearch gecodekernel)
target_link_libraries(gecodeint gecodekernel)
target_link_libraries(gecodeset gecodekernel)
target_link_libraries(gecodeminimodel gecodeint gecodeset gecodesearch)

include_directories(.)
add_executable(gecode-test ${TESTSRC} ${TESTHDR})
target_link_libraries(gecode-test gecodeminimodel)

enable_testing()
add_test(test gecode-test
  -iter 2 -test Branch::Int::Dense::3
  -test Int::Linear::Int::Int::Eq::Bnd::12::4
  -test Int::Distinct::Random
  -test Int::Arithmetic::Mult::XYZ::Bnd::C
  -test Int::Arithmetic::Mult::XYZ::Dom::A
  -test Search::BAB::Sol::BalGr::Binary::Binary::Binary::1::1)
