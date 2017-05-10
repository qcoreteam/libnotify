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
#define NOTIFY_ATOMIC_TEST_TYPE

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

#if !defined(NOTIFY_ATOMIC_INT32_IS_SUPPORTED)
#  error "AtomicInteger for 32-bit types must be supported!"
#endif
#if NOTIFY_POINTER_SIZE == 8 && !defined(NOTIFY_ATOMIC_INT64_IS_SUPPORTED)
#  error "AtomicInteger for 64-bit types must be supported on 64-bit builds!"
#endif

#define NOTIFY_TARGET_TEST_TYPES int, uint, long, ulong, n_ptrdiff, n_uintptr

#if (defined(__SIZEOF_WCHAR_T__) && (__SIZEOF_WCHAR_T__-0) > 2) \
    || (defined(WCHAR_MAX) && (WCHAR_MAX-0 > 0x10000))
#  define TYPE_SUPPORTED_wchar_t       1
#endif
#ifdef NOTIFY_COMPILER_UNICODE_STRINGS
#  define TYPE_SUPPORTED_char32_t      1
#endif


#ifdef NOTIFY_ATOMIC_INT16_IS_SUPPORTED
#  define TYPE_SUPPORTED_short         1
#  define TYPE_SUPPORTED_ushort        1
#  ifdef NOTIFY_COMPILER_UNICODE_STRINGS
#     define TYPE_SUPPORTED_char16_t   1
#  endif
#  ifndef TYPE_SUPPORTED_wchar_t
#     define TYPE_SUPPORTED_wchar_t    1
#  endif
#endif

#  define NOTIFY_ATOMIC_TYPE_SUPPORTED2(type)     TYPE_SUPPORTED_ ## type
#  define NOTIFY_ATOMIC_TYPE_SUPPORTED(type)      NOTIFY_ATOMIC_TYPE_SUPPORTED2(type)

#ifdef NOTIFY_ATOMIC_INT64_IS_SUPPORTED
#  define TYPE_SUPPORTED_ulonglong     1
#  define TYPE_SUPPORTED_n_longlong    1
#endif

#if NOTIFY_ATOMIC_TYPE_SUPPORTED(NOTIFY_ATOMIC_TEST_TYPE)
#  define TEST_TYPE NOTIFY_ATOMIC_TEST_TYPE
#else
#  define TEST_TYPE int
#  define NOTIFY_TEST_NOT_SUPPORTED
#endif

namespace
{

typedef signed char schar;
typedef TEST_TYPE Type;
typedef Type T; // shorthand

enum
{
   TypeIsUnsiged = Type(-1) > Type(1),
   TypeIsSigned = !TypeIsUnsiged
};

template <bool> struct LargeIntTemplate;
template <>
struct LargeIntTemplate<true>
{
   typedef n_uint64 Type;
};

template <>
struct LargeIntTemplate<false>
{
   typedef n_int64 Type;
};

typedef LargeIntTemplate<TypeIsUnsiged>::Type LargeInt;
template <typename T>
class AtomicIntegerTest : public ::testing::Test
{
};

typedef ::testing::Types<
      int,
      uint,
      long,
      ulong,
      n_ptrdiff,
      n_uintptr> AtomicDefaultValueTypes;
TYPED_TEST_CASE(AtomicIntegerTest, AtomicDefaultValueTypes);

#ifdef NOTIFY_ATOMIC_INT8_IS_SUPPORTED
template <typename T>
class AtomicIntegerInt8Test : public ::testing::Test
{
};
typedef ::testing::Types<char, uchar, schar> AtomicInt8ValueTypes;
TYPED_TEST_CASE(AtomicIntegerInt8Test, AtomicInt8ValueTypes);
#endif

}

template <typename TypeParam>
void static_checks(TypeParam &)
{
   //NOTIFY_STATIC_ASSERT(sizeof(notify::AtomicInteger<TypeParam>) == sizeof(TypeParam));
}

TYPED_TEST(AtomicIntegerTest, staticChecks)
{
   static_checks(*this, TypeParam());

}