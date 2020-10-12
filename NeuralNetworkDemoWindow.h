#ifndef NEURALNETWORKDEMOWINDOW_H
#define NEURALNETWORKDEMOWINDOW_H

#include <QMainWindow>
#include "ui_NeuralNetworkDemoWindow.h"

#include "ActivationFunctions.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkLayer.h"
#include "MnistResultsViewModel.h"

constexpr auto pi = 3.14159265358979323846;

class NeuralNetworkDemoWindow : public QMainWindow, public Ui::NeuralNetworkDemoWindow
{
    Q_OBJECT

public:
    explicit NeuralNetworkDemoWindow(QWidget *parent = nullptr);
    ~NeuralNetworkDemoWindow();

    void trainSineNeuralNet();
    void trainMnistNeuralNet();

private:
    template <size_t N> constexpr std::array<TrainingPointND<1, 1>, N> generateSineSample()
    {
        std::array<TrainingPointND<1, 1>, N> sineValues{};
        for (auto index = 0; index < N; ++index)
        {
            double x = (2. * pi) * static_cast<double>(index) / N;
            sineValues[index] = TrainingPointND<1, 1>{ VectorND<1>{x}, VectorND<1>{constexprSin(x)} };
        }
        return sineValues;
    }

    NeuralNetwork<NeuralNetworkLayer<IdentityActivationFunction, 1>, NeuralNetworkLayer<SigmoidActivationFunction, 8>, NeuralNetworkLayer<IdentityActivationFunction, 1>> _sineNet;
    NeuralNetwork<NeuralNetworkLayer<IdentityActivationFunction, 784>, NeuralNetworkLayer<ReluActivationFunction, 30>, NeuralNetworkLayer<SoftmaxActivationFunction, 10>> _mnistNet;

    MnistResultsViewModel _mnistResultsViewModel;
};

#endif // NEURALNETWORKDEMOWINDOW_H
