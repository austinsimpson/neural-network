/********************************************************************************
** Form generated from reading UI file 'NeuralNetworkDemoWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEURALNETWORKDEMOWINDOW_H
#define UI_NEURALNETWORKDEMOWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "GraphWidget.h"

QT_BEGIN_NAMESPACE

class Ui_NeuralNetworkDemoWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *_tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout;
    GraphWidget *_sinePlotWidget;
    GraphWidget *_sineNetArchitectureWidget;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QTableView *_mnistResultsView;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpinBox *_trainEpochsBox;
    QLabel *label_2;
    QSpinBox *_trainBatchSizeBox;
    QProgressBar *_trainProgressBar;
    QPushButton *_trainSineNetButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *NeuralNetworkDemoWindow)
    {
        if (NeuralNetworkDemoWindow->objectName().isEmpty())
            NeuralNetworkDemoWindow->setObjectName(QString::fromUtf8("NeuralNetworkDemoWindow"));
        NeuralNetworkDemoWindow->resize(800, 600);
        centralwidget = new QWidget(NeuralNetworkDemoWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        _tabWidget = new QTabWidget(centralwidget);
        _tabWidget->setObjectName(QString::fromUtf8("_tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout = new QHBoxLayout(tab);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        _sinePlotWidget = new GraphWidget(tab);
        _sinePlotWidget->setObjectName(QString::fromUtf8("_sinePlotWidget"));

        horizontalLayout->addWidget(_sinePlotWidget);

        _sineNetArchitectureWidget = new GraphWidget(tab);
        _sineNetArchitectureWidget->setObjectName(QString::fromUtf8("_sineNetArchitectureWidget"));

        horizontalLayout->addWidget(_sineNetArchitectureWidget);

        _tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        _mnistResultsView = new QTableView(tab_2);
        _mnistResultsView->setObjectName(QString::fromUtf8("_mnistResultsView"));

        verticalLayout_2->addWidget(_mnistResultsView);

        _tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(_tabWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        _trainEpochsBox = new QSpinBox(centralwidget);
        _trainEpochsBox->setObjectName(QString::fromUtf8("_trainEpochsBox"));
        _trainEpochsBox->setMaximum(100000000);
        _trainEpochsBox->setValue(100);

        horizontalLayout_2->addWidget(_trainEpochsBox);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        _trainBatchSizeBox = new QSpinBox(centralwidget);
        _trainBatchSizeBox->setObjectName(QString::fromUtf8("_trainBatchSizeBox"));
        _trainBatchSizeBox->setMaximum(10000);
        _trainBatchSizeBox->setValue(5000);

        horizontalLayout_2->addWidget(_trainBatchSizeBox);

        _trainProgressBar = new QProgressBar(centralwidget);
        _trainProgressBar->setObjectName(QString::fromUtf8("_trainProgressBar"));
        _trainProgressBar->setValue(0);

        horizontalLayout_2->addWidget(_trainProgressBar);

        _trainSineNetButton = new QPushButton(centralwidget);
        _trainSineNetButton->setObjectName(QString::fromUtf8("_trainSineNetButton"));

        horizontalLayout_2->addWidget(_trainSineNetButton);


        verticalLayout->addLayout(horizontalLayout_2);

        NeuralNetworkDemoWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(NeuralNetworkDemoWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        NeuralNetworkDemoWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(NeuralNetworkDemoWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        NeuralNetworkDemoWindow->setStatusBar(statusbar);

        retranslateUi(NeuralNetworkDemoWindow);

        _tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(NeuralNetworkDemoWindow);
    } // setupUi

    void retranslateUi(QMainWindow *NeuralNetworkDemoWindow)
    {
        NeuralNetworkDemoWindow->setWindowTitle(QCoreApplication::translate("NeuralNetworkDemoWindow", "MainWindow", nullptr));
        _tabWidget->setTabText(_tabWidget->indexOf(tab), QCoreApplication::translate("NeuralNetworkDemoWindow", "Sine Curve", nullptr));
        _tabWidget->setTabText(_tabWidget->indexOf(tab_2), QCoreApplication::translate("NeuralNetworkDemoWindow", "MNIST", nullptr));
        label->setText(QCoreApplication::translate("NeuralNetworkDemoWindow", "Epochs:", nullptr));
        label_2->setText(QCoreApplication::translate("NeuralNetworkDemoWindow", "Batch Size:", nullptr));
        _trainProgressBar->setFormat(QCoreApplication::translate("NeuralNetworkDemoWindow", "%v of %m", nullptr));
        _trainSineNetButton->setText(QCoreApplication::translate("NeuralNetworkDemoWindow", "Train", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NeuralNetworkDemoWindow: public Ui_NeuralNetworkDemoWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEURALNETWORKDEMOWINDOW_H
