#include "VectorND.h"

#include <ActivationFunctions.h>
#include <NeuralNetworkLayer.h>
#include <NeuralNetwork.h>

int main(int argc, char** argv)
{
    constexpr NeuralNetworkLayer<ReluActivationFunction> first{10};
    constexpr NeuralNetwork net
    {
        first
    };

    constexpr size_t networkSize = net.networkSize();
    return 0;
}
