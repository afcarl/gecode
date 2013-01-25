cmake_minimum_required(VERSION 2.6)

project(GECODE)

include(CheckCCompilerFlag)
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

macro(add_extension var ext)
  set(${var} )
  foreach (arg ${ARGN})
    set(${var} ${${var}} ${arg}.${ext})
  endforeach ()
endmacro()

macro(add_gecode_library name)
  set(sources )
  set(add_prefix FALSE)
  foreach (arg ${ARGN})
    if (arg STREQUAL "PREFIX")
      set(add_prefix TRUE)
    elseif (add_prefix)
      set(sources ${sources} gecode/${name}/${arg})
    else ()
      set(sources ${sources} ${arg})
    endif ()
  endforeach ()
  add_library(gecode${name} ${sources})
endmacro()

macro(add_prefix var prefix)
  set(${var} )
  foreach (arg ${ARGN})
    set(${var} ${${var}} ${prefix}/${arg})
  endforeach ()
endmacro()

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

include(CheckCXXCompilerFlag)
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

add_extension(sources cpp
  exception heap thread/thread thread/windows thread/pthreads)
add_extension(headers hpp
  block-allocator cast dynamic-array
  dynamic-stack exception heap
  macros random sort static-stack
  marked-pointer int-type auto-link
  thread thread/thread thread/windows thread/pthreads thread/none timer
  dynamic-queue bitset-base bitset)
add_gecode_library(support gecode/support.hh PREFIX ${sources} ${headers})

find_package(Threads)
target_link_libraries(gecodesupport ${CMAKE_THREAD_LIBS_INIT})

add_extension(sources cpp archive core memory-manager branch region)
add_extension(headers hpp
  archive array shared-array core exception
  macros memory-config memory-manager region modevent range-list
  propagator advisor view var
  branch brancher brancher-view brancher-tiebreak
  brancher-val allocators global-prop-info)
add_gecode_library(kernel gecode/kernel.hh gecode/kernel/wait.hh
  PREFIX ${sources} ${headers})
target_link_libraries(gecodekernel gecodesupport)

add_extension(sources cpp
  stop options parallel/engine dfs parallel/dfs
  bab parallel/bab restart parallel/restart)
add_gecode_library(search PREFIX ${sources}
  statistics.hpp stop.hpp options.hpp
  support.hh worker.hh
  sequential/path.hh sequential/dfs.hh sequential/bab.hh
  sequential/restart.hh
  parallel/path.hh parallel/engine.hh
  parallel/dfs.hh parallel/bab.hh
  parallel/restart.hh
  dfs.hpp bab.hpp restart.hpp)
target_link_libraries(gecodesearch gecodekernel)

add_gecode_library(int
  gecode/int/branch/post-view-int.cpp
  gecode/int/branch/post-view-bool.cpp
  gecode/int.hh
  PREFIX
  int-set.cpp var-imp/int.cpp var-imp/bool.cpp var/int.cpp
  var/bool.cpp array.cpp bool.cpp bool/eqv.cpp
  extensional/dfa.cpp extensional/tuple-set.cpp extensional.cpp
  dom.cpp rel.cpp precede.cpp element.cpp count.cpp
  arithmetic.cpp exec.cpp
  exec/when.cpp element/pair.cpp
  linear/int-post.cpp linear-int.cpp
  linear/bool-post.cpp linear-bool.cpp
  branch.cpp distinct.cpp sorted.cpp gcc.cpp
  channel.cpp channel/link-single.cpp channel/link-multi.cpp
  unshare.cpp sequence.cpp
  bin-packing.cpp bin-packing/propagate.cpp
  unary.cpp cumulative.cpp cumulatives.cpp
  circuit.cpp no-overlap.cpp nvalues.cpp
  member.cpp
  limits.hpp support-values.hh support-values.hpp
  exec.hh exec/when.hpp
  arithmetic/abs.hpp arithmetic/max.hpp arithmetic/mult.hpp
  arithmetic/sqr.hpp arithmetic/sqrt.hpp arithmetic/divmod.hpp
  bool/or.hpp bool/eq.hpp bool/lq.hpp bool/eqv.hpp bool/base.hpp
  bool/clause.hpp
  precede.hh precede/single.hpp
  branch/post-val-int.hpp branch/post-val-bool.hpp
  branch/select-val.hpp
  branch/select-values.hpp branch/select-view.hpp
  count.hh count/rel.hpp
  count/int-base.hpp count/int-eq.hpp
  count/int-gq.hpp count/int-lq.hpp
  count/view-base.hpp count/view-eq.hpp
  count/view-gq.hpp count/view-lq.hpp
  view-val-graph.hh view-val-graph/graph.hpp
  view-val-graph/bi-link.hpp view-val-graph/comb-ptr-flag.hpp
  view-val-graph/edge.hpp view-val-graph/node.hpp
  view-val-graph/iter-prune-val.hpp
  distinct/graph.hpp distinct/dom-ctrl.hpp
  distinct/bnd.hpp distinct/dom.hpp
  distinct/val.hpp distinct/ter-dom.hpp
  dom/range.hpp dom/spec.hpp
  element/int.hpp element/view.hpp element/pair.hpp
  gcc/bnd.hpp gcc/dom.hpp gcc/bnd-sup.hpp gcc/dom-sup.hpp
  gcc/val.hpp gcc/view.hpp gcc/post.hpp
  linear/post.hpp
  linear/int-noview.hpp linear/int-bin.hpp linear/int-ter.hpp
  linear/int-nary.hpp linear/int-dom.hpp
  linear/bool-int.hpp linear/bool-view.hpp linear/bool-scale.hpp
  extensional/dfa.hpp extensional/layered-graph.hpp
  extensional/tuple-set.hpp extensional/base.hpp
  extensional/basic.hpp extensional/incremental.hpp
  rel/eq.hpp rel/lex.hpp rel/lq-le.hpp rel/nq.hpp
  sorted/matching.hpp sorted/narrowing.hpp
  sorted/order.hpp sorted/propagate.hpp sorted/sortsup.hpp
  int-set-1.hpp int-set-2.hpp var-imp/delta.hpp var/print.hpp var/bool.hpp
  var-imp/int.hpp var-imp/bool.hpp var/int.hpp
  view/bool.hpp view/cached.hpp view/constint.hpp view/zero.hpp view/int.hpp
  view/minus.hpp view/print.hpp view/neg-bool.hpp view/bool-test.hpp
  view/offset.hpp view/rel-test.hpp view/scale.hpp view/iter.hpp
  arithmetic.hh array.hpp array-traits.hpp bool.hh branch.hh
  distinct.hh dom.hh
  element.hh exception.hpp  gcc.hh linear.hh
  propagator.hpp extensional.hh rel.hh
  sorted.hh var-imp.hpp view.hpp
  channel.hh channel/dom.hpp channel/val.hpp
  channel/base.hpp channel/link-single.hpp channel/link-multi.hpp
  sequence.hh sequence/int.hpp sequence/view.hpp
  sequence/set-op.hpp sequence/violations.hpp
  bin-packing.hh bin-packing/propagate.hpp
  task.hh task/fwd-to-bwd.hpp task/array.hpp task/sort.hpp
  task/iter.hpp task/tree.hpp task/purge.hpp task/prop.hpp
  task/man-to-opt.hpp
  unary.hh unary/task.hpp unary/task-view.hpp
  unary/tree.hpp unary/overload.hpp unary/detectable.hpp
  unary/not-first-not-last.hpp
  unary/edge-finding.hpp unary/subsumption.hpp
  unary/man-prop.hpp unary/opt-prop.hpp
  cumulative.hh cumulative/man-prop.hpp cumulative/opt-prop.hpp
  cumulative/task-view.hpp cumulative/overload.hpp
  cumulative/basic.hpp cumulative/task.hpp cumulative/edge-finding.hpp
  cumulative/tree.hpp
  cumulatives.hh cumulatives/val.hpp
  circuit.hh circuit/base.hpp circuit/val.hpp circuit/dom.hpp
  no-overlap.hh no-overlap/dim.hpp no-overlap/box.hpp
  no-overlap/base.hpp no-overlap/man.hpp no-overlap/opt.hpp
  nvalues.hh nvalues/range-event.hpp
  nvalues/sym-bit-matrix.hpp nvalues/graph.hpp
  nvalues/bool-base.hpp nvalues/bool-eq.hpp
  nvalues/bool-lq.hpp nvalues/bool-gq.hpp
  nvalues/int-base.hpp nvalues/int-eq.hpp
  nvalues/int-lq.hpp nvalues/int-gq.hpp
  val-set.hh val-set.hpp
  member.hh member/prop.hpp member/re-prop.hpp)
target_link_libraries(gecodeint gecodekernel)

add_gecode_library(set
  gecode/set/branch/post-view.cpp gecode/set.hh
  PREFIX
  branch.cpp cardinality.cpp convex.cpp convex/conv.cpp convex/hull.cpp
  dom.cpp rel.cpp
  rel-op.cpp rel-op-ternary.cpp rel-op-singleton.cpp
  rel-op/post-compl.cpp rel-op/post-nocompl.cpp
  rel-op/post-compl-cvv.cpp rel-op/post-nocompl-cvv.cpp
  rel-op/post-compl-vvc.cpp rel-op/post-nocompl-vvc.cpp
  rel-op/post-compl-cvc.cpp rel-op/post-nocompl-cvc.cpp
  rel-op-const-vvc.cpp rel-op-const-vcv.cpp rel-op-const-vcc.cpp
  rel-op-const-cvc.cpp rel-op-const-cvv.cpp
  int.cpp
  element.cpp sequence.cpp
  distinct.cpp distinct/atmostOne.cpp
  precede.cpp
  sequence/seq.cpp sequence/seq-u.cpp array.cpp var-imp/set.cpp
  var-imp/integerset.cpp var/set.cpp
  exec.cpp
  limits.hpp
  view.hpp exception.hpp int.hh element.hh var-imp.hpp
  int/card.hpp
  int/match.hpp int/channel-int.hpp int/minmax.hpp int/weights.hpp
  int/channel-bool.hpp
  element/inter.hpp element/union.hpp
  element/unionConst.hpp
  element/disjoint.hpp array.hpp array-traits.hpp
  var-imp/set.hpp var-imp/integerset.hpp
  var-imp/delta.hpp var-imp/iter.hpp
  var/set.hpp var/print.hpp
  view/complement.hpp view/const.hpp view/set.hpp view/singleton.hpp
  view/cached.hpp view/print.hpp
  rel/subset.hpp rel/re-subset.hpp rel/eq.hpp rel/nq.hpp rel/re-eq.hpp
  rel/lq.hpp rel/re-lq.hpp
  rel/common.hpp rel/nosubset.hpp sequence.hh
  rel-op/union.hpp rel-op/partition.hpp rel-op/subofunion.hpp
  rel-op/inter.hpp rel-op/superofinter.hpp
  rel-op/post.hpp rel-op/common.hpp rel.hh
  distinct.hh distinct/atmostOne.hpp
  sequence/seq.hpp sequence/seq-u.hpp
  sequence/common.hpp convex/conv.hpp convex/hull.hpp convex.hh
  precede.hh precede/single.hpp
  rel-op.hh
  branch.hh branch/select-val.hpp branch/select-view.hpp
  branch/post-val.hpp)
target_link_libraries(gecodeset gecodekernel)

add_gecode_library(minimodel gecode/minimodel.hh PREFIX
  lin-expr.cpp lin-rel.cpp bool-expr.cpp
  set-expr.cpp set-rel.cpp
  reg.cpp arithmetic.cpp
  lin-expr.hpp lin-rel.hpp exception.hpp matrix.hpp
  bool-expr.hpp set-expr.hpp set-rel.hpp
  optimize.hpp reg.hpp)
target_link_libraries(gecodeminimodel gecodeint gecodeset gecodesearch)

add_extension(int_test_sources cpp
  channel arithmetic basic bool count dom distinct element extensional
  rel linear gcc sorted unshare exec sequence
  mm-arithmetic mm-bool mm-lin mm-count mm-rel
  bin-packing unary cumulative cumulatives circuit
  no-overlap precede nvalues member)
add_prefix(int_test_sources test/int ${int_test_sources})

add_extension(set_test_sources cpp
  construct dom rel rel-op rel-op-const convex sequence int element
  precede distinct exec mm-set)
add_prefix(set_test_sources test/set ${set_test_sources})

add_prefix(headers test
  test.hh test.hpp int.hh int.hpp set.hh set.hpp
  branch.hh assign.hh)

include_directories(.)
add_executable(gecode-test test/int.cpp ${int_test_sources}
  test/set.cpp ${set_test_sources}
  test/branch.cpp test/branch/int.cpp test/branch/bool.cpp
  test/branch/set.cpp
  test/assign.cpp test/assign/int.cpp test/assign/bool.cpp
  test/assign/set.cpp
  test/search.cpp
  test/array.cpp
  test/test.cpp test/afc.cpp
  ${headers})
target_link_libraries(gecode-test gecodeminimodel)

enable_testing()
add_test(test gecode-test
  -iter 2 -test Branch::Int::Dense::3
  -test Int::Linear::Int::Int::Eq::Bnd::12::4
  -test Int::Distinct::Random
  -test Int::Arithmetic::Mult::XYZ::Bnd::C
  -test Int::Arithmetic::Mult::XYZ::Dom::A
  -test Search::BAB::Sol::BalGr::Binary::Binary::Binary::1::1)
