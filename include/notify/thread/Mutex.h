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
// Created by zzu_softboy on 16/05/2017.

#ifndef NOTIFY_THREAD_MUTEX_H
#define NOTIFY_THREAD_MUTEX_H

#include "notify/kernel/Global.h"
#include "Atomic.h"
#include <new>

#if NOTIFY_HAS_INCLUDE(<chrono>)
#  include <chrono>
#  include <limits>
#endif

#if NOTIFY_OS_LINUX
#  define NOTIFY_MUTEX_LOCK_NOEXCEPT NOTIFY_DECL_NOEXCEPT
#else
#  define NOTIFY_MUTEX_LOCK_NOEXCEPT
#endif

namespace notify
{

class MutexData;

class BasicMutex
{
public:
   inline void lock() NOTIFY_MUTEX_LOCK_NOEXCEPT
   {
      if (!fastTryLock()) {
         lockInternal();
      }
   }

   inline void unlock() NOTIFY_DECL_NOEXCEPT
   {
      // NOTIFY_ASSERT(m_apiPtr.load()); //mutex must be locked
      if (!fastTryUnlock())
      {
         unlockInternal();
      }
   }

   bool tryLock() NOTIFY_DECL_NOEXCEPT
   {
      return fastTryLock();
   }

   bool isRecursive() const NOTIFY_DECL_NOEXCEPT;
private:
   inline bool fastTryLock() NOTIFY_DECL_NOEXCEPT
   {
      return m_apiPtr.testAndSetAcquire(NOTIFY_NULLPTR, dummyLocked());
   }

   inline bool fastTryUnlock() NOTIFY_DECL_NOEXCEPT
   {
      return m_apiPtr.testAndSetAcquire(dummyLocked(), NOTIFY_NULLPTR);
   }

   inline bool fastTryLock(MutexData *&current) NOTIFY_DECL_NOEXCEPT
   {
      return m_apiPtr.testAndSetAcquire(NOTIFY_NULLPTR, dummyLocked(), current);
   }

   inline bool fastTryUnlock(MutexData *&current) NOTIFY_DECL_NOEXCEPT
   {
      return m_apiPtr.testAndSetAcquire(dummyLocked(), NOTIFY_NULLPTR, current);
   }

   void lockInternal() NOTIFY_MUTEX_LOCK_NOEXCEPT;
   bool lockInternal(int timeout) NOTIFY_MUTEX_LOCK_NOEXCEPT;
   void unlockInternal() NOTIFY_DECL_NOEXCEPT;

   static inline MutexData *dummyLocked()
   {
      return reinterpret_cast<MutexData *>(n_uintptr(1));
   }
private:
   BasicAtomicPointer<MutexData> m_apiPtr;
   friend class Mutex;
   friend class MutexData;
};

class Mutex : public BasicMutex
{
public:
   enum class RecursiveMode
   {
      NonRecursive,
      Recursive
   };
};

} // notify

#endif //NOTIFY_THREAD_MUTEX_H
