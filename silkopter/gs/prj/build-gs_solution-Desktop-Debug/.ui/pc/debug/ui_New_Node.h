/********************************************************************************
** Form generated from reading UI file 'New_Node.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEW_NODE_H
#define UI_NEW_NODE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_New_Node
{
public:
    QVBoxLayout *verticalLayout;
    QTreeView *init_params;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *ok;
    QPushButton *cancel;

    void setupUi(QWidget *New_Node)
    {
        if (New_Node->objectName().isEmpty())
            New_Node->setObjectName(QStringLiteral("New_Node"));
        New_Node->resize(702, 602);
        verticalLayout = new QVBoxLayout(New_Node);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        init_params = new QTreeView(New_Node);
        init_params->setObjectName(QStringLiteral("init_params"));

        verticalLayout->addWidget(init_params);

        widget = new QWidget(New_Node);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        ok = new QPushButton(widget);
        ok->setObjectName(QStringLiteral("ok"));

        horizontalLayout->addWidget(ok);

        cancel = new QPushButton(widget);
        cancel->setObjectName(QStringLiteral("cancel"));

        horizontalLayout->addWidget(cancel);


        verticalLayout->addWidget(widget);


        retranslateUi(New_Node);

        QMetaObject::connectSlotsByName(New_Node);
    } // setupUi

    void retranslateUi(QWidget *New_Node)
    {
        New_Node->setWindowTitle(QApplication::translate("New_Node", "Form", 0));
        ok->setText(QApplication::translate("New_Node", "Ok", 0));
        cancel->setText(QApplication::translate("New_Node", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class New_Node: public Ui_New_Node {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEW_NODE_H
