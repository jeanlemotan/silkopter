/********************************************************************************
** Form generated from reading UI file 'Silkopter.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SILKOPTER_H
#define UI_SILKOPTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Orientations.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_SilkopterClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QCustomPlot *plot;
    QCustomPlot *plot2;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    Orientations *orientation;
    Orientations *orientation2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SilkopterClass)
    {
        if (SilkopterClass->objectName().isEmpty())
            SilkopterClass->setObjectName(QStringLiteral("SilkopterClass"));
        SilkopterClass->resize(600, 400);
        centralWidget = new QWidget(SilkopterClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 20, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        plot = new QCustomPlot(centralWidget);
        plot->setObjectName(QStringLiteral("plot"));

        verticalLayout->addWidget(plot);

        plot2 = new QCustomPlot(centralWidget);
        plot2->setObjectName(QStringLiteral("plot2"));

        verticalLayout->addWidget(plot2);


        horizontalLayout->addLayout(verticalLayout);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMaximumSize(QSize(160, 16777215));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        orientation = new Orientations(widget);
        orientation->setObjectName(QStringLiteral("orientation"));
        orientation->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_2->addWidget(orientation);

        orientation2 = new Orientations(widget);
        orientation2->setObjectName(QStringLiteral("orientation2"));

        verticalLayout_2->addWidget(orientation2);


        horizontalLayout->addWidget(widget);

        SilkopterClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SilkopterClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        SilkopterClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SilkopterClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SilkopterClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SilkopterClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SilkopterClass->setStatusBar(statusBar);

        retranslateUi(SilkopterClass);

        QMetaObject::connectSlotsByName(SilkopterClass);
    } // setupUi

    void retranslateUi(QMainWindow *SilkopterClass)
    {
        SilkopterClass->setWindowTitle(QApplication::translate("SilkopterClass", "Silkopter", 0));
    } // retranslateUi

};

namespace Ui {
    class SilkopterClass: public Ui_SilkopterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SILKOPTER_H
