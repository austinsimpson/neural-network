#ifndef COSTFUNCTION_H
#define COSTFUNCTION_H

#include "VectorND.h"
#include "ConstexprMath.h"

enum class FunctionType
{
    LeastSquares = 1,
    CrossEntropy = 2
};

template <size_t N>
class CostFunction
{
public:
    constexpr CostFunction(FunctionType functionType): _functionType(functionType) {}

    constexpr double getValue(const VectorND<N>& approximation, const VectorND<N>& observed) const {
        double result = 0.;
        switch (_functionType)
        {
            case FunctionType::LeastSquares:
            {
                const VectorND<N> difference = approximation - observed;
                for (const auto value : difference) {
                    result += value * value;
                }
                break;
            }
            case FunctionType::CrossEntropy:
            {
                for (size_t index = 0; index < N; ++index)
                {
                    result += -1.*((observed[index] * constexprLog(approximation[index])) + (1. - observed[index]) * (constexprLog(1. - approximation[index])));
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
            case FunctionType::LeastSquares:
                return approximation - observed;
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
private:
    FunctionType _functionType;

};

#endif // COSTFUNCTION_H
