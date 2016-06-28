#pragma once

#include "value_editors/IValue_Editor.h"
#include <QStyledItemDelegate>

class Value_Editor_Factory;

//////////////////////////////////////////////////////////////////////////

class Properties_Delegate : public QStyledItemDelegate
{
	Q_OBJECT;
public:
    Properties_Delegate(QObject* parent, std::shared_ptr<Value_Editor_Factory> editor_factory);
    ~Properties_Delegate();

    void reset();
    void finish_editing();

    std::shared_ptr<Value_Editor_Factory> get_editor_factory();

protected:
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    //void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
	void destroyEditor(QWidget *editor, const QModelIndex &index) const override;
private:
    mutable QMap<QModelIndex, QStyle::State> m_states;

    std::shared_ptr<Value_Editor_Factory> m_editor_factory;
    //std::shared_ptr<const EntityBrowserProxyModel> m_proxyModel;

    struct Editor_Data
	{
        Editor_Data(std::shared_ptr<IValue_Editor> editor, std::shared_ptr<ts::IValue> backup)
			: editor(std::move(editor))
			, backup(std::move(backup))
		{}
        std::shared_ptr<IValue_Editor> editor;
        std::shared_ptr<ts::IValue> backup;
        bool value_was_commited = false;
        //q::util::Scoped_Connection editingFinishedConnection;
        //q::util::Scoped_Connection linkPressedConnection;
	};

    mutable std::shared_ptr<Editor_Data> m_editor_data;
};

//////////////////////////////////////////////////////////////////////////
