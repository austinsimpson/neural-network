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


    constexpr CostFunction<3> costFunction{FunctionType::CrossEntropy};

    constexpr VectorND<3> test{0.1999, 0.00001, 0.00001};
    constexpr VectorND<3> realValue{1., 0., 0.};

    constexpr auto cost = costFunction.getGradient(test, realValue);

    for (const auto val : cost)
    {
        std::cout << val << std::endl;
    }
    constexpr VectorND<3> deriv = SoftmaxActivationFunction().getDerivativeValue(test);

    return 0;
}
