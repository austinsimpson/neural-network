//Copyright(c) 2020 Austin Simpson
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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
