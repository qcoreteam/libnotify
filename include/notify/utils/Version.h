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
#include "notify/kernel/Global.h"

namespace notify
{

using std::vector;

class Version
{
   static NOTIFY_DECL_CONSTEXPR n_uint8 InlineSegmentMarker = NOTIFY_BYTE_ORDER == NOTIFY_LITTLE_ENDIAN ?
                                                       0 : sizeof(void*) - 1;
   static NOTIFY_DECL_CONSTEXPR n_uint8 InlineSegmentStartIdx = InlineSegmentMarker ? 1 : 0;
   static NOTIFY_DECL_CONSTEXPR n_uint8 InlineSegmentCount = sizeof(void*) - 1;
   NOTIFY_STATIC_ASSERT(InlineSegmentCount >= 3);

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
         if (dataFitsInline(segment.begin(), segment.size())) {

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
            *pointerSegments = *other.isUsingPointer();
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

      }
#endif

      void setInlineSize(int len)
      {
         inlineSegments[InlineSegmentMarker] = 1 + 2 * len;
      }

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
         if (major == n_int8(major) && min == n_int8(minor) && patch == n_int8(patch))
         {
            int data[] = {
                  major,
                  minor,
                  patch
            };
            setInlineData(data, len);
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
      static bool dataFitsInline(const int *data, int len)
      {
         if (len > InlineSegmentCount) {
            return false;
         }
         for (int i = 0; i < len; i++) {
            if (data[i] != n_int8(data[i])) {
               return false;
            }
         }
         return true;
      }

      void setInlineData(const int *data, int len)
      {
         dummy = 1 + len * 2;
#if NOTIFY_BYTE_ORDER == NOTIFY_LITTLE_ENDIAN
         for (int i = 0; i < len; i++) {
            dummy |= n_uintptr(data[i] & 0xFF) << (8 * (i + 1));
         }
#elif NOTIFY_BYTE_ORDER == NOTIFY_BIG_ENDIAN
         for (int i = 0; i < len; i++) {
            dummy |= n_uintptr(data[i] & 0xFF) << (8 * (sizeof(void *) - i - 1))
         }
#else
         setInlineSize(len);
         for (int i = 0; i < len; i++) {
            inlineSegments[InlineSegmentStartIdx + i] = data[i] & 0xFF;
         }
#endif
      }

      void setVector(int len, int major, int minor, int patch);
   } m_segments;
};

};

#endif //NOTIFY_UTILS_VERSION_H
