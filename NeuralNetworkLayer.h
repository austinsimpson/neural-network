#ifndef NEURALNETWORKLAYER_H
#define NEURALNETWORKLAYER_H

template <typename ActivationFunction>
class NeuralNetworkLayer
{
public:
    constexpr NeuralNetworkLayer(size_t neuronCount) : _neuronCount(neuronCount) {}

    constexpr size_t neuronCount() const
    {
        return _neuronCount;
    }

private:
    ActivationFunction _activationFunction;
    size_t _neuronCount;
};

#endif // NEURALNETWORKLAYER_H
