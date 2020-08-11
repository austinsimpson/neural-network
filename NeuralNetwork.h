#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "NeuralNetworkLayer.h"

class NeuralNetwork
{
public:
    template <typename... Layers>
    constexpr NeuralNetwork(Layers... layers) : _networkSize(arraySizeHelper(layers...))
    {
    }

    constexpr size_t networkSize() const
    {
        return _networkSize;
    }

private:
    template <typename U, typename... Layers>
    static constexpr size_t arraySizeHelper(const NeuralNetworkLayer<U>& layer, Layers const&... layers)
    {
        if constexpr (sizeof...(Layers) == 0)
        {
            return layer.neuronCount();
        }
        else
        {
            return layer.neuronCount() * arraySizeHelper(layers...);
        }
    }

    size_t _networkSize;


};

#endif // NEURALNETWORK_H
