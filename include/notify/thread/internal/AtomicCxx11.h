// Copyright 2017-20116 zzu_softboy <zzu_softboy@163.com>
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
// Created by zzu_softboy on 09/05/2017.

#ifndef NOTIFY_THREAD_INTERNAL_ATOMICCXX11_H
#define NOTIFY_THREAD_INTERNAL_ATOMICCXX11_H

#include "GenericAtomic.h"
#include <atomic>

// Attempt to detect whether the atomic operations exist in hardware
// or whether they are emulated by way of a lock.
//
// C++11 29.4 [atomics.lockfree] p1 says

// The ATOMIC_..._LOCK_FREE macros indicate the lock-free property of the
// corresponding atomic types, with the signed and unsigned variants grouped
// together. The properties also apply to the corresponding (partial)
// specializations of the atomic template. A value of 0 indicates that the
// types are never lock-free. A value of 1 indicates that the types are
// sometimes lock-free. A value of 2 indicates that the types are always
// lock-free.
//

namespace notify
{
namespace threadinternal
{

template <int N>
struct AtomicTraits
{
   static inline bool isLockFree();
};

#define NOTIFY_ATOMIC_INT32_IS_SUPPORTED
#if ATOMIC_INT_LOCK_FREE == 2
#  define NOTIFY_ATOMIC_INT_REFRENCE_COUNTING_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT32_REFRENCE_COUNTING_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT32_TEST_AND_SET_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT32_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT32_FETCH_AND_ADD_IS_ALWAYS_NATIVE
template <>
inline bool AtomicTraits<4>::isLockFree()
{
   return true;
};
#elif ATOMIC_INT_LOCK_FREE == 1
#  define NOTIFY_ATOMIC_INT_REFRENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT32_REFRENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT32_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT32_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT32_FETCH_AND_ADD_IS_SOMETIMES_NATIVE
template <>
inline bool AtomicTrait<4>::isLockFree()
{
   return false;
};
#else
#  define NOTIFY_ATOMIC_INT_REFRENCE_COUNTING_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT_TEST_AND_SET_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT_FETCH_AND_ADD_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT32_REFRENCE_COUNTING_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT32_TEST_AND_SET_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT32_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT32_FETCH_AND_ADD_IS_NEVER_NATIVE
template <>
inline bool AtomicTrait<4>::isLockFree()
{
   return false;
};
#endif

#if ATOMIC_POINTER_LOCK_FREE == 2
#  define NOTIFY_ATOMIC_POINTER_REFRENCE_COUNTING_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_POINTER_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_POINTER_FETCH_AND_ADD_IS_ALWAYS_NATIVE
#elif ATOMIC_POINTER_LOCK_FREE == 1
#  define NOTIFY_ATOMIC_POINTER_REFRENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_POINTER_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_POINTER_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_POINTER_FETCH_AND_ADD_IS_SOMETIMES_NATIVE
#else
#  define NOTIFY_ATOMIC_POINTER_REFRENCE_COUNTING_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_POINTER_TEST_AND_SET_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_POINTER_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_POINTER_FETCH_AND_ADD_IS_NEVER_NATIVE
#endif

template <>
struct AtomicOpsSupport<1>
{
   enum {
      IsSupported = 1
   };
};

#define NOTIFY_ATOMIC_INT8_IS_SUPPORTED
#if ATOMIC_CHAR_LOCK_FREE == 2
#  define NOTIFY_ATOMIC_INT8_REFRENCE_COUNTING_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT8_TEST_AND_SET_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT8_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT8_FETCH_AND_ADD_IS_ALWAYS_NATIVE
template <>
inline bool AtomicTraits<1>::isLockFree()
{
   return true;
};
#elif ATOMIC_CHAR_LOCK_FREE == 1
#  define NOTIFY_ATOMIC_INT8_REFRENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT8_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT8_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT8_FETCH_AND_ADD_IS_SOMETIMES_NATIVE
template <>
inline bool AtomicTraits<1>::isLockFree()
{
   return false;
};
#else
#  define NOTIFY_ATOMIC_INT8_REFRENCE_COUNTING_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT8_TEST_AND_SET_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT8_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT8_FETCH_AND_ADD_IS_NEVER_NATIVE
template <>
inline bool AtomicTraits<1>::isLockFree()
{
   return false;
};
#endif

template <>
struct AtomicOpsSupport<2>
{
   enum {
      IsSupported = 1
   };
};
#define NOTIFY_ATOMIC_INT16_IS_SUPPORTED
#if ATOMIC_CHAR_LOCK_FREE == 2
#  define NOTIFY_ATOMIC_INT16_REFRENCE_COUNTING_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT16_TEST_AND_SET_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT16_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#  define NOTIFY_ATOMIC_INT16_FETCH_AND_ADD_IS_ALWAYS_NATIVE
template <>
inline bool AtomicTraits<2>::isLockFree()
{
   return false;
};
#elif ATOMIC_CHAR_LOCK_FREE == 1
#  define NOTIFY_ATOMIC_INT16_REFRENCE_COUNTING_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT16_TEST_AND_SET_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT16_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#  define NOTIFY_ATOMIC_INT16_FETCH_AND_ADD_IS_SOMETIMES_NATIVE
template <>
inline bool AtomicTraits<2>::isLockFree()
{
   return false;
};
#else
#  define NOTIFY_ATOMIC_INT16_REFRENCE_COUNTING_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT16_TEST_AND_SET_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT16_FETCH_AND_STORE_IS_NEVER_NATIVE
#  define NOTIFY_ATOMIC_INT16_FETCH_AND_ADD_IS_NEVER_NATIVE
template <>
inline bool AtomicTraits<2>::isLockFree()
{
   return false;
};
#endif

template <>
struct AtomicOpsSupport<8>
{
   enum {
      IsSupported = 1
   };
};

#ifndef NOTIFY_NO_STD_ATOMIC64
#  define NOTIFY_ATOMIC_INT64_IS_SUPPORTED
#  if ATOMIC_CHAR_LOCK_FREE == 2
#     define NOTIFY_ATOMIC_INT64_REFRENCE_COUNTING_IS_ALWAYS_NATIVE
#     define NOTIFY_ATOMIC_INT64_TEST_AND_SET_IS_ALWAYS_NATIVE
#     define NOTIFY_ATOMIC_INT64_FETCH_AND_STORE_IS_ALWAYS_NATIVE
#     define NOTIFY_ATOMIC_INT64_FETCH_AND_ADD_IS_ALWAYS_NATIVE
template <>
inline bool AtomicTraits<8>::isLockFree()
{
   return true;
};
#  elif ATOMIC_CHAR_LOCK_FREE == 1
#     define NOTIFY_ATOMIC_INT64_REFRENCE_COUNTING_IS_SOMETIMES_NATIVE
#     define NOTIFY_ATOMIC_INT64_TEST_AND_SET_IS_SOMETIMES_NATIVE
#     define NOTIFY_ATOMIC_INT64_FETCH_AND_STORE_IS_SOMETIMES_NATIVE
#     define NOTIFY_ATOMIC_INT64_FETCH_AND_ADD_IS_SOMETIMES_NATIVE
template <>
inline bool AtomicTraits<8>::isLockFree()
{
   return false;
};
#  else
#     define NOTIFY_ATOMIC_INT64_REFRENCE_COUNTING_IS_NEVER_NATIVE
#     define NOTIFY_ATOMIC_INT64_TEST_AND_SET_IS_NEVER_NATIVE
#     define NOTIFY_ATOMIC_INT64_FETCH_AND_STORE_IS_NEVER_NATIVE
#     define NOTIFY_ATOMIC_INT64_FETCH_AND_ADD_IS_NEVER_NATIVE
template <>
inline bool AtomicTraits<8>::isLockFree()
{
   return false;
};
#  endif
#endif

template <typename ValueType>
struct AtomicOps
{
   typedef std::atomic<ValueType> AtomicType;

   template <typename T>
   static inline T load(const std::atomic<T> &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.load(std::memory_order_relaxed);
   }

   template <typename T>
   static inline T load(const volatile std::atomic<T> &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.load(std::memory_order_relaxed);
   }

   template <typename T>
   static inline T loadAcquire(const std::atomic<T> &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.load(std::memory_order_acquire);
   }

   template <typename T>
   static inline T loadAcquire(const volatile std::atomic<T> &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.load(std::memory_order_acquire);
   }

   template <typename T>
   static inline void store(std::atomic<T> &atomicValue, T newValue) NOTIFY_DECL_NOEXCEPT
   {
      atomicValue.store(newValue, std::memory_order_relaxed);
   }

   template <typename T>
   static inline void storeRelease(std::atomic<T> &atomicValue, T newValue) NOTIFY_DECL_NOEXCEPT
   {
      atomicValue.store(newValue, std::memory_order_release);
   }

   static inline bool isReferenceCountingNative() NOTIFY_DECL_NOEXCEPT
   {
      return isTestAndSetNative();
   }

   static inline bool isReferenceCountingWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return false;
   }

   template <typename T>
   static inline bool ref(std::atomic<T> &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      return ++atomicValue != 0;
   }

   template <typename T>
   static inline bool deref(std::atomic<T> &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      return --atomicValue != 0;
   }

   static inline bool isTestAndSetNative() NOTIFY_DECL_NOEXCEPT
   {
      return AtomicTraits<sizeof(ValueType)>::isLockFree();
   }

   static inline bool isTestAndSetWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return false;
   }

   template <typename T>
   static bool testAndSetRelaxed(std::atomic<T> &atomicValue,
                                 T expectedValue,
                                 T newValue,
                                 T *currentValue = NOTIFY_NULLPTR) NOTIFY_DECL_NOEXCEPT
   {
      bool tmp = atomicValue.compare_exchange_strong(expectedValue, newValue, std::memory_order_relaxed);
      if (currentValue) {
         *currentValue = expectedValue;
      }
      return tmp;
   }

   template <typename T>
   static bool testAndSetAcquire(std::atomic<T> &atomicValue,
                                 T expectedValue,
                                 T newValue,
                                 T *currentValue = NOTIFY_NULLPTR) NOTIFY_DECL_NOEXCEPT
   {
      bool tmp = atomicValue.compare_exchange_strong(expectedValue, newValue, std::memory_order_acquire);
      if (currentValue) {
         *currentValue = expectedValue;
      }
      return tmp;
   }

   template <typename T>
   static bool testAndSetRelease(std::atomic<T> &atomicValue,
                                 T expectedValue,
                                 T newValue,
                                 T *currentValue = NOTIFY_NULLPTR) NOTIFY_DECL_NOEXCEPT
   {
      bool tmp = atomicValue.compare_exchange_strong(expectedValue, newValue, std::memory_order_release);
      if (currentValue) {
         *currentValue = expectedValue;
      }
      return tmp;
   }

   template <typename T>
   static bool testAndSetOrdered(std::atomic<T> &atomicValue,
                                 T expectedValue,
                                 T newValue,
                                 T *currentValue = NOTIFY_NULLPTR) NOTIFY_DECL_NOEXCEPT
   {
      bool tmp = atomicValue.compare_exchange_strong(expectedValue, newValue, std::memory_order_acq_rel);
      if (currentValue) {
         *currentValue = expectedValue;
      }
      return tmp;
   }

   static inline bool isFetchAndStoreNative() NOTIFY_DECL_NOEXCEPT
   {
      return isTestAndSetNative();
   }

   static inline bool isFetchAndStoreWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return false;
   }

   template <typename T>
   static T fetchAndStoreRelaxed(std::atomic<T> &atomicValue, T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.exchange(newValue, std::memory_order_relaxed);
   }

   template <typename T>
   static T fetchAndStoreAcquire(std::atomic<T> &atomicValue, T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.exchange(newValue, std::memory_order_acquire);
   }

   template <typename T>
   static T fetchAndStoreRelease(std::atomic<T> &atomicValue, T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.exchange(newValue, std::memory_order_release);
   }

   template <typename T>
   static T fetchAndStoreOrdered(std::atomic<T> &atomicValue, T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.exchange(newValue, std::memory_order_acq_rel);
   }

   static inline bool isFetchAndAddNative() NOTIFY_DECL_NOEXCEPT
   {
      return isTestAndSetNative();
   }

   static inline bool isFetchAndAddWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return false;
   }

   template <typename T>
   static inline
   T fetchAndAddRelaxed(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_add(operand, std::memory_order_relaxed);
   }

   template <typename T>
   static inline
   T fetchAndAddAcquire(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_add(operand, std::memory_order_acquire);
   }

   template <typename T>
   static inline
   T fetchAndAddRelease(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_add(operand, std::memory_order_release);
   }

   template <typename T>
   static inline
   T fetchAndAddOrdered(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_add(operand, std::memory_order_acq_rel);
   }

   template <typename T>
   static inline
   T fetchAndSubRelaxed(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_sub(operand, std::memory_order_relaxed);
   }

   template <typename T>
   static inline
   T fetchAndSubAcquire(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_sub(operand, std::memory_order_acquire);
   }

   template <typename T>
   static inline
   T fetchAndSubRelease(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_sub(operand, std::memory_order_release);
   }

   template <typename T>
   static inline
   T fetchAndSubOrdered(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_sub(operand, std::memory_order_acq_rel);
   }

   template <typename T>
   static inline
   T fetchAndAndRelaxed(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_and(operand, std::memory_order_relaxed);
   }

   template <typename T>
   static inline
   T fetchAndAndAcquire(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_and(operand, std::memory_order_acquire);
   }

   template <typename T>
   static inline
   T fetchAndAndRelease(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_and(operand, std::memory_order_release);
   }

   template <typename T>
   static inline
   T fetchAndAndOrdered(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_and(operand, std::memory_order_acq_rel);
   }

   template <typename T>
   static inline
   T fetchAndOrRelaxed(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_or(operand, std::memory_order_relaxed);
   }

   template <typename T>
   static inline
   T fetchAndOrAcquire(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_or(operand, std::memory_order_acquire);
   }

   template <typename T>
   static inline
   T fetchAndOrRelease(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_or(operand, std::memory_order_release);
   }

   template <typename T>
   static inline
   T fetchAndOrOrdered(std::atomic<T> &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_or(operand, std::memory_order_acq_rel);
   }

   template <typename T>
   static inline
   T fetchAndXorRelaxed(std::atomic<T> &atomicValue,
                       typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_xor(operand, std::memory_order_relaxed);
   }

   template <typename T>
   static inline
   T fetchAndXorAcquire(std::atomic<T> &atomicValue,
                       typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_xor(operand, std::memory_order_acquire);
   }

   template <typename T>
   static inline
   T fetchAndXorRelease(std::atomic<T> &atomicValue,
                       typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_xor(operand, std::memory_order_release);
   }

   template <typename T>
   static inline
   T fetchAndXorOrdered(std::atomic<T> &atomicValue,
                       typename AtomicAdditiveType<T>::AdditiveType operand) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue.fetch_xor(operand, std::memory_order_acq_rel);
   }
};

#if defined(NOTIFY_COMPILER_CONSTEXPR) && defined(NOTIFY_COMPILER_DEFAULT_MEMBERS) && \
defined(NOTIFY_COMPILER_DELETE_MEMBERS)
#  define NOTIFY_BASIC_ATOMIC_INITIALIZER(a) { a }
#else
#  define NOTIFY_BASIC_ATOMIC_INITIALIZER(a) { ATOMIC_VAR_INIT(a) }
#endif

} // threadinternal
} // notify

#endif //NOTIFY_THREAD_INTERNAL_ATOMICCXX11_H
