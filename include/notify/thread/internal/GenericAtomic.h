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
// Created by zzu_softboy on 09/05/2017.

#ifndef NOTIFY_THREAD_INTERNAL_GENERICATOMIC_H
#define NOTIFY_THREAD_INTERNAL_GENERICATOMIC_H

#include "notify/kernel/Global.h"

namespace notify
{
namespace threadinternal
{
template<int>
struct AtomicOpsSupport
{
   enum {
      IsSupported = 0
   };
};

template <>
struct AtomicOpsSupport<4>
{
   enum {
      IsSupported = 1
   };
};

template <typename T>
struct AtomicAdditiveType
{
   typedef T AdditiveType;
   static const int AddScale = 1;
};

template <typename T>
struct AtomicAdditiveType<T *>
{
   typedef n_ptrdiff AdditiveType;
   static const int AddScale = sizeof(T);
};

template <typename BaseClass>
struct GenericAtomicOps
{
   template <typename T>
   struct AtomicType
   {
      typedef T Type;
      typedef T *PointerType;
   };

   template <typename T>
   static void acquireMemoryFence(const T &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::acquireMemoryFence(atomicValue);
   }

   template <typename T>
   static void releaseMemoryFence(const T &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::relaseMemoryFence(atomicValue);
   }

   template <typename T>
   static void orderedMemoryFence(const T &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::orderedMemoryFence(atomicValue);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE T load(const T &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      return atomicValue;
   }

   template <typename T, typename NewValueType>
   static NOTIFY_ALWAYS_INLINE void store(T &atomicValue, NewValueType newValue) NOTIFY_DECL_NOEXCEPT
   {
      atomicValue = newValue;
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE T loadAcquire(const T &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      T tmp = *static_cast<const volatile T *>(&atomicValue);
      BaseClass::acquireMemoryFence(atomicValue);
      return tmp;
   }

   template <typename T, typename NewValueType>
   static NOTIFY_ALWAYS_INLINE void storeRelease(T &atomicValue, NewValueType newValue) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::releaseMemoryFence(atomicValue);
      *static_cast<volatile T *>(&atomicValue) = newValue;
   }

   static inline NOTIFY_DECL_CONSTEXPR bool isReferenceCountingNative() NOTIFY_DECL_NOEXCEPT
   {
      return BaseClass::isReferenceCountingNative();
   }

   static inline NOTIFY_DECL_CONSTEXPR bool isReferenceCountingWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return BaseClass::isReferenceCountingWaitFree();
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE bool ref(T &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      return BaseClass::fetchAndAddRelaxed(atomicValue, 1) != T(-1);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE bool deref(T &atomicValue) NOTIFY_DECL_NOEXCEPT
   {
      return BaseClass::fetchAndAddRelaxed(atomicValue, -1) != T(1);
   }

#if 0
   // These functions have no default implementation
   // Archictectures must implement them
   static inline NOTIFY_DECL_CONSTEXPR bool isTestAndSetNative() NOTIFY_DECL_NOEXCEPT;
   static inline NOTIFY_DECL_CONSTEXPR bool isTestAndSetWaitFree() NOTIFY_DECL_NOEXCEPT;
   template <typename T, typename X>
   static inline bool testAndSetRelaxed(T &atomicValue, X expectedValue, X newValue) NOTIFY_DECL_NOEXCEPT;
   template <typename T, typename X>
   static inline bool testAndSetRelaxed(T &atomicValue,
                                        X expectedValue,
                                        X newValue,
                                        X *currentValue) NOTIFY_DECL_NOEXCEPT;
#endif

   template <typename T, typename X>
   static NOTIFY_ALWAYS_INLINE bool testAndSetAcquire(T &atomicValue,
                                                      X expectedValue,
                                                      X newValue) NOTIFY_DECL_NOEXCEPT
   {
      bool tmp = BaseClass::testAndSetRelaxed(atomicValue, expectedValue, newValue);
      BaseClass::acquireMemoryFence(atomicValue);
      return tmp;
   };

   template <typename T, typename X>
   static NOTIFY_ALWAYS_INLINE bool testAndSetRelease(T &atomicValue,
                                                      X expectedValue,
                                                      X newValue) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::releaseMemoryFence(atomicValue);
      return BaseClass::testAndSetRelaxed(atomicValue, expectedValue, newValue);
   };

   template <typename T, typename X>
   static NOTIFY_ALWAYS_INLINE bool testAndSetOrdered(T &atomicValue,
                                                      X expectedValue,
                                                      X newValue) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::orderedMemoryFence(atomicValue);
      return BaseClass::testAndSetRelaxed(atomicValue, expectedValue, newValue);
   };

   template <typename T, typename X>
   static NOTIFY_ALWAYS_INLINE bool testAndSetAcquire(T &atomicValue,
                                                      X expectedValue,
                                                      X newValue,
                                                      X *currentValue) NOTIFY_DECL_NOEXCEPT
   {
      bool tmp = BaseClass::testAndSetRelaxed(atomicValue, expectedValue, newValue, currentValue);
      BaseClass::acquireMemoryFence(atomicValue);
      return tmp;
   };

   template <typename T, typename X>
   static NOTIFY_ALWAYS_INLINE bool testAndSetRelease(T &atomicValue,
                                                      X expectedValue,
                                                      X newValue,
                                                      X *currentValue) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::releaseMemoryFence(atomicValue);
      return BaseClass::testAndSetRelaxed(atomicValue, expectedValue, newValue, currentValue);
   };

   template <typename T, typename X>
   static NOTIFY_ALWAYS_INLINE bool testAndSetOrdered(T &atomicValue,
                                                      X expectedValue,
                                                      X newValue,
                                                      X *currentValue) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::orderedMemoryFence(atomicValue);
      return BaseClass::testAndSetRelaxed(atomicValue, expectedValue, newValue, currentValue);
   };

   static inline NOTIFY_DECL_CONSTEXPR bool isFetchAndStoreNative() NOTIFY_DECL_NOEXCEPT
   {
      return false;
   }

   static inline NOTIFY_DECL_CONSTEXPR bool isFetchAndStoreWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return false;
   }

   template <typename T, typename X>
   static NOTIFY_ALWAYS_INLINE T fetchAndStoreRelaxed(T &atomicValue, X newValue) NOTIFY_DECL_NOEXCEPT
   {
      while(true)
      {
         T tmp = load(atomicValue);
         if (BaseClass::testAndSetRelaxed(atomicValue, tmp, newValue)) {
            return tmp;
         }
      }
   };

   template <typename T, typename X>
   static NOTIFY_ALWAYS_INLINE T fetchAndStoreAcquire(T &atomicValue, X newValue) NOTIFY_DECL_NOEXCEPT
   {
      T tmp = BaseClass::fetchAndStoreRelaxed(atomicValue, newValue);
      BaseClass::acquireMemoryFence(atomicValue);
      return tmp;
   };

   template <typename T, typename X>
   static NOTIFY_ALWAYS_INLINE T fetchAndStoreRelease(T &atomicValue, X newValue) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::releaseMemoryFence(atomicValue);
      return BaseClass::fetchAndStoreRelaxed(atomicValue, newValue);
   };

   template <typename T, typename X>
   static NOTIFY_ALWAYS_INLINE T fetchAndStoreOrdered(T &atomicValue, X newValue) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::releaseMemoryOrdered(atomicValue);
      return BaseClass::fetchAndStoreRelaxed(atomicValue, newValue);
   };

   static inline NOTIFY_DECL_CONSTEXPR bool isFetchAndAddNative() NOTIFY_DECL_NOEXCEPT
   {
      return false;
   }

   static inline NOTIFY_DECL_CONSTEXPR bool isFetchAndAddWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return false;
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndAddRelaxed(T &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      while(true)
      {
         T tmp = BaseClass::load(atomicValue);
         if (BaseClass::testAndSet(atomicValue, tmp, T(tmp + valueToAdd))) {
            return tmp;
         }
      }
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndAddAcquire(T &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      T tmp = fetchAndAddRelaxed(atomicValue, valueToAdd);
      BaseClass::acquireMemoryFence(atomicValue);
      return tmp;
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndAddRelease(T &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::releaseMemoryFence(atomicValue);
      return fetchAndAddRelaxed(atomicValue, valueToAdd);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndAddOrdered(T &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::orderedMemoryFence(atomicValue);
      return fetchAndAddRelaxed(atomicValue, valueToAdd);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndSubRelaxed(T &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType valueToSub) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndAddRelaxed(atomicValue, -valueToSub);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndSubAcquire(T &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType valueToSub) NOTIFY_DECL_NOEXCEPT
   {
      T tmp = fetchAndSubRelaxed(atomicValue, valueToSub);
      BaseClass::acquireMemoryFence(atomicValue);
      return tmp;
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndSubRelease(T &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType valueToSub) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::releaseMemoryFence(atomicValue);
      return fetchAndSubRelaxed(atomicValue, valueToSub);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndSubOrdered(T &atomicValue,
                        typename AtomicAdditiveType<T>::AdditiveType valueToSub) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::orderedMemoryFence(atomicValue);
      return fetchAndSubRelaxed(atomicValue, valueToSub);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndAndRelaxed(T &atomicValue,
                        typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      T tmp = load(atomicValue);
      while (true) {
         if (BaseClass::testAndSetRelaxed(atomicValue, tmp, T(tmp & operand), &tmp)) {
            return tmp;
         }
      }
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndAndAcquire(T &atomicValue,
                        typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      T tmp = BaseClass::fetchAndAndRelaxed(atomicValue, operand);
      BaseClass::acquireMemoryFence(atomicValue);
      return tmp;
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndAndRelease(T &atomicValue,
                        typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::releaseMemoryFence(atomicValue);
      return BaseClass::fetchAndAndRelaxed(atomicValue, operand);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndAndOrdered(T &atomicValue,
                        typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::orderedMemoryFence(atomicValue);
      return BaseClass::fetchAndAndRelaxed(atomicValue, operand);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndOrRelaxed(T &atomicValue,
                        typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      T tmp = load(atomicValue);
      while (true) {
         if (BaseClass::testAndSetRelaxed(atomicValue, tmp, T(tmp | operand), &tmp)) {
            return tmp;
         }
      }
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndOrAcquire(T &atomicValue,
                        typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      T tmp = BaseClass::fetchAndOrRelaxed(atomicValue, operand);
      BaseClass::acquireMemoryFence(atomicValue);
      return tmp;
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndOrRelease(T &atomicValue,
                        typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::releaseMemoryFence(atomicValue);
      return BaseClass::fetchAndOrRelaxed(atomicValue, operand);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndOrOrdered(T &atomicValue,
                        typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::orderedMemoryFence(atomicValue);
      return BaseClass::fetchAndOrRelaxed(atomicValue, operand);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndXorRelaxed(T &atomicValue,
                       typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      T tmp = load(atomicValue);
      while (true) {
         if (BaseClass::testAndSetRelaxed(atomicValue, tmp, T(tmp ^ operand), &tmp)) {
            return tmp;
         }
      }
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndXorAcquire(T &atomicValue,
                       typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      T tmp = BaseClass::fetchAndXorRelaxed(atomicValue, operand);
      BaseClass::acquireMemoryFence(atomicValue);
      return tmp;
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndXorRelease(T &atomicValue,
                       typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::releaseMemoryFence(atomicValue);
      return BaseClass::fetchAndXorRelaxed(atomicValue, operand);
   }

   template <typename T>
   static NOTIFY_ALWAYS_INLINE
   T fetchAndXorOrdered(T &atomicValue,
                       typename std::enable_if<std::is_integral<T>::value, T>::type operand) NOTIFY_DECL_NOEXCEPT
   {
      BaseClass::orderedMemoryFence(atomicValue);
      return BaseClass::fetchAndXorRelaxed(atomicValue, operand);
   }
};

} // threadinternal
} // notify


#endif // NOTIFY_THREAD_INTERNAL_GENERICATOMIC_H
