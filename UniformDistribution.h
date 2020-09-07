#ifndef UNIFORMDISTRIBUTION_H
#define UNIFORMDISTRIBUTION_H

#include "Pcg.h"

class UniformDistribution
{
public:
    constexpr UniformDistribution(double min, double max): _min(min), _max(max)
    {
        _pcg();
    }

    constexpr double operator()()
    {
        constexpr auto min = std::numeric_limits<uint32_t>::min();
        constexpr auto max = std::numeric_limits<uint32_t>::max();
        return ((static_cast<double>(_pcg() - min) / (max - min)) * (_max - _min)) + _min;
    }
private:
    double _min;
    double _max;
    PCG _pcg;
};
#endif // UNIFORMDISTRIBUTION_H
