#include "VectorND.h"

#include <ActivationFunctions.h>
#include <NeuralNetworkLayer.h>
#include <NeuralNetwork.h>

#include <iostream>

#include "TrainingPointND.h"

#include "CostFunction.h"

int main(int argc, char** argv)
{
    ReluActivationFunction relu;
    constexpr NeuralNetwork<NeuralNetworkLayer<ReluActivationFunction, 784>,
                            NeuralNetworkLayer<ReluActivationFunction, 50>,
                            NeuralNetworkLayer<SoftmaxActivationFunction, 10>> net{};

    //constexpr VectorND<10> result = net.evaluate(MnistDataSet[0].input());


    return 0;
}
