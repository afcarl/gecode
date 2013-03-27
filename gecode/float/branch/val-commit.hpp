/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Contributing authors:
 *     Vincent Barichard <Vincent.Barichard@univ-angers.fr>
 *
 *  Copyright:
 *     Christian Schulte, 2012
 *     Vincent Barichard, 2012
 *
 *  Last modified:
 *     $Date: 2012-10-02 15:49:50 +0200 (Tue, 02 Oct 2012) $ by $Author: schulte $
 *     $Revision: 13123 $
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

namespace Gecode { namespace Float { namespace Branch {

  forceinline
  ValCommitLq::ValCommitLq(Space& home, const ValBranch& vb)
    : ValCommit<FloatView,FloatNum>(home,vb) {}
  forceinline
  ValCommitLq::ValCommitLq(Space& home, bool shared, ValCommitLq& vc)
    : ValCommit<FloatView,FloatNum>(home,shared,vc) {}
  forceinline ModEvent
  ValCommitLq::commit(Space& home, unsigned int a, FloatView x, int, 
                      FloatNum n) {
    if (a == 0) {
      if ((x.min() == n) || (x.max() == n)) 
        return x.eq(home,x.min());
      else 
        return x.lq(home,n);
    } else {
      if ((x.min() == n) || (x.max() == n)) 
        return x.eq(home,x.max());
      else
        return x.gq(home,n);
    }
  }

  forceinline
  ValCommitGq::ValCommitGq(Space& home, const ValBranch& vb)
    : ValCommit<FloatView,FloatNum>(home,vb) {}
  forceinline
  ValCommitGq::ValCommitGq(Space& home, bool shared, ValCommitGq& vc)
    : ValCommit<FloatView,FloatNum>(home,shared,vc) {}
  forceinline ModEvent
  ValCommitGq::commit(Space& home, unsigned int a, FloatView x, int,
                      FloatNum n) {
    if (a == 0) {
      if ((x.min() == n) || (x.max() == n)) 
        return x.eq(home,x.max());
      else 
        return x.gq(home,n);
    } else {
      if ((x.min() == n) || (x.max() == n)) 
        return x.eq(home,x.min());
      else
        return x.lq(home,n);
    }
  }

  forceinline
  ValCommitLqGq::ValCommitLqGq(Space& home, const ValBranch& vb)
    : ValCommit<FloatView,std::pair<FloatNum,bool> >(home,vb) {}
  forceinline
  ValCommitLqGq::ValCommitLqGq(Space& home, bool shared, ValCommitLqGq& vc)
    : ValCommit<FloatView,std::pair<FloatNum,bool> >(home,shared,vc) {}
  forceinline ModEvent
  ValCommitLqGq::commit(Space& home, unsigned int a, FloatView x, int, 
                        Val nb) {
    FloatNum& n = nb.first;
    bool& b = nb.second;
    if ((a == 0) == b) {
      if ((x.min() == n) || (x.max() == n)) 
        return x.eq(home,x.min());
      else 
        return x.lq(home,n);
    } else {
      if ((x.min() == n) || (x.max() == n))
        return x.eq(home,x.max());
      else 
        return x.gq(home,n);
    }
  }

}}}

// STATISTICS: float-branch

