#include "vxpch.h"
#include "UUID.h"
#include <random>

UUID VERTEX_API get_uuid()
{
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 33);

    const char* v = "0123456789abcdefghiklmnopqrstvxyz";
    const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0,0,0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0,0 };

    std::string res;
    for (int i = 0; i < 38; i++) {
        if (dash[i]) res += "-";
        res += v[dist(rng)];
    }


    std::string res2;


    for (char c : res)
    {
        if (c != 0)
        {
            res2 += c;
        }
    }

    return res2;
}
