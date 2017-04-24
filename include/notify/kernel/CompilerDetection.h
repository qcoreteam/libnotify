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

#ifndef LIBNOTIFY_GLOBAL_H
# include "notify/kernel/Global.h"
#endif

#ifndef NOTIFY_COMPILERDETECTION_H
#define NOTIFY_COMPILERDETECTION_H

// The compiler, must be one of: (NOTIFY_CC_x)

//   SYM      - Digital Mars C/C++ (used to be Symantec C++)
//   MSVC     - Microsoft Visual C/C++, Intel C++ for Windows
//   BOR      - Borland/Turbo C++
//   WAT      - Watcom C++
//   GNU      - GNU C++
//   COMEAU   - Comeau C++
//   EDG      - Edison Design Group C++
//   OC       - CenterLine C++
//   SUN      - Forte Developer, or Sun Studio C++
//   MIPS     - MIPSpro C++
//   DEC      - DEC C++
//   HPACC    - HP aC++
//   USLC     - SCO OUDK and UDK
//   CDS      - Reliant C++
//   KAI      - KAI C++
//   INTEL    - Intel C++ for Linux, Intel C++ for Windows
//   HIGHC    - MetaWare High C/C++
//   PGI      - Portland Group C++
//   GHS      - Green Hills Optimizing C++ Compilers
//   RVCT     - ARM Realview Compiler Suite
//   CLANG    - C++ front-end for the LLVM compiler
//
//   Should be sorted most to least authoritative.

// Symantec C++ is now Digital Mars
#if defined(__DMC__) || defined(__SC__)
#  define NOTIFY_CC_SYM
// "explicit" semantics implemented in 8.1e but keyword recognized since 7.5
#  if defined(__SC__) && __SC__ < 0x750
#     error "Compiler not supported"
#  endif
#elif defined(_MSC_VER)
#  ifdef __clang__
#     define NOTIFY_CC_CLANG ((__clang_major__ * 100) + __clang_minor__)
#  endif
#  define NOTIFY_CC_MSVC (_MSC_VER)
#  define NOTIFY_CC_MSVC_NET
#  define NOTIFY_OUTOFLINE_TEMPLATE inline
#  if _MSC_VER < 1600
#     define NOTIFY_NO_TEMPLATE_FRIENDS
#  endif
#  define NOTIFY_COMPILER_MANGLES_RETURN_TYPE
#  define NOTIFY_FUNC_INFO __FUNCSIG__
#  define NOTIFY_ALIGNOF(type) __alignof(type)
#  define NOTIFY_DECL_ALIGN(n) __declspec(align(n))
#  define NOTIFY_ASSUME_IMPL(expr) __assume(expr)
#  define NOTIFY_UNREACHABLE_IMPL() __assume(0)
#  define NOTIFY_NORETURN __declspec(noreturn)
#  define NOTIFY_DECL_DEPRECATED __declspec(deprecated)
#  ifndef NOTIFY_CC_CLANG
#     define NOTIFY_DECL_DEPRECATED_X(text) __declspec(deprecated(text))
#  endif
#  define NOTIFY_DECL_EXPORT __declspec(dllexport)
#  define NOTIFY_DECL_IMPORT __declspec(dllimport)
#  if _MSC_VER >= 1800
#     define NOTIFY_MAKE_UNCHECKED_ARRAY_ITERATOR(x) stdext::make_unchecked_array_iterator(x)
#  endif
#  if _MSC_VER >= 1500
#     define NOTIFY_MAKE_CHECKED_ARRAY_ITERATOR(x, N) stdext::make_checked_array_iterator(x, size_t(N))
#  endif
// Intel C++ disguising as Visual C++: the `using' keyword avoids warnings
#  if defined(__INTEL_COMPILER)
#     define NOTIFY_DECL_VARIABLE_DEPRECATED
#     define NOTIFY_CC_INTEL  __INTEL_COMPILER
#  endif

// only defined for MSVC since that's the only compiler that actually optimizes for this
// might get overridden further down when NOTIFY_COMPILER_NOEXCEPT is detected
#  ifdef __cplusplus
#    define NOTIFY_DECL_NOTHROW  throw()
#  endif

#elif defined(__BORLANDC__) || defined(__TURBOC__)
#  define NOTIFY_CC_BOR
#  define NOTIFY_INLINE_TEMPLATE
#  if __BORLANDC__ < 0x502
#    error "Compiler not supported"
#  endif

#elif defined(__WATCOMC__)
#  define NOTIFY_CC_WAT

// ARM Realview Compiler Suite
// RVCT compiler also defines __EDG__ and __GNUC__ (if --gnu flag is given),
// so check for it before that
#elif defined(__ARMCC__) || defined(__CC_ARM)
#  define NOTIFY_CC_RVCT
// work-around for missing compiler intrinsics
#  define __is_empty(X) false
#  define __is_pod(X) false
#  define NOTIFY_DECL_DEPRECATED __attribute__ ((__deprecated__))
#  ifdef NOTIFY_OS_LINUX
#     define NOTIFY_DECL_EXPORT __attribute__((visibility("default")))
#     define NOTIFY_DECL_IMPORT __attribute__((visibllity("default")))
#     define NOTIFY_DECL_HIDDEN __attribute__((visibility("hidden")))
#  else
#     define NOTIFY_DECL_EXPORT __declspec(dllexport)
#     define NOTIFY_DECL_IMPORT __declspec(dllimport)
#  endif
#elif defined(__GNUC__)
#  define NOTIFY_DECL_GNU (__GNUC__ * 100 + __GNUC_MINOR__)
#  if defined(__MINGW32__)
#    define NOTIFY_CC_MINGW
#  endif
#  if defined(__INTEL_COMPILER)
// Intel C++ also masquerades as GCC
#     define NOTIFY_CC_INTEL (__INTEL_COMPILER)
#     ifdef __clang__
// Intel C++ masquerades as Clang masquerading as GCC
#        define NOTIFY_CC_CLANG 305
#     endif
#     define NOTIFY_ASSUME_IMPL(expr) __assume(expr)
#     define NOTIFY_UNREACHABLE_IMPL() __builtin_unreachable()
#     if __INTEL_COMPILER >= 1300 && !defined(__APPLE__)
#        define NOTIFY_DECL_DEPRECATED_X(text) __attribute__ ((__deprecated__(text)))
#     endif
#  elif defined(__clang__)
// Clang also masquerades as GCC
#     if defined(__apple_build_version__)
// http://en.wikipedia.org/wiki/Xcode#Toolchain_Versions
#        if __apple_build_version__ >= 7000053
#           define NOTIFY_CC_CLANG 306
#        elif __apple_build_version__ >= 6000051
#           define NOTIFY_CC_CLANG 305
#        elif __apple_build_version__ >= 5030038
#           define NOTIFY_CC_CLANG 304
#        elif __apple_build_version__ >= 5000275
#           define NOTIFY_CC_CLANG 303
#        elif __apple_build_version__ >= 4250024
#           define NOTIFY_CC_CLANG 302
#        elif __apple_build_version__ >= 3180045
#           define NOTIFY_CC_CLANG 301
#        elif __apple_build_version__ >= 2111001
#           define NOTIFY_CC_CLANG 300
#        else
#           error "Unknow Apple Clang version"
#        endif
#     else
#        define NOTIFY_CC_CLANG ((__clang_major__ * 100) + __clang_minor__)
#     endif
#     if __has_builtin(__builtin_assume)
#        define NOTIFY_ASSUME_IMPL(expr) __builtin_assume(expr)
#     else
#        define NOTIFY_ASSUME_IMPL(expr) if (expr) {} else __builtin_unreachable()
#     endif
#     define NOTIFY_UNREACHABLE_IMPL() __builtin_unreachable()
#     if !defined(__has_extension)
// Compatibility with older Clang versions
#        define __has_extension __has_feature
#     endif
#     if defined(__APPLE__)
// Apple/clang specific features
#        define NOTIFY_DECL_CF_RETURNS_RETAINED __attribute__((cf_returns_retained))
#        ifdef __OBJC__
#           define NOTIFY_DECL_NS_RETURNS_AUTORELEASED __attribute__((ns_returns_autoreleased))
#        endif
#     endif
#  else
// Plain GCC
#     if NOTIFY_CC_GNU >= 405
#        define NOTIFY_ASSUME_IMPL(expr) if (expr) {} __builtin_unreachable()
#        define NOTIFY_UNREACHABLE_IMPL() __builtin_unreachable()
#        define NOTIFY_DECL_DEPRECATED_X(text) __attribute__ ((__deprecated__(text)))
#     endif
#  endif

#  ifdef NOTIFY_OS_WIN
#     define NOTIFY_DECL_EXPORT __declspec(dllexport)
#     define NOTIFY_DECL_IMPORT __declspec(dllimport)
#  elif defined(NOTIFY_VISIBILITY_AVAILABLE)
#     define NOTIFY_DECL_EXPORT __attribute__((visibility("default")))
#     define NOTIFY_DECL_IMPORT __attribute__((visibility("default")))
#     define NOTIFY_DECL_HIDDEN __attribute__((visibility("hidden")))
#  endif

#  define NOTIFY_FUNC_INFO __PRETTY_FUNCTION__
#  define NOTIFY_ALIGNOF(type) __alignof__(type)
#  define NOTIFY_TYPEOF(expr) __typeof__(expr)
#  define NOTIFY_DECL_DEPRECATED __attribute__((__deprecated__))
#  define NOTIFY_DECL_ALIGN(n) __attribute((__aligned__(n)))
#  define NOTIFY_DECL_UNUSED __attribute__((__unused__))
#  define NOTIFY_LIKELY(expr) __builtin_expect(!!(expr), true)
#  define NOTIFY_UNLIKELY(expr) __builtin_expect(!!(expr), false)
#  define NOTIFY_NORETURN __attribute__((__noreturn__))
#  define NOTIFY_REQUIRE_RESULT __attribute__((__warn_unused_result__))
#  define NOTIFY_DECL_PURE_FUNCTION __attribute__((pure))
#  define NOTIFY_DECL_CONST_FUNCTION __attribute__((const))
#  define NOTIFY_PACKED __attribute__ ((__packed__))
#  ifndef __ARM_EABI__
#     define NOTIFY_NO_ARM_EABI
#  endif
#  if NOTIFY_CC_GNU >= 403 && !defined(NOTIFY_CC_CLANG)
#     define NOTIFY_ALLOC_SIZE(x) __attribute__((alloc_size(x)))
#  endif
#endif

#endif //NOTIFY_COMPILERDETECTION_H
