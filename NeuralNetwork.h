#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <algorithm>
#include <array>
#include <functional>
#include <optional>
#include <tuple>

#include "CostFunction.h"
#include "NeuralNetworkLayer.h"
#include "Pcg.h"
#include "TrainingPointND.h"
#include "UniformDistribution.h"
#include "VectorND.h"

template <typename Layer, typename NextLayer, typename... RemainingLayers>
static constexpr size_t arraySizeHelper()
{
    constexpr size_t weightMatrixSize = Layer::neuronCount() * NextLayer::neuronCount();
    constexpr size_t biasSize = NextLayer::neuronCount();
    if constexpr (sizeof...(RemainingLayers) == 0)
    {
        return weightMatrixSize + biasSize;
    }
    else
    {
        return weightMatrixSize + biasSize + arraySizeHelper<NextLayer, RemainingLayers...>();
    }
}

template <typename Layer, typename... RemainingLayers>
static constexpr size_t inputSizeHelper()
{
    return Layer::neuronCount();
}

template <typename Layer, typename... RemainingLayers>
static constexpr size_t outputSizeHelper()
{
    if constexpr (sizeof...(RemainingLayers) == 0)
    {
        return Layer::neuronCount();
    }
    else
    {
        return outputSizeHelper<RemainingLayers...>();
    }
}

template <typename Layer, typename... RemainingLayers>
static constexpr size_t maximalNeuronCountHelper()
{
    if constexpr (sizeof...(RemainingLayers) == 0)
    {
        return Layer::neuronCount();
    }
    else
    {
        constexpr size_t maximum = maximalNeuronCountHelper<RemainingLayers...>();
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

using real = double;

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
        buildWeightOffsets<0, Layers...>();
        buildNeuronCounts<0, Layers...>();
    }

    template <size_t N>
    constexpr NeuralNetwork
    (
        FunctionType costFunctionType, 
        const std::array<TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>, N>& trainingData,
        size_t epochs,
        size_t batchSize,
        real learningRate
    ): 
        _costFunction(costFunctionType)
    {
        initializeWeights();
        buildWeightOffsets<0, Layers...>();
        buildNeuronCounts<0, Layers...>();
        train(trainingData, epochs, batchSize, learningRate);
    }

    constexpr NeuralNetwork
    (
        const NeuralNetwork<Layers...>& other
    ):
        _weights{other._weights},
        _costFunction{other._costFunction},
        _neuronCounts{other._neuronCounts},
        _weightOffsets{other._weightOffsets}
    {

    }

    ~NeuralNetwork()
    {

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
        size_t layerIndex = 0
    )   const
    {
        VectorND<NextLayer::neuronCount()> nextInput;
        for (int nextLayerNeuronIndex = 0; nextLayerNeuronIndex < NextLayer::neuronCount(); ++nextLayerNeuronIndex)
        {
            real element = 0.;
            for (int thisLayerNeuronIndex = 0; thisLayerNeuronIndex < Layer::neuronCount(); ++thisLayerNeuronIndex)
            {
                element += getWeight(layerIndex, thisLayerNeuronIndex, nextLayerNeuronIndex) * input[thisLayerNeuronIndex];
            }
            element += getBias(layerIndex, nextLayerNeuronIndex);
            nextInput.setValue(nextLayerNeuronIndex, element);
        }
        nextInput = NextLayer::ActivationFunction().getValue(nextInput);
        if constexpr (sizeof...(Layers) == 0)
        {
            return nextInput;
        }
        else
        {
            return evaluate<NextLayer, Layers...>(nextInput, layerIndex + 1);
        }
    }

    template <size_t N>
    constexpr void train
    (
        const std::array<TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>, N>& trainingData, 
        size_t numberOfEpochs = 5000,
        size_t batchSize = 500, 
        real learningRate = 0.00001
    )
    {
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
                    _weights[weightIndex] -= learningRate * weightUpdates[weightIndex];
                }
            }
        }
    }

    template <size_t N>
    void train
    (
        const std::array<TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>, N>& trainingData,
        size_t numberOfEpochs,
        size_t batchSize,
        real learningRate,
        std::function<void(size_t)> function
    )
    {
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
                    _weights[weightIndex] -= learningRate * weightUpdates[weightIndex];
                }
            }
            if (function)
            {
                function(epochIndex);
            }
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

    constexpr auto& weights() const
    {
        return _weights;
    }

    constexpr const std::array<size_t, sizeof...(Layers)>& neuronCounts() const
    {
        return _neuronCounts;
    }

    constexpr real getWeight(size_t layerIndex, size_t sourceNeuronIndex, size_t destinationNeuronIndex) const
    {
        return _weights[getWeightOffset(layerIndex, sourceNeuronIndex, destinationNeuronIndex)];
    }

    constexpr real& getWeight(size_t layerIndex, size_t sourceNeuronIndex, size_t destinationNeuronIndex)
    {
        return _weights[getWeightOffset(layerIndex, sourceNeuronIndex, destinationNeuronIndex)];
    }

    constexpr real getBias(size_t layerIndex, size_t biasIndex) const
    {
        return _weights[getBiasOffset(layerIndex, biasIndex)];
    }

    constexpr real& getBias(size_t layerIndex, size_t biasIndex)
    {
        return _weights[getBiasOffset(layerIndex, biasIndex)];
    }

private:
    constexpr auto getWeightUpdates
    (
        const TrainingPointND<inputSizeHelper<Layers...>(), outputSizeHelper<Layers...>()>& trainingPoint
    )   const
    {
        std::array<real, arraySizeHelper<Layers...>()> weightUpdates{};
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
        std::array<real, arraySizeHelper<Layers...>()>& weightUpdates,
        const std::tuple<VectorND<Layer::neuronCount()>, VectorND<NextLayer::neuronCount()>, VectorND<RemainingLayers::neuronCount()>...>& preActivations,
        const std::tuple<VectorND<NextLayer::neuronCount()>, VectorND<RemainingLayers::neuronCount()>...>& deltas,
        size_t layerIndex = 0
    )   const
    {
        const auto& currentDelta = std::get<0>(deltas);
        const auto currentActivation = Layer::ActivationFunction().getValue(std::get<0>(preActivations));
        for (size_t nextLayerNeuronIndex = 0; nextLayerNeuronIndex < NextLayer::neuronCount(); ++nextLayerNeuronIndex)
        {
            for (size_t layerNeuronIndex = 0; layerNeuronIndex < Layer::neuronCount(); ++layerNeuronIndex)
            {
                weightUpdates[getWeightOffset(layerIndex, layerNeuronIndex, nextLayerNeuronIndex)] = currentDelta[nextLayerNeuronIndex] * currentActivation[layerNeuronIndex];
            }
            weightUpdates[getBiasOffset(layerIndex, nextLayerNeuronIndex)] = currentDelta[nextLayerNeuronIndex];
        }
        if constexpr (sizeof...(RemainingLayers) > 0)
        {
            weightUpdateHelper<NextLayer, RemainingLayers...>(weightUpdates, popFirst(preActivations), popFirst(deltas), layerIndex + 1);
        }
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
        size_t layerIndex = 0
    )   const
    {
        VectorND<NextLayer::neuronCount()> nextInput;
        for (int nextLayerNeuronIndex = 0; nextLayerNeuronIndex < NextLayer::neuronCount(); ++nextLayerNeuronIndex)
        {
            real element = 0.;
            for (int layerNeuronIndex = 0; layerNeuronIndex < Layer::neuronCount(); ++layerNeuronIndex)
            {
                element += getWeight(layerIndex, layerNeuronIndex, nextLayerNeuronIndex) * input[layerNeuronIndex];
            }
            element += getBias(layerIndex, nextLayerNeuronIndex);
            nextInput.setValue(nextLayerNeuronIndex, element);
        }
        if constexpr (sizeof...(RemainingLayers) > 0)
        {
            auto nextPreActivations = getPreActivations<NextLayer, RemainingLayers...>(NextLayer::ActivationFunction().getValue(nextInput), layerIndex + 1);
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
        const auto& nextDelta = std::get<0>(deltasInLaterLayers);
        const auto activationDerivativeAtLayer = Layer::ActivationFunction().getDerivativeValue(std::get<LayerIndex>(preActivations));

        VectorND<Layer::neuronCount()> errorAtThisLayer;
        for (size_t layerNeuronIndex = 0; layerNeuronIndex < Layer::neuronCount(); ++layerNeuronIndex)
        {
            real errorElement = 0.;
            for (size_t nextLayerNeuronIndex = 0; nextLayerNeuronIndex < NextLayer::neuronCount(); ++nextLayerNeuronIndex)
            {
                const real weight = getWeight(LayerIndex, layerNeuronIndex, nextLayerNeuronIndex);
                errorElement += weight * nextDelta[nextLayerNeuronIndex];
            }
            errorAtThisLayer.setValue(layerNeuronIndex, errorElement);
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


    template <int LayerIndex, typename Layer, typename NextLayer, typename... RemainingLayers>
    constexpr void buildWeightOffsets()
    {
        if constexpr (LayerIndex == 0)
        {
            _weightOffsets[0] = 0;
        }

        size_t offsetAtThisLayer = LayerIndex == 0 ? 0 : _weightOffsets[LayerIndex];
        size_t offsetAtNextLayer = Layer::neuronCount() * NextLayer::neuronCount() + NextLayer::neuronCount();
        _weightOffsets[LayerIndex + 1] = offsetAtThisLayer + offsetAtNextLayer;

        if constexpr (sizeof...(RemainingLayers) > 0)
        {
            buildWeightOffsets<LayerIndex + 1, NextLayer, RemainingLayers...>();
        }
    }

    template <size_t LayerIndex, typename Layer, typename... RemainingLayers>
    constexpr void buildNeuronCounts()
    {
        _neuronCounts[LayerIndex] = Layer::neuronCount();
        if constexpr (sizeof...(RemainingLayers) > 0)
        {
            buildNeuronCounts<LayerIndex + 1, RemainingLayers...>();
        }
    }


    constexpr size_t getWeightOffset(size_t layerIndex, size_t sourceNeuronIndex, size_t destinationNeuronIndex) const
    {
        size_t thisLayerNeuronCount = _neuronCounts[layerIndex];
        size_t nextLayerNeuronCount = _neuronCounts[layerIndex + 1];
        size_t offset = _weightOffsets[layerIndex];
        return destinationNeuronIndex * (thisLayerNeuronCount + 1) + sourceNeuronIndex + offset;
    }

    constexpr size_t getBiasOffset(size_t layerIndex, size_t biasIndex) const
    {
        size_t thisLayerNeuronCount = _neuronCounts[layerIndex];
        size_t offset = _weightOffsets[layerIndex];
        return (biasIndex * thisLayerNeuronCount) + thisLayerNeuronCount + biasIndex + offset;
    }

    static constexpr size_t _networkSize = arraySizeHelper<Layers...>();
    static constexpr size_t _inputDimension = inputSizeHelper<Layers...>();
    static constexpr size_t _outputDimension = outputSizeHelper<Layers...>();
    std::array<real, _networkSize> _weights;

    std::array<size_t, sizeof...(Layers)> _weightOffsets;
    std::array<size_t, sizeof...(Layers)> _neuronCounts;

    CostFunction<_outputDimension> _costFunction;
};



#endif // NEURALNETWORK_H
