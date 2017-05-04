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
   static void SetUpTestCase()
   {
      m_instanceDataSegments.push_back(vector<int>());
      m_instanceDataSegments.push_back(vector<int>());
      m_instanceDataSegments.push_back(vector<int>());
      m_instanceDataSegments.push_back(vector<int>());
      m_instanceDataSegments.push_back(vector<int>());
   }
   static void TearDownTestCase()
   {

   }
};

vector<vector<int>> VersionTest::m_instanceDataSegments = vector<vector<int>>();

TEST_F(VersionTest, constructorDefault) {
   notify::Version version;
   ASSERT_EQ(version.major(), 0);
   ASSERT_EQ(version.minor(), 0);
   ASSERT_EQ(version.patch(), 0);
   ASSERT_EQ(version.segments(), std::vector<int>());
}