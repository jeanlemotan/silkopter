#pragma once

#include <QStyledItemDelegate>
#include <QDoubleSpinBox>

class Custom_Item_Delegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    Custom_Item_Delegate(QObject* parent = 0)
        : QStyledItemDelegate(parent)
    {
    }

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE
    {
        QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
        if(QDoubleSpinBox* spinbox = qobject_cast<QDoubleSpinBox*>(editor))
        {
            spinbox->setDecimals(6);
        }
        return editor;
    }

    void setEditorData(QWidget* editor, const QModelIndex& index) const Q_DECL_OVERRIDE
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const Q_DECL_OVERRIDE
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE
    {
        QStyledItemDelegate::updateEditorGeometry(editor, option, index);
    }
};
