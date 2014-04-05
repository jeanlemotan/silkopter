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
#include <QtWidgets/QLabel>
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
    QCustomPlot *gyro_plot;
    QVBoxLayout *verticalLayout_2;
    QPushButton *gyro_calibrate;
    QLabel *gyro_bias;
    QWidget *accelerometer;
    QHBoxLayout *horizontalLayout_4;
    QCustomPlot *accel_plot;
    QVBoxLayout *verticalLayout_4;
    QPushButton *accel_calibrate;
    QLabel *accel_bias;
    QLabel *accel_scale;
    QCustomPlot *sonar_plot;
    QCustomPlot *baro_plot;

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
        gyro_plot = new QCustomPlot(gyroscope);
        gyro_plot->setObjectName(QStringLiteral("gyro_plot"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gyro_plot->sizePolicy().hasHeightForWidth());
        gyro_plot->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(gyro_plot);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gyro_calibrate = new QPushButton(gyroscope);
        gyro_calibrate->setObjectName(QStringLiteral("gyro_calibrate"));
        gyro_calibrate->setMinimumSize(QSize(140, 0));

        verticalLayout_2->addWidget(gyro_calibrate);

        gyro_bias = new QLabel(gyroscope);
        gyro_bias->setObjectName(QStringLiteral("gyro_bias"));
        gyro_bias->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_2->addWidget(gyro_bias);


        horizontalLayout_2->addLayout(verticalLayout_2);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(gyroscope);

        accelerometer = new QWidget(Sensors);
        accelerometer->setObjectName(QStringLiteral("accelerometer"));
        horizontalLayout_4 = new QHBoxLayout(accelerometer);
        horizontalLayout_4->setSpacing(1);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(2, 2, 2, 2);
        accel_plot = new QCustomPlot(accelerometer);
        accel_plot->setObjectName(QStringLiteral("accel_plot"));
        sizePolicy.setHeightForWidth(accel_plot->sizePolicy().hasHeightForWidth());
        accel_plot->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(accel_plot);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(1);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        accel_calibrate = new QPushButton(accelerometer);
        accel_calibrate->setObjectName(QStringLiteral("accel_calibrate"));
        accel_calibrate->setMinimumSize(QSize(140, 0));

        verticalLayout_4->addWidget(accel_calibrate);

        accel_bias = new QLabel(accelerometer);
        accel_bias->setObjectName(QStringLiteral("accel_bias"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(accel_bias->sizePolicy().hasHeightForWidth());
        accel_bias->setSizePolicy(sizePolicy1);
        accel_bias->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_4->addWidget(accel_bias);

        accel_scale = new QLabel(accelerometer);
        accel_scale->setObjectName(QStringLiteral("accel_scale"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(accel_scale->sizePolicy().hasHeightForWidth());
        accel_scale->setSizePolicy(sizePolicy2);
        accel_scale->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_4->addWidget(accel_scale);


        horizontalLayout_4->addLayout(verticalLayout_4);


        verticalLayout_3->addWidget(accelerometer);

        sonar_plot = new QCustomPlot(Sensors);
        sonar_plot->setObjectName(QStringLiteral("sonar_plot"));

        verticalLayout_3->addWidget(sonar_plot);

        baro_plot = new QCustomPlot(Sensors);
        baro_plot->setObjectName(QStringLiteral("baro_plot"));

        verticalLayout_3->addWidget(baro_plot);


        horizontalLayout->addLayout(verticalLayout_3);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Sensors);

        QMetaObject::connectSlotsByName(Sensors);
    } // setupUi

    void retranslateUi(QWidget *Sensors)
    {
        Sensors->setWindowTitle(QApplication::translate("Sensors", "Form", 0));
        gyro_calibrate->setText(QApplication::translate("Sensors", "Calibrate", 0));
        gyro_bias->setText(QApplication::translate("Sensors", "Bias: 0, 0, 0", 0));
        accel_calibrate->setText(QApplication::translate("Sensors", "Calibrate", 0));
        accel_bias->setText(QApplication::translate("Sensors", "Bias: 0, 0, 0", 0));
        accel_scale->setText(QApplication::translate("Sensors", "Scale: 1, 1, 1", 0));
    } // retranslateUi

};

namespace Ui {
    class Sensors: public Ui_Sensors {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENSORS_H
