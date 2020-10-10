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
