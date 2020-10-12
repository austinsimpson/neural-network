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
