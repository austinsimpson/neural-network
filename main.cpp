#include "ActivationFunctions.h"
#include "CostFunction.h"
#include "NeuralNetworkLayer.h"
#include "NeuralNetwork.h"
#include "MnistDataset.h"
#include "MnistTestDataset.h"
#include "VectorND.h"


#include "FunctionModel.h"
#include "GraphWidget.h"
#include "PointSetModel.h"

#include <cmath>
#include <iostream>

#include <QApplication>

constexpr auto pi = 3.14159265358979323846;

template <size_t N>
std::array<TrainingPointND<1, 1>, N> generateSineSample()
{
    std::array<TrainingPointND<1, 1>, N> sineValues{};
    for (auto index = 0; index < N; ++index)
    {
        double x = (2. * pi) * static_cast<double>(index) / N;
        sineValues[index] = TrainingPointND<1,1>{ VectorND<1>{x}, sin(x) };
    }
    return sineValues;
}

int main(int argc, char** argv)
{
    /*auto net = NeuralNetwork<NeuralNetworkLayer<IdentityActivationFunction, 784>,
                             NeuralNetworkLayer<ReluActivationFunction, 30>,
                             NeuralNetworkLayer<SoftmaxActivationFunction, 10>>();
    auto trainingMetrics = net.train(MnistDataSet);


    for (const auto& testPoint : MnistTestDataSet)
    {
        const auto testEval = net.evaluate(testPoint.input());
        std::cout << "Eval vs Real" << std::endl;
        for (int i = 0; i < 9; ++i)
        {
            std::cout << testEval[i] << "|" << testPoint.output()[i] << '\n';
        }
    }
    std::cout << std::endl;*/

    auto sineValues = generateSineSample<10000>();

    NeuralNetwork<NeuralNetworkLayer<IdentityActivationFunction, 1>,
                  NeuralNetworkLayer<SigmoidActivationFunction, 300>,
                  NeuralNetworkLayer<IdentityActivationFunction, 1>> net{ FunctionType::MeanSquaredError };

    auto trainingMetrics = net.train(sineValues, 1000, 500, 0.000001);

    QApplication app{argc, argv};
    GraphWidget widget{nullptr};
    widget.addLayer(FunctionModel{[&net](qreal input) -> qreal {
        return 0 <= input && input < 2. * pi ? net.evaluate(input)[0] : 0.;
    }});

    widget.addLayer(std::make_unique<GraphLayer>(new PointSetModel(QVector<VectorND<2>>::fromStdVector(trainingMetrics.getErrors()))));
    widget.show();

    return app.exec();
}
