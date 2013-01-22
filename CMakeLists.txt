cmake_minimum_required(VERSION 2.6)

project(GECODE)

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
target_link_libraries(gecodeminimodel gecodeset gecodekernel)
