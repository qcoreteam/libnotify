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
// Created by zzu_softboy on 05/05/2017.

#ifndef NOTIFY_KERNEL_TYPETRAITS_H
#define NOTIFY_KERNEL_TYPETRAITS_H

#include "notify/kernel/Global.h"

namespace notify
{
namespace internal
{

template <typename T>
struct not_t : std::integral_constant<bool, !T::value>
{};

template <typename T>
struct IsUnsignedEnum : std::integral_constant<bool, (T(0) < T(-1))>
{};

template <typename T>
struct IsSignedEnum
   : not_t<IsUnsignedEnum<T>>
{};

NOTIFY_STATIC_ASSERT((IsUnsignedEnum<n_uint8>::value));
NOTIFY_STATIC_ASSERT((!IsUnsignedEnum<n_int8>::value));

NOTIFY_STATIC_ASSERT((!IsSignedEnum<n_uint8>::value));
NOTIFY_STATIC_ASSERT((IsSignedEnum<n_int8>::value));

NOTIFY_STATIC_ASSERT((IsUnsignedEnum<n_uint16>::value));
NOTIFY_STATIC_ASSERT((!IsUnsignedEnum<n_int16>::value));

NOTIFY_STATIC_ASSERT((!IsSignedEnum<n_uint16>::value));
NOTIFY_STATIC_ASSERT((IsSignedEnum<n_int16>::value));

NOTIFY_STATIC_ASSERT((IsUnsignedEnum<n_uint32>::value));
NOTIFY_STATIC_ASSERT((!IsUnsignedEnum<n_int32>::value));

NOTIFY_STATIC_ASSERT((!IsSignedEnum<n_uint32>::value));
NOTIFY_STATIC_ASSERT((IsSignedEnum<n_int32>::value));

NOTIFY_STATIC_ASSERT((IsUnsignedEnum<n_uint64>::value));
NOTIFY_STATIC_ASSERT((!IsUnsignedEnum<n_int64>::value));

NOTIFY_STATIC_ASSERT((!IsSignedEnum<n_uint64>::value));
NOTIFY_STATIC_ASSERT((IsSignedEnum<n_int64>::value));

} // internal
} // notify

#endif //NOTIFY_KERNEL_TYPETRAITS_H
