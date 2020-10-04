#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <algorithm>
#include <array>
#include <tuple>
#include <vector>
#include <iostream>

#include "CostFunction.h"
#include "NeuralNetworkLayer.h"
#include "Pcg.h"
#include "TrainingPointND.h"
#include "TrainingMetrics.h"
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

template <typename Layer, typename NextLayer>
static constexpr size_t getWeightMatrixSize()
{
    return (Layer::neuronCount() * NextLayer::neuronCount()) + NextLayer::neuronCount();
}

template <typename First, typename... Rest>
static constexpr std::tuple<Rest...> popFirst(const std::tuple<First, Rest...>& tuple)
{
    return std::apply([](auto head, auto... tail) {
        return std::make_tuple(tail...);
    }, tuple);
}

template <typename... Layers>
class NeuralNetwork
{
public:
    constexpr NeuralNetwork
    (
        FunctionType costFunctionType = FunctionType::CrossEntropy
    ): 
        _costFunction(costFunctionType)
    {
        initializeWeights();
    }

    template <size_t N>
    constexpr NeuralNetwork
    (
        FunctionType costFunctionType, 
        const std::array<TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>, N>& trainingData
    ): 
        _costFunction(costFunctionType)
    {
        initializeWeights();
        train(trainingData);
    }

    constexpr void initializeWeights()
    {
        UniformDistribution dist{-1., 1.};
        for (int index = 0; index < _networkSize; ++index)
        {
            _weights[index] = dist();
        }
    }

    constexpr auto evaluate
    (
        const VectorND<inputSizeHelper<Layers...>()>& input
    )   const
    {
        return evaluate<Layers...>(input);
    }

    template <typename Layer, typename NextLayer,  typename... Layers>
    constexpr auto evaluate
    (
        const VectorND<Layer::neuronCount()>& input, 
        size_t offset = 0
    )   const
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
        nextInput = NextLayer::ActivationFunction().getValue(nextInput);
        if constexpr (sizeof...(Layers) == 0)
        {
            return nextInput;
        }
        else
        {
            return evaluate<NextLayer, Layers...>(nextInput, offset + getWeightMatrixSize<Layer, NextLayer>());
        }
    }

    template <size_t N>
    constexpr TrainingMetrics train
    (
        const std::array<TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>, N>& trainingData, 
        size_t numberOfEpochs = 5000,
        size_t batchSize = 500, 
        double learningRate = 0.00001
    )
    {
        TrainingMetrics result{trainingData.size()};

        PCG pcg;
        for (size_t epochIndex = 0; epochIndex < numberOfEpochs; ++epochIndex)
        {
            for (int trainingPointNumber = 0; trainingPointNumber < batchSize; ++trainingPointNumber)
            {
                int trainingPointIndex = pcg() % trainingData.size();
                const auto& trainingPoint = trainingData[trainingPointIndex];
                const auto weightUpdates = getWeightUpdates(trainingPoint);
                for (size_t weightIndex = 0; weightIndex < _weights.size(); ++weightIndex)
                {
                    if (isinf(weightUpdates[weightIndex]) == false && isnan(weightUpdates[weightIndex]) == false)
                        _weights[weightIndex] -= learningRate * weightUpdates[weightIndex];
                }
                const auto eval = evaluate(trainingPoint.input());
                const auto error = _costFunction.getValue(eval, trainingPoint.output());       
               
                std::cout << trainingPointIndex << '\n';

                result.addErrorPoint(epochIndex, trainingPointNumber, error);
            }
        }
        std::cout << std::endl;

        return result;
    }

    constexpr auto getWeightUpdates
    (
        const TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>& trainingPoint
    )   const
    {
        std::array<double, arraySizeHelper<Layers...>()> weightUpdates{};
        for (size_t index = 0; index < arraySizeHelper<Layers...>(); ++index)
        {
            weightUpdates[index] = 0.;
        }
        auto preActivations = getPreActivations(trainingPoint.input());
        auto deltas = getDeltas(trainingPoint, preActivations);
        weightUpdateHelper<Layers...>(weightUpdates, preActivations, deltas);
        return weightUpdates;
    }

    template <typename Layer, typename NextLayer, typename... RemainingLayers>
    constexpr void weightUpdateHelper
    (
        std::array<double, arraySizeHelper<Layers...>()>& weightUpdates, 
        const std::tuple<VectorND<Layer::neuronCount()>, VectorND<NextLayer::neuronCount()>, VectorND<RemainingLayers::neuronCount()>...>& preActivations, 
        const std::tuple<VectorND<NextLayer::neuronCount()>, VectorND<RemainingLayers::neuronCount()>...>& deltas, 
        size_t weightOffset = 0
    )   const
    {
        const auto& currentDelta = std::get<0>(deltas);
        const auto currentActivation = NextLayer::ActivationFunction().getValue(std::get<0>(preActivations));
        for (size_t nextLayerNeuronIndex = 0; nextLayerNeuronIndex < NextLayer::neuronCount(); ++nextLayerNeuronIndex)
        {
            for (size_t layerNeuronIndex = 0; layerNeuronIndex < Layer::neuronCount(); ++layerNeuronIndex)
            {
                weightUpdates[nextLayerNeuronIndex * (Layer::neuronCount() + 1) + layerNeuronIndex + weightOffset] = currentDelta[nextLayerNeuronIndex] * currentActivation[layerNeuronIndex];
            }
            weightUpdates[nextLayerNeuronIndex * (Layer::neuronCount() + 1) + nextLayerNeuronIndex + weightOffset] = currentDelta[nextLayerNeuronIndex];
        }
        if constexpr(sizeof...(RemainingLayers) > 0)
        {
            weightUpdateHelper<NextLayer, RemainingLayers...>(weightUpdates, popFirst(preActivations), popFirst(deltas), weightOffset + getWeightMatrixSize<Layer, NextLayer>());
        }
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

    constexpr auto getPreActivations
    (
        const VectorND<inputSizeHelper<Layers...>()>& input
    )   const
    {
        return std::tuple_cat(std::tuple<VectorND<inputSizeHelper<Layers...>()>>(input), getPreActivations<Layers...>(input));
    }

    template <typename Layer, typename NextLayer, typename... RemainingLayers>
    constexpr std::tuple<VectorND<NextLayer::neuronCount()>, VectorND<RemainingLayers::neuronCount()>...> getPreActivations
    (
        const VectorND<Layer::neuronCount()>& input, 
        size_t offset = 0
    )   const
    {
        VectorND<NextLayer::neuronCount()> nextInput;
        for (int i = 0; i < NextLayer::neuronCount(); ++i)
        {
            double element = 0.;
            for (int r = 0; r < Layer::neuronCount(); ++r)
            {
                element += _weights[i * (Layer::neuronCount() + 1) + r + offset] * input[r];
            }
            element += _weights[(i * Layer::neuronCount()) + Layer::neuronCount() + i + offset];
            nextInput.setValue(i, element);
        }
        if constexpr (sizeof...(RemainingLayers) > 0)
        {
            auto nextPreActivations = getPreActivations<NextLayer, RemainingLayers...>(NextLayer::ActivationFunction().getValue(nextInput), offset + getWeightMatrixSize<Layer, NextLayer>());
            return std::tuple_cat(std::tuple<VectorND<NextLayer::neuronCount()>>(nextInput), nextPreActivations);
        }
        else
        {
            return std::tuple(nextInput);
        }
    }

    constexpr auto getDeltas
    (
        const TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>& trainingPoint, 
        const std::tuple<VectorND<Layers::neuronCount()>...>& preActivations
    )   const
    {
        return popFirst(getDeltas<0, Layers...>(trainingPoint, preActivations));
    }

    template <size_t LayerIndex, typename Layer, typename NextLayer, typename... RemainingLayers>
    constexpr auto getDeltas
    (
        const TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>& trainingPoint, 
        const std::tuple<VectorND<Layers::neuronCount()>...>& preActivations, 
        size_t weightOffset = 0
    )   const
    {
        const auto deltasInLaterLayers = getDeltas<LayerIndex + 1, NextLayer, RemainingLayers...>(trainingPoint, preActivations, weightOffset + getWeightMatrixSize<Layer, NextLayer>());
        const auto& nextError = std::get<0>(deltasInLaterLayers);
        const auto activationDerivativeAtLayer = Layer::ActivationFunction().getDerivativeValue(std::get<LayerIndex>(preActivations));

        VectorND<Layer::neuronCount()> errorAtThisLayer;
        for (size_t rowIndex = 0; rowIndex < Layer::neuronCount(); ++rowIndex)
        {
            double errorElement = 0.;
            for (size_t columnIndex = 0; columnIndex < NextLayer::neuronCount(); ++columnIndex)
            {
                const double& weight = _weights[columnIndex * (Layer::neuronCount() + 1) + rowIndex + weightOffset];
                errorElement += weight * nextError[columnIndex];
            }
            errorAtThisLayer.setValue(rowIndex, errorElement);
        }

        errorAtThisLayer.pointwiseMultiply(activationDerivativeAtLayer);

        return std::tuple_cat(std::tuple(errorAtThisLayer), deltasInLaterLayers);
    }

    template <size_t LayerIndex, typename Layer>
    constexpr auto getDeltas(const TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>& trainingPoint, const std::tuple<VectorND<Layers::neuronCount()>...>& preActivations, size_t weightOffset = 0) const
    {
        //Get the last pre activation value, as in the summation of the weights and biases.
        const auto& lastPreActivation = std::get<sizeof...(Layers) - 1>(preActivations);
        const auto neuralNetOutput = Layer::ActivationFunction().getValue(lastPreActivation);
        const auto preActivationDerivative = Layer::ActivationFunction().getDerivativeValue(lastPreActivation);
        const auto costGradient = _costFunction.getGradient(neuralNetOutput, trainingPoint.output());
        return std::tuple(preActivationDerivative.pointwiseMultiply(costGradient));
    }


private:
    static constexpr size_t _networkSize = arraySizeHelper<Layers...>();
    static constexpr size_t _inputDimension = inputSizeHelper<Layers...>();
    static constexpr size_t _outputDimension = outputSizeHelper<Layers...>();
    std::array<double, _networkSize> _weights;
    CostFunction<_outputDimension> _costFunction;
};



#endif // NEURALNETWORK_H
