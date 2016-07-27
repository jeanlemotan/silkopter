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

    boost::signals2::signal<void()> sig_value_changed;

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
        Editor_Data(std::shared_ptr<IValue_Editor> editor)
			: editor(std::move(editor))
		{}
        std::shared_ptr<IValue_Editor> editor;
        bool value_was_commited = false;
        boost::signals2::scoped_connection value_changed_connection;
        boost::signals2::scoped_connection editing_finished_connection;
	};

    mutable std::shared_ptr<Editor_Data> m_editor_data;
};

//////////////////////////////////////////////////////////////////////////
