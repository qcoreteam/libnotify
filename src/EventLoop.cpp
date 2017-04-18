//
// Created by softboy on 18/04/2017.
//

#include "notify/kernel/Global.h"

namespace notify
{
    int get_sum(int left, int right)
    {
       return left + right;
    };

    Student::Student(int age):age(age)
    {
    }

    int Student::getAge() const
    {
       return age;
    }
}
