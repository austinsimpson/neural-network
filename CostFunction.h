#ifndef COSTFUNCTION_H
#define COSTFUNCTION_H

#include "VectorND.h"
#include "ConstexprMath.h"

enum class FunctionType
{
    MeanSquaredError = 1,
    CrossEntropy = 2
};

template <size_t N>
class CostFunction
{
public:
    constexpr CostFunction(FunctionType functionType): _functionType(functionType) {}
    constexpr CostFunction(const CostFunction<N>& other) : _functionType(other._functionType) {}

    double getValue(const VectorND<N>& approximation, const VectorND<N>& observed) const {
        double result = 0.;
        switch (_functionType)
        {
            case FunctionType::MeanSquaredError:
            {
                const VectorND<N> difference = approximation - observed;
                for (const auto value : difference) {
                    result += value * value;
                }
                result = result / 2.;
                break;
            }
            case FunctionType::CrossEntropy:
            {
                for (size_t index = 0; index < N; ++index)
                {
                    result += -1.*((observed[index] * log(approximation[index])) + (1. - observed[index]) * (log(1. - approximation[index])));
                }
                break;
            }
        }
        return result;
    }

    constexpr VectorND<N> getGradient(const VectorND<N>& approximation, const VectorND<N>& observed) const
    {
        VectorND<N> result{0.0};
        switch(_functionType)
        {
            case FunctionType::MeanSquaredError:
                result = approximation - observed;
                break;
            case FunctionType::CrossEntropy:
                for (size_t index = 0; index < N; ++index)
                {
                    result[index] = (approximation[index] - observed[index]) / (approximation[index] * (1. - approximation[index]));
                }
                break;
        }
        return result;
    }

    constexpr FunctionType type() const
    {
        return _functionType;
    }
private:
    FunctionType _functionType;

};

#endif // COSTFUNCTION_H
