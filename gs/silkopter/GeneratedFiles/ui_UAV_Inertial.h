/********************************************************************************
** Form generated from reading UI file 'UAV_Inertial.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UAV_INERTIAL_H
#define UI_UAV_INERTIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_UAV_Inertial
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QCustomPlot *acceleration_plot;
    QCustomPlot *velocity_plot;
    QCustomPlot *position_plot;
    QVBoxLayout *verticalLayout;
    QWidget *position_map;
    QWidget *attitude;

    void setupUi(QWidget *UAV_Inertial)
    {
        if (UAV_Inertial->objectName().isEmpty())
            UAV_Inertial->setObjectName(QStringLiteral("UAV_Inertial"));
        UAV_Inertial->resize(689, 639);
        horizontalLayout = new QHBoxLayout(UAV_Inertial);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        acceleration_plot = new QCustomPlot(UAV_Inertial);
        acceleration_plot->setObjectName(QStringLiteral("acceleration_plot"));

        verticalLayout_2->addWidget(acceleration_plot);

        velocity_plot = new QCustomPlot(UAV_Inertial);
        velocity_plot->setObjectName(QStringLiteral("velocity_plot"));

        verticalLayout_2->addWidget(velocity_plot);

        position_plot = new QCustomPlot(UAV_Inertial);
        position_plot->setObjectName(QStringLiteral("position_plot"));

        verticalLayout_2->addWidget(position_plot);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        position_map = new QWidget(UAV_Inertial);
        position_map->setObjectName(QStringLiteral("position_map"));

        verticalLayout->addWidget(position_map);

        attitude = new QWidget(UAV_Inertial);
        attitude->setObjectName(QStringLiteral("attitude"));

        verticalLayout->addWidget(attitude);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(UAV_Inertial);

        QMetaObject::connectSlotsByName(UAV_Inertial);
    } // setupUi

    void retranslateUi(QWidget *UAV_Inertial)
    {
        UAV_Inertial->setWindowTitle(QApplication::translate("UAV_Inertial", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class UAV_Inertial: public Ui_UAV_Inertial {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UAV_INERTIAL_H
