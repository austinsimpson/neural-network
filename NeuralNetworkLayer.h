#ifndef NEURALNETWORKLAYER_H
#define NEURALNETWORKLAYER_H

template <typename ActivationFunctionT, size_t NeuronCount>
class NeuralNetworkLayer
{
public:
    constexpr NeuralNetworkLayer() {}

    static constexpr size_t neuronCount()
    {
        return NeuronCount;
    }

    static constexpr ActivationFunctionT ActivationFunction()
    {
        return _activationFunction;
    }

private:
    static ActivationFunctionT _activationFunction;
};

#endif // NEURALNETWORKLAYER_H
