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

#ifndef NOTIFY_THREAD_BASICATOMIC_H
#define NOTIFY_THREAD_BASICATOMIC_H

#include "notify/kernel/Global.h"

// #if defined(NOTIFY_COMPILER_ATOMICS) && (defined(NOTIFY_COMPILER_CONSTEXPR) || defined(NOTIFY_OS_QNX))
// #include "internal/AtomicCxx11.h"
// #elif defined(NOTIFY_CC_MSVC)
// #else
// #  error "libnotify requires C++11 support"
// #endif
#include "internal/AtomicCxx11.h"

#if defined(NOTIFY_COMPILER_CONSTEXPR) && defined(NOTIFY_COMPILER_DEFAULT_MEMBERS) && defined(NOTIFY_COMPILER_DELETE_MEMBERS)
# if defined(NOTIFY_CC_CLANG) && NOTIFY_CC_CLANG < 303
   /*
      Do not define NOTIFY_BASIC_ATOMIC_HAS_CONSTRUCTORS for Clang before version 3.3.
      For details about the bug: see http://llvm.org/bugs/show_bug.cgi?id=12670
    */
# else
#  define NOTIFY_BASIC_ATOMIC_HAS_CONSTRUCTORS
# endif
#endif
namespace notify
{

template <typename T>
class BasicAtomicInteger
{
public:
   typedef threadinternal::AtomicOps<T> OpsCls;
   NOTIFY_STATIC_ASSERT_X(std::is_integral<T>::value, "template parameter is not an integral type");
   NOTIFY_STATIC_ASSERT_X(threadinternal::AtomicOpsSupport<sizeof(T)>::IsSupported,
                          "template parameter is an integral of a size not supported on this platform");
   typename OpsCls::AtomicType m_atomicValue;

   T load() const NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::load(m_atomicValue);
   }

   void store(T newValue) NOTIFY_DECL_NOEXCEPT
   {
      OpsCls::store(m_atomicValue, newValue);
   }

   T loadAcquire() const NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::loadAcquire(m_atomicValue);
   }

   void storeRelease(T newValue) NOTIFY_DECL_NOEXCEPT
   {
      OpsCls::storeRelease(m_atomicValue, newValue);
   }

   operator T() const NOTIFY_DECL_NOEXCEPT
   {
      return loadAcquire();
   }

   T operator=(T newValue) NOTIFY_DECL_NOEXCEPT
   {
      storeRelease(newValue);
      return newValue;
   }

   static bool isReferenceCountingNative() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isReferenceCountingNative();
   }

   static bool isReferenceCountingWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isReferenceCountingWaitFree();
   }

   bool ref() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::ref(m_atomicValue);
   }

   bool deref() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::deref(m_atomicValue);
   }

   static bool isTestAndSetNative() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isTestAndSetNative();
   }

   static bool isTestAndSetWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isTestAndSetWaitFree();
   }

   bool testAndSetRelaxed(T expectedValue, T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetRelaxed(m_atomicValue, expectedValue, newValue);
   }

   bool testAndSetAcquire(T expectedValue, T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetAcquire(m_atomicValue, expectedValue, newValue);
   }

   bool testAndSetRelease(T expectedValue, T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetRelease(m_atomicValue, expectedValue, newValue);
   }

   bool testAndSetOrdered(T expectedValue, T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetOrdered(m_atomicValue, expectedValue, newValue);
   }

   bool testAndSetRelaxed(T expectedValue, T newValue, T &currentValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetRelaxed(m_atomicValue, expectedValue, newValue, &currentValue);
   }

   bool testAndSetAcquire(T expectedValue, T newValue, T &currentValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetAcquire(m_atomicValue, expectedValue, newValue, &currentValue);
   }

   bool testAndSetRelease(T expectedValue, T newValue, T &currentValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetRelease(m_atomicValue, expectedValue, newValue, &currentValue);
   }

   bool testAndSetOrdered(T expectedValue, T newValue, T &currentValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetOrdered(m_atomicValue, expectedValue, newValue, &currentValue);
   }

   static bool isFetchAndStoreNative() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isFetchAndStoreNative();
   }

   static bool isFetchAndStoreWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isFetchAndStoreWaitFree();
   }

   T fetchAndStoreRelaxed(T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndStoreRelaxed(m_atomicValue, newValue);
   }

   T fetchAndStoreAcquire(T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndStoreAcquire(m_atomicValue, newValue);
   }

   T fetchAndStoreRelease(T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndStoreRelease(m_atomicValue, newValue);
   }

   T fetchAndStoreOrdered(T newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndStoreOrdered(m_atomicValue, newValue);
   }

   static bool isFetchAndAddNative() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isFetchAndAddNative();
   }

   static bool isFetchAndAddWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isFetchAndAddWaitFree();
   }

   T fetchAndAddRelaxed(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAddRelaxed(m_atomicValue, operand);
   }

   T fetchAndAddAcquire(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAddAcquire(m_atomicValue, operand);
   }

   T fetchAndAddRelease(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAddRelease(m_atomicValue, operand);
   }

   T fetchAndAddOrdered(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAddOrdered(m_atomicValue, operand);
   }

   T fetchAndSubRelaxed(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndSubRelaxed(m_atomicValue, operand);
   }

   T fetchAndSubAcquire(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndSubAcquire(m_atomicValue, operand);
   }

   T fetchAndSubRelease(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndSubRelease(m_atomicValue, operand);
   }

   T fetchAndSubOrdered(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndSubOrdered(m_atomicValue, operand);
   }

   T fetchAndAndRelaxed(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAndRelaxed(m_atomicValue, operand);
   }

   T fetchAndAndAcquire(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAndAcquire(m_atomicValue, operand);
   }

   T fetchAndAndRelease(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAndRelease(m_atomicValue, operand);
   }

   T fetchAndAndOrdered(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAndOrdered(m_atomicValue, operand);
   }

   T fetchAndOrRelaxed(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndOrRelaxed(m_atomicValue, operand);
   }

   T fetchAndOrAcquire(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndOrAcquire(m_atomicValue, operand);
   }

   T fetchAndOrRelease(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndOrRelease(m_atomicValue, operand);
   }

   T fetchAndOrOrdered(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndOrOrdered(m_atomicValue, operand);
   }

   T fetchAndXorRelaxed(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndXorRelaxed(m_atomicValue, operand);
   }

   T fetchAndXorAcquire(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndXorAcquire(m_atomicValue, operand);
   }

   T fetchAndXorRelease(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndXorRelease(m_atomicValue, operand);
   }

   T fetchAndXorOrdered(T operand) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndXorOrdered(m_atomicValue, operand);
   }

   T operator++() NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(1) + 1;
   }

   T operator++(int) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(1);
   }

   T operator--() NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(1) - 1;
   }

   T operator--(int) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(1);
   }

   T operator+=(T v) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(v) + v;
   }

   T operator-=(T v) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(v) - v;
   }

   T operator&=(T v) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndAndOrdered(v) & v;
   }

   T operator|=(T v) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndOrOrdered(v) | v;
   }

   T operator^=(T v) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndXorOrdered(v) ^ v;
   }

#ifdef NOTIFY_BASIC_ATOMIC_HAS_CONSTRUCTORS
   BasicAtomicInteger() = default;
   constexpr BasicAtomicInteger(T value) NOTIFY_DECL_NOEXCEPT : m_atomicValue(value)
   {}
   BasicAtomicInteger(const BasicAtomicInteger &) = delete;
   BasicAtomicInteger &operator=(const BasicAtomicInteger &) = delete;
   BasicAtomicInteger &operator=(const BasicAtomicInteger &) volatile = delete;
#endif
};

typedef BasicAtomicInteger<int> BasicAtomicInt;

template <typename T>
class BasicAtomicPointer
{
public:
   typedef T *Type;
   typedef threadinternal::AtomicOps<T> OpsCls;
   typedef typename OpsCls::AtomicType AtomicType;
   AtomicType m_atomicValue;

   AtomicType load() const NOTIFY_DECL_NOEXCEPT
   {
      OpsCls::load(m_atomicValue);
   }

   void store(AtomicType newValue) NOTIFY_DECL_NOEXCEPT
   {
      OpsCls::store(m_atomicValue, newValue);
   }

   operator AtomicType() const NOTIFY_DECL_NOEXCEPT
   {
      return loadAcquire();
   }

   AtomicType operator=(AtomicType newValue)
   {
      storeRelease(newValue);
      return newValue;
   }

   // Atomic API, implemented in AtomicCxx11.h
   AtomicType loadAcquire() const NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::loadAcquire(m_atomicValue);
   }

   void storeRelease(AtomicType newValue) NOTIFY_DECL_NOEXCEPT
   {
      OpsCls::storeRelease(m_atomicValue, newValue);
   }

   static bool isTestAndSetNative() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isTestAndSetNative();
   }

   static bool isTestAndSetWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isTestAndSetWaitFree();
   }

   bool testAndSetRelaxed(AtomicType expectedValue, AtomicType newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetRelaxed(m_atomicValue, expectedValue, newValue);
   }

   bool testAndSetAcquire(AtomicType expectedValue, AtomicType newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetAcquire(m_atomicValue, expectedValue, newValue);
   }

   bool testAndSetRelease(AtomicType expectedValue, AtomicType newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetRelease(m_atomicValue, expectedValue, newValue);
   }

   bool testAndSetOrdered(AtomicType expectedValue, AtomicType newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetOrdered(m_atomicValue, expectedValue, newValue);
   }

   bool testAndSetRelaxed(AtomicType expectedValue,
                          AtomicType newValue,
                          AtomicType &currentValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetRelaxed(m_atomicValue, expectedValue, newValue, &currentValue);
   }

   bool testAndSetAcquire(AtomicType expectedValue,
                          AtomicType newValue,
                          AtomicType &currentValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetAcquire(m_atomicValue, expectedValue, newValue, &currentValue);
   }

   bool testAndSetRelease(AtomicType expectedValue,
                          AtomicType newValue,
                          AtomicType &currentValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetRelease(m_atomicValue, expectedValue, newValue, &currentValue);
   }

   bool testAndSetOrdered(AtomicType expectedValue,
                          AtomicType newValue,
                          AtomicType &currentValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::testAndSetOrdered(m_atomicValue, expectedValue, newValue, &currentValue);
   }

   static bool isFetchAndStoreNative() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isFetchAndStoreNative();
   }

   static bool isFetchAndStoreWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isFetchAndStoreWaitFree();
   }

   AtomicType fetchAndStoreRelaxed(AtomicType newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndStoreRelaxed(m_atomicValue, newValue);
   }

   AtomicType fetchAndStoreAcquire(AtomicType newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndStoreAcquire(m_atomicValue, newValue);
   }

   AtomicType fetchAndStoreRelease(AtomicType newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndStoreRelease(m_atomicValue, newValue);
   }

   AtomicType fetchAndStoreOrdered(AtomicType newValue) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndStoreOrdered(m_atomicValue, newValue);
   }

   static bool isFetchAndAddNative() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isFetchAndAddNative();
   }

   static bool isFetchAndAddWaitFree() NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::isFetchAndAddWaitFree();
   }

   AtomicType fetchAndAddRelaxed(n_ptrdiff valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAddRelaxed(m_atomicValue, valueToAdd);
   }

   AtomicType fetchAndAddAcquire(n_ptrdiff valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAddAcquire(m_atomicValue, valueToAdd);
   }

   AtomicType fetchAndAddRelease(n_ptrdiff valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAddRelease(m_atomicValue, valueToAdd);
   }

   AtomicType fetchAndAddOrdered(n_ptrdiff valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndAddOrdered(m_atomicValue, valueToAdd);
   }

   AtomicType fetchAndSubRelaxed(n_ptrdiff valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndSubRelaxed(m_atomicValue, valueToAdd);
   }

   AtomicType fetchAndSubAcquire(n_ptrdiff valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndSubAcquire(m_atomicValue, valueToAdd);
   }

   AtomicType fetchAndSubRelease(n_ptrdiff valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndSubRelease(m_atomicValue, valueToAdd);
   }

   AtomicType fetchAndSubOrdered(n_ptrdiff valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      return OpsCls::fetchAndSubOrdered(m_atomicValue, valueToAdd);
   }

   AtomicType operator++() NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(1) + 1;
   }

   AtomicType operator++(int) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(1);
   }

   AtomicType operator--() NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(1) - 1;
   }

   AtomicType operator--(int) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(1);
   }

   AtomicType operator+=(n_ptrdiff valueToAdd) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndAddOrdered(valueToAdd) + valueToAdd;
   }

   AtomicType operator-=(n_ptrdiff valueToSub) NOTIFY_DECL_NOEXCEPT
   {
      return fetchAndSubOrdered(valueToSub) - valueToSub;
   }

#ifdef NOTIFY_BASIC_ATOMIC_HAS_CONSTRUCTORS
   BasicAtomicPointer() = default;
   constexpr BasicAtomicPointer(AtomicType value) NOTIFY_DECL_NOEXCEPT : m_atomicValue(value)
   {}
   BasicAtomicPointer(const BasicAtomicPointer &) = delete;
   BasicAtomicPointer &operator=(const BasicAtomicPointer &) = delete;
   BasicAtomicPointer &operator=(const BasicAtomicPointer &) volatile = delete;
#endif
};

#ifndef NOTIFY_BASIC_ATOMIC_INITIALIZER
#  define NOTIFY_BASIC_ATOOMIC_INITIALIZER(a) { (a) }
#endif

} // notify


#endif // NOTIFY_THREAD_BASICATOMIC_H
