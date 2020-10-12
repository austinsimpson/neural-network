TARGET = NeuralNetwork

QT += core gui widgets

CONFIG += c++latest
QMAKE_CXXFLAGS += -fconstexpr-steps:100000000
HEADERS += \
    ActivationFunctions.h \
    ConstexprMath.h \
    CostFunction.h \
    FunctionModel.h \
    GraphLayer.h \
    GraphWidget.h \
    MnistDataset.h \
    MnistResultsViewModel.h \
    MnistTestDataset.h \
    NeuralNetwork.h \
    NeuralNetworkDemoWindow.h \
    NeuralNetworkGraphLayer.h \
    NeuralNetworkLayer.h \
    Pcg.h \
    PointSetModel.h \
    Range.h \
    TrainingPointND.h \
    UniformDistribution.h \
    VectorND.h \
    VectorNDUtility.h

SOURCES += \
    FunctionModel.cpp \
    GraphLayer.cpp \
    GraphWidget.cpp \
    MnistResultsViewModel.cpp \
    NeuralNetworkDemoWindow.cpp \
    PointSetModel.cpp \
    main.cpp

FORMS += \
    NeuralNetworkDemoWindow.ui

