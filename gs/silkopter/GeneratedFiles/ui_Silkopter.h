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
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Sensors.h"
#include "UAV_Inertial.h"

QT_BEGIN_NAMESPACE

class Ui_SilkopterClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tab_3;
    QWidget *info_tab;
    QWidget *sensors_tab;
    QVBoxLayout *verticalLayout;
    Sensors *sensors;
    QWidget *uav_inertial_tab;
    QVBoxLayout *verticalLayout_2;
    UAV_Inertial *uav_inertial;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SilkopterClass)
    {
        if (SilkopterClass->objectName().isEmpty())
            SilkopterClass->setObjectName(QStringLiteral("SilkopterClass"));
        SilkopterClass->resize(884, 684);
        centralWidget = new QWidget(SilkopterClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        tab_3 = new QTabWidget(centralWidget);
        tab_3->setObjectName(QStringLiteral("tab_3"));
        info_tab = new QWidget();
        info_tab->setObjectName(QStringLiteral("info_tab"));
        tab_3->addTab(info_tab, QString());
        sensors_tab = new QWidget();
        sensors_tab->setObjectName(QStringLiteral("sensors_tab"));
        verticalLayout = new QVBoxLayout(sensors_tab);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        sensors = new Sensors(sensors_tab);
        sensors->setObjectName(QStringLiteral("sensors"));

        verticalLayout->addWidget(sensors);

        tab_3->addTab(sensors_tab, QString());
        uav_inertial_tab = new QWidget();
        uav_inertial_tab->setObjectName(QStringLiteral("uav_inertial_tab"));
        verticalLayout_2 = new QVBoxLayout(uav_inertial_tab);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        uav_inertial = new UAV_Inertial(uav_inertial_tab);
        uav_inertial->setObjectName(QStringLiteral("uav_inertial"));

        verticalLayout_2->addWidget(uav_inertial);

        tab_3->addTab(uav_inertial_tab, QString());

        horizontalLayout->addWidget(tab_3);

        SilkopterClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SilkopterClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 884, 21));
        SilkopterClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SilkopterClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SilkopterClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SilkopterClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SilkopterClass->setStatusBar(statusBar);

        retranslateUi(SilkopterClass);

        tab_3->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(SilkopterClass);
    } // setupUi

    void retranslateUi(QMainWindow *SilkopterClass)
    {
        SilkopterClass->setWindowTitle(QApplication::translate("SilkopterClass", "Silkopter", 0));
        tab_3->setTabText(tab_3->indexOf(info_tab), QApplication::translate("SilkopterClass", "Info", 0));
        tab_3->setTabText(tab_3->indexOf(sensors_tab), QApplication::translate("SilkopterClass", "Sensors", 0));
        tab_3->setTabText(tab_3->indexOf(uav_inertial_tab), QApplication::translate("SilkopterClass", "AHRS", 0));
    } // retranslateUi

};

namespace Ui {
    class SilkopterClass: public Ui_SilkopterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SILKOPTER_H
