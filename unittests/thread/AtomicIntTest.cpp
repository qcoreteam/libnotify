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
// Created by zzu_softboy on 12/05/2017.

#include "gtest/gtest.h"

#include "notify/thread/Atomic.h"
#include <limits.h>
#include <vector>
#include <utility>
#include <array>
#include <tuple>

namespace
{

template <int IntVal>
static inline void assembly_marker(void *ptr = NOTIFY_NULLPTR)
{
   //puts((char *)ptr + IntVal);
};

template <typename T, typename Atomic>
static void warning_free_helper_template()
{
   T expectedValue = 0;
   T newValue = 0;
   T valueToAdd = 0;

   assembly_marker<0>();
   Atomic i = NOTIFY_BASIC_ATOMIC_INITIALIZER(0);
   printf("%d\n", int(i.loadAcquire()));
   assembly_marker<1>(&i);

   i.load();
   assembly_marker<11>(&i);
   i.loadAcquire();
   assembly_marker<12>(&i);

   i.store(newValue);
   assembly_marker<21>(&i);
   i.storeRelease(newValue);
   assembly_marker<22>(&i);

   i.ref();
   assembly_marker<31>(&i);
   i.deref();
   assembly_marker<32>(&i);

   i.testAndSetRelaxed(expectedValue, newValue);
   assembly_marker<41>(&i);
   i.testAndSetAcquire(expectedValue, newValue);
   assembly_marker<42>(&i);
   i.testAndSetRelease(expectedValue, newValue);
   assembly_marker<43>(&i);
   i.testAndSetOrdered(expectedValue, newValue);
   assembly_marker<44>(&i);

   i.fetchAndStoreRelaxed(newValue);
   assembly_marker<51>(&i);
   i.fetchAndStoreAcquire(newValue);
   assembly_marker<52>(&i);
   i.fetchAndStoreRelease(newValue);
   assembly_marker<53>(&i);
   i.fetchAndStoreOrdered(newValue);
   assembly_marker<54>(&i);

   i.fetchAndAddRelaxed(valueToAdd);
   assembly_marker<61>(&i);
   i.fetchAndAddAcquire(valueToAdd);
   assembly_marker<62>(&i);
   i.fetchAndAddRelease(valueToAdd);
   assembly_marker<63>(&i);
   i.fetchAndAddOrdered(valueToAdd);
   assembly_marker<64>(&i);
};
typedef std::array<int, 3> Array3;
typedef std::tuple<int, int, int, bool> TestAndSetItemType;
typedef std::array<int, 2> Array2;
class AtomicIntTest : public ::testing::Test
{
public:
   static void SetUpTestCase();
protected:
   static std::vector<int> m_constructorValues;
   static std::vector<std::pair<int, int>> m_assignOperatorValues;
   static std::vector<Array3> m_refValues;
   static std::vector<Array3> m_derefValues;
   static std::vector<TestAndSetItemType> m_testAndSetValues;
   static std::vector<Array2> m_fetchAndStoreValues;
   static std::vector<Array2> m_fetchAndAddValues;
};

void AtomicIntTest::SetUpTestCase()
{

}

std::vector<int> AtomicIntTest::m_constructorValues = {
      31337, 0, 1, -1, 2, -2, 3, -3, INT_MAX, INT_MIN + 1
};

std::vector<std::pair<int, int>> AtomicIntTest::m_assignOperatorValues = {
      {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, 1}, {1, -1}
};

std::vector<Array3> AtomicIntTest::m_refValues = {
      {0, 1, 1},
      {-1, 0, 0},
      {1, 1, 2}
};

std::vector<Array3> AtomicIntTest::m_derefValues = {
      {0, 1, -1},
      {1, 0, 0},
      {2, 1, 1}
};

std::vector<TestAndSetItemType> AtomicIntTest::m_testAndSetValues = {
      std::make_tuple(0, 0, 0, true),
      std::make_tuple(0, 0, 1, true),
      std::make_tuple(0, 0, -1, true),
      std::make_tuple(1, 1, 0, true),
      std::make_tuple(1, 1, 1, true),
      std::make_tuple(1, 1, -1, true),
      std::make_tuple(-1, -1, 0, true),
      std::make_tuple(-1, -1, 1, true),
      std::make_tuple(-1, -1, -1, true),
      std::make_tuple(INT_MIN + 1, INT_MIN + 1, INT_MIN + 1, true),
      std::make_tuple(INT_MIN + 1, INT_MIN + 1, 1, true),
      std::make_tuple(INT_MIN + 1, INT_MIN + 1, -1, true),
      std::make_tuple(INT_MAX, INT_MAX, INT_MAX, true),
      std::make_tuple(INT_MAX, INT_MAX, 1, true),
      std::make_tuple(INT_MAX, INT_MAX, -1, true),

      // fail situations
      std::make_tuple(0, 1, ~0, false),
      std::make_tuple(0, -1, ~0, false),
      std::make_tuple(1, 0, ~0, false),
      std::make_tuple(-1, 0, ~0, false),
      std::make_tuple(1, -1, ~0, false),
      std::make_tuple(-1, 1, ~0, false),
      std::make_tuple(INT_MIN+1, INT_MAX, ~0, false),
      std::make_tuple(INT_MAX, INT_MIN + 1, ~0, false)
};

std::vector<Array2> AtomicIntTest::m_fetchAndStoreValues = {
      {0, 1},
      {1, 2},
      {3, 8}
};

std::vector<Array2> AtomicIntTest::m_fetchAndAddValues = {
      {0, 1},
      {1, 0},
      {1, 2},
      {2, 1},
      {10, 21},
      {31, 40},
      {51, 62},
      {72, 81},
      {810, 721},
      {631, 540},
      {451, 362},
      {272, 181},
      {1810, 8721},
      {3631, 6540},
      {5451, 4362},
      {7272, 2181},

      {0, -1},
      {1, 0},
      {1, -2},
      {2, -1},
      {10, -21},
      {31, -40},
      {51, -62},
      {72, -81},
      {810, -721},
      {631, -540},
      {451, -362},
      {272, -181},
      {1810, -8721},
      {3631, -6540},
      {5451, -4362},
      {7272, -2181},

      {0, 1},
      {-1, 0},
      {-2, 1},
      {-10, 21},
      {-31, 40},
      {-51, 62},
      {-72, 81},
      {-810, 721},
      {-631, 540},
      {-451, 362},
      {-272, 181},
      {-1810, 8721},
      {-3631, 6540},
      {-5451, 4362},
      {-7272, 2181}
};

void warning_free_helper()
{
   warning_free_helper_template<int, notify::BasicAtomicInt>();

   warning_free_helper_template<int, notify::BasicAtomicInteger<int>>();
   warning_free_helper_template<unsigned int, notify::BasicAtomicInteger<unsigned int>>();
#ifdef NOTIFY_COMPILER_UNICODE_STRINGS
   warning_free_helper_template<n_int16, notify::BasicAtomicInteger<char32_t>>();
#endif

   warning_free_helper_template<int, notify::BasicAtomicInteger<n_ptrdiff>>();
   warning_free_helper_template<unsigned int, notify::BasicAtomicInteger<n_uintptr>>();

   warning_free_helper_template<int, notify::BasicAtomicInteger<long int>>();
   warning_free_helper_template<unsigned int, notify::BasicAtomicInteger<unsigned long int>>();

#ifdef NOTIFY_ATOMIC_INT8_IS_SUPPORTED
   warning_free_helper_template<char, notify::BasicAtomicInteger<char>>();
   warning_free_helper_template<signed char, notify::BasicAtomicInteger<signed char>>();
   warning_free_helper_template<unsigned char, notify::BasicAtomicInteger<unsigned char>>();
#endif

#ifdef NOTIFY_ATOMIC_INT16_IS_SUPPORTED
   warning_free_helper_template<n_int16, notify::BasicAtomicInteger<n_int16>>();
   warning_free_helper_template<n_uint16, notify::BasicAtomicInteger<n_uint16>>();
#  ifdef NOTIFY_COMPILER_UNICODE_STRINGS
   warning_free_helper_template<n_int16, notify::BasicAtomicInteger<char16_t>>();
#  endif
#endif

#ifdef NOTIFY_ATOMIC_INT64_IS_SUPPORTED
#  if !defined(__i386__) || (defined(NOTIFY_CC_GNU)  && defined(__OPTIMIZE__))
   warning_free_helper_template<n_longlong, notify::BasicAtomicInteger<n_longlong>>();
   warning_free_helper_template<n_ulonglong, notify::BasicAtomicInteger<n_ulonglong>>();
#  endif
#endif
}

template <typename T>
struct TypeInStruct
{
   T val;
};

}

TEST_F(AtomicIntTest, warningFree)
{
   void (*foo)() = &warning_free_helper;
   (void)foo;
}

TEST_F(AtomicIntTest, alignment)
{
   using notify::BasicAtomicInt;
   using notify::AtomicInt;
   using notify::BasicAtomicInteger;
#ifdef NOTIFY_ALIGNOF
   char dummy1[NOTIFY_ALIGNOF(BasicAtomicInt) == NOTIFY_ALIGNOF(TypeInStruct<int>) ? 1 : -1];
   char dummy2[NOTIFY_ALIGNOF(AtomicInt) == NOTIFY_ALIGNOF(TypeInStruct<int>) ? 1 : -1];
#endif
#ifdef NOTIFY_ATOMIC_INT8_IS_SUPPORTED
   ASSERT_EQ(NOTIFY_ALIGNOF(BasicAtomicInteger<char>), NOTIFY_ALIGNOF(TypeInStruct<char>));
#endif

#ifdef NOTIFY_ATOMIC_INT16_IS_SUPPORTED
   ASSERT_EQ(NOTIFY_ALIGNOF(BasicAtomicInteger<short>), NOTIFY_ALIGNOF(TypeInStruct<short>));
#endif

#ifdef NOTIFY_ATOMIC_INT32_IS_SUPPORTED
   ASSERT_EQ(NOTIFY_ALIGNOF(BasicAtomicInteger<int>), NOTIFY_ALIGNOF(TypeInStruct<int>));
#endif

#ifdef NOTIFY_ATOMIC_INT64_IS_SUPPORTED
   ASSERT_EQ(NOTIFY_ALIGNOF(BasicAtomicInteger<n_longlong>), NOTIFY_ALIGNOF(TypeInStruct<n_longlong>));
#endif
}

TEST_F(AtomicIntTest, constructor)
{
   using notify::AtomicInt;
   for (const int &value : this->m_constructorValues)
   {
      AtomicInt atomic1(value);
      ASSERT_EQ(atomic1.load(), value);
      AtomicInt atomic2 = atomic1;
      ASSERT_EQ(atomic2.load(), value);
   }
}

TEST_F(AtomicIntTest, copyonstructor)
{
   using notify::AtomicInt;
   for (const int &value : this->m_constructorValues)
   {
      AtomicInt atomic1(value);
      ASSERT_EQ(atomic1.load(), value);
      AtomicInt atomic2(atomic1);
      ASSERT_EQ(atomic2.load(), value);
      AtomicInt atomic3 = atomic1;
      ASSERT_EQ(atomic3.load(), value);
      AtomicInt atomic4(atomic2);
      ASSERT_EQ(atomic4.load(), value);
      AtomicInt atomic5 = atomic2;
      ASSERT_EQ(atomic5.load(), value);
   }
}

TEST_F(AtomicIntTest, assignmentOperator)
{
   using notify::AtomicInt;
   for (const std::pair<int, int> &valPair : this->m_assignOperatorValues)
   {
      int value = valPair.first;
      int newval = valPair.second;
      AtomicInt atomic1 = value;
      atomic1 = newval;
      ASSERT_EQ(atomic1.load(), newval);
      atomic1 = value;
      ASSERT_EQ(atomic1.load(), value);

      AtomicInt atomic2 = newval;
      atomic1 = atomic2;
      ASSERT_EQ(atomic1.load(), atomic2.load());
   }
}

TEST_F(AtomicIntTest, isReferenceCountingNative)
{
   using notify::AtomicInt;
#if defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_ALWAYS_NATIVE)
   ASSERT_TRUE(AtomicInt::isReferenceCountingNative());
#  if (defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_NOT_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE)
   ASSERT_TRUE(AtomicInt::isReferenceCountingNative() || !AtomicInt::isReferenceCountingNative());
#  if (defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_ALWAYS_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_NOT_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_NOT_NATIVE)
   ASSERT_FALSE(AtomicInt::isReferenceCountingNative());
#  if (defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_ALWAYS_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_SOMETIMES_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

TEST_F(AtomicIntTest, isReferenceCountingWaitFree)
{
   using notify::AtomicInt;
#if defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_WAIT_FREE)
   ASSERT_TREUE(AtomicInt::isReferenceCountingWaitFree());
   ASSERT_TREUE(AtomicInt::isReferenceCountingNative());
#  if defined(NOTIFY_ATOMIC_INT_REFERENCE_COUNTING_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
   ASSERT_TRUE(!AtomicInt::isReferenceCountingWaitFree());
#endif
}

TEST_F(AtomicIntTest, ref)
{
   using notify::AtomicInt;
   for (const Array3 &item : this->m_refValues)
   {
      int value = item[0];
      int result = item[1];
      int expected = item[2];
      AtomicInt atomic = value;
      ASSERT_EQ(atomic.ref() ? 1 : 0, result);
      ASSERT_EQ(atomic.load(), expected);
   }
}

TEST_F(AtomicIntTest, deref)
{
   using notify::AtomicInt;
   for (const Array3 &item : this->m_derefValues)
   {
      int value = item[0];
      int result = item[1];
      int expected = item[2];
      AtomicInt atomic = value;
      ASSERT_EQ(atomic.deref() ? 1 : 0, result);
      ASSERT_EQ(atomic.load(), expected);
   }
}

TEST_F(AtomicIntTest, isTestAndSetNative)
{
   using notify::AtomicInt;
#if defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_ALWAYS_NATIVE)
   ASSERT_TRUE(AtomicInt::isTestAndSetNative());
#  if (defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_SOMETIMES_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_NOT_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_SOMETIMES_NATIVE)
   ASSERT_TRUE(AtomicInt::isTestAndSetNative() || !AtomicInt::isTestAndSetNative());
#  if (defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_ALWAYS_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_NOT_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_NOT_NATIVE)
   ASSERT_FALSE(AtomicInt::isTestAndSetNative());
#  if (defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_ALWAYS_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_SOMETIMES_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

TEST_F(AtomicIntTest, isTestAndSetWaitFree)
{
   using notify::AtomicInt;
#if defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_WAIT_FREE)
   ASSERT_TREUE(AtomicInt::isTestAndSetWaitFree());
   ASSERT_TREUE(AtomicInt::isTestAndSetNative());
#  if defined(NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
   ASSERT_TRUE(!AtomicInt::isTestAndSetWaitFree());
#endif
}

TEST_F(AtomicIntTest, testAndSet)
{
   using notify::AtomicInt;
   for (const TestAndSetItemType &item : this->m_testAndSetValues)
   {
      int value = std::get<0>(item);
      int expected = std::get<1>(item);
      int newval = std::get<2>(item);
      bool result = std::get<3>(item);
      {
         AtomicInt atomic = value;
         ASSERT_EQ(atomic.testAndSetRelaxed(expected, newval), result);
      }
      {
         AtomicInt atomic = value;
         ASSERT_EQ(atomic.testAndSetAcquire(expected, newval), result);
      }
      {
         AtomicInt atomic = value;
         ASSERT_EQ(atomic.testAndSetRelease(expected, newval), result);
      }
      {
         AtomicInt atomic = value;
         ASSERT_EQ(atomic.testAndSetOrdered(expected, newval), result);
      }
#ifdef NOTIFY_ATOMIC_INT32_IS_SUPPORTED
      {
         AtomicInt atomic = value;
         int currentVal = 0xdeadbeef;
         ASSERT_EQ(atomic.testAndSetRelaxed(expected, newval, currentVal), result);
         if (!result) {
            ASSERT_EQ(currentVal, value);
         }
      };

      {
         AtomicInt atomic = value;
         int currentVal = 0xdeadbeef;
         ASSERT_EQ(atomic.testAndSetAcquire(expected, newval, currentVal), result);
         if (!result) {
            ASSERT_EQ(currentVal, value);
         }
      }

      {
         AtomicInt atomic = value;
         int currentVal = 0xdeadbeef;
         ASSERT_EQ(atomic.testAndSetRelease(expected, newval, currentVal), result);
         if (!result) {
            ASSERT_EQ(currentVal, value);
         }
      }

      {
         AtomicInt atomic = value;
         int currentVal = 0xdeadbeef;
         ASSERT_EQ(atomic.testAndSetOrdered(expected, newval, currentVal), result);
         if (!result) {
            ASSERT_EQ(currentVal, value);
         }
      }

#endif
   }
}

TEST_F(AtomicIntTest, isFetchAndStoreNative)
{
   using notify::AtomicInt;
#if defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_ALWAYS_NATIVE)
   ASSERT_TRUE(AtomicInt::isFetchAndStoreNative());
#  if (defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_SOMETIMES_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_NOT_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_SOMETIMES_NATIVE)
   ASSERT_TRUE(AtomicInt::isFetchAndStoreNative() || !AtomicInt::isFetchAndStoreNative());
#  if (defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_ALWAYS_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_NOT_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_NOT_NATIVE)
   ASSERT_FALSE(AtomicInt::isFetchAndStoreNative());
#  if (defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_ALWAYS_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_SOMETIMES_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

TEST_F(AtomicIntTest, isFetchAndStoreWaitFree)
{
   using notify::AtomicInt;
#if defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_WAIT_FREE)
   ASSERT_TREUE(AtomicInt::isFetchAndStoreWaitFree());
   ASSERT_TREUE(AtomicInt::isFetchAndStoreNative());
#  if defined(NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
   ASSERT_TRUE(!AtomicInt::isFetchAndStoreWaitFree());
#endif
}

TEST_F(AtomicIntTest, fetchAndStore)
{
   using notify::AtomicInt;
   for (const Array2 &item : this->m_fetchAndStoreValues)
   {
      int value = std::get<0>(item);
      int newVal = std::get<0>(item);
      {
         AtomicInt atomic = value;
         ASSERT_EQ(atomic.fetchAndStoreRelaxed(newVal), value);
         ASSERT_EQ(atomic.load(), newVal);
      }
      {
         AtomicInt atomic = value;
         ASSERT_EQ(atomic.fetchAndStoreAcquire(newVal), value);
         ASSERT_EQ(atomic.load(), newVal);
      }
      {
         AtomicInt atomic = value;
         ASSERT_EQ(atomic.fetchAndStoreRelease(newVal), value);
         ASSERT_EQ(atomic.load(), newVal);
      }
      {
         AtomicInt atomic = value;
         ASSERT_EQ(atomic.fetchAndStoreOrdered(newVal), value);
         ASSERT_EQ(atomic.load(), newVal);
      }
   }
}

TEST_F(AtomicIntTest, isFetchAndAddNative)
{
   using notify::AtomicInt;
#if defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_ALWAYS_NATIVE)
   ASSERT_TRUE(AtomicInt::isFetchAndAddNative());
#  if (defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_SOMETIMES_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_NOT_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_SOMETIMES_NATIVE)
   ASSERT_TRUE(AtomicInt::isFetchAndAddNative() || !AtomicInt::isFetchAndAddNative());
#  if (defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_ALWAYS_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_NOT_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#elif defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_NOT_NATIVE)
   ASSERT_FALSE(AtomicInt::isFetchAndAddNative());
#  if (defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_ALWAYS_NATIVE) \
   || (defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_SOMETIMES_NATIVE)))
#     error "Define only one of NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE"
#  endif
#else
#  error "NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_{ALWAYS,SOMTIMES,NOT}_NATIVE is not defined"
#endif
}

TEST_F(AtomicIntTest, isFetchAndAddWaitFree)
{
   using notify::AtomicInt;
#if defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_WAIT_FREE)
   ASSERT_TREUE(AtomicInt::isFetchAndAddWaitFree());
   ASSERT_TREUE(AtomicInt::isFetchAndAddNative());
#  if defined(NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_NOT_NATIVE)
#    error "Reference counting cannot be wait-free and unsupported at the same time!"
#  endif
#else
   ASSERT_TRUE(!AtomicInt::isFetchAndAddWaitFree());
#endif
}

TEST_F(AtomicIntTest, fetchAndAdd)
{
   using notify::AtomicInt;
   for (const Array2 &item : this->m_fetchAndAddValues)
   {
      int value1 = std::get<0>(item);
      int value2 = std::get<1>(item);
      int result;
      {
         AtomicInt atomic = value1;
         result = atomic.fetchAndAddRelaxed(value2);
         ASSERT_EQ(result, value1);
         ASSERT_EQ(atomic.load(), value1 + value2);
      }

      {
         AtomicInt atomic = value1;
         result = atomic.fetchAndAddAcquire(value2);
         ASSERT_EQ(result, value1);
         ASSERT_EQ(atomic.load(), value1 + value2);
      }

      {
         AtomicInt atomic = value1;
         result = atomic.fetchAndAddRelease(value2);
         ASSERT_EQ(result, value1);
         ASSERT_EQ(atomic.load(), value1 + value2);
      }

      {
         AtomicInt atomic = value1;
         result = atomic.fetchAndAddOrdered(value2);
         ASSERT_EQ(result, value1);
         ASSERT_EQ(atomic.load(), value1 + value2);
      }
   }
}

TEST_F(AtomicIntTest, operators)
{
   using notify::BasicAtomicInt;
   using notify::AtomicInt;

   {
      BasicAtomicInt atomic = NOTIFY_BASIC_ATOMIC_INITIALIZER(0);
      atomic = 1;
      ASSERT_EQ(int(atomic), 1);
   }

   AtomicInt atomic = 0;
   int x = ++atomic;
   ASSERT_EQ(int(atomic), x);
   ASSERT_EQ(int(atomic), 1);

   x = atomic++;
   ASSERT_EQ(int(atomic), x + 1);
   ASSERT_EQ(int(atomic), 2);

   x = atomic--;
   ASSERT_EQ(int(atomic), x - 1);
   ASSERT_EQ(int(atomic), 1);

   x = --atomic;
   ASSERT_EQ(int(atomic), x);
   ASSERT_EQ(int(atomic), 0);

   x = (atomic += 1);
   ASSERT_EQ(int(atomic), x);
   ASSERT_EQ(int(atomic), 1);

   x = (atomic -= 1);
   ASSERT_EQ(int(atomic), x);
   ASSERT_EQ(int(atomic), 0);

   x = (atomic |= 0xf);
   ASSERT_EQ(int(atomic), x);
   ASSERT_EQ(int(atomic), 0xf);

   x = (atomic &= 0x17);
   ASSERT_EQ(int(atomic), x);
   ASSERT_EQ(int(atomic), 7);

   x = (atomic ^= 0x14);
   ASSERT_EQ(int(atomic), x);
   ASSERT_EQ(int(atomic), 0x13);

   x = (atomic ^= atomic);
   ASSERT_EQ(int(atomic), x);
   ASSERT_EQ(int(atomic), 0);
}

TEST_F(AtomicIntTest, fetchAndAddLoop)
{
   int interations = 10000000;
#if defined(NOTIFY_OS_HPUX)
   interations = 10000000;
#endif
   using notify::AtomicInt;
   AtomicInt val;
   for (int i = 0; i < interations; ++i) {
      const int prev = val.fetchAndAddRelaxed(1);
      ASSERT_EQ(prev, val.load() - 1);
   }
}


// TODO test with thread