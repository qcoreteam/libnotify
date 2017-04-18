//
// Created by softboy on 18/04/2017.
//

#include "gtest/gtest.h"
#include "notify/kernel/Global.h"

#include <iostream>
namespace
{
   TEST(EventMgrTest, AddListener) {
      std::cout << "fuck google test" << std::endl;
      std::cout << "libnotify version " << NOTIFY_VERSION << std::endl;
      std::cout << notify::get_sum(1, 2) << std::endl;
      notify::Student stu(123);
      std::cout << "Student age " << stu.getAge() << std::endl;
   }
}