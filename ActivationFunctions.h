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
        return VectorND<InputDimension>(1.);
    }

};

class SoftmaxActivationFunction
{
public:
    constexpr SoftmaxActivationFunction() {}

    template <size_t InputDimension>
    constexpr VectorND<InputDimension> getValue(const VectorND<InputDimension>& input) const
    {
        double sum = 0.;
        for (const double value: input)
        {
            sum += constexprExp(value);
        }

        VectorND<InputDimension> result{};
        for (int index = 0; index < InputDimension; ++index)
        {
            result.setValue(index, constexprExp(input[index]) / sum);
        }
        return result;
    }

    template <size_t InputDimension>
    constexpr VectorND<InputDimension> getDerivativeValue(const VectorND<InputDimension>& input) const
    {
        constexpr VectorND<InputDimension> oneVec{1.0};
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
        constexpr VectorND<InputDimension> result;
        for (int index = 0; index < InputDimension; ++index)
        {
            result[index] = input[index] < 0. ? 0. : 1.;
        }
        return result;
    }
};

#endif // ACTIVATIONFUNCTIONS_H
