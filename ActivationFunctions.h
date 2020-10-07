#ifndef ACTIVATIONFUNCTIONS_H
#define ACTIVATIONFUNCTIONS_H

#include "ConstexprMath.h"
#include "VectorND.h"

#include <numeric>

class IdentityActivationFunction
{
public:
    constexpr IdentityActivationFunction() {};

    template <size_t InputDimension>
    constexpr VectorND<InputDimension> getValue(const VectorND<InputDimension>& input) const
    {
        return input;
    }

    template <size_t InputDimension>
    constexpr VectorND<InputDimension> getDerivativeValue(const VectorND<InputDimension>& input) const
    {
        return VectorND<InputDimension>::ones();
    }

};

class SoftmaxActivationFunction
{
public:
    constexpr SoftmaxActivationFunction() {}

    template <size_t InputDimension>
    VectorND<InputDimension> getValue(const VectorND<InputDimension>& input) const
    {
        auto maximum = input.maxElement();
        double sum = 0.;
        for (const double value: input)
        {
            sum += exp(value - maximum);
        }

        VectorND<InputDimension> result{};
        for (int index = 0; index < InputDimension; ++index)
        {
            result.setValue(index, exp(input[index] - maximum) / sum);
        }
        return result;
    }

    template <size_t InputDimension>
    constexpr VectorND<InputDimension> getDerivativeValue(const VectorND<InputDimension>& input) const
    {
        constexpr VectorND<InputDimension> oneVec = VectorND<InputDimension>::ones();
        VectorND<InputDimension> value = getValue(input);
        value.pointwiseMultiply(oneVec - value);
        return value;
    }
};

class ReluActivationFunction
{
public:
    constexpr ReluActivationFunction() {}

    template <size_t InputDimension>
    constexpr VectorND<InputDimension> getValue(const VectorND<InputDimension>& input) const
    {
        VectorND<InputDimension> result{};
        for (int index = 0; index < InputDimension; ++index)
        {
            result.setValue(index, input[index] < 0. ? 0. : input[index]);
        }
        return result;
    }

    template <size_t InputDimension>
    constexpr VectorND<InputDimension> getDerivativeValue(const VectorND<InputDimension>& input) const
    {
        VectorND<InputDimension> result;
        for (int index = 0; index < InputDimension; ++index)
        {
            result.setValue(index, input[index] < 0. ? 0. : 1.);
        }
        return result;
    }
};

class SigmoidActivationFunction
{
public:
    constexpr SigmoidActivationFunction() {}

    template <size_t InputDimension> 
    constexpr VectorND<InputDimension> getValue(const VectorND<InputDimension>& input) const
    {
        VectorND<InputDimension> result{};
        for (int index = 0; index < InputDimension; ++index)
        {
            if (input[index] < 0)
            {
                result[index] = 1. / (1. + constexprExp(-input[index]));
            }
            else
            {
                double exponent = constexprExp(input[index]);
                result[index] = exponent / (1. + exponent);
            }
        }
        return result;
    }

    template <size_t InputDimension>
    constexpr VectorND<InputDimension> getDerivativeValue(const VectorND<InputDimension>& input) const
    {
        VectorND<InputDimension> oneVector = VectorND<InputDimension>::ones();
        VectorND<InputDimension> functionValue = getValue(input);
        VectorND<InputDimension> result = oneVector - functionValue;
        return result.pointwiseMultiply(functionValue);
    }

};

#endif // ACTIVATIONFUNCTIONS_H
