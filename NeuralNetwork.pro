TARGET = NeuralNetwork

QT += core gui widgets

CONFIG += c++latest

HEADERS += \
    ActivationFunctions.h \
    ConstexprMath.h \
    CostFunction.h \
    FunctionModel.h \
    GraphLayer.h \
    GraphWidget.h \
    MnistDataset.h \
    MnistTestDataset.h \
    NeuralNetwork.h \
    NeuralNetworkLayer.h \
    Pcg.h \
    PointSetModel.h \
    Range.h \
    TrainingMetrics.h \
    TrainingPointND.h \
    UniformDistribution.h \
    VectorND.h

SOURCES += \
    FunctionModel.cpp \
    GraphLayer.cpp \
    GraphWidget.cpp \
    PointSetModel.cpp \
    main.cpp

