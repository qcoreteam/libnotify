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

TYPED_TEST(AtomicIntegerTest, operatorInteger)
{
   for (const TypeParam &value : this->m_backendValues)
   {
      notify::AtomicInteger<TypeParam> atomic(value);
      TypeParam value2 = atomic;
      ASSERT_EQ(value2, atomic.load());
      ASSERT_EQ(value2, TypeParam(value));
   }
}

TYPED_TEST(AtomicIntegerTest, loadAcquireStoreRelease)
{
   for (const TypeParam &value : this->m_backendValues)
   {
      notify::AtomicInteger<TypeParam> atomic(value);
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));
      atomic.storeRelease(~value);
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(~value));
      atomic.storeRelease(value);
      ASSERT_EQ(atomic.load(), TypeParam(value));
   }
}

TYPED_TEST(AtomicIntegerTest, refDeref)
{
   for (const TypeParam &value : this->m_backendValues)
   {
      const bool needToPreventOverflow = AtomicIntegerTest<TypeParam>::TypeIsSigned && value == std::numeric_limits<TypeParam>::max();
      const bool needToPreventUnderlow = AtomicIntegerTest<TypeParam>::TypeIsSigned && value == std::numeric_limits<TypeParam>::min();
      TypeParam nextValue = TypeParam(value);
      TypeParam prevValue = TypeParam(value);
      if (!needToPreventOverflow) {
         ++nextValue;
      }
      if (!needToPreventUnderlow) {
         --prevValue;
      }
      notify::AtomicInteger<TypeParam> atomic(value);
      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic.ref(), (nextValue != 0));
         ASSERT_EQ(atomic.load(), nextValue);
         ASSERT_EQ(atomic.deref(), (value != 0));
      }
      ASSERT_EQ(atomic.load(), TypeParam(value));
      if (!needToPreventUnderlow) {
         ASSERT_EQ(atomic.deref(), (prevValue != 0));
         ASSERT_EQ(atomic.load(), prevValue);
         ASSERT_EQ(atomic.ref(), (value != 0));
      }
      ASSERT_EQ(atomic.load(), TypeParam(value));

      if (!needToPreventOverflow) {
         ASSERT_EQ(++atomic, nextValue);
         ASSERT_EQ(--atomic, TypeParam(value));
      }

      if (!needToPreventUnderlow) {
         ASSERT_EQ(--atomic, prevValue);
         ASSERT_EQ(++atomic, TypeParam(value));
      }

      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic++, TypeParam(value));
         ASSERT_EQ(atomic--, nextValue);
      }

      if (!needToPreventUnderlow) {
         ASSERT_EQ(atomic--, TypeParam(value));
         ASSERT_EQ(atomic++, prevValue);
      }
   }
}

TYPED_TEST(AtomicIntegerTest, testAndSet)
{
   for (const TypeParam &value : this->m_backendValues)
   {
      TypeParam newValue = ~TypeParam(value);
      notify::AtomicInteger<TypeParam> atomic(value);

      ASSERT_TRUE(atomic.testAndSetRelaxed(value, newValue));
      ASSERT_EQ(atomic.load(), newValue);
      ASSERT_TRUE(!atomic.testAndSetRelaxed(value, newValue));
      ASSERT_TRUE(atomic.testAndSetRelaxed(newValue, value));
      ASSERT_EQ(atomic.load(), TypeParam(value));

      ASSERT_TRUE(atomic.testAndSetAcquire(value, newValue));
      ASSERT_EQ(atomic.load(), newValue);
      ASSERT_TRUE(!atomic.testAndSetAcquire(value, newValue));
      ASSERT_TRUE(atomic.testAndSetAcquire(newValue, value));
      ASSERT_EQ(atomic.load(), TypeParam(value));

      ASSERT_TRUE(atomic.testAndSetRelease(value, newValue));
      ASSERT_EQ(atomic.loadAcquire(), newValue);
      ASSERT_TRUE(!atomic.testAndSetRelease(value, newValue));
      ASSERT_TRUE(atomic.testAndSetRelease(newValue, value));
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));

      ASSERT_TRUE(atomic.testAndSetOrdered(value, newValue));
      ASSERT_EQ(atomic.loadAcquire(), newValue);
      ASSERT_TRUE(!atomic.testAndSetOrdered(value, newValue));
      ASSERT_TRUE(atomic.testAndSetOrdered(newValue, value));
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));
   }
}

TYPED_TEST(AtomicIntegerTest, testAndSet3)
{
   for (const TypeParam &value : this->m_backendValues)
   {
      TypeParam newValue = ~TypeParam(value);
      TypeParam oldValue;
      notify::AtomicInteger<TypeParam> atomic(value);

      ASSERT_TRUE(atomic.testAndSetRelaxed(value, newValue, oldValue));
      ASSERT_EQ(atomic.load(), newValue);
      ASSERT_TRUE(!atomic.testAndSetRelaxed(value, newValue, oldValue));
      ASSERT_EQ(oldValue, newValue);
      ASSERT_TRUE(atomic.testAndSetRelaxed(newValue, value, oldValue));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(oldValue, newValue);

      ASSERT_TRUE(atomic.testAndSetAcquire(value, newValue, oldValue));
      ASSERT_EQ(atomic.load(), newValue);
      ASSERT_TRUE(!atomic.testAndSetAcquire(value, newValue, oldValue));
      ASSERT_EQ(oldValue, newValue);
      ASSERT_TRUE(atomic.testAndSetAcquire(newValue, value, oldValue));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(oldValue, newValue);

      ASSERT_TRUE(atomic.testAndSetRelease(value, newValue, oldValue));
      ASSERT_EQ(atomic.loadAcquire(), newValue);
      ASSERT_TRUE(!atomic.testAndSetRelease(value, newValue, oldValue));
      ASSERT_EQ(oldValue, newValue);
      ASSERT_TRUE(atomic.testAndSetRelease(newValue, value, oldValue));
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));
      ASSERT_EQ(oldValue, newValue);

      ASSERT_TRUE(atomic.testAndSetOrdered(value, newValue, oldValue));
      ASSERT_EQ(atomic.loadAcquire(), newValue);
      ASSERT_TRUE(!atomic.testAndSetOrdered(value, newValue, oldValue));
      ASSERT_EQ(oldValue, newValue);
      ASSERT_TRUE(atomic.testAndSetOrdered(newValue, value, oldValue));
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));
      ASSERT_EQ(oldValue, newValue);
   }
}

TYPED_TEST(AtomicIntegerTest, fetchAndStore)
{
   for (const TypeParam &value : this->m_backendValues) {
      TypeParam newValue = ~TypeParam(value);
      TypeParam oldValue;
      notify::AtomicInteger<TypeParam> atomic(value);

      ASSERT_EQ(atomic.fetchAndStoreRelaxed(newValue), value);
      ASSERT_EQ(atomic.load(), newValue);
      ASSERT_EQ(atomic.fetchAndStoreRelaxed(value), newValue);
      ASSERT_EQ(atomic.load(), value);

      ASSERT_EQ(atomic.fetchAndStoreAcquire(newValue), value);
      ASSERT_EQ(atomic.load(), newValue);
      ASSERT_EQ(atomic.fetchAndStoreAcquire(value), newValue);
      ASSERT_EQ(atomic.load(), value);

      ASSERT_EQ(atomic.fetchAndStoreRelease(newValue), value);
      ASSERT_EQ(atomic.loadAcquire(), newValue);
      ASSERT_EQ(atomic.fetchAndStoreRelease(value), newValue);
      ASSERT_EQ(atomic.loadAcquire(), value);

      ASSERT_EQ(atomic.fetchAndStoreOrdered(newValue), value);
      ASSERT_EQ(atomic.loadAcquire(), newValue);
      ASSERT_EQ(atomic.fetchAndStoreOrdered(value), newValue);
      ASSERT_EQ(atomic.loadAcquire(), value);
   }
}

TYPED_TEST(AtomicIntegerTest, fetchAndAdd)
{
   for (const TypeParam &value : this->m_backendValues) {
      notify::AtomicInteger<TypeParam> atomic(value);
      TypeParam parcel1 = 43;
      TypeParam parcel2 = TypeParam(0 - parcel1);
      const bool needToPreventOverflow =
            AtomicIntegerTest<TypeParam>::TypeIsSigned && value > std::numeric_limits<TypeParam>::max() + parcel2;
      const bool needToPreventUnderflow =
            AtomicIntegerTest<TypeParam>::TypeIsSigned && value < std::numeric_limits<TypeParam>::min() + parcel1;
      TypeParam newValue1 = TypeParam(value);
      if (!needToPreventOverflow) {
         newValue1 += parcel1;
      }
      TypeParam newValue2 = TypeParam(value);
      if (!needToPreventUnderflow) {
         newValue2 += parcel2;
      }
      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic.fetchAndAddRelaxed(parcel1), value);
         ASSERT_EQ(atomic.load(), newValue1);
         ASSERT_EQ(atomic.fetchAndAddRelaxed(parcel2), newValue1);
      }
      ASSERT_EQ(atomic.load(), TypeParam(value));

      if (!needToPreventUnderflow) {
         ASSERT_EQ(atomic.fetchAndAddRelaxed(parcel2), value);
         ASSERT_EQ(atomic.load(), newValue2);
         ASSERT_EQ(atomic.fetchAndAddRelaxed(parcel1), newValue2);
      }
      ASSERT_EQ(atomic.load(), TypeParam(value));

      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic.fetchAndAddAcquire(parcel1), value);
         ASSERT_EQ(atomic.load(), newValue1);
         ASSERT_EQ(atomic.fetchAndAddAcquire(parcel2), newValue1);
      }
      ASSERT_EQ(atomic.load(), TypeParam(value));

      if (!needToPreventUnderflow) {
         ASSERT_EQ(atomic.fetchAndAddAcquire(parcel2), value);
         ASSERT_EQ(atomic.load(), newValue2);
         ASSERT_EQ(atomic.fetchAndAddAcquire(parcel1), newValue2);
      }
      ASSERT_EQ(atomic.load(), TypeParam(value));

      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic.fetchAndAddRelease(parcel1), value);
         ASSERT_EQ(atomic.loadAcquire(), newValue1);
         ASSERT_EQ(atomic.fetchAndAddRelease(parcel2), newValue1);
      }
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));

      if (!needToPreventUnderflow) {
         ASSERT_EQ(atomic.fetchAndAddRelease(parcel2), value);
         ASSERT_EQ(atomic.loadAcquire(), newValue2);
         ASSERT_EQ(atomic.fetchAndAddRelease(parcel1), newValue2);
      }
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));

      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic.fetchAndAddOrdered(parcel1), value);
         ASSERT_EQ(atomic.loadAcquire(), newValue1);
         ASSERT_EQ(atomic.fetchAndAddOrdered(parcel2), newValue1);
      }
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));

      if (!needToPreventUnderflow) {
         ASSERT_EQ(atomic.fetchAndAddOrdered(parcel2), value);
         ASSERT_EQ(atomic.loadAcquire(), newValue2);
         ASSERT_EQ(atomic.fetchAndAddOrdered(parcel1), newValue2);
      }
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));

      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic += parcel1, newValue1);
         ASSERT_EQ(atomic += parcel2, TypeParam(value));
      }

      if (!needToPreventUnderflow) {
         ASSERT_EQ(atomic += parcel2, newValue2);
         ASSERT_EQ(atomic += parcel1, TypeParam(value));
      }
   }
}

TYPED_TEST(AtomicIntegerTest, fetchAndSub)
{
   for (const TypeParam &value : this->m_backendValues) {
      notify::AtomicInteger<TypeParam> atomic(value);
      TypeParam parcel1 = 43;
      TypeParam parcel2 = TypeParam(0 - parcel1);
      const bool needToPreventOverflow =
            AtomicIntegerTest<TypeParam>::TypeIsSigned && value > std::numeric_limits<TypeParam>::max() - parcel1;
      const bool needToPreventUnderflow =
            AtomicIntegerTest<TypeParam>::TypeIsSigned && value < std::numeric_limits<TypeParam>::min() - parcel2;
      TypeParam newValue1 = TypeParam(value);
      if (!needToPreventUnderflow) {
         newValue1 -= parcel1;
      }
      TypeParam newValue2 = TypeParam(value);
      if (!needToPreventOverflow) {
         newValue2 -= parcel2;
      }
      if (!needToPreventUnderflow) {
         ASSERT_EQ(atomic.fetchAndSubRelaxed(parcel1), TypeParam(value));
         ASSERT_EQ(atomic.load(), newValue1);
         ASSERT_EQ(atomic.fetchAndSubRelaxed(parcel2), newValue1);
      }
      ASSERT_EQ(atomic.load(), TypeParam(value));
      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic.fetchAndSubRelaxed(parcel2), TypeParam(value));
         ASSERT_EQ(atomic.load(), newValue2);
         ASSERT_EQ(atomic.fetchAndSubRelaxed(parcel1), newValue2);
      }
      ASSERT_EQ(atomic.load(), TypeParam(value));

      if (!needToPreventUnderflow) {
         ASSERT_EQ(atomic.fetchAndSubAcquire(parcel1), TypeParam(value));
         ASSERT_EQ(atomic.load(), newValue1);
         ASSERT_EQ(atomic.fetchAndSubAcquire(parcel2), newValue1);
      }
      ASSERT_EQ(atomic.load(), TypeParam(value));
      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic.fetchAndSubAcquire(parcel2), TypeParam(value));
         ASSERT_EQ(atomic.load(), newValue2);
         ASSERT_EQ(atomic.fetchAndSubAcquire(parcel1), newValue2);
      }
      ASSERT_EQ(atomic.load(), TypeParam(value));

      if (!needToPreventUnderflow) {
         ASSERT_EQ(atomic.fetchAndSubRelease(parcel1), TypeParam(value));
         ASSERT_EQ(atomic.loadAcquire(), newValue1);
         ASSERT_EQ(atomic.fetchAndSubRelease(parcel2), newValue1);
      }
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));
      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic.fetchAndSubRelease(parcel2), TypeParam(value));
         ASSERT_EQ(atomic.loadAcquire(), newValue2);
         ASSERT_EQ(atomic.fetchAndSubRelease(parcel1), newValue2);
      }
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));

      if (!needToPreventUnderflow) {
         ASSERT_EQ(atomic.fetchAndSubOrdered(parcel1), TypeParam(value));
         ASSERT_EQ(atomic.loadAcquire(), newValue1);
         ASSERT_EQ(atomic.fetchAndSubOrdered(parcel2), newValue1);
      }
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));
      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic.fetchAndSubOrdered(parcel2), TypeParam(value));
         ASSERT_EQ(atomic.loadAcquire(), newValue2);
         ASSERT_EQ(atomic.fetchAndSubOrdered(parcel1), newValue2);
      }
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));

      if (!needToPreventUnderflow) {
         ASSERT_EQ(atomic -= parcel1, newValue1);
         ASSERT_EQ(atomic -= parcel2, TypeParam(value));
      }

      if (!needToPreventOverflow) {
         ASSERT_EQ(atomic -= parcel2, newValue2);
         ASSERT_EQ(atomic -= parcel1, TypeParam(value));
      }
   }
}

TYPED_TEST(AtomicIntegerTest, fetchAndOr)
{
   for (const TypeParam &value : this->m_backendValues) {
      notify::AtomicInteger<TypeParam> atomic(value);
      TypeParam zero = 0;
      TypeParam one = 1;
      TypeParam minusOne = TypeParam(~0);

      ASSERT_EQ(atomic.fetchAndOrRelaxed(zero), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndOrRelaxed(one), TypeParam(value));
      ASSERT_EQ(atomic.load(), (value | one));
      ASSERT_EQ(atomic.fetchAndOrRelaxed(minusOne), (value | one));
      ASSERT_EQ(atomic.load(), minusOne);

      atomic.store(value);
      ASSERT_EQ(atomic.fetchAndOrAcquire(zero), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndOrAcquire(one), TypeParam(value));
      ASSERT_EQ(atomic.load(), (value | one));
      ASSERT_EQ(atomic.fetchAndOrAcquire(minusOne), (value | one));
      ASSERT_EQ(atomic.load(), minusOne);

      atomic.store(value);
      ASSERT_EQ(atomic.fetchAndOrRelease(zero), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndOrAcquire(one), TypeParam(value));
      ASSERT_EQ(atomic.load(), (value | one));
      ASSERT_EQ(atomic.fetchAndOrRelease(minusOne), (value | one));
      ASSERT_EQ(atomic.load(), minusOne);

      atomic.store(value);
      ASSERT_EQ(atomic.fetchAndOrOrdered(zero), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndOrAcquire(one), TypeParam(value));
      ASSERT_EQ(atomic.load(), (value | one));
      ASSERT_EQ(atomic.fetchAndOrOrdered(minusOne), (value | one));
      ASSERT_EQ(atomic.load(), minusOne);

      atomic.store(value);
      ASSERT_EQ(atomic |= zero, TypeParam(value));
      ASSERT_EQ(atomic |= one, TypeParam(value | one));
      ASSERT_EQ(atomic |= minusOne, minusOne);
   }
}

TYPED_TEST(AtomicIntegerTest, fetchAndAnd)
{
   for (const TypeParam &value : this->m_backendValues) {
      notify::AtomicInteger<TypeParam> atomic(value);
      TypeParam zero = 0;
      TypeParam one = 0x1;
      TypeParam minusOne = TypeParam(~0);

      ASSERT_EQ(atomic.fetchAndAndRelaxed(minusOne), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndAndRelaxed(one), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value & one));
      ASSERT_EQ(atomic.fetchAndAndRelaxed(zero), TypeParam(value & one));
      ASSERT_EQ(atomic.load(), zero);

      atomic.store(value);
      ASSERT_EQ(atomic.fetchAndAndAcquire(minusOne), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndAndAcquire(one), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value & one));
      ASSERT_EQ(atomic.fetchAndAndAcquire(zero), TypeParam(value & one));
      ASSERT_EQ(atomic.load(), zero);

      atomic.store(value);
      ASSERT_EQ(atomic.fetchAndAndRelease(minusOne), TypeParam(value));
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndAndRelease(one), TypeParam(value));
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value & one));
      ASSERT_EQ(atomic.fetchAndAndRelease(zero), TypeParam(value & one));
      ASSERT_EQ(atomic.loadAcquire(), zero);

      atomic.store(value);
      ASSERT_EQ(atomic.fetchAndAndOrdered(minusOne), TypeParam(value));
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndAndOrdered(one), TypeParam(value));
      ASSERT_EQ(atomic.loadAcquire(), TypeParam(value & one));
      ASSERT_EQ(atomic.fetchAndAndOrdered(zero), TypeParam(value & one));
      ASSERT_EQ(atomic.loadAcquire(), zero);

      atomic.store(value);
      ASSERT_EQ(atomic &= minusOne, TypeParam(value));
      ASSERT_EQ(atomic &= one, TypeParam(value & one));
      ASSERT_EQ(atomic &= zero, zero);
   }
}


TYPED_TEST(AtomicIntegerTest, fetchAndXor)
{
   for (const TypeParam &value : this->m_backendValues) {
      notify::AtomicInteger<TypeParam> atomic(value);
      TypeParam zero = 0;
      TypeParam pattern(TypeParam(N_INT64_C(0xcccccccccccccccc)));
      TypeParam minusOne = TypeParam(~0);

      ASSERT_EQ(atomic.fetchAndXorRelaxed(zero), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndXorRelaxed(pattern), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value ^ pattern));
      ASSERT_EQ(atomic.fetchAndXorRelaxed(pattern), TypeParam(value ^ pattern));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndXorRelaxed(minusOne), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(~value));
      ASSERT_EQ(atomic.fetchAndXorRelaxed(minusOne), TypeParam(~value));
      ASSERT_EQ(atomic.load(), TypeParam(value));

      ASSERT_EQ(atomic.fetchAndXorAcquire(zero), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndXorAcquire(pattern), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value ^ pattern));
      ASSERT_EQ(atomic.fetchAndXorAcquire(pattern), TypeParam(value ^ pattern));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndXorAcquire(minusOne), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(~value));
      ASSERT_EQ(atomic.fetchAndXorAcquire(minusOne), TypeParam(~value));
      ASSERT_EQ(atomic.load(), TypeParam(value));

      ASSERT_EQ(atomic.fetchAndXorRelease(zero), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndXorRelease(pattern), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value ^ pattern));
      ASSERT_EQ(atomic.fetchAndXorRelease(pattern), TypeParam(value ^ pattern));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndXorRelease(minusOne), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(~value));
      ASSERT_EQ(atomic.fetchAndXorRelease(minusOne), TypeParam(~value));
      ASSERT_EQ(atomic.load(), TypeParam(value));

      ASSERT_EQ(atomic.fetchAndXorOrdered(zero), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndXorOrdered(pattern), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(value ^ pattern));
      ASSERT_EQ(atomic.fetchAndXorOrdered(pattern), TypeParam(value ^ pattern));
      ASSERT_EQ(atomic.load(), TypeParam(value));
      ASSERT_EQ(atomic.fetchAndXorOrdered(minusOne), TypeParam(value));
      ASSERT_EQ(atomic.load(), TypeParam(~value));
      ASSERT_EQ(atomic.fetchAndXorOrdered(minusOne), TypeParam(~value));
      ASSERT_EQ(atomic.load(), TypeParam(value));

      ASSERT_EQ(atomic ^= zero, TypeParam(value));
      ASSERT_EQ(atomic ^= pattern, TypeParam(value ^ pattern));
      ASSERT_EQ(atomic ^= pattern, TypeParam(value));
      ASSERT_EQ(atomic ^= minusOne, TypeParam(~value));
      ASSERT_EQ(atomic ^= minusOne, TypeParam(value));
   }
}