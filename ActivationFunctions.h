#ifndef ACTIVATIONFUNCTIONS_H
#define ACTIVATIONFUNCTIONS_H

class ReluActivationFunction
{
public:
    constexpr ReluActivationFunction() {}

    constexpr double getValue(double input) const
    {
        if (input <= 0.0)
        {
            return 0.0;
        }
        else
        {
            return input;
        }
    }

    constexpr double getDerivativeValue(double input) const
    {
        if (input <= 0.0)
        {
            return 0.0;
        }
        else
        {
            return 1.0;
        }
    }
};


class SteepReluActivationFunction
{
public:
    constexpr SteepReluActivationFunction() {}

    constexpr double getValue(double input) const
    {
        if (input <= 0.0)
        {
            return 0.0;
        }
        else
        {
            return input * 3;
        }
    }

    constexpr double getDerivativeValue(double input) const
    {
        if (input <= 0.0)
        {
            return 0.0;
        }
        else
        {
            return 3.0;
        }
    }
};

#endif // ACTIVATIONFUNCTIONS_H
