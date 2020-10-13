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

#include "NeuralNetworkDemoWindow.h"

#include "CostFunction.h"

#include "NeuralNetworkGraphLayer.h"

#include <memory>
#include <iostream>

#include "MnistDataset.h"
#include "MnistTestDataset.h"

NeuralNetworkDemoWindow::NeuralNetworkDemoWindow
(
    QWidget* parent
):
    QMainWindow(parent),
    Ui::NeuralNetworkDemoWindow(),
    _sineNet{FunctionType::MeanSquaredError}
{
    setupUi(this);
    _trainProgressBar->setVisible(false);

    _sinePlotWidget->addLayer(FunctionModel{[this](qreal input) -> qreal {
        return _sineNet.evaluate(VectorND<1>{input})[0];
    }});
    _sinePlotWidget->centerOnLogicalCoordinate(QPointF{ 0., 0.});

    _sineNetArchitectureWidget->setShouldRenderGridlines(false);
    _sineNetArchitectureWidget->addLayer(std::make_unique<NeuralNetworkGraphLayer<NeuralNetworkLayer<IdentityActivationFunction, 1>,
                                                                                  NeuralNetworkLayer<SigmoidActivationFunction, 8>,
                                                                                  NeuralNetworkLayer<IdentityActivationFunction, 1>>>(_sineNet));

    _sineNetArchitectureWidget->centerOnLogicalCoordinate(QPointF{1.0, 0.});

    connect(_trainSineNetButton, &QPushButton::clicked, [this] () {
        if (_tabWidget->currentIndex() == 0)
        {
            trainSineNeuralNet();
            _sineNetArchitectureWidget->update();
            _sinePlotWidget->update();
        }
        else
        {
            trainMnistNeuralNet();
        }
    });

    _mnistResultsView->setModel(&_mnistResultsViewModel);
}

NeuralNetworkDemoWindow::~NeuralNetworkDemoWindow()
{
}

void NeuralNetworkDemoWindow::trainSineNeuralNet()
{
    _trainProgressBar->setVisible(true);
    _trainProgressBar->setMaximum(_trainEpochsBox->value());
    std::array<TrainingPointND<1, 1>, 10000> sineValues = generateSineSample<10000>();
    _sineNet.train(sineValues, _trainEpochsBox->value(), _trainBatchSizeBox->value(), 0.005, [this](size_t epochIndex){ _trainProgressBar->setValue(epochIndex); qApp->processEvents(); });
    _trainProgressBar->setVisible(false);
}


void NeuralNetworkDemoWindow::trainMnistNeuralNet()
{
    _trainProgressBar->setVisible(true);
    _trainProgressBar->setMaximum(_trainEpochsBox->value());
    _mnistNet.train(MnistDataSet, _trainEpochsBox->value(), _trainBatchSizeBox->value(), 0.00001, [this](size_t epochIndex){ _trainProgressBar->setValue(epochIndex); qApp->processEvents(); });
    _trainProgressBar->setVisible(false);

    QVector<MnistResultItem> testDataEvaluation;
    for (const auto& testDataItem : MnistTestDataSet)
    {
        MnistResultItem toAdd;
        toAdd.imageData = testDataItem.input();
        toAdd.actualClass = testDataItem.output();
        toAdd.prediction = _mnistNet.evaluate(testDataItem.input());
        testDataEvaluation.append(toAdd);
    }

    _mnistResultsViewModel.setItems(testDataEvaluation);
    _mnistResultsView->update();
}
