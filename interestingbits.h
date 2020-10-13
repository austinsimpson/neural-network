template <typename... Layers>
class NeuralNetwork
{
public:
    ...

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
    ...

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

    ...

    static constexpr size_t _networkSize = arraySizeHelper<Layers...>();
    static constexpr size_t _inputDimension = inputSizeHelper<Layers...>();
    static constexpr size_t _outputDimension = outputSizeHelper<Layers...>();
    std::array<real, _networkSize> _weights;

    std::array<size_t, sizeof...(Layers)> _weightOffsets;
    std::array<size_t, sizeof...(Layers)> _neuronCounts;

    CostFunction<_outputDimension> _costFunction;
};



#endif // NEURALNETWORK_H
