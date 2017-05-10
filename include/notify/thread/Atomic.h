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
// Created by softboy on 5/9/17.
//

#ifndef NOTIFY_THREAD_ATOMIC_H
#define NOTIFY_THREAD_ATOMIC_H

#include "notify/kernel/Global.h"
#include "BasicAtomic.h"

namespace notify
{

template <typename T>
class AtomicInteger : public BasicAtomicInteger<T>
{
public:
#ifdef NOTIFY_BASIC_ATOMIC_HAS_CONSTRUCTORS
   constexpr AtomicInteger(T value = 0) NOTIFY_DECL_NOEXCEPT : BasicAtomicInteger<T>(value)
   {}
#else
   inline AtomicInteger(T value = 0) NOTIFY_DECL_NOEXCEPT
   {
      this->m_atomicValue = value;
   }
#endif
   inline AtomicInteger(const AtomicInteger &other) NOTIFY_DECL_NOEXCEPT
#ifdef NOTIFY_BASIC_ATOMIC_HAS_CONSTRUCTORS
         : BasicAtomicInteger<T>()
#endif
   {
      this->storeRelease(other.loadAcquire());
   }

   inline AtomicInteger &operator=(const AtomicInteger &other) NOTIFY_DECL_NOEXCEPT
   {
      this->storeRelease(other.loadAcquire());
      return *this;
   }
};

class AtomicInt : public AtomicInteger<int>
{
public:
#ifdef NOTIFY_BASIC_ATOMIC_HAS_CONSTRUCTORS
   constexpr
#endif
   AtomicInt(int value = 0) NOTIFY_DECL_NOEXCEPT : AtomicInteger<int>(value)
   {}
};

template <typename T>
class AtomicPointer : public BasicAtomicPointer<T>
{
public:
#ifdef NOTIFY_BASIC_ATOMIC_HAS_CONSTRUCTORS
   constexpr AtomicPointer(T *value = 0) NOTIFY_DECL_NOEXCEPT : BasicAtomicPointer<T>(value)
   {}
#else
   inline AtomicPointer(T *value = 0) NOTIFY_DECL_NOEXCEPT
   {
      this->store(value);
   }
#endif
   inline AtomicPointer(const AtomicPointer<T> &other) NOTIFY_DECL_NOEXCEPT
   {
      this->storeRelease(other.loadAcquire());
   }

   inline AtomicPointer<T> &operator=(const AtomicPointer<T> &other) NOTIFY_DECL_NOEXCEPT
   {
      this->storeRelease(other.loadAcquire());
      return *this;
   }
};

#ifdef NOTIFY_BASIC_ATOMIC_HAS_CONSTRUCTORS
#  undef NOTIFY_BASIC_ATOMIC_HAS_CONSTRUCTORS
#endif

template <typename T>
inline void atomic_assign(T *&d, T *x)
{
   if (d == x){
      return;
   }
   x->ref.ref();
   if (!d->ref.deref()){
      delete d;
   }
   d = x;
}

template <typename T>
inline void atomic_detach(T *&d)
{
   if (d->ref.load() == 1) {
      return;
   }
   T *x = d;
   d = new T(*d);
   if (!x->ref.deref()) {
      delete x;
   }
}

} // notify

#endif //NOTIFY_THREAD_ATOMIC_H
