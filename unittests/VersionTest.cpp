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

#include "gtest/gtest.h"
#include "notify/utils/Version.h"
#include <vector>
#include <map>
#include <iostream>
#include <string>

using namespace std;
using notify::Version;

class VersionTest : public ::testing::Test
{
protected:
   static vector<vector<int>> m_instanceDataSegments;
   static vector<Version> m_instanceDataVersions;
   static vector<string> m_instanceDataExpectedStrings;
   static vector<string> m_instanceDataConstructionStrings;
   static void SetUpTestCase()
   {
      m_instanceDataSegments.push_back(vector<int>());
      m_instanceDataSegments.push_back(vector<int>());
      m_instanceDataSegments.push_back(vector<int>());
      m_instanceDataSegments.push_back(vector<int>());
      m_instanceDataSegments.push_back(vector<int>());
      m_instanceDataSegments.push_back(vector<int>({1}));
      m_instanceDataSegments.push_back(vector<int>({1, 2}));
      m_instanceDataSegments.push_back(vector<int>({0}));
      m_instanceDataSegments.push_back(vector<int>({0, 1}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4, 5}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4, 5, 6}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4, 5, 7}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4, 5, 7, 8}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4, 5, 7, 8}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4, 5, 7, 8}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4, 5, 7, 8}));
      m_instanceDataSegments.push_back(vector<int>({0, 1, 2, 3, 4, 5, 7, 8}));
      m_instanceDataSegments.push_back(vector<int>({10, 9}));
      m_instanceDataSegments.push_back(vector<int>({10, 0}));
      m_instanceDataSegments.push_back(vector<int>({127, 9}));
      m_instanceDataSegments.push_back(vector<int>({127, 0}));
      m_instanceDataSegments.push_back(vector<int>({127, 0}));
      m_instanceDataSegments.push_back(vector<int>({128, 9}));
      m_instanceDataSegments.push_back(vector<int>({128, 0}));
      m_instanceDataSegments.push_back(vector<int>({128, 0}));
      
      m_instanceDataVersions.push_back(Version(vector<int>()));
      m_instanceDataVersions.push_back(Version(vector<int>()));
      m_instanceDataVersions.push_back(Version(vector<int>()));
      m_instanceDataVersions.push_back(Version(vector<int>()));
      m_instanceDataVersions.push_back(Version());
      m_instanceDataVersions.push_back(Version(vector<int>({1})));
      m_instanceDataVersions.push_back(Version(vector<int>({1, 2})));
      m_instanceDataVersions.push_back(Version(vector<int>({0})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1})));
      m_instanceDataVersions.push_back(Version(0, 1, 2));
      m_instanceDataVersions.push_back(Version(0, 1, 2));
      m_instanceDataVersions.push_back(Version(0, 1, 2));
      m_instanceDataVersions.push_back(Version(0, 1, 2));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4, 5})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4, 5, 6})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4, 5, 7})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4, 5, 7, 8})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4, 5, 7, 8})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4, 5, 7, 8})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4, 5, 7, 8})));
      m_instanceDataVersions.push_back(Version(vector<int>({0, 1, 2, 3, 4, 5, 7, 8})));
      m_instanceDataVersions.push_back(Version(vector<int>({10, 9})));
      m_instanceDataVersions.push_back(Version(vector<int>({10, 0})));
      m_instanceDataVersions.push_back(Version(vector<int>({127, 9})));
      m_instanceDataVersions.push_back(Version(vector<int>({127, 0})));
      m_instanceDataVersions.push_back(Version(vector<int>({127, 0})));
      m_instanceDataVersions.push_back(Version(vector<int>({128, 9})));
      m_instanceDataVersions.push_back(Version(vector<int>({128, 0})));
      m_instanceDataVersions.push_back(Version(vector<int>({128, 0})));

      m_instanceDataExpectedStrings.push_back(string());
      m_instanceDataExpectedStrings.push_back(string());
      m_instanceDataExpectedStrings.push_back(string());
      m_instanceDataExpectedStrings.push_back(string());
      m_instanceDataExpectedStrings.push_back(string(""));
      m_instanceDataExpectedStrings.push_back(string("1"));
      m_instanceDataExpectedStrings.push_back(string("1.2"));
      m_instanceDataExpectedStrings.push_back(string("0"));
      m_instanceDataExpectedStrings.push_back(string("0.1"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4.5"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4.5.6"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4.5.6.7"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4.5.6.7.8"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4.5.6.7.8"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4.5.6.7.8"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4.5.6.7.8"));
      m_instanceDataExpectedStrings.push_back(string("0.1.2.3.4.5.6.7.8"));
      m_instanceDataExpectedStrings.push_back(string("10.9"));
      m_instanceDataExpectedStrings.push_back(string("10.0"));
      m_instanceDataExpectedStrings.push_back(string("10.0"));
      m_instanceDataExpectedStrings.push_back(string("127.9"));
      m_instanceDataExpectedStrings.push_back(string("127.0"));
      m_instanceDataExpectedStrings.push_back(string("127.0"));
      m_instanceDataExpectedStrings.push_back(string("128.9"));
      m_instanceDataExpectedStrings.push_back(string("128.0"));
      m_instanceDataExpectedStrings.push_back(string("128.0"));

      m_instanceDataConstructionStrings.push_back(string());
      m_instanceDataConstructionStrings.push_back(string("text"));
      m_instanceDataConstructionStrings.push_back(string(" text"));
      m_instanceDataConstructionStrings.push_back(string("Empty String"));
      m_instanceDataConstructionStrings.push_back(string("-1.-2"));
      m_instanceDataConstructionStrings.push_back(string("1.-2-3"));
      m_instanceDataConstructionStrings.push_back(string("1.2-3"));
      m_instanceDataConstructionStrings.push_back(string("0"));
      m_instanceDataConstructionStrings.push_back(string("0.1"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2alpha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2-alpha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.alpha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3alpha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.alpha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4.alpha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4 alpha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4 alp ha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4alp ha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4alpha "));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4.5alpha "));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4.5.6alpha "));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4.5.6.7alpha "));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4.5.6.7.8alpha "));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4.5.6.7.8.alpha "));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4.5.6.7.8 alpha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4.5.6.7.8 alp ha"));
      m_instanceDataConstructionStrings.push_back(string("0.1.2.3.4.5.6.7.8alp ha"));
      m_instanceDataConstructionStrings.push_back(string("10.09"));
      m_instanceDataConstructionStrings.push_back(string("10.0x"));
      m_instanceDataConstructionStrings.push_back(string("10.0xTest"));
      m_instanceDataConstructionStrings.push_back(string("127.09"));
      m_instanceDataConstructionStrings.push_back(string("127.0x"));
      m_instanceDataConstructionStrings.push_back(string("127.0xTest"));
      m_instanceDataConstructionStrings.push_back(string("128.09"));
      m_instanceDataConstructionStrings.push_back(string("128.0x"));
      m_instanceDataConstructionStrings.push_back(string("128.0xTest"));
   }
   static void TearDownTestCase()
   {
      
   }
};

vector<vector<int>> VersionTest::m_instanceDataSegments = vector<vector<int>>();
vector<Version> VersionTest::m_instanceDataVersions = vector<Version>();
vector<string> VersionTest::m_instanceDataExpectedStrings = vector<string>();
vector<string> VersionTest::m_instanceDataConstructionStrings = vector<string>();

TEST_F(VersionTest, constructorDefault) {
   notify::Version version;
   ASSERT_EQ(version.major(), 0);
   ASSERT_EQ(version.minor(), 0);
   ASSERT_EQ(version.patch(), 0);
   ASSERT_EQ(version.segments(), vector<int>());
}

TEST_F(VersionTest, constructorVersions)
{
   int len = m_instanceDataSegments.size();
   for (int i = 0; i < len; i++) {
      const vector<int> &segment = m_instanceDataSegments.at(i);
      const Version &expectedVersion = m_instanceDataVersions.at(i);
      Version version(segment);
      //std::cout << version.major() << std::endl;
      ASSERT_EQ(version.major(), expectedVersion.major());
      ASSERT_EQ(version.minor(), expectedVersion.minor());
      ASSERT_EQ(version.patch(), expectedVersion.patch());
      ASSERT_EQ(version.segments(), expectedVersion.segments());
   }
}

TEST_F(VersionTest, constructorExplicit)
{
   Version v1(1);
   vector<int> v2segs{1, 0, 0};
   Version v2(v2segs);
   ASSERT_EQ(v1.segments(), v2.segments());

   Version v3(1, 2);
   vector<int> v4segs{1, 2, 0};
   Version v4(v4segs);
   ASSERT_EQ(v3.segments(), v4.segments());

   Version v5(1, 2, 3);
   vector<int> v6segs{1, 2, 3};
   Version v6(v6segs);
   ASSERT_EQ(v5.segments(), v6.segments());

#ifdef NOTIFY_COMPILER_INITIALIZER_LISTS
   Version v7(4, 5, 6);
   Version v8{4, 5, 6};
   ASSERT_EQ(v7.segments(), v8.segments());
#endif
}

TEST_F(VersionTest, constructorCopy)
{
   int len = m_instanceDataSegments.size();
   for (int i = 0; i < len; i++) {
      const vector<int> &segment = m_instanceDataSegments.at(i);
      Version original(segment);
      Version version(original);
      const Version &expectedVersion = m_instanceDataVersions.at(i);
      //std::cout << version.major() << std::endl;
      ASSERT_EQ(version.major(), expectedVersion.major());
      ASSERT_EQ(version.minor(), expectedVersion.minor());
      ASSERT_EQ(version.patch(), expectedVersion.patch());
      ASSERT_EQ(version.segments(), expectedVersion.segments());
   }
}

TEST_F(VersionTest, compare)
{
   {
      Version left;
      Version right;
      ASSERT_EQ(left == right, true);
      ASSERT_EQ(left != right, false);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left;
      Version right(0);
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0};
      Version right;
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0};
      Version right{0};
      ASSERT_EQ(left == right, true);
      ASSERT_EQ(left != right, false);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{1, 0};
      Version right{1, 0};
      ASSERT_EQ(left == right, true);
      ASSERT_EQ(left != right, false);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{1};
      Version right{1, 0};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{1, 0};
      Version right{1};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, 1, 2};
      Version right{0, 1};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, 1};
      Version right{0, 1, 2};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0, 1, 2};
      Version right{0, 1, 2};
      ASSERT_EQ(left == right, true);
      ASSERT_EQ(left != right, false);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, 1, 2};
      Version right{1, 1, 2};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{1, 1, 2};
      Version right{0, 1, 2};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{1};
      Version right{-1};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{-1};
      Version right{1};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0, 1};
      Version right{0, -1};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, -1};
      Version right{0, 1};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0, -1};
      Version right{0};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0};
      Version right{0, -1};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, 127, 2};
      Version right{0, 127};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, 127};
      Version right{0, 127, 2};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0, 127, 2};
      Version right{127, 127, 2};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{127, 127, 2};
      Version right{0, 127, 2};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{127};
      Version right{-128};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{-128};
      Version right{127};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0, 127};
      Version right{0, -128};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, -128};
      Version right{0, 127};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0, -128};
      Version right{0};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0};
      Version right{0, -128};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, 128, 2};
      Version right{0, 128};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, 128, 2};
      Version right{0, 128, 2};
      ASSERT_EQ(left == right, true);
      ASSERT_EQ(left != right, false);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, 128, 2};
      Version right{128, 128, 2};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{128};
      Version right{-129};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{-129};
      Version right{128};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0, 128};
      Version right{0, -129};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left{0, -129};
      Version right{0, 128};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }

   {
      Version left{0, -129};
      Version right{0};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left{0};
      Version right{0, -129};
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1});
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      ASSERT_EQ(left == right, true);
      ASSERT_EQ(left != right, false);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 1, 1, 2});
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 1, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 1});
      Version right({0, 1, 2, 3, 4, 5, 6, -1});
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, -1});
      Version right({0, 1, 2, 3, 4, 5, 6, 1});
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, -1});
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, -1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1});
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, -1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0});
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, true);
      ASSERT_EQ(left <= right, true);
      ASSERT_EQ(left > right, false);
      ASSERT_EQ(left >= right, false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, -1});
      ASSERT_EQ(left == right, false);
      ASSERT_EQ(left != right, true);
      ASSERT_EQ(left < right, false);
      ASSERT_EQ(left <= right, false);
      ASSERT_EQ(left > right, true);
      ASSERT_EQ(left >= right, true);
   }
}

TEST_F(VersionTest, isPrefixOf)
{

   {
      Version left;
      Version right;
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left;
      Version right(0);
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left{0};
      Version right;
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0};
      Version right{0};
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left{1, 0};
      Version right{1, 0};
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left{1};
      Version right{1, 0};
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left{1, 0};
      Version right{1};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, 1, 2};
      Version right{0, 1};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, 1};
      Version right{0, 1, 2};
      ASSERT_EQ(left.isPrefixOf(right), true);

   }
   {
      Version left{0, 1, 2};
      Version right{0, 1, 2};
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left{0, 1, 2};
      Version right{1, 1, 2};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{1, 1, 2};
      Version right{0, 1, 2};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{1};
      Version right{-1};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{-1};
      Version right{1};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, 1};
      Version right{0, -1};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, -1};
      Version right{0, 1};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, -1};
      Version right{0};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0};
      Version right{0, -1};
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left{0, 127, 2};
      Version right{0, 127};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, 127};
      Version right{0, 127, 2};
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left{0, 127, 2};
      Version right{127, 127, 2};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{127, 127, 2};
      Version right{0, 127, 2};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{127};
      Version right{-128};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{-128};
      Version right{127};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, 127};
      Version right{0, -128};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, -128};
      Version right{0, 127};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, -128};
      Version right{0};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0};
      Version right{0, -128};
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left{0, 128, 2};
      Version right{0, 128};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, 128, 2};
      Version right{0, 128, 2};
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left{0, 128, 2};
      Version right{128, 128, 2};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{128};
      Version right{-129};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{-129};
      Version right{128};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, 128};
      Version right{0, -129};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0, -129};
      Version right{0, 128};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }

   {
      Version left{0, -129};
      Version right{0};
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left{0};
      Version right{0, -129};
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1});
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 1, 1, 2});
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 1, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 1});
      Version right({0, 1, 2, 3, 4, 5, 6, -1});
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, -1});
      Version right({0, 1, 2, 3, 4, 5, 6, 1});
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, -1});
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, -1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1});
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, -1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0});
      ASSERT_EQ(left.isPrefixOf(right), false);
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, -1});
      ASSERT_EQ(left.isPrefixOf(right), true);
   }
}

TEST_F(VersionTest, commonPrefix)
{
   {
      Version left;
      Version right;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      Version commonPrefix;
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left;
      Version right(0);
      Version calculatedPrefix = Version::commonPrefix(left, right);
      Version commonPrefix;
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0};
      Version right;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      Version commonPrefix;
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0};
      Version right{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      Version commonPrefix{0};
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{1, 0};
      Version right{1, 0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      Version commonPrefix{1, 0};
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{1};
      Version right{1, 0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      Version commonPrefix{1};
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{1, 0};
      Version right{1};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      Version commonPrefix{1};
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 1, 2};
      Version right{0, 1};
      Version commonPrefix{0, 1};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 1};
      Version right{0, 1, 2};
      Version commonPrefix{0, 1};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 1, 2};
      Version right{0, 1, 2};
      Version commonPrefix{0, 1, 2};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 1, 2};
      Version right{1, 1, 2};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{1, 1, 2};
      Version right{0, 1, 2};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{1};
      Version right{-1};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{-1};
      Version right{1};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 1};
      Version right{0, -1};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, -1};
      Version right{0, 1};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, -1};
      Version right{0};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0};
      Version right{0, -1};
      ASSERT_EQ(left.isPrefixOf(right), true);
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 127, 2};
      Version right{0, 127};
      ASSERT_EQ(left.isPrefixOf(right), false);
      Version commonPrefix{0, 127};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 127};
      Version right{0, 127, 2};
      ASSERT_EQ(left.isPrefixOf(right), true);
      Version commonPrefix{0, 127};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 127, 2};
      Version right{127, 127, 2};
      ASSERT_EQ(left.isPrefixOf(right), false);
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{127, 127, 2};
      Version right{0, 127, 2};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{127};
      Version right{-128};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{-128};
      Version right{127};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 127};
      Version right{0, -128};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, -128};
      Version right{0, 127};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, -128};
      Version right{0};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0};
      Version right{0, -128};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 128, 2};
      Version right{0, 128};
      Version commonPrefix{0, 128};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 128, 2};
      Version right{128, 128, 2};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 128, 2};
      Version right{128, 128, 2};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{128};
      Version right{-129};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{-129};
      Version right{128};
      Version commonPrefix;
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, 128};
      Version right{0, -129};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0, -129};
      Version right{0, 128};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }

   {
      Version left{0, -129};
      Version right{0};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left{0};
      Version right{0, -129};
      Version commonPrefix{0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6, 0, 1};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6, 0, 1};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6, 0, 1, 2};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 1, 1, 2});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 1, 1, 2});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1, 2});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 1});
      Version right({0, 1, 2, 3, 4, 5, 6, -1});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, -1});
      Version right({0, 1, 2, 3, 4, 5, 6, 1});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, 1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, -1});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6, 0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, -1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, 1});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6, 0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0, -1});
      Version right({0, 1, 2, 3, 4, 5, 6, 0});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6, 0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
   {
      Version left({0, 1, 2, 3, 4, 5, 6, 0});
      Version right({0, 1, 2, 3, 4, 5, 6, 0, -1});
      Version commonPrefix{0, 1, 2, 3, 4, 5, 6, 0};
      Version calculatedPrefix = Version::commonPrefix(left, right);
      ASSERT_EQ(calculatedPrefix, commonPrefix);
      ASSERT_EQ(calculatedPrefix.segments(), commonPrefix.segments());
   }
}