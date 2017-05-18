//
// Created by softboy on 5/16/17.
//

#ifndef NOTIFY_THREAD_INTERNAL_MUTEXPRIVATE_H
#define NOTIFY_THREAD_INTERNAL_MUTEXPRIVATE_H

#include "notify/thread/Mutex.h"
#include "notify/thread/Atomic.h"

#ifdef NOTIFY_OS_MAC
#include <mach/semaphore.h>
#elif defined(NOTIFY_OS_LINUX) && !defined(NOTIFY_LINUXBASE)
#  define NOTIFY_LINUX_FUTEX
#elif defined(NOTIFY_OS_UNIX)
#  if _POSIX_VERSION-0 >= 200112L || _XOPEN_VERSION-0 >= 600
#     include <semaphore.h>
#     define NOTIFY_UNIX_SEMAPHORE
#  endif
#endif
#if defined(NOTIFY_OS_UNIX) && defined(NOTIFY_HAVE_PTHREAD_H)
#  include <pthread.h>
#endif

namespace notify
{

class MutexData
{
public:
   bool m_recursive;
   MutexData(Mutex::RecursiveMode mode = Mutex::RecursiveMode::NonRecursive)
         : m_recursive(mode == Mutex::RecursiveMode::Recursive)
   {}
};

#if !defined(NOTIFY_LINUX_FUTEX)

class MutexPrivate : public MutexData
{
public:
   MutexPrivate();
   ~MutexPrivate();

   bool wait(int timeout = -1);
   void wakeUp() NOTIFY_DECL_NOEXCEPT;

   AtomicInt m_refCount;
   int m_id;

   bool ref()
   {
      // NOTIFY_ASSERT(m_refCount.load() >= 0);
      int c;
      do {
         c = m_refCount.load();
         if (c == 0) {
            return false;
         }
      } while (!m_refCount.testAndSetRelaxed(c, c + 1));
      // NOTIFY_ASSERT(m_refCount.load() >= 0);
      return true;
   }

   void deref()
   {
      // NOTIFY_ASSERT(m_refCount.load() >= 0);
      if (!m_refCount.deref())
      {
         release();
      }
      // NOTIFY_ASSERT(m_refCount.load() >= 0);
   }
   void release();
   static MutexPrivate *allocate();
   void derefWaiters(int value) NOTIFY_DECL_NOEXCEPT;
public:
   AtomicInt m_waiters; // Number of threads waiting on this mutex. (may be offset by -BigNumber)
   // Boolean indicating that a timed wait timed out.
   // When it is true, a reference is held.
   // It is there to avoid a race that happens if unlock happens right
   // when the mutex is unlocked.
   AtomicInt m_possibleUnlocked;
   constexpr static int BigNumber = 0x100000;
#ifdef NOTIFY_OS_MAC
   semaphore_t m_machSemaphore;
#elif defined(NOTIFY_UNIX_SEMAPHORE)
   sem_t m_semaphore;
#elif defined(NOTIFY_OS_UNIX)
   bool m_wakeup;
   pthread_mutex_t m_mutex;
   pthread_cond_t m_cond;
#endif
};
#endif // NOTIFY_LINUX_FUTEX

#ifdef NOTIFY_OS_UNIX

void notify_initialize_pthread_cond(pthread_cond_t *cond, const char *where);
void notify_abstime_for_timeout(struct timespec *ts, int timeout);

#endif

} // notify

#endif //NOTIFY_THREAD_INTERNAL_MUTEXPRIVATE_H
