#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <algorithm>
#include <array>

#include "CostFunction.h"
#include "NeuralNetworkLayer.h"
#include "TrainingPointND.h"
#include "UniformDistribution.h"
#include "VectorND.h"

template <typename Layer, typename NextLayer, typename... Layers>
static constexpr size_t arraySizeHelper()
{
    constexpr size_t weightMatrixSize = Layer::neuronCount() * NextLayer::neuronCount();
    constexpr size_t biasSize = NextLayer::neuronCount();
    if constexpr (sizeof...(Layers) == 0)
    {
        return weightMatrixSize + biasSize;
    }
    else
    {
        return weightMatrixSize + biasSize + arraySizeHelper<NextLayer, Layers...>();
    }
}

template <typename Layer, typename... Layers>
static constexpr size_t inputSizeHelper()
{
    return Layer::neuronCount();
}

template <typename Layer, typename... Layers>
static constexpr size_t outputSizeHelper()
{
    if constexpr (sizeof...(Layers) == 0)
    {
        return Layer::neuronCount();
    }
    else
    {
        return outputSizeHelper<Layers...>();
    }
}


template <typename Layer, typename... Layers>
static constexpr size_t maximalNeuronCountHelper()
{
    if constexpr (sizeof...(Layers) == 0)
    {
        return Layer::neuronCount();
    }
    else
    {
        constexpr size_t maximum = maximalNeuronCountHelper<Layers...>();
        return Layer::neuronCount() < maximum ? maximum : Layer::neuronCount();
    }
}

template <typename... Layers>
class NeuralNetwork
{
public:
    constexpr NeuralNetwork(FunctionType functionType = FunctionType::CrossEntropy) : _costFunction(functionType)
    {
        UniformDistribution dist{-1., 1.};
        for (int index = 0; index < _networkSize; ++index)
        {
            _weights[index] = dist();
        }
    }

    constexpr auto evaluate(const VectorND<inputSizeHelper<Layers...>()>& input) const
    {
        return evaluate<Layers...>(input);
    }

    template <typename Layer, typename NextLayer,  typename... Layers>
    constexpr auto evaluate(const VectorND<Layer::neuronCount()>& input, size_t offset = 0) const
    {
        VectorND<NextLayer::neuronCount()> nextInput;
        for (int i = 0; i < NextLayer::neuronCount(); ++i)
        {
            double element = 0.;
            for (int r = 0; r < Layer::neuronCount(); ++r)
            {
                element += _weights[i * Layer::neuronCount() + i + r + offset] * input[r];
            }
            element += _weights[(i * Layer::neuronCount()) + Layer::neuronCount() + i + offset];
            nextInput.setValue(i, element);
        }
        if constexpr (sizeof...(Layers) == 0)
        {
            return NextLayer::ActivationFunction().getValue(nextInput);
        }
        else
        {
            return evaluate<NextLayer, Layers...>(NextLayer::ActivationFunction().getValue(nextInput), offset + (Layer::neuronCount() * NextLayer::neuronCount()) + NextLayer::neuronCount());
        }
    }

    template <size_t T>
    constexpr void train(const std::array<TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>, T>& trainingData, size_t numberOfEpochs, double learningRate = 0.01)
    {
        for (size_t index = 0; index < numberOfEpochs; ++index)
        {
            for (const auto& trainingPoint : trainingData)
            {
                const auto weightUpdates = getWeightUpdates(trainingPoint);
                for (size_t weightIndex = 0; weightIndex < _weights.size(); ++weightIndex)
                {
                    _weights[weightIndex] -= learningRate * weightUpdates[weightIndex];
                }
            }
        }
    }

    constexpr std::array<double, arraySizeHelper<Layers...>()> getWeightUpdates(const TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>& trainingPoint)
    {

    }

    constexpr auto getActivations(const VectorND<inputSizeHelper<Layers...>>& inputPoint) const
    {
        std::array<VectorND<maximalNeuronCountHelper<Layers...>()>, sizeof...(Layers)> result;
        getActivations<Layers...>(inputPoint, result, 0);
        return result;
    }

    template <typename Layer, typename NextLayer, typename... RemainingLayers>
    constexpr void getActivations(const VectorND<Layer::neuronCount()>& input, std::array<VectorND<maximalNeuronCountHelper<Layers...>()>, sizeof...(Layers)>& accumulator, size_t offset = 0)
    {

    }

    constexpr size_t networkSize() const
    {
        return _networkSize;
    }

    constexpr size_t inputDimension() const
    {
        return _inputDimension;
    }

    constexpr size_t outputDimension() const
    {
        return _outputDimension;
    }

    constexpr auto weights() const
    {
        return _weights;
    }

private:
    static constexpr size_t _networkSize = arraySizeHelper<Layers...>();
    static constexpr size_t _inputDimension = inputSizeHelper<Layers...>();
    static constexpr size_t _outputDimension = outputSizeHelper<Layers...>();
    std::array<double, _networkSize> _weights;
    CostFunction<_outputDimension> _costFunction;
};



#endif // NEURALNETWORK_H
