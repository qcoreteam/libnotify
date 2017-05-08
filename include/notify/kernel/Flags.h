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

#include "notify/kernel/TypeTraits.h"

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
public:
#if defined(NOTIFY_CC_MSVC)
   using Int = int;
#else
   using Int = typename std::conditional<
         internal::IsUnsignedEnum<Enum>::value,
         unsigned int,
         signed int
   >::type;
#endif
   using EnumType = Enum;
   NOTIFY_DECL_CONSTEXPR inline Flags() NOTIFY_DECL_NOEXCEPT : m_data(0) {}
   NOTIFY_DECL_CONSTEXPR inline Flags(Enum flag) NOTIFY_DECL_NOEXCEPT : m_data(Int(flag)) {}
   NOTIFY_DECL_CONSTEXPR inline Flags(Flag flag) NOTIFY_DECL_NOEXCEPT : m_data(flag) {}
   NOTIFY_DECL_CONSTEXPR inline Flags(Int flag) NOTIFY_DECL_NOEXCEPT : m_data(flag) {}

#ifdef NOTIFY_COMPILER_INITIALIZER_LISTS
   NOTIFY_DECL_CONSTEXPR inline Flags(std::initializer_list<Enum> flags) NOTIFY_DECL_NOEXCEPT
      : m_data(initializerListHelper(flags.begin(), flags.end())){}
#endif

   NOTIFY_DECL_RELAXED_CONSTEXPR inline Flags &operator&=(int mask) NOTIFY_DECL_NOEXCEPT
   {
      m_data &= mask;
      return *this;
   }

   NOTIFY_DECL_RELAXED_CONSTEXPR inline Flags &operator&=(uint mask) NOTIFY_DECL_NOEXCEPT
   {
      m_data &= mask;
      return *this;
   }

   NOTIFY_DECL_RELAXED_CONSTEXPR inline Flags &operator&=(Enum mask) NOTIFY_DECL_NOEXCEPT
   {
      m_data &= Int(mask);
      return *this;
   }

   NOTIFY_DECL_RELAXED_CONSTEXPR inline Flags &operator|=(Flags flag) NOTIFY_DECL_NOEXCEPT
   {
      m_data |= flag.m_data;
      return *this;
   }

   NOTIFY_DECL_RELAXED_CONSTEXPR inline Flags &operator|=(Enum flag) NOTIFY_DECL_NOEXCEPT
   {
      m_data |= Int(flag);
      return *this;
   }

   NOTIFY_DECL_RELAXED_CONSTEXPR inline Flags &operator^=(Flags flag) NOTIFY_DECL_NOEXCEPT
   {
      m_data ^= flag.m_data;
      return *this;
   }

   NOTIFY_DECL_RELAXED_CONSTEXPR inline Flags &operator^=(Enum flag) NOTIFY_DECL_NOEXCEPT
   {
      m_data ^= Int(flag);
      return *this;
   }

   NOTIFY_DECL_CONSTEXPR inline operator Int() const NOTIFY_DECL_NOEXCEPT
   {
      return m_data;
   }

   NOTIFY_DECL_CONSTEXPR inline Flags operator|(Flags flag) const NOTIFY_DECL_NOEXCEPT
   {
      return Flags(Flag(m_data | flag.m_data));
   }

   NOTIFY_DECL_CONSTEXPR inline Flags operator|(Enum flag) const NOTIFY_DECL_NOEXCEPT
   {
      return Flags(Flag(m_data | Int(flag)));
   }

   NOTIFY_DECL_CONSTEXPR inline Flags operator^(Flags flag) const NOTIFY_DECL_NOEXCEPT
   {
      return Flags(Flag(m_data ^ flag.m_data));
   }

   NOTIFY_DECL_CONSTEXPR inline Flags operator^(Enum flag) const NOTIFY_DECL_NOEXCEPT
   {
      return Flags(Flag(m_data ^ Int(flag)));
   }

   NOTIFY_DECL_CONSTEXPR inline Flags operator&(int mask) const NOTIFY_DECL_NOEXCEPT
   {
      return Flags(Flag(m_data & mask));
   }

   NOTIFY_DECL_CONSTEXPR inline Flags operator&(uint mask) const NOTIFY_DECL_NOEXCEPT
   {
      return Flags(Flag(m_data & mask));
   }

   NOTIFY_DECL_CONSTEXPR inline Flags operator&(Enum mask) const NOTIFY_DECL_NOEXCEPT
   {
      return Flags(Flag(m_data & Int(mask)));
   }

   NOTIFY_DECL_CONSTEXPR inline Flags operator~() const NOTIFY_DECL_NOEXCEPT
   {
      return Flags(Flag(~m_data));
   }

   NOTIFY_DECL_CONSTEXPR inline bool operator!() const NOTIFY_DECL_NOEXCEPT
   {
      return !m_data;
   }

   NOTIFY_DECL_CONSTEXPR inline bool testFlag(Enum flag) const NOTIFY_DECL_NOEXCEPT
   {
      return (m_data & Int(flag)) == Int(flag) && (Int(flag) != 0 || m_data == Int(flag));
   }

   NOTIFY_DECL_RELAXED_CONSTEXPR inline Flags &setFlag(Enum flag, bool on = true) NOTIFY_DECL_NOEXCEPT
   {
      return on ? (*this |= Int(flag)) : (*this &= ~Int(flag));
   }

private:
#ifdef NOTIFY_COMPILER_INITIALIZER_LISTS
   NOTIFY_DECL_CONSTEXPR static inline Int initializerListHelper(
         typename std::initializer_list<Enum>::const_iterator begin,
         typename std::initializer_list<Enum>::const_iterator end
   ) NOTIFY_DECL_NOEXCEPT
   {
      return (begin == end ? Int(0) : (Int(*begin) | initializerListHelper(begin + 1, end)));
   }
#endif
private:
   Int m_data;
};

};

#define NOTIFY_DECLARE_FLAGS(FlagsType, Enum) \
   using FlagsType = notify::Flags<Enum>;

#define NOTIFY_DECLARE_OPERATORS_FOR_FLAGS(FlagsType) \
NOTIFY_DECL_CONSTEXPR inline notify::Flags<FlagsType::EnumType> \
   operator|(FlagsType::EnumType flag1, FlagsType::EnumType flag2) NOTIFY_DECL_NOEXCEPT \
{\
   return notify::Flags<FlagsType::EnumType>(flag1) | flag2;\
}\
NOTIFY_DECL_CONSTEXPR inline notify::Flags<FlagsType::EnumType> \
   operator|(FlagsType::EnumType flag1, notify::Flags<FlagsType::EnumType> flag2) NOTIFY_DECL_NOEXCEPT \
{\
   return flag2 | flag1; \
}

#endif //NOTIFY_KERNEL_FLAGS_H
