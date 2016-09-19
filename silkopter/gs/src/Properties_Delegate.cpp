#include "value_editors/IValue_Editor.h"
#include "value_editors/Value_Editor_Factory.h"
#include "value_editors/Empty_Value_Editor.h"
//#include "value_editors/Vector_Element_Wrapper_Editor.h"

#include "Properties_Delegate.h"
#include "Properties_Model.h"
#include "Properties_Browser.h"

#include <QDoubleSpinBox>
#include <QToolTip>
#include <QtEvents>
#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>

//////////////////////////////////////////////////////////////////////////

Properties_Delegate::Properties_Delegate(QObject* parent, std::shared_ptr<Value_Editor_Factory> editor_factory)
    : QStyledItemDelegate(parent)
    , m_editor_factory(editor_factory)
{
}

//////////////////////////////////////////////////////////////////////////

Properties_Delegate::~Properties_Delegate()
{
}

//////////////////////////////////////////////////////////////////////////

std::shared_ptr<Value_Editor_Factory> Properties_Delegate::get_editor_factory()
{
    return m_editor_factory;
}

//////////////////////////////////////////////////////////////////////////

QWidget* Properties_Delegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Properties_Model* model = (Properties_Model*)index.model();
    if (!model)
    {
        return nullptr;
    }

    Properties_Model::Editor_Data ed = model->get_editor_data_from_index(index);
    QASSERT(ed.value);
    if (!ed.value)
    {
        return nullptr;
    }

    m_editor_data.reset();

    std::shared_ptr<IValue_Editor> value_editor = m_editor_factory->create_editor(ed.value, ed.editor, ed.suffix);
    if (!value_editor)
    {
        value_editor = std::make_shared<Empty_Value_Editor>(ed.value, ed.editor, ed.suffix);
    }

    m_editor_data = std::make_shared<Editor_Data>(std::move(value_editor));

    bool is_read_only = model->is_index_read_only(index);
    if (is_read_only)
    {
        m_editor_data->editor->set_read_only_override(true);
    }

    m_editor_data->value_changed_connection = ed.value->sig_value_changed.connect([this]()
    {
        sig_value_changed();
    });

    m_editor_data->editing_finished_connection = m_editor_data->editor->sig_editing_finished.connect(std::bind(&Properties_Delegate::finish_editing, const_cast<Properties_Delegate*>(this)));

    QWidget* editor_widget = m_editor_data->editor->get_widget();

    editor_widget->setParent(parent);
    m_states[index] = QStyle::State_Raised;
    updateEditorGeometry(editor_widget, option, index);

    return editor_widget;
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::finish_editing()
{
    if (m_editor_data)
    {
        Q_EMIT commitData(m_editor_data->editor->get_widget());
    }
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::paint(QPainter* painter, const QStyleOptionViewItem& defaultOption, const QModelIndex& index) const
{
    QStyleOptionViewItem option = defaultOption;

    QStyle* style = qApp->style();

    if (index.column() == 1) // Value
    {
        Properties_Model* model = (Properties_Model*)index.model();
        std::shared_ptr<ts::IValue> value = model->get_value_from_index(index);
        std::shared_ptr<ts::IOptional_Value> v = std::dynamic_pointer_cast<ts::IOptional_Value>(value);
        if (v)
        {
            if (v->is_set())
            {
                option.font.setBold(true);
            }
            else
            {
                //option.text = v->GetEmptyString().c_str();
                QStyledItemDelegate::paint(painter, option, index);
                return;
            }
        }

    }
    QStyledItemDelegate::paint(painter, option, index);
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (m_editor_data)
    {
        m_editor_data->editor->refresh_editor();
    }
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::setModelData(QWidget* editor, QAbstractItemModel* _model, const QModelIndex& index) const
{
    QASSERT(m_editor_data);

    Properties_Model const* model = dynamic_cast<Properties_Model const*>(index.model());
    QASSERT(model);

    if (m_editor_data)
    {
        m_editor_data->editor->refresh_value();
        sig_value_changed();
    }
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    if (m_editor_data)
    {
        //if the value was not committed but changed we need to revert
        if (!m_editor_data->value_was_commited)
        {
            sig_value_changed();
        }
        m_editor_data.reset();
    }
}

//////////////////////////////////////////////////////////////////////////

QSize Properties_Delegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    static QSize size;
    if (size.isEmpty())
    {
        QDoubleSpinBox w;
        size = w.sizeHint();
    }
    return size;
}

//////////////////////////////////////////////////////////////////////////

bool Properties_Delegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

//////////////////////////////////////////////////////////////////////////

void Properties_Delegate::reset()
{
    m_editor_data.reset();
}

//////////////////////////////////////////////////////////////////////////

