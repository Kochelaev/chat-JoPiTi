#include "test.h"
#include <iostream>

Test::Test()
{
    std::cout<< "test construct" <<std::endl;
}

void Test::method()
{
    std::cout<< "test method" <<std::endl;
}
