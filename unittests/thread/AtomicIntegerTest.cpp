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
#include <vector>
#include <iostream>

#if !defined(NOTIFY_ATOMIC_INT32_IS_SUPPORTED)
#  error "AtomicInteger for 32-bit types must be supported!"
#endif
#if NOTIFY_POINTER_SIZE == 8 && !defined(NOTIFY_ATOMIC_INT64_IS_SUPPORTED)
#  error "AtomicInteger for 64-bit types must be supported on 64-bit builds!"
#endif

#ifdef NOTIFY_COMPILER_UNICODE_STRINGS
#  define NOTIFY_ATOMIC_UNICODE32_TEST_TYPES char32_t
#endif

namespace
{

typedef signed char schar;

#define NOTIFY_ATOMIC_BASIC_TEST_TYPES int, uint, long, ulong, n_ptrdiff, n_uintptr

#ifdef NOTIFY_ATOMIC_INT8_IS_SUPPORTED
#define NOTIFY_ATOMIC_INT8_TEST_TYPES char, uchar, schar
#endif

#ifdef NOTIFY_ATOMIC_INT16_IS_SUPPORTED
#  define NOTIFY_ATOMIC_INT16_TEST_TYPES short, ushort, wchar_t
#  ifdef NOTIFY_COMPILER_UNICODE_STRINGS
#     define NOTIFY_ATOMIC_UNICODE16_TEST_TYPES char16_t
#  endif
#endif

#ifdef NOTIFY_ATOMIC_INT64_IS_SUPPORTED
#  define NOTIFY_ATOMIC_INT64_TEST_TYPES n_longlong, n_ulonglong
#endif

template <bool> struct LargeIntTemplate;
template <>     struct LargeIntTemplate<true>  { typedef n_uint64 Type; };
template <>     struct LargeIntTemplate<false> { typedef n_int64 Type; };


template <typename T>
class AtomicIntegerTest : public ::testing::Test
{
public:
   enum {
      TypeIsUnsigned = T(-1) > T(0),
      TypeIsSigned = !TypeIsUnsigned
   };
   typedef typename LargeIntTemplate<TypeIsUnsigned>::Type LargeInt;
   typedef std::numeric_limits<T> Limits;
protected:
   static std::vector<LargeInt> m_backendValues;
public:
   static void SetUpTestCase();
};

template<typename T>
std::vector<typename AtomicIntegerTest<T>::LargeInt> AtomicIntegerTest<T>::m_backendValues;

template <typename T>
void AtomicIntegerTest<T>::SetUpTestCase()
{
   m_backendValues.push_back(LargeInt(0));
   m_backendValues.push_back(LargeInt(1));
   m_backendValues.push_back(LargeInt(23));
   if (TypeIsSigned) {
      m_backendValues.push_back(n_int64(-1));
      m_backendValues.push_back(n_int64(-50));
   }

   if (TypeIsSigned && Limits::min() < n_int64(SCHAR_MIN)) {
      m_backendValues.push_back(n_int64(SCHAR_MIN));
   }
   if (Limits::max() > LargeInt(SCHAR_MAX)) {
      m_backendValues.push_back(LargeInt(SCHAR_MAX));
   }
   if (Limits::max() > LargeInt(UCHAR_MAX)) {
      m_backendValues.push_back(LargeInt(UCHAR_MAX));
   }
   if (TypeIsSigned && Limits::min() < -n_int64(UCHAR_MAX)) {
      m_backendValues.push_back(-n_int64(UCHAR_MAX));
   }
   if (Limits::max() > LargeInt(SHRT_MAX)) {
      m_backendValues.push_back(LargeInt(SHRT_MAX));
   }
   if (TypeIsSigned && Limits::min() < n_int64(SHRT_MIN)) {
      m_backendValues.push_back(n_int64(SHRT_MAX));
   }
   if (Limits::max() > LargeInt(USHRT_MAX)) {
      m_backendValues.push_back(LargeInt(USHRT_MAX));
   }
   if (TypeIsSigned && Limits::min() < -n_int64(USHRT_MAX)) {
      m_backendValues.push_back(-n_int64(USHRT_MAX));
   }
   if (Limits::max() > LargeInt(INT_MAX)) {
      m_backendValues.push_back(LargeInt(INT_MAX));
   }
   if (TypeIsSigned && Limits::min() < n_int64(INT_MIN)) {
      m_backendValues.push_back(n_int64(INT_MIN));
   }
   if (Limits::max() > LargeInt(UINT_MAX)) {
      m_backendValues.push_back(LargeInt(UINT_MAX));
   }
   if (TypeIsSigned && Limits::min() < -n_int64(UINT_MAX)) {
      m_backendValues.push_back(-n_int64(UINT_MAX));
   }
   if (Limits::max() > LargeInt(std::numeric_limits<n_int64>::max())) {
      m_backendValues.push_back(LargeInt(std::numeric_limits<n_int64>::max()));
   }
   if (TypeIsSigned) {
      m_backendValues.push_back(n_int64(Limits::min()));
   }
   m_backendValues.push_back(LargeInt(Limits::max()));
}

typedef ::testing::Types<
      NOTIFY_ATOMIC_BASIC_TEST_TYPES
#ifdef NOTIFY_ATOMIC_INT8_TEST_TYPES
      ,NOTIFY_ATOMIC_INT8_TEST_TYPES
#endif
#ifdef NOTIFY_ATOMIC_INT16_TEST_TYPES
      ,NOTIFY_ATOMIC_INT16_TEST_TYPES
#endif
#ifdef NOTIFY_ATOMIC_UNICODE16_TEST_TYPES
      ,NOTIFY_ATOMIC_UNICODE16_TEST_TYPES
#endif
#ifdef NOTIFY_ATOMIC_UNICODE32_TEST_TYPES
      ,NOTIFY_ATOMIC_UNICODE32_TEST_TYPES
#endif
#ifdef NOTIFY_ATOMIC_INT64_TEST_TYPES
      ,NOTIFY_ATOMIC_INT64_TEST_TYPES
#endif
> AtomicValueTypes;
TYPED_TEST_CASE(AtomicIntegerTest, AtomicValueTypes);
template <bool> inline void boolean_helper() {}

}

TYPED_TEST(AtomicIntegerTest, staticChecks)
{
   typedef struct{
      TypeParam type;
   } TypeStruct;
   NOTIFY_STATIC_ASSERT(sizeof(notify::AtomicInteger<TypeParam>) == sizeof(TypeParam));
   NOTIFY_STATIC_ASSERT(alignof(notify::AtomicInteger<TypeParam>) == alignof(TypeStruct));
   (void) notify::AtomicInteger<TypeParam>::isReferenceCountingNative();
   (void) notify::AtomicInteger<TypeParam>::isReferenceCountingWaitFree();
   (void) notify::AtomicInteger<TypeParam>::isTestAndSetNative();
   (void) notify::AtomicInteger<TypeParam>::isTestAndSetWaitFree();
   (void) notify::AtomicInteger<TypeParam>::isFetchAndStoreNative();
   (void) notify::AtomicInteger<TypeParam>::isFetchAndStoreWaitFree();
   (void) notify::AtomicInteger<TypeParam>::isFetchAndAddNative();
   (void) notify::AtomicInteger<TypeParam>::isFetchAndAddWaitFree();
}

TYPED_TEST(AtomicIntegerTest, constructor)
{
   for (const TypeParam &value : this->m_backendValues)
   {
      notify::AtomicInteger<TypeParam > atomic(value);
      ASSERT_EQ(atomic.load(), TypeParam(value));
      notify::AtomicInteger<TypeParam > atomic2 = value;
      ASSERT_EQ(atomic2.load(), TypeParam(value));
      ASSERT_GE(atomic.load(), std::numeric_limits<TypeParam>::min());
      ASSERT_LE(atomic.load(), std::numeric_limits<TypeParam>::max());
   }
}

TYPED_TEST(AtomicIntegerTest, copy)
{
   for (const TypeParam &value : this->m_backendValues)
   {
      notify::AtomicInteger<TypeParam> atomic(value);
      notify::AtomicInteger<TypeParam> atomicCopy(atomic);
      ASSERT_EQ(atomic.load(), atomicCopy.load());
      notify::AtomicInteger<TypeParam> atomicCopy2 = atomic;
      ASSERT_EQ(atomic.load(), atomicCopy2.load());
      notify::AtomicInteger<TypeParam> atomicCopy3(std::move(atomic));
      ASSERT_EQ(atomic.load(), atomicCopy3.load());
      notify::AtomicInteger<TypeParam> atomicCopy4 = std::move(atomic);
      ASSERT_EQ(atomic.load(), atomicCopy4.load());
   }
}

TYPED_TEST(AtomicIntegerTest, assign)
{
   for (const TypeParam &value : this->m_backendValues)
   {
      notify::AtomicInteger<TypeParam> atomic(value);
      notify::AtomicInteger<TypeParam> atomicCopy;
      atomicCopy = atomic;  // operator=(const AtomicInteger &)
      ASSERT_EQ(atomic.load(), atomicCopy.load());
      notify::AtomicInteger<TypeParam> atomicCopy2;
      atomicCopy2 = atomic.load(); // AtomicInteger(T value = 0) constructor
      notify::AtomicInteger<TypeParam> atomicCopy3;
      atomicCopy3 = std::move(atomicCopy);
      ASSERT_EQ(atomic.load(), atomicCopy3.load());
      notify::AtomicInteger<TypeParam> atomicCopy4;
      atomicCopy4 = std::move(atomicCopy2);
      ASSERT_EQ(atomic.load(), atomicCopy4.load());
   }
}