TARGET = NeuralNetwork

QT += core gui widgets

CONFIG += c++latest

HEADERS += \
    ActivationFunctions.h \
    ConstexprMath.h \
    CostFunction.h \
    MnistDataset.h \
    NeuralNetwork.h \
    NeuralNetworkLayer.h \
    Pcg.h \
    Range.h \
    TrainingPointND.h \
    UniformDistribution.h \
    VectorND.h

SOURCES += \
    main.cpp

