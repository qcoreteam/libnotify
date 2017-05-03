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
// Created by zzu_softboy on 03/05/2017.

#ifndef NOTIFY_KERNEL_FLAGS_H
#define NOTIFY_KERNEL_FLAGS_H

#include "notify/kernel/Global.h"
#ifdef NOTIFY_COMPILER_INITIALIZER_LISTS
#include <initializer_list>
#endif

namespace notify
{
class Flag
{
   int i;
public:
   NOTIFY_DECL_CONSTEXPR inline Flag(int ai) NOTIFY_DECL_NOEXCEPT : i(ai)
   {}
   NOTIFY_DECL_CONSTEXPR inline operator int() const NOTIFY_DECL_NOEXCEPT
   {
      return i;
   }
#if !defined(__LP64__)
   NOTIFY_DECL_CONSTEXPR inline Flag(long ai) NOTIFY_DECL_NOEXCEPT : i(int(ai)){}
   NOTIFY_DECL_CONSTEXPR inline Flag(ulong ai) NOTIFY_DECL_NOEXCEPT : i(int(long(ai))){}
#endif
   NOTIFY_DECL_CONSTEXPR inline Flag(uint ai) NOTIFY_DECL_NOEXCEPT : i(int(ai)){}
   NOTIFY_DECL_CONSTEXPR inline Flag(short ai) NOTIFY_DECL_NOEXCEPT : i(int(ai)){}
   NOTIFY_DECL_CONSTEXPR inline Flag(ushort ai) NOTIFY_DECL_NOEXCEPT : i(int(uint(ai))){}
   NOTIFY_DECL_CONSTEXPR inline operator uint() const NOTIFY_DECL_NOEXCEPT
   {
      return uint(i);
   }
};

template<typename Enum>
class Flags
{
   NOTIFY_STATIC_ASSERT_X((sizeof(Enum) <= sizeof(int)),
                          "Flags uses an int as storage, so an enum with underlying "
                                "short than int will overflow.");
};

};

#endif //NOTIFY_KERNEL_FLAGS_H