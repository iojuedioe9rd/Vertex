#include "rand.h"
#include<cstdlib>

int rand(int min, int max)
{
    return min + ( std::rand() % max);
}
