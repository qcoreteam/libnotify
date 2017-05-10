// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Created by zzu_softboy on 10/05/2017.

#include "gtest/gtest.h"
// for test only
#define NOTIFY_ATOMIC_FORCE_CXX11
#ifdef NOTIFY_ATOMIC_FORCE_CXX11
#  if defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1500 && (__cplusplus >= 201103L || defined(__INTEL_CXX11_MODE__))
#  elif defined(__clang__) &&  (__cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__))
#    if !__has_feature(cxx_constexpr) || !__has_feature(cxx_atomic) || !__has_include(<atomic>)
#      undef NOTIFY_ATOMIC_FORCE_CXX11
#    endif
#  elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 407 && (__cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__))
#  elif defined(_MSC_VER) && _MSC_VER >= 1900
#  else
#     undef NOTIFY_ATOMIC_FORCE_CXX11
#  endif
#  ifndef NOTIFY_ATOMIC_FORCE_CXX11
#    undef NOTIFY_ATOMIC_TEST_TYPE
#    define NOTIFY_ATOMIC_TEST_TYPE unsupported
#  endif
#endif

#include "notify/thread/Atomic.h"
#include <limits.h>
#include <wchar.h>

namespace
{


}

TEST(AtomicIntegerTest, staticChecks)
{

}