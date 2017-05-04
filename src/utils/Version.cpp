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

#include "notify/utils/Version.h"

namespace notify
{

vector<int> Version::segments() const
{
   if (m_segments.isUsingPointer()) {
      return *m_segments.pointerSegments;
   }
   vector<int> result;
   result.resize(segmentCount());
   for (int i = 0; i < segmentCount(); i++) {
      result[i] = segmentAt(i);
   }
   return result;
}

Version Version::normalized() const
{
   int i;
   for (i = m_segments.size(); i; i--) {
      if (m_segments.at(i - 1) != 0) {
         break;
      }
   }
   Version result(*this);
   result.m_segments.resize(i);
   return result;
}

bool Version::isPrefixOf(const Version &other) const NOTIFY_DECL_NOEXCEPT
{
   if (segmentCount() > other.segmentCount()) {
      return false;
   }
   for (int i = 0; i < segmentCount(); i++) {
      if (segmentAt(i) != other.segmentAt(i)) {
         return false;
      }
   }
   return true;
}

int Version::compare(const Version &left, const Version &right) NOTIFY_DECL_NOEXCEPT
{
   int commonLen;
   if (NOTIFY_LIKELY(!left.m_segments.isUsingPointer() && !right.m_segments.isUsingPointer()))
   {
      const n_int8 *leftPtr = left.m_segments.inlineSegments + InlineSegmentStartIdx;
      const n_int8 *rightPtr = right.m_segments.inlineSegments + InlineSegmentStartIdx;
      commonLen = std::min(left.m_segments.size(), right.m_segments.size());
      for (int i = 0; i < commonLen; i++) {
         if (int x = leftPtr[i] - rightPtr[i]) {
            return x;
         }
      }
   } else {
      commonLen = std::min(left.segmentCount(), right.segmentCount());
      for (int i = 0; i < commonLen; i++) {
         if (left.segmentAt(i) != right.segmentAt(i)) {
            return left.segmentAt(i) - right.segmentAt(i);
         }
      }
   }

   if (left.segmentCount() > commonLen) {
      if (left.segmentAt(commonLen) != 0) {
         return left.segmentAt(commonLen);
      } else {
         return 1;
      }
   } else if (right.segmentCount() > commonLen) {
      if (right.segmentAt(commonLen) != 0) {
         return -right.segmentAt(commonLen);
      } else {
         return -1;
      }
   }
   return 0;
}

Version Version::commonPrefix(const Version &left, const Version &right)
{
   int commonLen = std::min(left.segmentCount(), right.segmentCount());
   int i;
   for (i = 0; i < commonLen; i++) {
      if (left.segmentAt(i) != right.segmentAt(i)) {
         break;
      }
   }
   if (0 == i) {
      return Version();
   }
   Version result(!left.m_segments.isUsingPointer() ? left : right);
   result.m_segments.resize(i);
   return result;
}

string Version::toString() const
{
   string version;
   version.reserve(std::max(segmentCount() * 2 - 1, 0));
   bool first = true;
   for (int i = 0; i < segmentCount(); i++) {
      if (!first) {
         version += '.';
      }
      version += std::to_string(segmentAt(i));
      first = false;
   }
   return version;
}

Version Version::fromString(const string &string, int *suffixIndex)
{
   std::vector<int> segment;
   std::size_t start = 0;
   std::size_t end = 0;
   std::size_t lastGoodEnd = 0;
   std::size_t len = string.length();
   do {
      try {
         n_ulonglong value = std::stoull(string, &end);
         segment.push_back(int(value));
         start = end + 1;
         lastGoodEnd = end;
      } catch (...) {
         break;
      }
   } while (start < len && (end < len && string[end] == '.'));
   if (suffixIndex) {
      *suffixIndex = lastGoodEnd;
   }
   return Version(std::move(segment));
}

void Version::SegmentStorage::setVector(int len, int major, int minor, int patch)
{
   pointerSegments = new vector<int>;
   pointerSegments->resize(len);
   pointerSegments->data()[0] = major;
   if (len > 1) {
      pointerSegments->data()[1] = minor;
      if (len > 2) {
         pointerSegments->data()[2] = patch;
      }
   }
}

}; // notify