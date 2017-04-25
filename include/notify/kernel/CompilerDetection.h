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
// IBM compiler versions are a bit messy. There are actually two products:
// the C product, and the C++ product. The C++ compiler is always packaged
// with the latest version of the C compiler. Version numbers do not always
// match. This little table (I'm not sure it's accurate) should be helpful:
// C++ product                C product
// C Set 3.1                  C Compiler 3.0
// ...                        ...
// C++ Compiler 3.6.6         C Compiler 4.3
// ...                        ...
// Visual Age C++ 4.0         ...
// ...                        ...
// Visual Age C++ 5.0         C Compiler 5.0
// ...                        ...
// Visual Age C++ 6.0         C Compiler 6.0
// Now:
// __xlC__    is the version of the C compiler in hexadecimal notation
//
// is only an approximation of the C++ compiler version
// __IBMCPP__ is the version of the C++ compiler in decimal notation
//
// but it is not defined on older compilers like C Set 3.1
#elif defined(__xlC__)
#  define NOTIFY_CC_XLC
#  define NOTIFY_FULL_TEMPLATE_INSTANTIATION
#  if __xlC__ < 0x400
#     error "Compiler not supported"
#  elif __xlC__ >= 0x0600
#     define NOTIFY_ALIGNOF(type) __alignof__(type)
#     define NOTIFY_TYPEOF(expr) __typeof__(expr)
#     define NOTIFY_DECL_ALIGN(n) __attribute__((__aligned__(n)))
#     define NOTIFY_PACKED __attribute__((__packed__))
#  endif
// Older versions of DEC C++ do not define __EDG__ or __EDG - observed
// on DEC C++ V5.5-004. New versions do define  __EDG__ - observed on
// Compaq C++ V6.3-002.
// This compiler is different enough from other EDG compilers to handle
// it separately anyway.
#elif defined(__DECCXX) || defined(__DECC)
#  define NOTIFY_CC_DEC
// Compaq C++ V6 compilers are EDG-based but I'm not sure about older
// DEC C++ V5 compilers.
#  if defined(__EDG__)
#     define NOTIFY_CC_EDG
#  endif
// Compaq has disabled EDG's _BOOL macro and uses _BOOL_EXISTS instead
// - observed on Compaq C++ V6.3-002.
// In any case versions prior to Compaq C++ V6.0-005 do not have bool.
#  if !defined(_BOOL_EXISTS)
#     error "Compiler not supported"
#  endif
// Spurious (?) error messages observed on Compaq C++ V6.5-014.
// Apply to all versions prior to Compaq C++ V6.0-000 - observed on
// DEC C++ V5.5-004.
#  if __DECCXX_VER < 60060000
#     define NOTIFY_BROKEN_TEMPLATE_SPECIALIZATION
#  endif
// avoid undefined symbol problems with out-of-line template members
#  define NOTIFY_OUTOFLINE_TEMPLATE inline

// The Portland Group C++ compiler is based on EDG and does define __EDG__
// but the C compiler does not
#elif defined(__PGI)
#  define NOTIFY_CC_PGI
#  if defined(__EDG__)
#     define NOTIFY_CC_EDG
#  endif

// Compilers with EDG front end are similar. To detect them we test:
// __EDG documented by SGI, observed on MIPSpro 7.3.1.1 and KAI C++ 4.0b
// __EDG__ documented in EDG online docs, observed on Compaq C++ V6.3-002
// and PGI C++ 5.2-4
#elif !defined(NOTIFY_OS_HPUX) && (defined(__EDG) || defined(__EDG__))
// From the EDG documentation (does not seem to apply to Compaq C++ or GHS C):
// _BOOL
//    Defined in C++ mode when bool is a keyword. The name of this
//    predefined macro is specified by a configuration flag. _BOOL
//    is the default.
// __BOOL_DEFINED
//    Defined in Microsoft C++ mode when bool is a keyword.
#  define NOTIFY_CC_EDG
#  if !defined(_BOOL) && !defined(__BOOL_DEFINED) && !defined(__ghs)
#     error "Compiler not supported"
#  endif
// The Comeau compiler is based on EDG and does define __EDG__
#  if defined(__COMO__)
#     define NOTIFY_CC_COMEAU
// The `using' keyword was introduced to avoid KAI C++ warnings
// but it's now causing KAI C++ errors instead. The standard is
// unclear about the use of this keyword, and in practice every
// compiler is using its own set of rules. Forget it.
#  elif defined(__KCC)
#     define NOTIFY_CC_KAI
// Using the `using' keyword avoids Intel C++ for Linux warnings
#  elif defined(__INTEL_COMPILER)
#     define NOTIFY_CC_INTEL (__INTEL_COMPILER)
// Uses CFront, make sure to read the manual how to tweak templates.
#  elif defined(__ghs)
#     define NOTIFY_CC_GHS
#     define NOTIFY_DECL_DEPRECATED __attribute__ ((__deprecated__))
#     define NOTIFY_FUNC_INFO __PRETTY_FUNCTION__
#     define NOTIFY_TYPEOF(expr) __typeof__(expr)
#     define NOTIFY_ALIGNOF(type) __alignof__(type)
#     define NOTIFY_UNREACHABLE_IMPL()
#     if defined(__cplusplus)
#        define NOTIFY_COMPILER_AUTO_TYPE
#        define NOTIFY_COMPILER_STATIC_ASSERT
#        define NOTIFY_COMPILER_RANGE_FOR
#        if __GHS_VERSION_NUMBER >= 201505
#           define NOTIFY_COMPILER_ALIGNAS
#           define NOTIFY_COMPILER_ALIGNOF
#           define NOTIFY_COMPILER_ATOMICS
#           define NOTIFY_COMPILER_ATTRIBUTES
#           define NOTIFY_COMPILER_AUTO_FUNCTION
#           define NOTIFY_COMPILER_CLASS_ENUM
#           define NOTIFY_COMPILER_CONSTEXPR
#           define NOTIFY_COMPILER_DECLTYPE
#           define NOTIFY_COMPILER_DEFAULT_MEMBERS
#           define NOTIFY_COMPILER_DELETE_MEMBERS
#           define NOTIFY_COMPILER_DELEGATING_CONSTRUCTORS
#           define NOTIFY_COMPILER_EXPLICIT_CONVERSIONS
#           define NOTIFY_COMPILER_EXPLICIT_OVERRIDES
#           define NOTIFY_COMPILER_EXTERN_TEMPLATES
#           define NOTIFY_COMPILER_INHERITING_CONSTRUCTORS
#           define NOTIFY_COMPILER_INITIALIZER_LISTS
#           define NOTIFY_COMPILER_LAMBDA
#           define NOTIFY_COMPILER_NONSTATIC_MEMBER_INIT
#           define NOTIFY_COMPILER_NOEXCEPT
#           define NOTIFY_COMPILER_NULLPTR
#           define NOTIFY_COMPILER_RANGE_FOR
#           define NOTIFY_COMPILER_RAW_STRINGS
#           define NOTIFY_COMPILER_REF_QUALIFIERS
#           define NOTIFY_COMPILER_RVALUE_REFS
#           define NOTIFY_COMPILER_STATIC_ASSERT
#           define NOTIFY_COMPILER_TEMPLATE_ALIAS
#           define NOTIFY_COMPILER_THREAD_LOCAL
#           define NOTIFY_COMPILER_THREADSAFE_STATICS
#           define NOTIFY_COMPILER_UDL
#           define NOTIFY_COMPILER_UNICODE_STRINGS
#           define NOTIFY_COMPILER_UNIFORM_INIT
#           define NOTIFY_COMPILER_UNRESTRICTED_UNIONS
#           define NOTIFY_COMPILER_VARIADIC_MACROS
#           define NOTIFY_COMPILER_VARIADIC_TEMPLATES
#        endif
#     endif
#  elif defined(__DCC__)
#     define NOTIFY_CC_DIAB
#     if !defined(__bool)
#        error "Compiler not supported"
#     endif
// The UnixWare 7 UDK compiler is based on EDG and does define __EDG__
#  elif defined(__USLC__) && defined(__SCO_VERSION__)
#     define NOTIFY_CC_USLC
// The latest UDK 7.1.1b does not need this, but previous versions do
#     if !defined(__SCO_VERSION__) || (__SCO_VERSION__ < 302200010)
#        define NOTIFY_OUTOFLINE_TEMPLATE inline
#     endif
// Never tested!
#  elif defined(CENTERLINE_CLPP) || defined(OBJECTCENTER)
#     define NOTIFY_CC_OC
// CDS++ defines __EDG__ although this is not documented in the Reliant
// documentation. It also follows conventions like _BOOL and this documented
#  elif defined(sinix)
#     define NOTIFY_CC_CDS
// The MIPSpro compiler defines __EDG
#  elif defined(__sgi)
#     define NOTIFY_CC_MIPS
#     define NOTIFY_NO_TEMPLATE_FRIENDS
#     if defined(_COMPILER_VERSION) && (_COMPILER_VERSION > 740)
#        define NOTIFY_OUTOFLINE_TEMPLATE inline
#        pragma set woff 3624,3625,3649 // turn off some harmless warnings
#     endif
#  endif
// VxWorks' DIAB toolchain has an additional EDG type C++ compiler
// (see __DCC__ above). This one is for C mode files (__EDG is not defined)
#elif defined(_DIAB_TOOL)
#  define NOTIFY_CC_DIAB
#  define NOTIFY_FUNC_INFO __PRETTY_FUNCTION__
//  Never tested!
#elif defined(__HIGHC__)
#  define NOTIFY_CC_HIGHC
#elif defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#  define NOTIFY_CC_SUN
#  define NOTIFY_COMPILER_MANGLES_RETURN_TYPE
// 5.0 compiler or better
//  'bool' is enabled by default but can be disabled using -features=nobool
//  in which case _BOOL is not defined
//      this is the default in 4.2 compatibility mode triggered by -compat=4
#  if __SUNPRO_CC >= 0x500
#     define NOTIFY_NO_TEMPLATE_TEMPLATE_PARAMETERS
// see http://developers.sun.com/sunstudio/support/Ccompare.html
#     if __SUNPRO_CC >= 0x590
#        define NOTIFY_ALIGNOF(type) __alignof__(type)
#        define NOTIFY_TYPEOF(expr) __typeof__(expr)
#        define NOTIFY_DECL_ALIGN(n) __attribute__((__aligned__(n)))
#     endif
#     if __SUNPRO_CC >= 0x550
#        define NOTIFY_DECL_EXPORT __global
#     endif
#     if __SUNPRO_CC < 0x5a0
#        define NOTIFY_NO_TEMPLATE_FRIENDS
#     endif
#     if !defined(_BOOL)
#        error "Compiler not supported"
#     endif
// 4.2 compiler or older
#  else
#     error "Compiler not supported"
#  endif
// CDS++ does not seem to define __EDG__ or __EDG according to Reliant
// documentation but nevertheless uses EDG conventions like _BOOL
#elif defined(sinix)
#  define NOTIFY_CC_EDG
#  define NOTIFY_CC_CDS
#  if !defined(_BOOL)
#     error "Compiler not supported"
#  endif
#  define NOTIFY_BROKEN_TEMPLATE_SPECIALIZATION

#elif defined(NOTIFY_OS_HPUX)
// __HP_aCC was not defined in first aCC releases
#  if defined(__HP_aCC) || __cplusplus >= 199707L
#     define NOTIFY_NO_TEMPLATE_FRIENDS
#     define NOTIFY_CC_HPACC
#     define NOTIFY_FUNC_INFO __PRETTY_FUNCTION__
#     if __HP_aCC-0 < 060000
#        define NOTIFY_NO_TEMPLATE_TEMPLATE_PARAMETERS
#        define NOTIFY_DECL_EXPORT __declspec(dllexport)
#        define NOTIFY_DECL_IMPORT __declspec(dllimport)
#     endif
#     if __HP_aCC-0 >= 061200
#        define NOTIFY_DECL_ALIGN(n) __attribute__((aligned(n)))
#     endif
#     if __HP_aCC-0 >= 062000
#        define NOTIFY_DECL_EXPORT __attribute__((visibility("default")))
#        define NOTIFY_DECL_HIDDEN __attribute__((visibility("hidden")))
#        define NOTIFY_DECL_IMPORT NOTIFY_DECL_EXPORT
#     endif
#  else
#     error "Compiler not supported"
#  endif
#else
#  error "libnotify has not been tested with this compiler"
#endif

// C++11 support
//
// Paper              Macro                                     SD-6 macro
// N2341              NOTIFY_COMPILER_ALIGNAS
// N2341              NOTIFY_COMPILER_ALIGNOF
// N2427              NOTIFY_COMPILER_ATOMICS
// N2761              NOTIFY_COMPILER_ATTRIBUTES                __cpp_attributes = 200809
// N2541              NOTIFY_COMPILER_AUTO_FUNCTION
// N1984 N2546        NOTIFY_COMPILER_AUTO_TYPE
// N2437              NOTIFY_COMPILER_CLASS_ENUM
// N2235              NOTIFY_COMPILER_CONSTEXPR                 __cpp_constexpr = 200704
// N2343 N3276        NOTIFY_COMPILER_DECLTYPE                  __cpp_decltype = 200707
// N2346              NOTIFY_COMPILER_DEFAULT_MEMBERS
// N2346              NOTIFY_COMPILER_DELETE_MEMBERS
// N1986              NOTIFY_COMPILER_DELEGATING_CONSTRUCTORS
// N2437              NOTIFY_COMPILER_EXPLICIT_CONVERSIONS
// N3206 N3272        NOTIFY_COMPILER_EXPLICIT_OVERRIDES
// N1987              NOTIFY_COMPILER_EXTERN_TEMPLATES
// N2540              NOTIFY_COMPILER_INHERITING_CONSTRUCTORS
// N2672              NOTIFY_COMPILER_INITIALIZER_LISTS
// N2658 N2927        NOTIFY_COMPILER_LAMBDA                    __cpp_lambdas = 200907
// N2756              NOTIFY_COMPILER_NONSTATIC_MEMBER_INIT
// N2855 N3050        NOTIFY_COMPILER_NOEXCEPT
// N2431              NOTIFY_COMPILER_NULLPTR
// N2930              NOTIFY_COMPILER_RANGE_FOR
// N2442              NOTIFY_COMPILER_RAW_STRINGS               __cpp_raw_strings = 200710
// N2439              NOTIFY_COMPILER_REF_QUALIFIERS
// N2118 N2844 N3053  NOTIFY_COMPILER_RVALUE_REFS               __cpp_rvalue_references = 200610
// N1720              NOTIFY_COMPILER_STATIC_ASSERT             __cpp_static_assert = 200410
// N2258              NOTIFY_COMPILER_TEMPLATE_ALIAS
// N2659              NOTIFY_COMPILER_THREAD_LOCAL
// N2660              NOTIFY_COMPILER_THREADSAFE_STATICS
// N2765              NOTIFY_COMPILER_UDL                       __cpp_user_defined_literals = 200809
// N2442              NOTIFY_COMPILER_UNICODE_STRINGS           __cpp_unicode_literals = 200710
// N2640              NOTIFY_COMPILER_UNIFORM_INIT
// N2544              NOTIFY_COMPILER_UNRESTRICTED_UNIONS
// N1653              NOTIFY_COMPILER_VARIADIC_MACROS
// N2242 N2555        NOTIFY_COMPILER_VARIADIC_TEMPLATES        __cpp_variadic_templates = 200704
//
// For any future version of the C++ standard, we use only the SD-6 macro.
// For full listing, see
// http://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations
//
// C++ extensions:
//    NOTIFY_COMPILER_RESTRICTED_VLA       variable-length arrays, prior to __cpp_runtime_arrays

#ifdef __cplusplus
#  if __cplusplus < 201103L && !(defined(NOTIFY_CC_MSVC) && NOTIFY_CC_MSVC  >= 1800)
#     error "libnotify requires a C++11 compiler and yours does not seem to be that."
#  endif
#endif

#ifdef NOTIFY_CC_INTEL
#  define NOTIFY_COMPILER_RESTRICTED_VLA
#  define NOTIFY_COMPILER_VARIADIC_MACROS // C++11 feature supported as an extension in other modes, too
#  define NOTIFY_COMPILER_THREADSAFE_STATICS
#  if __INTEL_COMPILER < 1200
#     define NOTIFY_NO_TEMPLATE_FRIENDS
#  endif
#  if __INTEL_COMPILER >= 1310 && !defined(_WIN32)
// ICC supports C++14 binary literals in C, C++98, and C++11 modes
// at least since 13.1, but I can't test further back
#     define NOTIFY_COMPILER_BINARY_LITERALS
#  endif
#  if __cplusplus >= 201103L || defined(__INTEL_CXX11_MODE__)
#     if __INTEL_COMPILER >= 1200
#        define NOTIFY_COMPILER_AUTO_TYPE
#        define NOTIFY_COMPILER_CLASS_ENUM
#        define NOTIFY_COMPILER_DECLTYPE
#        define NOTIFY_COMPILER_DEFAULT_MEMBERS
#        define NOTIFY_COMPILER_DELETE_MEMBERS
#        define NOTIFY_COMPILER_EXTERN_TEMPLATES
#        define NOTIFY_COMPILER_LAMBDA
#        define NOTIFY_COMPILER_RVALUE_REFS
#        define NOTIFY_STATIC_ASSERT
#        define NOTIFY_VARIADIC_MACROS
#     endif
#     if __INTEL_COMPILER >= 1210
#        define NOTIFY_COMPILER_ATTRIBUTES
#        define NOTIFY_COMPILER_AUTO_FUNCTION
#        define NOTIFY_COMPILER_NULLPTR
#        define NOTIFY_COMPILER_TEMPLATE_ALIAS
#        ifndef _CHAR16T // MSVC headers
#           define NOTIFY_COMPILER_UNICODE_STRINGS
#        endif
#        define NOTIFY_COMPILER_VARIADIC_TEMPLATES
#     endif
#     if __INTEL_COMPILER >= 1300
#        define NOTIFY_COMPILER_ATOMICS
//       constexpr support is only partial
//#      define NOTIFY_COMPILER_CONSTEXPR
#        define NOTIFY_COMPILER_INITIALIZER_LISTS
#        define NOTIFY_COMPILER_UNIFORM_INIT
#        define NOTIFY_COMPILER_NOEXCEPT
#     endif
#     if __INTEL_COMPILER >= 1400
// Intel issue ID 6000056211, bug DPD200534796
//#        define NOTIFY_COMPILER_CONSTEXPR
#        define NOTIFY_COMPILER_DELEGATING_CONSTRUCTORS
#        define NOTIFY_COMPILER_EXPLICIT_CONVERSIONS
#        define NOTIFY_COMPILER_EXPLICIT_OVERRIDES
#        define NOTIFY_COMPILER_NONSTATIC_MEMBER_INIT
#        define NOTIFY_COMPILER_RANGE_FOR
#        define NOTIFY_COMPILER_RAW_STRINGS
#        define NOTIFY_COMPILER_REF_QUALIFIERS
#        define NOTIFY_COMPILER_UNICODE_STRINGS
#        define NOTIFY_COMPILER_UNRESTRICTED_UNIONS
#     endif
#     if __INTEL_COMPILER >= 1500
#        if __INTEL_COMPILER * 100 + __INTEL_COMPILER_UPDATE >= 150001
//       the bug mentioned above is fixed in 15.0.1
#           define NOTIFY_COMPILER_CONSTEXPR
#        endif
#        define NOTIFY_COMPILER_ALIGNAS
#        define NOTIFY_COMPILER_ALIGNOF
#        define NOTIFY_COMPILER_INHERITING_CONSTRUCTORS
// C++11 thread_local is broken on OS X (Clang doesn't support it either)
#        ifndef NOTIFY_OS_OSX
#           define NOTIFY_COMPILER_THREAD_LOCAL
#        endif
#        define NOTIFY_COMPILER_UDL
#     endif
#     ifdef _MSC_VER
#        if _MSC_VER == 1700
//       <initializer_list> is missing with MSVC 2012 (it's present in 2010, 2013 and up)
#           undef NOTIFY_COMPILER_INITIALIZER_LISTS
#        endif
#        if _MSC_VER < 1900
//          ICC disables unicode string support when compatibility mode with MSVC 2013 or lower is active
#           undef NOTIFY_COMPILER_UNICODE_STRINGS
//          Even though ICC knows about ref-qualified members, MSVC 2013 or lower doesn't, so
//          certain member functions may be missing from the DLLs.
#           undef NOTIFY_COMPILER_REF_QUALIFIERS
//          Disable constexpr unless the MS headers have constexpr in all the right places too
//          (like std::numeric_limits<T>::max())
#           undef NOTIFY_COMPILER_CONSTEXPR
#        endif
#     endif
#  endif
#endif // NOTIFY_CC_INTEL

#if defined(NOTIFY_CC_CLANG) && !defined(NOTIFY_CC_INTEL)
// General C++ features
#  define NOTIFY_COMPILER_RESTRICTED_VLA
#  define NOTIFY_COMPILER_THREADSAFE_STATICS
#  if __has_feature(attribute_deprecated_with_message)
#     define NOTIFY_DECL_DEPRECATED_X(text) __attribute__((__deprecated__(text)))
#  endif

// Clang supports binary literals in C, C++98 and C++11 modes
// It's been supported "since the dawn of time itself" (cf. commit 179883)
#  if __has_extension(cxx_binary_literals)
#     define NOTIFY_COMPILER_BINARY_LITERALS
#  endif

// Variadic macros are supported for gnu++98, c++11, c99 ... since 2.9
#  if NOTIFY_CC_CLANG >= 209
#     if !defined(__STRICT_ANSI__) || defined(__GXX_EXPERIMENTAL_CXX0X__) \
         || (defined(__cplusplus) && (__cplusplus >= 201103L)) \
         || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
#        define NOTIFY_COMPILER_VARIADIC_MACROS
#     endif
#  endif

// C++11 features, see http://clang.llvm.org/cxx_status.html
#  if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
// Detect C++ features using __has_feature(), see http://clang.llvm.org/docs/LanguageExtensions.html#cxx11
#     if __has_feature(cxx_alignas)
#        define NOTIFY_COMPILER_ALIGNAS
#        define NOTIFY_COMPILER_ALIGNOF
#     endif
#     if __has_feature(cxx_atomic) && __has_include(<atomic>)
#        define NOTIFY_COMPILER_ATOMICS
#     endif
#     if __has_feature(cxx_attributes)
#        define NOTIFY_COMPILER_ATTRIBUTES
#     endif
#     if __has_feature(cxx_auto_type)
#        define NOTIFY_COMPILER_AUTO_FUNCTION
#        define NOTIFY_COMPILER_AUTO_TYPE
#     endif
#     if __has_feature(cxx_strong_enums)
#        define NOTIFY_COMPILER_CLASS_ENUM
#     endif
#     if __has_feature(cxx_constexpr) && NOTIFY_CC_CLANG > 302 // CLANG 3.2 has bad/partial support
#        define NOTIFY_COMPILER_CONSTEXPR
#     endif
#     if __has_feature(cxx_decltype) /* && __has_feature(cxx_decltype_incomplete_return_types) */
#        define NOTIFY_COMPILER_DECLTYPE
#     endif
#     if __has_feature(cxx_defaulted_functions)
#        define NOTIFY_COMPILER_DEFAULT_MEMBERS
#     endif
#     if __has_feature(cxx_deleted_functions)
#        define NOTIFY_COMPILER_DELETE_MEMBERS
#     endif
#     if __has_feature(cxx_delegating_constructors)
#        define NOTIFY_COMPILER_DELEGATING_CONSTRUCTORS
#     endif
#     if __has_feature(cxx_explicit_conversions)
#        define NOTIFY_COMPILER_EXPLICIT_CONVERSIONS
#     endif
#     if __has_feature(cxx_override_control)
#        define NOTIFY_COMPILER_EXPLICIT_OVERRIDES
#     endif
#     if __has_feature(cxx_inheriting_constructors)
#        define NOTIFY_COMPILER_INHERITING_CONSTRUCTORS
#     endif
#     if __has_feature(cxx_generalized_initializers)
#        define NOTIFY_COMPILER_UNIFORM_INIT /* both covered by this feature macro, according to docs */
#     endif
#     if __has_feature(cxx_lambdas)
#        define NOTIFY_COMPILER_LAMBDA
#     endif
#     if __has_feature(cxx_noexcept)
#        define NOTIFY_COMPILER_NOEXCEPT
#     endif
#     if __has_feature(cxx_nonstatic_member_init)
#        define NOTIFY_COMPILER_NONSTATIC_MEMBER_INIT
#     endif
#     if __has_feature(cxx_nullptr)
#        define NOTIFY_COMPILER_NULLPTR
#     endif
#     if __has_feature(cxx_range_for)
#        define NOTIFY_COMPILER_RANGE_FOR
#     endif
#     if __has_feature(cxx_raw_string_literals)
#        define NOTIFY_COMPILER_RAW_STRINGS
#     endif
#     if __has_feature(cxx_reference_qualified_functions)
#        define NOTIFY_COMPILER_REF_QUALIFIERS
#     endif
#     if __has_feature(cxx_rvalue_references)
#        define NOTIFY_COMPILER_RVALUE_REFS
#     endif
#     if __has_feature(cxx_static_assert)
#        define NOTIFY_COMPILER_STATIC_ASSERT
#     endif
#     if __has_feature(cxx_alias_templates)
#        define NOTIFY_COMPILER_TEMPLATE_ALIAS
#     endif
#     if __has_feature(cxx_thread_local)
#        if !defined(__FreeBSD__) /* FreeBSD clang fails on __cxa_thread_atexit */
#           define NOTIFY_COMPILER_THREAD_LOCAL
#        endif
#     endif
#     if __has_feature(cxx_user_literals)
#        define NOTIFY_COMPILER_UDL
#     endif
#     if __has_feature(cxx_unicode_literals)
#        define NOTIFY_COMPILER_UNICODE_STRINGS
#     endif
#     if __has_feature(cxx_unrestricted_unions)
#        define NOTIFY_COMPILER_UNRESTRICTED_UNIONS
#     endif
#     if __has_feature(cxx_variadic_templates)
#        define NOTIFY_COMPILER_VARIADIC_TEMPLATES
#     endif
// Features that have no __has_feature() check
#     if NOTIFY_CC_CLANG >= 209 // since clang 2.9
#        define NOTIFY_COMPILER_EXTERN_TEMPLATES
#     endif
#  endif

// C++1y features, deprecated macros. Do not update this list.
#  if __cplusplus > 201103L
#     if __has_feature(cxx_generic_lambda)
#        define NOTIFY_COMPILER_GENERIC_LAMBDA
#     endif
#     if __has_feature(cxx_init_capture)
#        define NOTIFY_COMPILER_LAMBDA_CAPTURES
#     endif
#     if __has_feature(cxx_relaxed_constexpr)
#        define NOTIFY_COMPILER_RELAXED_CONSTEXPR_FUNCTIONS
#     endif
#     if __has_feature(cxx_decltype_auto) && __has_feature(cxx_return_type_deduction)
#        define NOTIFY_COMPILER_RETURN_TYPE_DEDUCTION
#     endif
#     if __has_feature(cxx_variable_templates)
#        define NOTIFY_COMPILER_VARIABLE_TEMPLATES
#     endif
#     if __has_feature(cxx_runtime_array)
#        define NOTIFY_COMPILER_VLA
#     endif
#  endif

#  if defined(__has_warning)
#     if __has_warning("-Wunused-private-field")
#  endif
#endif // NOTIFY_CC_CLANG

#endif //NOTIFY_COMPILERDETECTION_H
