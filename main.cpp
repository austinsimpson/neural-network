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
#include "NeuralNetworkDemoWindow.h"

int main(int argc, char** argv)
{
    QApplication app{ argc, argv };

    NeuralNetworkDemoWindow w;
    w.show();
    
    return app.exec();
}
