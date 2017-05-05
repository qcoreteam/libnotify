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
// Created by zzu_softboy on 04/05/2017.

#ifndef NOTIFY_UTILS_VERSION_H
#define NOTIFY_UTILS_VERSION_H

#include <vector>
#include <array>
#include <string>

#include "notify/kernel/Global.h"

namespace notify
{

using std::vector;
using std::array;
using std::string;

class Version
{
   static NOTIFY_DECL_CONSTEXPR n_uint8 InlineSegmentMarker = NOTIFY_BYTE_ORDER == NOTIFY_LITTLE_ENDIAN ?
                                                       0 : sizeof(void*) - 1;
   static NOTIFY_DECL_CONSTEXPR n_uint8 InlineSegmentStartIdx = !InlineSegmentMarker ? 1 : 0;
   static NOTIFY_DECL_CONSTEXPR n_uint8 InlineSegmentCount = sizeof(void*) - 1;
   NOTIFY_STATIC_ASSERT(InlineSegmentCount >= 3)

   struct SegmentStorage
   {
      // Note: we alias the use of dummy and inline_segments in the use of the
      // union below. This is undefined behavior in C++98, but most compilers implement
      // the C++11 behavior. The one known exception is older versions of Sun Studio.
      union
      {
         n_uintptr dummy;
         n_int8 inlineSegments[sizeof(void*)];
         vector<int> *pointerSegments;
      };

      SegmentStorage() NOTIFY_DECL_NOEXCEPT : dummy(1)
      {}

      SegmentStorage(const vector<int> &segment)
      {
         if (dataFitsInline(segment)) {
            setInlineData(segment);
         } else {
            pointerSegments = new vector<int>(segment);
         }
      }

      SegmentStorage(const SegmentStorage &other)
      {
         if (other.isUsingPointer()) {
            pointerSegments = other.pointerSegments;
         } else {
            dummy = other.dummy;
         }
      }

      SegmentStorage &operator=(const SegmentStorage &other)
      {
         if (isUsingPointer() && other.isUsingPointer()) {
            *pointerSegments = *other.pointerSegments;
         } else if (other.isUsingPointer()) {
            pointerSegments = new vector<int>(*other.pointerSegments);
         } else {
            if (isUsingPointer()) {
               delete pointerSegments;
            }
            dummy = other.dummy;
         }
         return *this;
      }

#ifdef NOTIFY_COMPILER_RVALUE_REFS
      SegmentStorage(SegmentStorage &&other) NOTIFY_DECL_NOEXCEPT
         : dummy(other.dummy)
      {
         other.dummy = 1;
      }

      SegmentStorage &operator=(SegmentStorage &&other) NOTIFY_DECL_NOEXCEPT
      {
         swap(dummy, other.dummy);
         return *this;
      }

      explicit SegmentStorage(vector<int> &&segment)
      {
         if (dataFitsInline(segment)) {
            setInlineData(segment);
         } else {
            pointerSegments = new vector<int>(std::move(segment));
         }
      }
#endif
#ifdef NOTIFY_COMPILER_INITIALIZER_LISTS
      SegmentStorage(std::initializer_list<int> args)
      {
         if (dataFitsInline(args)) {
            setInlineData(args);
         } else {
            pointerSegments = new vector<int>(args);
         }
      }
#endif

      bool isUsingPointer() const NOTIFY_DECL_NOEXCEPT
      {
         return (inlineSegments[InlineSegmentMarker] & 1) == 0;
      }

      int size() const NOTIFY_DECL_NOEXCEPT
      {
         return isUsingPointer() ? pointerSegments->size() : (inlineSegments[InlineSegmentMarker] >> 1);
      }

      void setInlineSize(int len)
      {
         inlineSegments[InlineSegmentMarker] = 1 + 2 * len;
      }

      void resize(int len)
      {
         if (isUsingPointer()) {
            pointerSegments->resize(len);
         } else {
            setInlineSize(len);
         }
      }

      int at(int index) const
      {
         return isUsingPointer() ?
                pointerSegments->at(index) :
                inlineSegments[InlineSegmentStartIdx + index];
      }

      void setSegments(int len, int major, int minor = 0, int patch = 0)
      {
         if (major == n_int8(major) && minor == n_int8(minor) && patch == n_int8(patch))
         {
            setInlineData(array<int, 3>{major, minor, patch});
         } else {
            setVector(len, major, minor, patch);
         }
      }

      ~SegmentStorage()
      {
         if (isUsingPointer()) {
            delete pointerSegments;
         }
      }
   private:
      template <typename T>
      static bool dataFitsInline(const T &data)
      {
         int len = data.size();
         if (len > InlineSegmentCount) {
            return false;
         }
         auto iterator = data.begin();
         for (int i = 0; i < len; i++) {
            if (*iterator != n_int8(*iterator)) {
               return false;
            }
            iterator++;
         }
         return true;
      }

      template <typename T>
      void setInlineData(const T &data)
      {
         int len = data.size();
         auto iterator = data.begin();
         dummy = 1 + len * 2;
#if NOTIFY_BYTE_ORDER == NOTIFY_LITTLE_ENDIAN
         for (int i = 0; i < len; i++) {
            dummy |= n_uintptr(*iterator & 0xFF) << (8 * (i + 1));
            iterator++;
         }
#elif NOTIFY_BYTE_ORDER == NOTIFY_BIG_ENDIAN
         for (int i = 0; i < len; i++) {
            dummy |= n_uintptr(iterator & 0xFF) << (8 * (sizeof(void *) - i - 1));
            iterator++;
         }
#else
         setInlineSize(len);
         for (int i = 0; i < len; i++) {
            inlineSegments[InlineSegmentStartIdx + i] = *iterator & 0xFF;
         }
#endif
      }

      void setVector(int len, int major, int minor, int patch);
   } m_segments;

public:
   inline Version() NOTIFY_DECL_NOEXCEPT
      : m_segments()
   {}

   inline explicit Version(const vector<int> &segment)
      : m_segments(segment)
   {}

#ifdef NOTIFY_COMPILER_RVALUE_REFS
   explicit Version(vector<int> &&segment)
      : m_segments(std::move(segment))
   {}
#endif
#ifdef NOTIFY_COMPILER_INITIALIZER_LISTS
   inline Version(std::initializer_list<int> args)
      : m_segments(args)
   {}
#endif

   inline explicit Version(int major)
   {
      m_segments.setSegments(1, major);
   }

   inline explicit Version(int major, int minor)
   {
      m_segments.setSegments(2, major, minor);
   }

   inline explicit Version(int major, int minor, int patch)
   {
      m_segments.setSegments(3, major, minor, patch);
   }

   inline bool isNull() const NOTIFY_DECL_NOEXCEPT NOTIFY_REQUIRE_RESULT
   {
      return segmentCount() == 0;
   }

   inline bool isNormalized() const NOTIFY_DECL_NOEXCEPT NOTIFY_REQUIRE_RESULT
   {
      return isNull() || segmentAt(segmentCount() - 1) != 0;;
   }

   inline int major() const NOTIFY_DECL_NOEXCEPT NOTIFY_REQUIRE_RESULT
   {
      return segmentAt(0);
   }

   inline int minor() const NOTIFY_DECL_NOEXCEPT NOTIFY_REQUIRE_RESULT
   {
      return segmentAt(1);
   }

   inline int patch() const NOTIFY_DECL_NOEXCEPT NOTIFY_REQUIRE_RESULT
   {
      return segmentAt(2);
   }

   Version normalized() const NOTIFY_REQUIRE_RESULT;

   vector<int> segments() const NOTIFY_REQUIRE_RESULT;

   inline int segmentAt(int index) const NOTIFY_DECL_NOEXCEPT NOTIFY_REQUIRE_RESULT
   {
      return (m_segments.size() > index) ? m_segments.at(index) : 0;
   }

   inline int segmentCount() const NOTIFY_DECL_NOEXCEPT NOTIFY_REQUIRE_RESULT
   {
      return m_segments.size();
   }

   bool isPrefixOf(const Version &other) const NOTIFY_DECL_NOEXCEPT NOTIFY_REQUIRE_RESULT;

   static int compare(const Version &left, const Version &right) NOTIFY_DECL_NOEXCEPT NOTIFY_REQUIRE_RESULT;

   static NOTIFY_DECL_PURE_FUNCTION Version commonPrefix(const Version &left,
                                                         const Version &right) NOTIFY_REQUIRE_RESULT;

   string toString() const NOTIFY_REQUIRE_RESULT;

   static NOTIFY_DECL_PURE_FUNCTION Version fromString(const string &string,
                                                       int *suffixIndex = NOTIFY_NULLPTR) NOTIFY_REQUIRE_RESULT;
};

NOTIFY_REQUIRE_RESULT inline bool operator>(const Version &left, const Version &right) NOTIFY_DECL_NOEXCEPT
{
   return notify::Version::compare(left, right) > 0;
}

NOTIFY_REQUIRE_RESULT inline bool operator>=(const Version &left, const Version &right) NOTIFY_DECL_NOEXCEPT
{
   return notify::Version::compare(left, right) >= 0;
}

NOTIFY_REQUIRE_RESULT inline bool operator<(const Version &left, const Version &right) NOTIFY_DECL_NOEXCEPT
{
   return notify::Version::compare(left, right) < 0;
}

NOTIFY_REQUIRE_RESULT inline bool operator<=(const Version &left, const Version &right) NOTIFY_DECL_NOEXCEPT
{
   return notify::Version::compare(left, right) <= 0;
}

NOTIFY_REQUIRE_RESULT inline bool operator==(const Version &left, const Version &right) NOTIFY_DECL_NOEXCEPT
{
   return notify::Version::compare(left, right) == 0;
}

NOTIFY_REQUIRE_RESULT inline bool operator!=(const Version &left, const Version &right) NOTIFY_DECL_NOEXCEPT
{
   return notify::Version::compare(left, right) != 0;
}

}

#endif //NOTIFY_UTILS_VERSION_H
