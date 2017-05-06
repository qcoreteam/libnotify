//
// Created by softboy on 5/3/17.
//

#include "gtest/gtest.h"
#include "notify/kernel/Flags.h"
#include <iostream>

namespace notifytest
{

enum Option
{
   NoOptions = 0x0,
   ShowTabs = 0x1,
   ShowAll = 0x2,
   SqueezeBlank = 0x4,
   CloseBtn = 0x8
};

NOTIFY_DECLARE_FLAGS(Options, Option);
NOTIFY_DECLARE_OPERATORS_FOR_FLAGS(Options);

template <unsigned int N, typename T>
bool verify_const_expr(T n)
{
   return n == N;
};

}

TEST(FlagsTest, classicEnum) {
   notifytest::Options opts = notifytest::Option::ShowAll | notifytest::Option::ShowTabs;
   ASSERT_EQ(opts, 3);
   ASSERT_TRUE(opts.testFlag(notifytest::Option::ShowAll));
   ASSERT_TRUE(opts.testFlag(notifytest::Option::ShowTabs));
   opts &= ~notifytest::Option::ShowTabs;
   ASSERT_EQ(opts, 2);
   opts = 0;
   ASSERT_FALSE(opts.testFlag(notifytest::Option::ShowAll));
   ASSERT_FALSE(opts.testFlag(notifytest::Option::ShowTabs));
}

TEST(FlagsTest, constExpr)
{
   notifytest::Options opts = notifytest::Option::ShowTabs | notifytest::Option::ShowAll;
   switch (opts) {
      case notifytest::Option::ShowTabs:
         ASSERT_TRUE(false);
         break;
      case notifytest::Option::ShowAll:
         ASSERT_TRUE(false);
         break;
      case int(notifytest::Option::ShowTabs | notifytest::Option::ShowAll):
         ASSERT_TRUE(true);
         break;
      default:
         ASSERT_TRUE(false);
         break;
   }
   ASSERT_TRUE(notifytest::verify_const_expr<notifytest::Option::ShowTabs | notifytest::Option::ShowAll>
                     (3));
   ASSERT_TRUE(notifytest::verify_const_expr<(notifytest::Option::ShowTabs | notifytest::Option::ShowAll) &
         notifytest::Option::ShowTabs>(notifytest::Option::ShowTabs));
}

TEST(FlagsTest, signedness)
{
   NOTIFY_STATIC_ASSERT(notify::internal::IsUnsignedEnum<notifytest::Option>::value ==
                        notify::internal::IsUnsignedEnum<notifytest::Options::Int>::value);
   NOTIFY_STATIC_ASSERT(notify::internal::IsSignedEnum<notifytest::Option>::value ==
                              notify::internal::IsSignedEnum<notifytest::Options::Int>::value);
}

#if defined(NOTIFY_COMPILER_CLASS_ENUM)
namespace
{
enum class MyClassTypeEnum
{
   Opt1 = 1,
   Opt2,
   Opt3,
   Opt4
};

NOTIFY_DECLARE_FLAGS(MyClassTypeEnums, MyClassTypeEnum);
NOTIFY_DECLARE_OPERATORS_FOR_FLAGS(MyClassTypeEnums);

enum class MyNoOpClassTypeEnum
{
   Opt1 = 1,
   Opt2,
   Opt3,
   Opt4
};
NOTIFY_DECLARE_FLAGS(MyNoOpClassTypeEnums, MyNoOpClassTypeEnum);

}

#endif

TEST(FlagsTest, classEnum)
{
#ifdef NOTIFY_COMPILER_CLASS_ENUM
   MyClassTypeEnum opt1 = MyClassTypeEnum::Opt1;
   MyClassTypeEnum opt2 = MyClassTypeEnum::Opt2;
   MyClassTypeEnums flag1(opt1);
   ASSERT_EQ(flag1, 1);
   MyClassTypeEnums flag2(opt2);
   ASSERT_EQ(flag2, 2);
   MyClassTypeEnums flag3;
   ASSERT_EQ(flag3, 0);
   MyClassTypeEnums flag4(opt1 | opt2);
   ASSERT_EQ(flag4, 3);
   ASSERT_TRUE(flag4.testFlag(MyClassTypeEnum::Opt1));
   ASSERT_FALSE(flag4.testFlag(MyClassTypeEnum::Opt4));
   ASSERT_FALSE(flag3);
   ASSERT_EQ(flag4 & 1, 1);

   ASSERT_EQ(flag4 & int(1), 1);
   ASSERT_EQ(flag4 & uint(1), 1);
   ASSERT_EQ(flag4 & MyClassTypeEnum::Opt1, 1);
#endif
}
