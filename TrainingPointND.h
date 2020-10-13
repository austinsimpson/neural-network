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

#ifndef TRAININGPOINTND_H
#define TRAININGPOINTND_H

#include "VectorND.h"

template <size_t InputDimension, size_t OutputDimension>
class TrainingPointND
{
public:
    constexpr TrainingPointND() : _input{}, _output{} {}
    constexpr TrainingPointND(const VectorND<InputDimension>& inputVector, const VectorND<OutputDimension>& outputVector): _input(inputVector), _output(outputVector)  {}
    constexpr TrainingPointND(const TrainingPointND<InputDimension, OutputDimension>& other) : _input{ other._input }, _output{ other._output } {}

    constexpr VectorND<InputDimension> input() const
    {
        return _input;
    }

    constexpr VectorND<OutputDimension> output() const
    {
        return _output;
    }

    constexpr TrainingPointND<InputDimension, OutputDimension>& operator=
    (
        const TrainingPointND<InputDimension, OutputDimension>& other
    )
    {
        _input = other._input;
        _output = other._output;
        return *this;
    }

private:
    VectorND<InputDimension> _input;
    VectorND<OutputDimension> _output;
};

#endif // TRAININGPOINTND_H
