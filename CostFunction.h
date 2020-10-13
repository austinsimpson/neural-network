//Copyright(c) 2020 Austin Simpson
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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
