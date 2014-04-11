/********************************************************************************
** Form generated from reading UI file 'Sensors.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENSORS_H
#define UI_SENSORS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Sensors
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QWidget *gyroscope;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QCustomPlot *gyroscope_plot;
    QVBoxLayout *verticalLayout_2;
    QPushButton *gyroscope_calibrate;
    QWidget *accelerometer;
    QHBoxLayout *horizontalLayout_4;
    QCustomPlot *accelerometer_plot;
    QVBoxLayout *verticalLayout_4;
    QPushButton *accelerometer_calibrate;
    QCustomPlot *sonar_plot;
    QCustomPlot *barometer_plot;

    void setupUi(QWidget *Sensors)
    {
        if (Sensors->objectName().isEmpty())
            Sensors->setObjectName(QStringLiteral("Sensors"));
        Sensors->resize(631, 593);
        verticalLayout = new QVBoxLayout(Sensors);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(1);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        gyroscope = new QWidget(Sensors);
        gyroscope->setObjectName(QStringLiteral("gyroscope"));
        horizontalLayout_3 = new QHBoxLayout(gyroscope);
        horizontalLayout_3->setSpacing(1);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        gyroscope_plot = new QCustomPlot(gyroscope);
        gyroscope_plot->setObjectName(QStringLiteral("gyroscope_plot"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gyroscope_plot->sizePolicy().hasHeightForWidth());
        gyroscope_plot->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(gyroscope_plot);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gyroscope_calibrate = new QPushButton(gyroscope);
        gyroscope_calibrate->setObjectName(QStringLiteral("gyroscope_calibrate"));
        gyroscope_calibrate->setMinimumSize(QSize(140, 0));

        verticalLayout_2->addWidget(gyroscope_calibrate);


        horizontalLayout_2->addLayout(verticalLayout_2);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(gyroscope);

        accelerometer = new QWidget(Sensors);
        accelerometer->setObjectName(QStringLiteral("accelerometer"));
        horizontalLayout_4 = new QHBoxLayout(accelerometer);
        horizontalLayout_4->setSpacing(1);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(2, 2, 2, 2);
        accelerometer_plot = new QCustomPlot(accelerometer);
        accelerometer_plot->setObjectName(QStringLiteral("accelerometer_plot"));
        sizePolicy.setHeightForWidth(accelerometer_plot->sizePolicy().hasHeightForWidth());
        accelerometer_plot->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(accelerometer_plot);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(1);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        accelerometer_calibrate = new QPushButton(accelerometer);
        accelerometer_calibrate->setObjectName(QStringLiteral("accelerometer_calibrate"));
        accelerometer_calibrate->setMinimumSize(QSize(140, 0));

        verticalLayout_4->addWidget(accelerometer_calibrate);


        horizontalLayout_4->addLayout(verticalLayout_4);


        verticalLayout_3->addWidget(accelerometer);

        sonar_plot = new QCustomPlot(Sensors);
        sonar_plot->setObjectName(QStringLiteral("sonar_plot"));

        verticalLayout_3->addWidget(sonar_plot);

        barometer_plot = new QCustomPlot(Sensors);
        barometer_plot->setObjectName(QStringLiteral("barometer_plot"));

        verticalLayout_3->addWidget(barometer_plot);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Sensors);

        QMetaObject::connectSlotsByName(Sensors);
    } // setupUi

    void retranslateUi(QWidget *Sensors)
    {
        Sensors->setWindowTitle(QApplication::translate("Sensors", "Form", 0));
        gyroscope_calibrate->setText(QApplication::translate("Sensors", "Calibrate", 0));
        accelerometer_calibrate->setText(QApplication::translate("Sensors", "Calibrate", 0));
    } // retranslateUi

};

namespace Ui {
    class Sensors: public Ui_Sensors {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENSORS_H
