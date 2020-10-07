#include "ActivationFunctions.h"
#include "CostFunction.h"
#include "NeuralNetworkLayer.h"
#include "NeuralNetwork.h"
#include "VectorND.h"

#include "ConstexprMath.h"

#include "FunctionModel.h"
#include "GraphWidget.h"
#include "PointSetModel.h"

#include <cmath>
#include <iostream>

#include <QApplication>

constexpr auto pi = 3.14159265358979323846;

template <size_t N>
constexpr std::array<TrainingPointND<1, 1>, N> generateSineSample()
{
    std::array<TrainingPointND<1, 1>, N> sineValues{};
    for (auto index = 0; index < N; ++index)
    {
        double x = (2. * pi) * static_cast<double>(index) / N;
        sineValues[index] = TrainingPointND<1, 1>{ VectorND<1>{x}, VectorND<1>{constexprSin(x)} };
    }
    return sineValues;
}

int main(int argc, char** argv)
{
    QApplication app{ argc, argv };
 
    constexpr std::array<TrainingPointND<1, 1>, 50> sineValues = generateSineSample<50>();

    constexpr NeuralNetwork<NeuralNetworkLayer<IdentityActivationFunction, 1>,
                            NeuralNetworkLayer<SigmoidActivationFunction, 8>,
                            NeuralNetworkLayer<IdentityActivationFunction, 1>> net{ FunctionType::MeanSquaredError, sineValues, 30, 50, 0.005 };

    GraphWidget widget{nullptr};
    widget.addLayer(FunctionModel{[&net](qreal input) -> qreal {
        return net.evaluate(VectorND<1>{input})[0];
    }});

    widget.show();

    
    return app.exec();
}
