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

using std::string;
using std::vector;
using notify::Version;

class VersionTest : public ::testing::Test
{
protected:
   static vector<vector<int>> m_instanceDataSegments;
   static vector<Version> m_instanceDataVersions;
   static vector<string> m_instanceDataExpectedStrings;
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
   }
   static void TearDownTestCase()
   {
      
   }
};

vector<vector<int>> VersionTest::m_instanceDataSegments = vector<vector<int>>();
vector<Version> VersionTest::m_instanceDataVersions = vector<Version>();
vector<string> VersionTest::m_instanceDataExpectedStrings = vector<string>();

TEST_F(VersionTest, constructorDefault) {
   notify::Version version;
   ASSERT_EQ(version.major(), 0);
   ASSERT_EQ(version.minor(), 0);
   ASSERT_EQ(version.patch(), 0);
   ASSERT_EQ(version.segments(), vector<int>());
}
