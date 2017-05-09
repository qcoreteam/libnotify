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
// Created by zzu_softboy on 24/04/2017.

#ifndef NOTIFY_KERNEL_GLOBAL_H
#define NOTIFY_KERNEL_GLOBAL_H

#include "notify/kernel/PrivateConfig.h"
#include "notify/kernel/Config.h"

#include <type_traits>
#include <cstddef>

#define NOTIFY_STRINGIFY2(x) #x
#define NOTIFY_STRINGIFY(x) NOTIFY_STRINGIFY2(x)

#include "notify/kernel/SystemDetection.h"
#include "notify/kernel/ProcessorDetection.h"
#include "notify/kernel/CompilerDetection.h"

// 定义一些数据类型的别名

using n_int8 = signed char;
using n_uint8 = unsigned char;
using n_int16 = short;
using n_uint16 = unsigned short;
using n_uint32 = unsigned int;
using n_int32 = int;

#if defined(NOTIFY_OS_WIN) && !defined(NOTIFY_CC_GNU)
#  define N_INT64_C(c) c ## i64
#  define N_UINT64_C(c) c ## ui64
using n_int64 = __int64;
using n_uint64 = unsigned __int64;
#else
#  define N_INT64_C(c) static_cast<long long>(c ## LL)
#  define N_UINT64_C(c) static_cast<unsigned long long>(c ## ULL)
using n_int64 = long long;
using n_uint64 = unsigned long long;
#endif

using n_longlong = n_int64;
using n_ulonglong = n_uint64;

using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;

#if defined(__ELF__)
#  define NOTIFY_OF_ELF
#endif

#if defined(__MACH__) && defined(__APPLE__)
#  define NOTIFY_OF_MACH_O
#endif

inline void notify_noop(void) {}

template <typename T>
static inline T *notify_get_ptr_helper(T *ptr)
{
   return ptr;
}

template <typename T>
static inline typename T::pointer notify_get_ptr_helper(const T &p)
{
   return p.data();
}

#define NOTIFY_DECLARE_PRIVATE(Class)\
   inline Class##Private* getImplPtr()\
{\
   return reinterpret_cast<Class##Private *>(notify_get_ptr_helper(implPtr))\
   }\
   inline const Class##Private* getImplPtr() const\
{\
   return reinterpret_cast<const Class##Provate *>(notify_get_ptr_helper(implPtr))\
   }\
   friend class Class##Private;

#define NOTIFY_DECLARE_PUBLIC(Class)\
   inline Class* getApiPtr()\
{\
   return static_cast<Class *>(apiPtr);\
   }\
   inline const Class* getApiPtr() const\
{\
   return static_const<const Class *>(apiPtr);\
   }\
   friend class Class;

#define NOTIFY_D(Class) Class##Private * const implPtr = getImplPtr()
#define NOTIFY_Q(Class) Class * const apiPtr = getApiPtr()

#ifdef NOTIFY_COMPILER_STATIC_ASSERT
#  define NOTIFY_STATIC_ASSERT(condition) static_assert(bool(condition), #condition)
#  define NOTIFY_STATIC_ASSERT_X(condition, message) static_assert(bool(condition), message)
#else
// Intentionally undefined
template <bool Test> class NStaticAssertFailure;
template <> class NStaticAssertFailure<true> {};

#  define NOTIFY_STATIC_ASSERT_PRIVATE_JOIN(A, B) NOTIFY_STATIC_ASSERT_PRIVATE_JOIN_IMPL(A, B)
#  define NOTIFY_STATIC_ASSERT_PRIVATE_JOIN_IMPL(A, B) A ## B
#  ifdef __COUNTER__
#     define NOTIFY_STATIC_ASSERT(condition) \
   enum {NOTIFY_STATIC_ASSERT_PRIVATE_JOIN(n_static_assert_result, __COUNTER__) = sizeof(NStaticAssertFailure<!!(condition)>)};
#  else
#     define NOTIFY_STATIC_ASSERT(condition) \
   enum {NOTIFY_STATIC_ASSERT_PRIVATE_JOIN(n_static_assert_result, __LINE__) = sizeof(NStaticAssertFailure<!!(condition)>)};
#  endif // __COUNTER__
#  define NOTIFY_STATIC_ASSERT_X(condition, message) NOTIFY_STATIC_ASSERT(condition)
#endif

template <int> struct NIntegerForSize;
template <> struct NIntegerForSize<1> { using Unsigned = n_uint8; using Signed = n_int8; };
template <> struct NIntegerForSize<2> { using Unsigned = n_uint16; using Signed = n_int16; };
template <> struct NIntegerForSize<4> { using Unsigned = n_uint32; using Singed = n_int32; };
template <> struct NIntegerForSize<8> { using Unsigned = n_uint64; using Signed = n_int64; };
#if defined(NOTIFY_CC_GNU) && defined(__SIZEOF_INT128__)
template <> struct NIntegerForSize<16>
{
   __extension__ using Unsigned = unsigned __int128;
   __extension__ using Signed = __int128;
};
template <typename T> struct NIntegerForSizeof : NIntegerForSize<sizeof(T)>
{};
using n_registerint = NIntegerForSize<NOTIFY_PROCESSOR_WORDSIZE>::Signed;
using n_registeruint = NIntegerForSize<NOTIFY_PROCESSOR_WORDSIZE>::Unsigned;
using n_uintptr = NIntegerForSizeof<void*>::Unsigned;
using n_ptrdiff = NIntegerForSizeof<void*>::Signed;
using n_intptr = n_ptrdiff;
#endif

namespace internal
{
namespace swapexceptiontester
{

using std::swap;
template <typename T>
void check_swap(T &t) NOTIFY_DECL_NOEXCEPT_EXPR(noexcept(swap(t, t)));

} // swapexceptiontester
} // internal

template <typename T>
inline void swap(T &left, T &right)
NOTIFY_DECL_NOEXCEPT_EXPR(noexcept(internal::swapexceptiontester::check_swap(left)))
{
   using std::swap;
   swap(left, right);
}

namespace notify
{
using HANDLE = void *;
} // notify

#ifdef NOTIFY_CC_MSVC
#  define NOTIFY_NEVER_INLINE __declspec(noinline)
#  define NOTIFY_ALWAYS_INLINE __forceinline
#elif defined(NOTIFY_CC_GNU)
#  define NOTIFY_NEVER_INLINE __attribute__((noinline))
#  define NOTIFY_ALWAYS_INLINE inline __attribute__((always_inline))
#else
#  define NOTIFY_NEVER_INLINE
#  define NOTIFY_ALWAYS_INLINE inline
#endif

#endif //NOTIFY_KERNEL_GLOBAL_H
